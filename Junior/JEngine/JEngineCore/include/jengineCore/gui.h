#pragma once

#include "jengineCore/types.h"

#include <vector>
#include <functional>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace jengine
{
    class JGui
    {
    public:
        JGui(GLFWwindow* window, types::JSizeWindow sizeWindow);
        virtual ~JGui();

        JGui(const JGui&) = delete;
        JGui(JGui&&) = delete;
        JGui& operator=(const JGui&) = delete;
        JGui& operator=(JGui&&) = delete;

        void onUpdate();
        void updateWindowSize(types::JSizeWindow sizeWindow);

        void colorEdit4(const char* name, float* color);
        void checkBox(const char* name, bool* v);

    private:

        std::vector<std::function<void()>> mUiElement;
        GLFWwindow* mWindow{ nullptr };
        types::JSizeWindow mSizeWindow;
    };
}
