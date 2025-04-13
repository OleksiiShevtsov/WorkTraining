#pragma once

#include "core/sorting_transformer.h"
#include "core/intersection_transformer.h"
#include "core/unique_reverse_transformer.h"

namespace at
{
    enum class TransformType {
        Sorting,
        Intersection,
        UniqueReverse
    };

    template <typename Container>
    class TransformerFactory
    {
    public:
        static std::unique_ptr<ArrayTransformer<Container>> createTransformer(const TransformType& type)
        {
            switch (type) {
            case TransformType::Sorting:
                return std::make_unique<SortingTransformer<Container>>();
            case TransformType::Intersection:
                return std::make_unique<IntersectionTransformer<Container>>();
            case TransformType::UniqueReverse:
                return std::make_unique<UniqueReverseTransformer<Container>>();
            default:
                throw std::invalid_argument("Unknown transform type");
            }

            return nullptr;
        }
    };
}
