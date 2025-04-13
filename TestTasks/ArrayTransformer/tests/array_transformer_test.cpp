#pragma once

#include <gtest/gtest.h>

#include "core/types.h"
#include "core/sorting_transformer.h"
#include "core/intersection_transformer.h"
#include "core/unique_reverse_transformer.h"

namespace at
{
    namespace tests
    {
        types::Container getTestArrays()
        {
            return {
                { 3, 2, 1, 0 },
                { 1, 13, 9, -7, 11, 0, 30 },
                { 10, -10, 30, 20, 0, 40 }
            };
        }

        TEST(TransformerTest, sortedArraysTest)
        {
            types::Container arrays = getTestArrays();

            SortingTransformer<types::Container> sortingTransformer;
            types::Container sortedArrays = sortingTransformer.transform(arrays);

            ASSERT_EQ((*sortedArrays.begin()).at(0), 0);
            ASSERT_EQ((*sortedArrays.begin()).at(1), 1);
            ASSERT_EQ((*sortedArrays.begin()).at(2), 2);
            ASSERT_EQ((*sortedArrays.begin()).at(3), 3);

            ASSERT_EQ((*(sortedArrays.begin() + 1)).at(0), -7);
            ASSERT_EQ((*(sortedArrays.begin() + 1)).at(1), 0);
            ASSERT_EQ((*(sortedArrays.begin() + 1)).at(2), 1);
            ASSERT_EQ((*(sortedArrays.begin() + 1)).at(3), 9);
            ASSERT_EQ((*(sortedArrays.begin() + 1)).at(4), 11);
            ASSERT_EQ((*(sortedArrays.begin() + 1)).at(5), 13);
            ASSERT_EQ((*(sortedArrays.begin() + 1)).at(6), 30);

            ASSERT_EQ((*(sortedArrays.begin() + 2)).at(0), -10);
            ASSERT_EQ((*(sortedArrays.begin() + 2)).at(1), 0);
            ASSERT_EQ((*(sortedArrays.begin() + 2)).at(2), 10);
            ASSERT_EQ((*(sortedArrays.begin() + 2)).at(3), 20);
            ASSERT_EQ((*(sortedArrays.begin() + 2)).at(4), 30);
            ASSERT_EQ((*(sortedArrays.begin() + 2)).at(5), 40);
        }

        TEST(SortingTransformerTest, intersectionTransformerTest)
        {
            types::Container arrays = getTestArrays();

            IntersectionTransformer<types::Container> intersectionTransformer;
            types::Container intersectionArrays = intersectionTransformer.transform(arrays);

            ASSERT_EQ(intersectionArrays.size(), 2);

            ASSERT_EQ((*(intersectionArrays.begin() + 0)).size(), 2);
            ASSERT_EQ((*(intersectionArrays.begin() + 1)).size(), 1);

            ASSERT_EQ((*(intersectionArrays.begin() + 0)).at(0), 0);
            ASSERT_EQ((*(intersectionArrays.begin() + 0)).at(1), 30);

            ASSERT_EQ((*(intersectionArrays.begin() + 1)).at(0), 0);
        }

        TEST(SortingTransformerTest, uniqueReverseTransformerTest)
        {
            types::Container arrays = getTestArrays();

            UniqueReverseTransformer<types::Container> uniqueReverseTransformer;
            types::Container uniqueReverseArrays = uniqueReverseTransformer.transform(arrays);

            ASSERT_EQ((*uniqueReverseArrays.begin()).at(0), 40);
            ASSERT_EQ((*uniqueReverseArrays.begin()).at(1), 30);
            ASSERT_EQ((*uniqueReverseArrays.begin()).at(2), 20);
            ASSERT_EQ((*uniqueReverseArrays.begin()).at(3), 13);
            ASSERT_EQ((*uniqueReverseArrays.begin()).at(4), 11);
            ASSERT_EQ((*uniqueReverseArrays.begin()).at(5), 10);
            ASSERT_EQ((*uniqueReverseArrays.begin()).at(6), 9);
            ASSERT_EQ((*uniqueReverseArrays.begin()).at(7), 3);
            ASSERT_EQ((*uniqueReverseArrays.begin()).at(8), 2);
            ASSERT_EQ((*uniqueReverseArrays.begin()).at(9), 1);
            ASSERT_EQ((*uniqueReverseArrays.begin()).at(10), 0);
            ASSERT_EQ((*uniqueReverseArrays.begin()).at(11), -7);
            ASSERT_EQ((*uniqueReverseArrays.begin()).at(12), -10);
        }
    }
}
