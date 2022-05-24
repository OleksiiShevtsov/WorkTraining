#include <stdexcept>

struct SimpleString{
    SimpleString( size_t maxSize )
        : m_maxSize{ maxSize },
          m_length{ 0 }
    {
        printf("CNTRUCT\n");
        if ( m_maxSize == 0 ){
            throw std::runtime_error{ "Max size must be at least 1." };
        }
        m_buffer = new char[m_maxSize];
        m_buffer[0] = 0;
    }

    SimpleString( const SimpleString& other )
        : m_maxSize{ other.m_maxSize },
          m_buffer{ new char[other.m_maxSize] },
          m_length{ other.m_length }{
        printf("CNTRUCT COPY\n");
        strncpy(m_buffer, other.m_buffer, m_maxSize );
    }

    SimpleString( SimpleString&& other ) noexcept
        : m_maxSize{ other.m_maxSize },
          m_buffer{ other.m_buffer },
          m_length{ other.m_length }{
        printf("CNTRUCT MOVE\n");
        other.m_length = 0;
        other.m_buffer = nullptr;
        other.m_maxSize = 0;
    }

    ~SimpleString()
    {
        delete[] m_buffer;
    }

    SimpleString& operator=( const SimpleString& other )
    {
        if(this == &other){ return *this; }
        const auto newBuffer = new char[other.m_maxSize];
        delete[] m_buffer;
        m_buffer = newBuffer;
        m_maxSize = other.m_maxSize;
        m_length = other.m_length;
        strncpy(m_buffer, other.m_buffer, m_maxSize );

        return *this;
    }

    SimpleString& operator=( SimpleString&& other ) noexcept
    {
        if(this == &other){ return *this; }
        delete[] m_buffer;
        m_buffer = other.m_buffer;
        m_maxSize = other.m_maxSize;
        m_length = other.m_length;

        other.m_length = 0;
        other.m_buffer = nullptr;
        other.m_maxSize = 0;

        return *this;
    }

    void print() const
    {
        printf("%s\n", m_buffer);
    }

    bool appendLine( const char* newString )
    {
        const size_t newStringLength = strlen(newString);
        if( newStringLength + m_length + 2 > m_maxSize ){ return false; }
        strncpy(m_buffer + m_length, newString, m_maxSize - m_length );

        m_length += newStringLength;
        m_buffer[m_length++] = '\n';
        m_buffer[m_length] = 0;

        return true;
    }

private:
    size_t m_maxSize;
    char* m_buffer;
    size_t m_length;
};
