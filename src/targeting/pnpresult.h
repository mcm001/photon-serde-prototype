#pragma once

#include "struct/pnpresult_struct.h"

class PnpResult : public PnpResultStruct {
public:
    // PnpResult(PnpResultStruct&& data) : PnpResultStruct(data) {}
    PnpResult(PnpResultStruct data) : PnpResultStruct(data) {}

    double Ambiguity() const { return ambiguity; }
};
