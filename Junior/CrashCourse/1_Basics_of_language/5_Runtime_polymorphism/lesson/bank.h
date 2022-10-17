#include <cstdio>
#include <stdexcept>

struct ILogger{
    virtual ~ILogger() = default;
    virtual void logTransfer( long from, long to, double amount ) = 0;
};

struct ConsoleLogger : ILogger{
    void logTransfer( long from, long to, double amount ) override {
        printf("[cons] %ld -> %ld: %f\n", from, to, amount);
    }
};

struct FileLogger : ILogger{
    void logTransfer( long from, long to, double amount ) override {
        printf("[file] %ld, %ld, %f\n", from, to, amount);
    }
};

struct Bank{

    Bank( ILogger* logger )
        : m_logger{ logger }{}

    void setLogger( ILogger* logger ){
        m_logger = logger;
    }

    void makeTransfer( long from, long to, double amount ){
        if( m_logger ){ m_logger->logTransfer( from, to, amount ); }
    }

private:
    ILogger *m_logger{};
};
