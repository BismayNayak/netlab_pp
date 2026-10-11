#include "netlab/packet_codec.hpp"
#include "netlab/bytes.hpp"

namespace netlab{
    std::array<std::uint8_t,4> serialize_header(const PacketHeader& header){
        std::array<std::uint8_t,4> wire_buffer;
        wire_buffer[0]=header.version;
        wire_buffer[1]=header.type;

        std::array<uint8_t,2> length_bytes= netlab::encode_u16_be(header.payload_length);
        wire_buffer[2]=length_bytes[0];
        wire_buffer[3]=length_bytes[1];

        return wire_buffer;
    }

    std::optional<PacketHeader>parse_header(std::span<const std::uint8_t> bytes){
        if (bytes.size()<4) return std::nullopt;

        PacketHeader header;
        header.version=bytes[0];
        header.type=bytes[1];

        header.payload_length=netlab::decode_u16_be(bytes.subspan(2,2));

        return header;
    }

}