#pragma once

#include <any>
#include <map>
#include <string>
#include <iostream>

using namespace std; // I know it's bad...

#define FRIENDS_OF_TYPE(type)                                                                      \
    friend void print(const type& in);                                                             \
    friend any  serialize(const type& src);                                                        \
    friend void deserialize(const any& src, type& dst)

// helpers that don't expand to anything - used by the type parser
#define FIELD    // indicates the start of a field definition inside of a type
#define INLINE   // class attribute - emitted reflection methods should not be marked as inline
#define CALLBACK // field attribute - the given callback will be called after the field changes
#define ATTRIBUTES(...) // list attributes and tags in a comma-separated fashion using this

namespace tag {
    struct special {};
}

inline void print(int in) { cout << "  [print: int]    " << in << endl; }
inline void print(float in) { cout << "  [print: float]  " << in << endl; }
inline void print(const string& in) { cout << "  [print: string] " << in << endl; }
inline void print(const string& in, tag::special) { cout << "  [print: string - overload] " << in << endl; }

inline any serialize(int in) { return in; }
inline any serialize(float in) { return in; }
inline any serialize(string in) { return in; }

inline void deserialize(const any& src, int& dst) { dst = any_cast<int>(src); }
inline void deserialize(const any& src, float& dst) { dst = any_cast<float>(src); }
inline void deserialize(const any& src, string& dst) { dst = any_cast<string>(src); }
