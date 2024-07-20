#pragma once

#include "targeting/resultlist.h"

template <>
struct Struct<ResultList> {
  static ResultList Unpack(Packet& packet) {
    return ResultListStruct {
        packet.Unpack<std::vector<PnpResult>>(),
        packet.Unpack<std::optional<int>>(),
    };
  }
  static void Pack(Packet& packet, const ResultList& value) {
        packet.Pack<std::vector<PnpResult>>(value.things);
        packet.Pack<std::optional<int>>(value.iteration);
  }
};
