#include <cstdio>
#include <cstdint>
#include <cstdarg>

namespace fco {
    struct CountIf {
        CountIf(char c) : m_c{ c } {}
        size_t operator()(const char* str) const {
            size_t index{ 0 }, result{ 0 };
            while (str[index]) {
                if (str[index] == m_c) { result++; }
                index++;
            }
            return result;
        }

    private:
        const char m_c;
    };
} // namespace fco

void functionCallOperatorCheck(){
    fco::CountIf countIf{ 'a' };

    printf( "number of letters in a word \"alex\": %zu\n", countIf( "alex" ) );
    printf( "number of letters in a word \"amazon\": %zu\n", countIf( "amazon" ) );
}
