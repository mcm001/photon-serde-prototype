#include <iostream>
#include <atomic>
#include <vector>
#include <optional>
#include <span>
#include <cxxabi.h>
#include <optional>
#include <concepts>

// cursed
template<typename T>
std::string type_name()
{
    int status;
    std::string tname = typeid(T).name();
    char *demangled_name = abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
    if(status == 0) {
        tname = demangled_name;
        std::free(demangled_name);
    }   
    return tname;
}

template<typename T>
concept arithmetic = std::integral<T> or std::floating_point<T>;

// Struct is where all our actual ser/de methods are implemented
template <typename T>
struct Struct{};

class Packet;

template <typename T>
concept PhotonStructSerializable = requires(Packet& packet, const T& value) {
    typename Struct<typename std::remove_cvref_t<T>>;
  // Unpack myself from a packet
  { Struct<typename std::remove_cvref_t<T>>::Unpack(packet) } -> std::same_as<typename std::remove_cvref_t<T>>;
  // Pack myself into a packet
  { Struct<typename std::remove_cvref_t<T>>::Pack(packet, value) } -> std::same_as<void>;
};

class Packet {
public:
    template <typename T>
    requires (arithmetic<T>)
    void Pack(T value) {
        std::cout << "- Packed number " << value << " of len " << sizeof(T) << " at offset " << writePtr - data.data() << std::endl;

        T& ref = *((T*)writePtr);
        ref = value;

        writePtr += sizeof(T);
    }

    template <typename T>
    requires (PhotonStructSerializable<T>)
    void Pack(T value) {
        std::cout << "packing struct " << type_name<T>() << " at " << writePtr - data.data() << std::endl; 
        Struct<typename std::remove_cvref_t<T>>::Pack(*this, value);
    }

    template <typename T>
    requires (arithmetic<T>)
    T Unpack() {
        T& ref = *((T*)readPtr);
        std::cout << "- unpacking number at " << readPtr - data.data() << "! got " << ref << std::endl; 
        readPtr += sizeof(T);
        return ref;
    }

    template <typename T>
    requires (PhotonStructSerializable<T>)
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
