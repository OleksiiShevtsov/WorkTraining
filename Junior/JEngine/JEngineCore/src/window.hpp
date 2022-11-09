#pragma once

#include "jengineCore/gui.h"
#include "jengineCore/log.h"
#include "jengineCore/settings.h"
#include "rendering/OpenGL/index_buffer.hpp"
#include "rendering/OpenGL/shader_program.hpp"
#include "rendering/OpenGL/vertex_array.hpp"

#include <GLFW/glfw3.h>

namespace jengine 
{
    class JWindow
    {
    public:
        JWindow(unsigned int windowWidth, unsigned int windowHight, const char* title);
        virtual ~JWindow();

        JWindow(const JWindow&) = delete;
        JWindow(JWindow&&) = delete;
        JWindow& operator=(const JWindow&) = delete;
        JWindow& operator=(JWindow&&) = delete;

        void onUpdate();
        bool isWindowActive();
        void closeWindow();

        unsigned int getWidth() const { return mData.sizeWindow.width; }
        unsigned int getHeight() const { return mData.sizeWindow.height; }

        void setEventCallBack(const types::JEventCallBackFn& callBack);

    private:

        int init();
        void shutdown();

        static unsigned int sNumberWindowsCreated;

        types::JWindowData mData;
        types::JColor mBackgroundColor{ 0.2f, 0.2f, 0.2f, 0.0f };
        std::unique_ptr<JGui> mGui;
        std::unique_ptr<JShaderProgram> mShaderProgram;
        std::unique_ptr<JVertexBuffer> mVertexBufferPositionsColors;
        std::unique_ptr<JIndexBuffer> mIndexBuffer;
        std::unique_ptr<JVertexArray> mVertexArray1Buffer;
        GLFWwindow* mWindow{ nullptr };

        bool mCloseWindow{ false };
    };
}
