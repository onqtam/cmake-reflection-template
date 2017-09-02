import sys

source = open(sys.argv[1], 'r')

current_type = ""
attributes = {}
types = {}

for line in source:
    words = line.split()
    # skip empty lines
    if len(words) < 1:
        continue
    
    # start a type - also skips forward declarations
    if words[0] in {"struct", "class"} and current_type == "" and len(words) > 1 and ";" not in line:
        if words[1] in ["HAPI", "HA_EMPTY_BASE"] and len(words) > 2:
            current_type = words[2]
        else:
            current_type = words[1]
        types[current_type] = {"attributes" : attributes, "fields" : []}
        # reset attributes for the following fields
        attributes = {}
        continue
    # end a type
    if line.strip() == "};" and current_type != "":
        current_type = ""
        continue
    # detect a field - requires the "FIELD" preprocessor identifier to be used at the start of it's definition
    if current_type != "" and words[0] == "FIELD":
        find_field_name_ender = lambda line: next((i for i, ch  in enumerate(line) if ch in {"{", "=", ";"}), None)
        ind = find_field_name_ender(line)
        # if the name is on a separate line from the type - perhaps the next one
        while not ind:
            line = next(source)
            ind = find_field_name_ender(line)
        # get the last word by splitting into words the line up until the index we found
        field = line[:ind].split()[-1]
        # add the field to the current type
        types[current_type]["fields"].append({"name" : field, "attributes" : attributes})
        # reset attributes for the following fields
        attributes = {}
    # detect field or type attributes
    if words[0].startswith("ATTRIBUTES("):
        # split the contents between the 2 most outer brackets
        attributes_list = line.partition('(')[-1].rpartition(')')[0].split(",")
        # remove the whitespace in each attribute
        for index, val in enumerate(attributes_list): attributes_list[index] = val.strip()
        # stuff the attributes in a dict
        for attribute in attributes_list:
            if attribute.startswith("tag::"):
                attributes["tag"] = attribute
            elif attribute.startswith("CALLBACK"):
                attributes["callback"] = attribute.partition('(')[-1].rpartition(')')[0]
            else:
                attributes[attribute] = True

source.close()

# always generate the output - even if no types are reflected for the currently parsed source file
# otherwise the build system will always run the custom commands for each source that doesn't have generated output for it
gen = open(sys.argv[2], 'w+')
gen.writelines('#pragma once\n\n')

for type in types:
    # do not continue with the current type if it has no fields
    if not types[type]["fields"]:
        continue
    
    inline = ('inline ') if "INLINE" in types[type]["attributes"] else ''
    
    gen.writelines('%svoid print(const %s& in) {\n' % (inline, type))
    for field in types[type]["fields"]:
        tag = (', ' + field["attributes"]["tag"] + '()') if "tag" in field["attributes"] else ''
        gen.writelines('    print(in.%s%s);\n' % (field["name"], tag))
    gen.writelines('}\n\n')
    
    gen.writelines('%sany serialize(const %s& src) {\n' % (inline, type))
    gen.writelines('    map<string, any> out;\n')
    for field in types[type]["fields"]:
        gen.writelines('    out["%s"] = serialize(src.%s);\n' % (field["name"], field["name"]))
    gen.writelines('    return out;\n')
    gen.write('}\n\n')
    
    gen.writelines('%svoid deserialize(const any& src, %s& dst) {\n' % (inline, type))
    gen.writelines('    const auto& data = any_cast<const map<string, any>&>(src);\n')
    for field in types[type]["fields"]:
        callback = (field["attributes"]["callback"] + '(dst); ') if "callback" in field["attributes"] else ''
        gen.writelines('    if(data.count("%s")) { deserialize(data.at("%s"), dst.%s); %s}\n' % (field["name"], field["name"], field["name"], callback))
    gen.writelines('}\n\n')

gen.close()
