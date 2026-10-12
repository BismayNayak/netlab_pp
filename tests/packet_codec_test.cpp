#include "netlab/packet_codec.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <span>

int failures = 0;

void expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
        return;
    }
    std::cerr<<"PASS :"<<message<<'\n';
}

int main() {
    // Test 1 — Exact serialized bytes (Wire Format)
    {
        netlab::PacketHeader header{
            .version = 3,
            .type = 7,
            .payload_length = 0x1234
        };

        auto bytes = netlab::serialize_header(header);

        expect(bytes[0] == 0x03, "version byte");
        expect(bytes[1] == 0x07, "type byte");
        expect(bytes[2] == 0x12, "payload length high byte");
        expect(bytes[3] == 0x34, "payload length low byte");
    }

    // Test 2 — Round Trip Property: parse(serialize(x)) == x
    {
        netlab::PacketHeader original{
            .version = 5,
            .type = 1,
            .payload_length = 1024
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
        // 0-byte input
        std::array<std::uint8_t, 0> buf0{};
        auto res0 = netlab::parse_header(buf0);
        expect(!res0.has_value(), "0-byte input returns std::nullopt");

        // 1-byte input
        std::array<std::uint8_t, 1> buf1{0x01};
        auto res1 = netlab::parse_header(buf1);
        expect(!res1.has_value(), "1-byte input returns std::nullopt");

        // 2-byte input
        std::array<std::uint8_t, 2> buf2{0x01, 0x02};
        auto res2 = netlab::parse_header(buf2);
        expect(!res2.has_value(), "2-byte input returns std::nullopt");

        // 3-byte input
        std::array<std::uint8_t, 3> buf3{0x01, 0x02, 0x00};
        auto res3 = netlab::parse_header(buf3);
        expect(!res3.has_value(), "3-byte input returns std::nullopt");
    }

    // Test 4 — More than four bytes (Trailing Payload / Slicing)
    {
        std::array<std::uint8_t, 6> packet{
            0x01, 0x02, 0x01, 0xF4, // Header bytes (version=1, type=2, payload_length=500)
            0xAA, 0xBB              // Trailing payload bytes
        };

        auto parsed = netlab::parse_header(packet);

        expect(parsed.has_value(), "packet with trailing payload parses successfully");

        if (parsed.has_value()) {
            expect(parsed->version == 1, "trailing payload version is 1");
            expect(parsed->type == 2, "trailing payload type is 2");
            expect(parsed->payload_length == 500, "trailing payload length is 500");
        }
    }

    // Summary & Exit Status Code
    if (failures == 0) {
        std::cout << "All packet codec tests passed\n";
        return 0;
    }

    std::cerr << failures << " test(s) failed\n";
    return 1;
}