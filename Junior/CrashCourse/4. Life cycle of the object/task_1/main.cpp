#include <cstdio>
#include <cstddef>
#include <iostream>
#include <typeinfo>
#include <cwchar> // for wprintf

struct JBall{
    JBall( int x, int y, int r ):
        m_x{ x }, m_y{ y }, m_r{ r }{ countJball++; }
    ~JBall(){ countJball--; };
    int getX() const { return m_x; }
    int getY() const { return m_y; }
    void setX( int x ) { m_x = x; }
    void setY( int y ) { m_y = y; }
    void print(){ printf("x: %d, y: %d\n", m_x, m_y); }
    static void printCountJball(){ printf("countJball: %d\n", countJball); }

    static int countJball;
private:
    int m_x;
    int m_y;
    const int m_r;
};

int JBall::countJball{ 0 };

void localVariableThread (){
    // функция создает в потоке уникальную статическую переменную
    static thread_local int localVariableInThread = 1;
}

int main()
{
    int* newIVariable = new int{ 100 };

    delete newIVariable;

    return 0;
}
