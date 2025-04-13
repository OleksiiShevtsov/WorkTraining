#pragma once

namespace at
{
    template <typename Container>
    class ArrayTransformer
    {
    public:
        virtual ~ArrayTransformer() = default;
        virtual Container  transform(const Container& input) = 0;
    };
}
