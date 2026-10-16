#include "netlab/packet_codec.hpp"
#include "netlab/bytes.hpp"
#include "netlab/checksum.hpp"

namespace netlab{
    std::array<std::uint8_t, 6> serialize_header(const PacketHeader& header) {
        std::array<std::uint8_t, 6> wire_buffer;

        wire_buffer[0] = header.version;
        wire_buffer[1] = header.type;

        auto len_bytes = encode_u16_be(header.payload_length);
        wire_buffer[2] = len_bytes[0];
        wire_buffer[3] = len_bytes[1];

        // Compute checksum over the first 4 bytes
        std::uint16_t cksum = netlab::compute_checksum(std::span<const std::uint8_t>(wire_buffer.data(), 4));

        auto cksum_bytes = encode_u16_be(cksum);
        wire_buffer[4] = cksum_bytes[0];
        wire_buffer[5] = cksum_bytes[1];

        return wire_buffer;
    }

    std::optional<PacketHeader> parse_header(std::span<const std::uint8_t> bytes) {
        if (bytes.size() < 6) {
            return std::nullopt;
        }
        std::uint16_t stored_checksum = decode_u16_be(bytes.subspan(4, 2));

        std::uint16_t computed_checksum = netlab::compute_checksum(bytes.subspan(0, 4));

        if (stored_checksum != computed_checksum) {
            return std::nullopt;
        }

        PacketHeader header;
        header.version = bytes[0];
        header.type = bytes[1];
        header.payload_length = decode_u16_be(bytes.subspan(2, 2));

        return header;
    }

}