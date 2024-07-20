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

#include "Packet.h"
#include "targeting/pnpresult.h"

template <> struct Struct<PnpResult> {
  static PnpResult Unpack(Packet &packet) {
    return PnpResultStruct{
        .best = packet.Unpack<Vec2>(),
        .alt = packet.Unpack<Vec2>(),
        .ambiguity = packet.Unpack<double>(),
    };
  }
  static void Pack(Packet &packet, const PnpResult &value) {
    packet.Pack<Vec2>(value.best);
    packet.Pack<Vec2>(value.alt);
    packet.Pack<double>(value.ambiguity);
  }
};
