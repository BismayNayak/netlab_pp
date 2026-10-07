#include "netlab/bytes.hpp"

namespace netlab {

    std::array<std::uint8_t, 2>
    encode_u16_be(std::uint16_t value) {
        return {
            static_cast<std::uint8_t>((value >> 8) & 0xFF),
            static_cast<std::uint8_t>(value & 0xFF)
        };
    }

    std::uint16_t
    decode_u16_be(std::span<const std::uint8_t> bytes) {
        if (bytes.size() < 2) {
            return 0;
        }

        return static_cast<std::uint16_t>(
            (static_cast<std::uint16_t>(bytes[0]) << 8) |
            static_cast<std::uint16_t>(bytes[1])
        );
    }

}

