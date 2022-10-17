#pragma once

#include "logger.h"
#include "account.h"

struct Bank{

    Bank( ILogger* logger )
        : m_logger{ logger }
        , m_accountDatabase{ new InMemoryAccauntDatabase{ 1 } } {
    }

    ~Bank(){
        delete m_accountDatabase;
    }

    void setLogger( ILogger* logger ){
        m_logger = logger;
    }

    void makeTransfer( long from, long to ){
        if( m_logger ){ m_logger->logTransfer( from, to, m_accountDatabase->getAmount() ); }
    }

private:
    ILogger *m_logger;
    IAccountDatabase *m_accountDatabase;
};
