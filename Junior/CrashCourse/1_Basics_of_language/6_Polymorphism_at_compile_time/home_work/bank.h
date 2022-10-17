#pragma once

#include "account.h"

template< typename T >
struct Bank{

    Bank( T* account )
        : m_account{ account }{
    }

    ~Bank(){
        delete m_account;
    }

    T* performAnOperation(){
        return m_account;
    }

private:
    T *m_account;
};
