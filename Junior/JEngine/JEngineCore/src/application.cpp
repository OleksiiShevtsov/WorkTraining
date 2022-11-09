#include "jengineCore/application.h"
#include "window.hpp"

jengine::JApplication::JApplication()
{
    JLOG_INFO("start JApplication");
}

jengine::JApplication::~JApplication()
{
    JLOG_INFO("stop JApplication");
}

int jengine::JApplication::engage( unsigned int width, unsigned int hight, const char* title )
{
    mJWindow = std::make_unique< JWindow >( width, hight, title );

    mEventDispatcher.addEventListener<JEventWindowResize>(
        [](JEventWindowResize event)
        {
            JLOG_INFO("[WindowResize] Changed size to {0}x{1}", event.width, event.height);
        }
    );

    mEventDispatcher.addEventListener<JEventWindowClose>(
        [&](JEventWindowClose event)
        {
            JLOG_INFO("[WindowClose] Window close");
            mJWindow->closeWindow();
        }
    );

    mEventDispatcher.addEventListener<JEventMouseMoved>(
        [&](JEventMouseMoved event)
        {
            // JLOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
        }
    );

    mJWindow->setEventCallBack(
        [&](JBaseEvent& event)
        {
            mEventDispatcher.dispatch(event);
        }
    );

    while(mJWindow->isWindowActive())
    {
         mJWindow->onUpdate();
         onUpdate();
    }

    // TODO
    mJWindow = nullptr;

    return 0;
}
