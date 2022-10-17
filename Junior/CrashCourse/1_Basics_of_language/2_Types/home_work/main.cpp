#include <cstdio>

enum class Operation
{
    ADD,
    SUBTRACT,
    MULTYPLAY,
    DIVIDE
};

struct Calculator{
    Calculator(Operation operation):
        m_operation{operation}
    {
    }

    int calc(int a, int b){
        switch (m_operation) {
            case Operation::ADD: return a + b; break;
            case Operation::SUBTRACT: return a - b; break;
            case Operation::MULTYPLAY: return a * b; break;
            case Operation::DIVIDE: return a / b; break;
        }
    }

private:
    Operation m_operation;
};

int main()
{
    Calculator calculator(Operation::MULTYPLAY);

    printf("res: %d\n", calculator.calc(10, 10));

    return 0;
}
