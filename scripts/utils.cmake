set(gen_folder ${CMAKE_BINARY_DIR}/gen/gen)

# a routine to delete all generated code when the parser is modified
add_custom_command(
    OUTPUT ${gen_folder}/touched.txt # dummy output file so the custom target can be attached to this command
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${gen_folder}              # 1st command to execute
    COMMAND ${CMAKE_COMMAND} -E make_directory   ${gen_folder}              # 2nd command to execute
    COMMAND ${CMAKE_COMMAND} -E touch            ${gen_folder}/touched.txt  # 3rd command to execute
    DEPENDS ${CMAKE_SOURCE_DIR}/scripts/parse_source.py # the parser script as a dependency
    COMMENT "[codegen] deleting all generated files because the parser has been modified")

add_custom_target(parser_modified DEPENDS ${gen_folder}/touched.txt) # so parsed targets can depend on it

# attaches custom commands for each file (header and source) to be parsed when modified and have the codegen regenerated
# will add a "CMake Rules" folder to the target because MAIN_DEPENDENCY cannot be used on source files
function(target_parse_sources target)
    set(allowed_file_extensions h|cpp|hpp|hh|cc|cxx)
    set(excluded_file_patterns "precompiled|_tests")
    
    # get the sources of the target and iterate over each of them
    get_target_property(sources ${target} SOURCES)
	foreach(src ${sources})
        # if the current file matches the criteria
        if(src MATCHES \\.\(${allowed_file_extensions}\)$ AND NOT src MATCHES ${excluded_file_patterns})
            set(src ${CMAKE_CURRENT_SOURCE_DIR}/${src}) # full path to input file
            
            # name of generated file
            get_filename_component(src_name ${src} NAME)
            set(gen ${gen_folder}/${src_name}.inl)
            
            add_custom_command(
                OUTPUT ${gen}
                DEPENDS ${src} # cannot use MAIN_DEPENDENCY - see this: https://gitlab.kitware.com/cmake/cmake/issues/16580
                COMMAND python ${CMAKE_SOURCE_DIR}/scripts/parse_source.py ${src} ${gen} # call the python script with the 2 filenames
                COMMENT "[codegen] parsing ${src}")
            
            # add the output to the target - so the custom command is attached somewhere
            target_sources(${target} PRIVATE ${gen})
            # mark the output files as generated
            set_source_files_properties(${gen} PROPERTIES GENERATED TRUE)
            # group the output files in a folder
            source_group("gen" FILES ${gen})
        endif()
    endforeach()
    
    # add the folder with the generated sources to the include directories of the target
    target_include_directories(${target} PRIVATE ${CMAKE_BINARY_DIR}/gen)
    # make the target depend on the parser_modified target so when the parser is modified all files get re-parsed
    add_dependencies(${target} parser_modified)
endfunction()
