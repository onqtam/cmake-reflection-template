#pragma once

#include "common.h"

ATTRIBUTES(INLINE)
class Foo {
    FRIENDS_OF_TYPE(Foo);

    FIELD int   a = 5;

    FIELD float b = 42.f;
};

#include <gen/foo.h.inl>
