#pragma once

#include <stdexcept>
#include <cstdio>

struct ILogger{
    virtual ~ILogger() = default;
    virtual void logTransfer( long from, long to, double amount ) = 0;
};

struct ConsoleLogger : ILogger{
    ConsoleLogger( const char *data )
        : m_data{ data }{}

    void logTransfer( long from, long to, double amount ) override {
        printf("%s %ld -> %ld: %f\n", m_data, from, to, amount);
    }

private:
    const char *m_data;
};

struct FileLogger : ILogger{
    void logTransfer( long from, long to, double amount ) override {
        printf("[file] %ld, %ld, %f\n", from, to, amount);
    }
};
