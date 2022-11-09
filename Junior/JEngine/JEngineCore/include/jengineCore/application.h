#pragma once

#include "jengineCore/log.h"
#include "jengineCore/event.h"

#include <memory>

namespace jengine {

    class JApplication{
    public:
        JApplication();
        virtual ~JApplication();

        JApplication(const JApplication&) = delete;
        JApplication(JApplication&&) = delete;
        JApplication& operator=(const JApplication&) = delete;
        JApplication& operator=(JApplication&&) = delete;

        virtual int engage( unsigned int windowWidth, unsigned int windowHight, const char* title );
        virtual void onUpdate(){}

    private:

        JEventDispatcher mEventDispatcher;
        std::unique_ptr< class JWindow > mJWindow;
    };
}
