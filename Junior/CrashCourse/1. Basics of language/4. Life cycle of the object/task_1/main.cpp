#include <cstdio>
#include <cstddef>
#include <iostream>
#include <typeinfo>
#include <cwchar> // for wprintf

#include "simple_string_owner.h"
#include "jball.h"

void localVariableThread (){
    // функция создает в потоке уникальную статическую переменную
    static thread_local int localVariableInThread = 1;
}

////////////////////////////////////////////////////////////////////

struct Human{

    //using the default copy constructor
    //Human(const Human& other) = default;

    //copy constructor not allowed
    //Human(const Human& other) = delete;

    Human( const char* name ):
        m_name{ name }{ printf( "%s constructed;\n", m_name ); }
    ~Human(){ printf( "%s destructed;\n", m_name ); }
private:
    const char* m_name;
};

static Human humanStatic{"Static"};
thread_local Human humanThread{"Thread"};

/////////////////////////////////////////////////////////////////////

void refType(int &val){
    printf( "lvalue reference %d\n", val );
}

void refType(int &&val){
    printf( "rvalue reference %d\n", val );
}

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
    }  catch (...) {
        printf("error%s\n");
    }

    //  SimpleString  /////////////////////////////////////////////////

    SimpleString str(14);
    str.appendLine("text1");
    str.appendLine("text2");
    str.print();

    //SimpleStringOwner strOwner("text3");

    SimpleString strCopy{ str };
    strCopy.print();

    SimpleString strAppropriation{ 10 };
    strAppropriation = strCopy;
    strAppropriation.print();

    SimpleStringOwner strMove( str );
    str.print();

    //////////////////////////////////////////////////////////////////////

    int value = 10;
    refType(value);
    // leads lvalue to rvalue
    refType(std::move(value));
    refType(20);

    return 0;
}
