#include "jengineCore/gui.h"

jengine::JGui::JGui(GLFWwindow* window, types::JSizeWindow sizeWindow) :
    mWindow{ window },
    mSizeWindow{ sizeWindow }
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
}

jengine::JGui::~JGui() {}

void jengine::JGui::onUpdate()
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = static_cast<float>(mSizeWindow.width);
    io.DisplaySize.y = static_cast<float>(mSizeWindow.height);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();

    for (auto& element : mUiElement) {
        element();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void jengine::JGui::updateWindowSize(types::JSizeWindow sizeWindow)
{
    mSizeWindow = sizeWindow;
}

void jengine::JGui::colorEdit4(const char* name, float* color)
{
    mUiElement.push_back([name, color]()
        {
            ImGui::Begin(name);
            ImGui::ColorEdit4(name, color);
            ImGui::End();
        }
    );
}

void jengine::JGui::checkBox(const char* name, bool* v)
{
    mUiElement.push_back([name, v]()
        {
            ImGui::Checkbox(name, v);
        }
    );
}
