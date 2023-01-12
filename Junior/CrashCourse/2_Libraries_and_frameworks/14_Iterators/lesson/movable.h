#pragma once

struct Movable
{
    Movable( int id ) : mId{ id }{}
    Movable( Movable&& m )
    {
        mId = m.mId;
        m.mId = -1;
    }
    int mId;
};
