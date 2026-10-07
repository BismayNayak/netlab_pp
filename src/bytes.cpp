#include <array>
#include <iostream>
#include <span>
#include <cstdint>
#include <iomanip>
std::array <std::uint8_t,2> encode_u16_be (std::uint16_t value){
    return {
        static_cast<std::uint8_t>((value>>8)& 0xFF),
        static_cast<std::uint8_t> (value & 0xFF)
    };
}

std::uint16_t decode_u8_be (std::span<const std::uint8_t> bytes){
    if (bytes.size() < 2) return 0;

    return (
        static_cast<std::uint16_t> (
            static_cast<std::uint16_t>(bytes[0])<<8 | 
            static_cast<std::uint16_t>(bytes[1])
        )
    );
}

int main(){
    std::uint16_t value=0x1234;
    std::array<std::uint8_t,2> encode = encode_u16_be(value);
    
    std::cout<<"Encoded bytes :";

    for (std::uint8_t bytes : encode){
        std::cout
            <<std::hex
            <<std::setw(2)
            <<std::setfill('0')
            <<static_cast<int> (bytes)
            <<' ';
    }

    std::cout<<'\n';
    
    std::uint16_t decode = decode_u8_be(encode);

    std::cout
        <<"Decoded Value :0x"
        <<std::hex
        <<decode
        <<'\n';
}