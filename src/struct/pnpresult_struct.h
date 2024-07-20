#pragma once

#include "vec2.h"

struct PnpResultStruct {
    Vec2 best;
    Vec2 alt;
    double ambiguity;
};

