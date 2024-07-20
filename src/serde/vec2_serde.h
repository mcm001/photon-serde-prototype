template <>
struct Struct<Vec2> {
  static Vec2 Unpack(Packet& packet) {
    return Vec2Struct {
        .x = packet.Unpack<double>(),
        .y = packet.Unpack<double>(),
    };
  }
  static void Pack(Packet& packet, const Vec2& value) {
    packet.Pack<double>(value.x);
    packet.Pack<double>(value.y);
  }
};
