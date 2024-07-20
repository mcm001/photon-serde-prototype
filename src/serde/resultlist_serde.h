/*
 * Copyright (C) Photon Vision.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <optional>
#include <vector>

#include "targeting/resultlist.h"

// sub-class serde includes
#include "pnpresult_serde.h"

template <> struct Struct<ResultList> {
  static ResultList Unpack(Packet &packet) {
    return ResultList{ResultListStruct{
        packet.Unpack<std::vector<PnpResult>>(),
        packet.Unpack<std::optional<int>>(),
    }};
  }
  static void Pack(Packet &packet, const ResultList &value) {
    packet.Pack<std::vector<PnpResult>>(value.things);
    packet.Pack<std::optional<int>>(value.iteration);
  }
};
