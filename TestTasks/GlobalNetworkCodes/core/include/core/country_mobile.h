#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace gnc
{
    struct CountryMobile
    {
        struct Operator
        {
            uint16_t mnc;
            std::string name;
        };

        uint16_t mcc;
        uint16_t mncLength;
        std::string name;
        std::vector<Operator> operators;
    };
};

