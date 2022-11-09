#pragma once

#include <array>
#include <functional>

namespace jengine 
{
    enum class JEventType 
    {
        WindowResize = 0,
        WindowClose,

        KeyPressed,
        KeyReleased,

        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,

        EventCount
    };

    struct JBaseEvent 
    {
        virtual ~JBaseEvent() = default;
        virtual JEventType getType() const = 0;
    };

    class JEventDispatcher 
    {
    public:
        template<typename EvType>
        void addEventListener(std::function<void(EvType&)> callback)
        {
            auto baseCallback = [func = std::move(callback)](JBaseEvent& event)
            {
                if (event.getType() == EvType::type)
                {    
                    func(static_cast<EvType&>(event));
                }
            };
            mEventCallbacks[static_cast<size_t>(EvType::type)] = std::move(baseCallback);
        }

        void dispatch(JBaseEvent& event) 
        {
            auto& callback = mEventCallbacks[static_cast<size_t>(event.getType())];
            if (callback) {
                callback(event);
            }
        }

    private:
        std::array<std::function<void(JBaseEvent&)>, static_cast<size_t>(JEventType::EventCount)> mEventCallbacks;
    };

    struct JEventWindowResize : public JBaseEvent 
    {
        JEventWindowResize(const unsigned int width, const unsigned int height) :
            width{ width },
            height{ height }{}

        virtual JEventType getType() const override 
        {
            return type;
        }

        unsigned int width;
        unsigned int height;
        static const JEventType type = JEventType::WindowResize;
    };

    struct JEventWindowClose : public JBaseEvent
    {
        virtual JEventType getType() const override
        {
            return type;
        }

        static const JEventType type = JEventType::WindowClose;
    };
    
    struct JEventMouseMoved : public JBaseEvent 
    {
        JEventMouseMoved(const double x, const double y) :
            x{ x },
            y{ y } {}

        virtual JEventType getType() const override 
        {
            return type;
        }

        double x;
        double y;
        static const JEventType type = JEventType::MouseMoved;
    };
}
