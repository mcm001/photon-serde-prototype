#pragma once

#include "struct/vec2_struct.h"

class Vec2 : public Vec2Struct {
public:
    // Vec2(Vec2Struct&& data) : Vec2Struct(data) {}
    Vec2(Vec2Struct data) : Vec2Struct(data) {}
};
