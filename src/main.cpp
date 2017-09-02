#include "foo.h"

int main() {
    Foo obj;
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
