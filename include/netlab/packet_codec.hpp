#pragma once

#include <cstdint>
#include <array>
#include <optional>
#include <span>
namespace netlab {

    struct PacketHeader{
        std::uint8_t version;
        std::uint8_t type;
        std::uint16_t payload_length;
    };

    std::array<std::uint8_t,6>serialize_header(const PacketHeader& header);
    std::optional<PacketHeader>parse_header(std::span<const uint8_t> bytes);

}
