#pragma once

struct IAccount{

    virtual ~IAccount() = default;
    virtual int getAmount() const = 0;

protected:
    int m_amount;
    long m_id;
};

struct CheckingAccaunt : IAccount{

    CheckingAccaunt( long id ){
        m_id = id;
        m_amount = 1000;
    }

    int getAmount() const {
        return m_amount;
    }

    void transferToAccount( int amount ) {
        m_amount = m_amount - amount;
    }

    void toDeposit( int amount ) {
        m_amount = m_amount + amount;
    }
};

struct SavingsDatabase : IAccount{

    SavingsDatabase( long id ){
        m_id = id;
        m_amount = 1000;
    }

    int getAmount() const {
        return m_amount;
    }
};
