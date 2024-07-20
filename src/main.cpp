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

#include "Packet.h"
#include "serde/pnpresult_serde.h"
#include "serde/resultlist_serde.h"
#include "serde/vec2_serde.h"
#include "targeting/resultlist.h"

int main() {
  Packet packet;

  Vec2 a{Vec2Struct{1, 2}};
  Vec2 b{Vec2Struct{3, 4}};
  PnpResultStruct test{a, b, -99};

  PnpResultStruct test2{Vec2{{100, 101}}, Vec2{{101, 102}}, -87};

  std::vector<PnpResult> resultList{PnpResult{test}, PnpResult{test2}};

  ResultListStruct inner{resultList, 32};
  ResultList list{inner};

  packet.Pack<ResultList>(list);

  auto unpacked = packet.Unpack<ResultList>();
  // packet.Pack<PnpResult>(unpacked);

  (void)unpacked;
  return 0;
}
