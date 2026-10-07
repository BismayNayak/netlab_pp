#include "netlab/bytes.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>

int main() {
    std::uint16_t value = 0x1234;

    auto encoded = netlab::encode_u16_be(value);

    std::cout << "Encoded bytes: ";

    for (std::uint8_t byte : encoded) {
        std::cout
            << std::hex
            << std::setw(2)
            << std::setfill('0')
            << static_cast<int>(byte)
            << ' ';
    }

    std::cout << '\n';

    std::uint16_t decoded =
        netlab::decode_u16_be(encoded);

    std::cout
        << "Decoded value: 0x"
        << std::hex
        << decoded
        << '\n';
}