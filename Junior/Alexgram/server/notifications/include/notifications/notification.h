#pragma once

#include "common/notification/enum.h"
#include "common/types.h"

#include <optional>
#include <string_view>

namespace sylk
{
class INotification
{
public:
    virtual ~INotification() = default;

    virtual common::ID getPlayerId() const                 = 0;
    virtual std::optional< common::ID > getTableId() const = 0;
};
} // namespace sylk
