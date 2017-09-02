#pragma once

#include "common.h"

ATTRIBUTES(INLINE)
class Foo
{
    FRIENDS_OF_TYPE(Foo);

    FIELD int   a = 5;
    FIELD float b = 42.f;

    FIELD std::string str = "normal";
    ATTRIBUTES(tag::special, CALLBACK(Foo::some_callback))
    FIELD std::string tagged_str = "tagged!";

    static void some_callback(Foo& src) { cout << "  [callback] Foo::tagged_str changed!" << endl; }
};

#include <gen/foo.h.inl>
