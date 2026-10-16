#include "netlab/checksum.hpp"

namespace netlab {
    std::uint16_t compute_checksum(std::span<const std::uint8_t> bytes) {
        std::uint32_t sum = 0;
        std::size_t i = 0;

        while (i + 1 <= bytes.size()) {
            std::uint16_t word = static_cast<std::uint16_t>(
                (static_cast<std::uint16_t>(bytes[i]) << 8) | 
                static_cast<std::uint16_t>(bytes[i + 1])
            );
            sum += word;
            i += 2;
        }

        if (i < bytes.size()) {
            std::uint16_t word = static_cast<std::uint16_t>(static_cast<std::uint16_t>(bytes[i]) << 8);
            sum += word; 
        }

        while (sum >> 16) {
            sum = (sum & 0xFFFF) + (sum >> 16);
        }

        return static_cast<std::uint16_t>(~sum);
    }
}
