# Serde Prototype

## Goals

- As fast as possible (only slightly slower than packed structs, ideally)
- Support for variable length arrays and optional types
- Allow deserialization into user-defined, possibly nested, types. See [ResultList](src/targeting/resultlist.h) for an example of this.

## Design

The code for a single type is split across 3 files. Let's look at PnpResult:
- [The struct definition](src/struct/pnpresult_struct.h): This is the data the object holds. Auto-generated. The data this object holds can be primitives or other, fully-deserialized types (like Vec2)
- [The user class](src/targeting/pnpresult_struct.h): This is the fully-deserialized PnpResult type. This contains extra functions users might need to expose like `Amgiguity`, or other computed helper things.
- [The serde interface](src/serde/pnpresult_struct.h): This is a template specilization for converting the user class to/from bytes

## Prior art

- Protobuf: slow on embedded platforms (at least quickbuf is)
- Wpi's struct: no VLAs/optionals
- Rosmsg: I'm not using ros, but I'm stealing their message hash idea
