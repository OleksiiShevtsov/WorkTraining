#pragma once

#include "country_mobile.h"

#include <unordered_map>

namespace gnc::type
{
    // TOODU add status
    using QueryResult = std::vector<std::vector<std::string>>;
    using CountriesMobile = std::unordered_map<std::string, CountryMobile>; // map<code, CountryMobile>
};
