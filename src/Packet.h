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

#include <cxxabi.h>

#include <atomic>
#include <concepts>
#include <iostream>
#include <optional>
#include <span>
#include <string>
#include <vector>

// cursed
template <typename T> std::string type_name() {
  int status;
  std::string tname = typeid(T).name();
  char *demangled_name =
      abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
  if (status == 0) {
    tname = demangled_name;
    std::free(demangled_name);
  }
  return tname;
}

template <typename T>
concept arithmetic = std::integral<T> || std::floating_point<T>;

// Struct is where all our actual ser/de methods are implemented
template <typename T> struct Struct {};

class Packet;

template <typename T>
concept PhotonStructSerializable = requires(Packet &packet, const T &value) {
  typename Struct<typename std::remove_cvref_t<T>>;
  // Unpack myself from a packet
  {
    Struct<typename std::remove_cvref_t<T>>::Unpack(packet)
  } -> std::same_as<typename std::remove_cvref_t<T>>;
  // Pack myself into a packet
  {
    Struct<typename std::remove_cvref_t<T>>::Pack(packet, value)
  } -> std::same_as<void>;
};

class Packet {
public:
  template <typename T>
    requires(arithmetic<T>)
  void Pack(T value) {
    std::cout << "- Packed number " << value << " of len " << sizeof(T)
              << " at offset " << writePtr - data.data() << std::endl;

    T &ref = *((T *)writePtr);
    ref = value;

    writePtr += sizeof(T);
  }

  template <typename T>
    requires(PhotonStructSerializable<T>)
  void Pack(T value) {
    std::cout << "packing struct " << type_name<T>() << " at "
              << writePtr - data.data() << std::endl;
    Struct<typename std::remove_cvref_t<T>>::Pack(*this, value);
  }

  template <typename T>
    requires(arithmetic<T>)
  T Unpack() {
    T &ref = *((T *)readPtr);
    std::cout << "- unpacking number at " << readPtr - data.data() << "! got "
              << ref << std::endl;
    readPtr += sizeof(T);
    return ref;
  }

  template <typename T>
    requires(PhotonStructSerializable<T>)
  T Unpack() {
    std::cout << "unpacking struct " << type_name<T>() << std::endl;

    auto ret = Struct<typename std::remove_cvref_t<T>>::Unpack(*this);
    return ret;
  }

  std::vector<uint8_t> data;
  uint8_t *readPtr;
  uint8_t *writePtr;

  Packet() {
    data.reserve(2000);
    readPtr = data.data();
    writePtr = data.data();
  }
};

// support encoding vectors
template <typename T>
  requires(PhotonStructSerializable<T> || arithmetic<T>)
struct Struct<std::vector<T>> {
  static std::vector<T> Unpack(Packet &packet) {
    uint8_t len = packet.Unpack<uint8_t>();
    std::cout << "unpacking list " << type_name<T>() << ", len=" << len
              << std::endl;
    std::vector<T> ret;
    ret.reserve(len);
    for (size_t i = 0; i < len; i++) {
      ret.push_back(packet.Unpack<T>());
    }
    return ret;
  }
  static void Pack(Packet &packet, const std::vector<T> &value) {
    std::cout << "packing list " << type_name<T>() << " of len " << value.size()
              << std::endl;
    packet.Pack<uint8_t>(value.size());
    for (const auto &thing : value) {
      packet.Pack<T>(thing);
    }
  }
};

// support encoding optional types
template <typename T>
  requires(PhotonStructSerializable<T> || arithmetic<T>)
struct Struct<std::optional<T>> {
  static std::optional<T> Unpack(Packet &packet) {
    std::cout << "unpacking optional " << type_name<T>() << std::endl;
    if (packet.Unpack<uint8_t>() == 1u) {
      return packet.Unpack<T>();
    } else {
      return std::nullopt;
    }
  }
  static void Pack(Packet &packet, const std::optional<T> &value) {
    std::cout << "packing optional " << type_name<T>() << std::endl;
    packet.Pack<uint8_t>(value.has_value());
    if (value) {
      packet.Pack<T>(*value);
    }
  }
};
