#include "foo.h"

class Bar {
    FRIENDS_OF_TYPE(Bar);

    FIELD Foo foo; // compound types just work

    FIELD std::string str = "normal";

    ATTRIBUTES(tag::special, CALLBACK(Bar::some_callback))
    FIELD std::string tagged_str = "tagged!";

    static void some_callback(Bar& src) { cout << "  [callback] Bar::tagged_str changed!" << endl; }
};

int main() {
    Bar obj;
    auto original_state = serialize(obj);

    cout << endl << "[info] initial state of object:" << endl;
    print(obj);

    cout << endl << "[info] about to try to set a specific value of object:" << endl;

    map<string, any> new_data;
    new_data.insert({"tagged_str", string(">>> NEW STRING VALUE !!! <<<")});
    deserialize(new_data, obj);

    cout << endl << "[info] after setting the value of a specific field:" << endl;
    print(obj);

    cout << endl << "[info] about to set the original state of the object:" << endl;

    deserialize(original_state, obj);

    cout << endl << "[info] after deserialization with original state:" << endl;
    print(obj);

    system("pause");
}

#include <gen/main.cpp.inl>
