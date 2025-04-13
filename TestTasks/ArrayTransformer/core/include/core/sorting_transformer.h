#pragma once

#include "core/array_transformer.h"

namespace at
{
    template <typename Container>
    class SortingTransformer : public ArrayTransformer<Container>
    {
    public:
        Container transform(const Container& input) override
        {
            Container sorted = input;
            for (auto it = sorted.begin(); it != sorted.end(); ++it)
            {
                sort(it->begin(), it->end());
            }
            return sorted;
        }

    private:
        template <typename Iterator>
        void sort(Iterator first, Iterator last)
        {
            bool swapped;
            for (auto i = first; i != last; ++i)
            {
                swapped = false;
                for (auto j = first; j < last - 1 - (i - first); ++j)
                {
                    if (*j > *(j + 1))
                    {
                        swap(*j, *(j + 1));
                        swapped = true;
                    }
                }
                if (!swapped) break;
            }
        }

        template <typename Iterator>
        void swap(Iterator& first, Iterator& second)
        {
            Iterator tempIterator = first;
            first = second;
            second = tempIterator;
        }
    };
}
