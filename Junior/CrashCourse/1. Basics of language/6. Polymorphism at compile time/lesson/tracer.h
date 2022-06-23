#pragma once

struct Tracer{

    Tracer( const char* name )
        : m_name{ name }{
        printf("%s constructor.\n", m_name);
    }

    ~Tracer(){
        printf("%s destructor.\n", m_name);
    }

private:
    const char* const m_name;
};
