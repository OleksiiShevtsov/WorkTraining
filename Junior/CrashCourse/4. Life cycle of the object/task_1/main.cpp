#include <cstdio>
#include <cstddef>
#include <iostream>
#include <typeinfo>
#include <cwchar> // for wprintf

struct JBall{
    JBall( int x, int y, int r ):
        m_x{ x }, m_y{ y }, m_r{ r }{
        if(m_r < 0){
            throw std::runtime_error{"Radius can not be less than 0"};
        }
        countJball++; }
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

//////////////////////////////////////////////////////////////////////

struct Human{
    Human( const char* name ):
        m_name{ name }{ printf( "%s constructed;\n", m_name ); }
    ~Human(){ printf( "%s destructed;\n", m_name ); }
private:
    const char* m_name;
};

static Human humanStatic{"Static"};
thread_local Human humanThread{"Thread"};

/////////////////////////////////////////////////////////////////////

int main()
{
    int* newIntVariable = new int{ 100 };
    int* newArray = new int[2]{ 100, 2000 };

    delete newIntVariable;
    delete[] newArray;

    //  Life cycle of the object  ////////////////////////////////////

    Human humanMain("Main");
    Human* humanDynamic = new Human("Dynamic");
    delete humanDynamic;

    //  exception  ///////////////////////////////////////////////////

    try {
        JBall jBall(10, 10, -10);
    }  catch (const std::runtime_error& e) {
        printf("message: %s\n", e.what());
    }



    return 0;
}
