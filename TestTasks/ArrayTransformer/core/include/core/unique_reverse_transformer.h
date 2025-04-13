#pragma once

#include "array_transformer.h"

namespace at
{
    template <typename Container>
    class UniqueReverseTransformer : public ArrayTransformer<Container>
    {
    public:
        Container transform(const Container& input) override
        {
            Container uniqueReverseArrays;

            typename Container::value_type mergedReverseArray;
            for (const auto& array : input)
            {
                mergedReverseArray.insert(mergedReverseArray.end(), array.begin(), array.end());
            }

            reverseSort(mergedReverseArray);

            uniqueReverseArrays.insert(uniqueReverseArrays.end(), getUniqueElements(mergedReverseArray));

            return uniqueReverseArrays;
        }

    private:

        typename Container::value_type getUniqueElements(const typename Container::value_type& input)
        {
            typename Container::value_type uniqueElements;

            for (const auto& element : input)
            {
                if (!contains(uniqueElements, element))
                {
                    uniqueElements.insert(uniqueElements.end(), element);
                }
            }

            return uniqueElements;
        }

        void reverseSort(typename Container::value_type& input)
        {
            for (auto firstIterator = input.begin(); firstIterator != input.end(); ++firstIterator)
            {
                for (auto secondIterator = input.begin(); secondIterator != firstIterator; ++secondIterator)
                {
                    if (*firstIterator > *secondIterator)
                    {
                        auto tempIterator = *firstIterator;
                        *firstIterator = *secondIterator;
                        *secondIterator = tempIterator;
                    }
                }
            }
        }

        bool contains(const typename Container::value_type& input, const typename Container::value_type::value_type& value)
        {
            for (const auto& item : input)
            {
                if (item == value)
                {
                    return true;
                }
            }
            return false;
        }
    };
}