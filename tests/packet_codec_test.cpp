#include "netlab/packet_codec.hpp"

#include <array>
#include <vector>
#include <cstdint>
#include <iostream>
#include <span>

int failures = 0;

void expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }
}

int main() {
    // Test 1 — Exact 6-Byte Serialized Wire Format
    {
        netlab::PacketHeader header{
            .version = 1,
            .type = 2,
            .payload_length = 500
        };

        auto bytes = netlab::serialize_header(header);

        expect(bytes[0] == 0x01, "version byte");
        expect(bytes[1] == 0x02, "type byte");
        expect(bytes[2] == 0x01, "payload length high byte");
        expect(bytes[3] == 0xF4, "payload length low byte");
        expect(bytes[4] == 0xFD, "checksum high byte");
        expect(bytes[5] == 0x09, "checksum low byte");
    }

    // Test 2 — Round Trip Property: parse(serialize(x)) == x
    {
        netlab::PacketHeader original{
            .version = 3,
            .type = 7,
            .payload_length = 0x1234
        };

        auto bytes = netlab::serialize_header(original);
        auto parsed = netlab::parse_header(bytes);

        expect(parsed.has_value(), "valid header parses successfully");

        if (parsed.has_value()) {
            expect(parsed->version == original.version, "round-trip version matches");
            expect(parsed->type == original.type, "round-trip type matches");
            expect(parsed->payload_length == original.payload_length, "round-trip payload_length matches");
        }
    }

    // Test 3 — Short Inputs (Buffer Underflow Protection)
    {
        for (std::size_t size = 0; size < 6; ++size) {
            std::vector<std::uint8_t> short_buf(size, 0);
            auto res = netlab::parse_header(short_buf);
            expect(!res.has_value(), "buffer smaller than 6 bytes returns std::nullopt");
        }
    }

    // Test 4 — Corruption Testing (Payload Bit Flip)
    {
        netlab::PacketHeader header{
            .version = 1,
            .type = 2,
            .payload_length = 500
        };

        auto bytes = netlab::serialize_header(header);

        // Corrupt a bit in payload length (0xF4 -> 0xF5 via XOR)
        auto corrupted_bytes = bytes;
        corrupted_bytes[3] ^= 0x01;

        auto parsed = netlab::parse_header(corrupted_bytes);
        expect(!parsed.has_value(), "corrupted header data causes checksum failure and rejects packet");
    }

    // Test 5 — Checksum Corruption Testing
    {
        netlab::PacketHeader header{
            .version = 1,
            .type = 2,
            .payload_length = 500
        };

        auto bytes = netlab::serialize_header(header);

        // Corrupt the checksum byte itself (0x09 -> 0x08)
        auto corrupted_bytes = bytes;
        corrupted_bytes[5] ^= 0x01;

        auto parsed = netlab::parse_header(corrupted_bytes);
        expect(!parsed.has_value(), "corrupted checksum bytes cause parse failure");
    }

    // Summary & Exit Status Code
    if (failures == 0) {
        std::cout << "All packet codec tests passed\n";
        return 0;
    }

    std::cerr << failures << " test(s) failed\n";
    return 1;
}