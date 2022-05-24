#include "simple_string.h"

struct SimpleStringOwner{
    SimpleStringOwner( const char* newString )
        :m_string{ 100 }
    {
        if( !m_string.appendLine(newString) ){
            throw std::runtime_error{ "Not enough memory!" };
        }
        m_string.print();
    }

    SimpleStringOwner( SimpleString& string )
        :m_string{ std::move( string ) }
    {

    }

    ~SimpleStringOwner(){
        //m_string.print();
    }

private:
    SimpleString m_string;
};
