#include <cstdio>
#include <cstddef>
#include <iostream>
#include <cwchar> // for wprintf

struct JBall{
    int getX(){return m_x;}
    int getY(){return m_y;}
    void setX(int x){m_x = x;}
    void setY(int y){m_y = y;}
private:
    int m_x;
    int m_y;
};

int main()
{
    uint32_t i{};
    uint32_t *i_ptr{ &i };
    printf("i: %d, i_ptr: %p;\n", i, i_ptr);
    *i_ptr = 100;
    printf("*i_ptr: %d, i_ptr: %p;\n", *i_ptr, i_ptr);

    JBall jball;
    JBall *jball_ptr{ &jball };
    jball_ptr->setX(10);
    jball_ptr->setY(20);

    printf("jball x: %d, ", jball_ptr->getX());
    printf("jball y: %d\n", jball_ptr->getY());

    return 0;
}
