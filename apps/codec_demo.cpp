#include <iostream>
#include <iomanip>
#include "netlab/packet_codec.hpp"
#include "netlab/bytes.hpp"

using namespace netlab;

int main() {
    PacketHeader original{
        .version = 1,
        .type = 2,
        .payload_length = 500 
    };

    auto wire_bytes = serialize_header(original);

    std::cout << "Wire bytes: ";
    for (std::uint8_t b : wire_bytes) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(b) << ' ';
    }
    std::cout << "\n\n";

    const std::span<std::uint8_t> view(wire_bytes);
    auto parsed = parse_header(view);

    if (parsed) {
        std::cout << "Parsed successfully!\n";
        std::cout << "Version:        " << std::dec << static_cast<int>(parsed->version) << '\n';
        std::cout << "Type:           " << static_cast<int>(parsed->type) << '\n';
        std::cout << "Payload Length: " << parsed->payload_length << '\n';
    }
}