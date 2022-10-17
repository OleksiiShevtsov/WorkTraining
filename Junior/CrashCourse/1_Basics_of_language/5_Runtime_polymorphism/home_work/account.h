#pragma once

struct IAccountDatabase{

    virtual ~IAccountDatabase() = default;
    virtual void setAmount( int amount ) = 0;
    virtual int getAmount() const = 0;

protected:
    int m_amount;
    long m_id;
};

struct InMemoryAccauntDatabase : IAccountDatabase{

    InMemoryAccauntDatabase( long id ){
        m_id = id;
        m_amount = 1000;
    }

    void setAmount( int amount ) {
        m_amount = amount;
    }

    int getAmount() const {
        return m_amount;
    }
};
