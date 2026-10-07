#include <array>
#include <iostream>
#include <span>
#include <cstdint>
#include <iomanip>

std::array <std::uint8_t,2> encode_u16_be (std::uint16_t value);
std::uint16_t decode_u8_be (std::span<const std::uint8_t> bytes); 