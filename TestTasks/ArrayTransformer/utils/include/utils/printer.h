#pragma once

#include <cstdio>

namespace at
{
    class Printer
    {
    public:
        template <typename Container>
        static void print(const Container& container, const char* label = "")
        {
            if (*label)
            {
                printf("%s: ", label);
            }

            for (const auto& row : container)
            {
                putchar('\n');

                for (const auto& column : row)
                {
                    printf("%d ", column);
                }
            }

            printf("\n\n");
        }
    };
}