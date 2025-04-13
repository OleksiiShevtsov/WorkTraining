#pragma once

#include "core/array_transformer.h"
#include "core/sorting_transformer.h"

namespace at
{
    template <typename Container>
    class IntersectionTransformer : public ArrayTransformer<Container>
    {
    public:
        Container transform(const Container& input) override
        {
            SortingTransformer<Container> sortingTransformer;
            Container sortedArrays = sortingTransformer.transform(input);

            manualSortArrays(sortedArrays);

            typename Container::value_type intersectionTwo = findIntersection(
                sortedArrays.begin()->begin(), sortedArrays.begin()->end(),
                (sortedArrays.begin() + 1)->begin(), (sortedArrays.begin() + 1)->end()
            );

            typename Container::value_type intersectionAll = *sortedArrays.begin();
            for (auto iterator = sortedArrays.begin(); iterator != sortedArrays.end(); ++iterator)
            {
                intersectionAll = findIntersection(
                    intersectionAll.begin(), intersectionAll.end(),
                    iterator->begin(), iterator->end()
                );
            }

            Container intersection;
            intersection.insert(intersection.end(), intersectionTwo);
            intersection.insert(intersection.end(), intersectionAll);

            return intersection;
        }
    private:

        void manualSortArrays(Container& arrays)
        {
            for (auto iteratorFirst = arrays.begin(); iteratorFirst != arrays.end(); ++iteratorFirst)
            {
                for (auto iteratorSecond = arrays.begin(); iteratorSecond != iteratorFirst; ++iteratorSecond)
                {
                    if (iteratorFirst->size() > iteratorSecond->size())
                    {
                        auto tempIterator = *iteratorFirst;
                        *iteratorFirst = *iteratorSecond;
                        *iteratorSecond = tempIterator;
                    }
                }
            }
        }

        typename Container::value_type findIntersection(
            typename Container::value_type::const_iterator beginFirst,
            typename Container::value_type::const_iterator endFirst,
            typename Container::value_type::const_iterator beginSecond,
            typename Container::value_type::const_iterator endSecond)
        {
            typename Container::value_type intersectionBetweenTwo;

            while (beginFirst != endFirst && beginSecond != endSecond)
            {
                if (*beginFirst < *beginSecond)
                {
                    ++beginFirst;
                }
                else if (*beginSecond < *beginFirst)
                {
                    ++beginSecond;
                }
                else
                {
                    intersectionBetweenTwo.insert(intersectionBetweenTwo.end(), *beginFirst);
                    ++beginFirst;
                    ++beginSecond;
                }
            }

            return intersectionBetweenTwo;
        }
    };
};
