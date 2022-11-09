#include "window.hpp"
#include "rendering/resources/resources.hpp"

jengine::JWindow::JWindow( unsigned int width, unsigned int hight, const char* title ) :
    mData{ { width, hight }, std::move(title) }
{
    // TODO hendl error
    int result = init();
}

jengine::JWindow::~JWindow()
{
    shutdown();
}

void jengine::JWindow::onUpdate()
{
    glClearColor(mBackgroundColor[0], mBackgroundColor[1], mBackgroundColor[2], mBackgroundColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    mShaderProgram->bind();
    mVertexArray1Buffer->bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mVertexArray1Buffer->getIndicesCount()), GL_UNSIGNED_INT, nullptr);

    mGui->onUpdate();

    glfwSwapBuffers(mWindow);
    glfwPollEvents();
}

bool jengine::JWindow::isWindowActive() 
{
    return !mCloseWindow;
}

void jengine::JWindow::closeWindow() 
{
    mCloseWindow = true;
}

void jengine::JWindow::setEventCallBack( const types::JEventCallBackFn& callBack )
{
    mData.eventCallbackFn = callBack;
}

unsigned int jengine::JWindow::sNumberWindowsCreated = 0;

int jengine::JWindow::init()
{
    JLOG_INFO("Creating window '{0}' width size {1}x{2}", mData.title, mData.sizeWindow.width, mData.sizeWindow.height);

    if(JWindow::sNumberWindowsCreated < settings::maxNumberWindows)
    {
        if (!glfwInit())
        {
            JLOG_CRITICAL("Failed to initialize GLFW");
            return -1;
        }
        JWindow::sNumberWindowsCreated++;
    }
    
    mWindow = glfwCreateWindow(mData.sizeWindow.width, mData.sizeWindow.height, mData.title.c_str(), nullptr, nullptr);

    if (!mWindow)
    {
        JLOG_CRITICAL("Failed create window '{}'", mData.title);
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(mWindow);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        JLOG_CRITICAL("Failed to initialize GLAD");
        return -3;
    }

    glfwSetWindowUserPointer(mWindow, &mData);
    glfwSetWindowSizeCallback(mWindow,
        [](GLFWwindow* pWindow, int width, int height)
        {
            types::JWindowData& data = *static_cast<types::JWindowData*>(glfwGetWindowUserPointer(pWindow));
            data.sizeWindow.width = width;
            data.sizeWindow.height = height;
            JEventWindowResize event( width, height );
            data.eventCallbackFn(event);
        }
    );

    glfwSetWindowCloseCallback(mWindow,
        [](GLFWwindow* pWindow)
        {
            types::JWindowData& data = *static_cast<types::JWindowData*>(glfwGetWindowUserPointer(pWindow));
            JEventWindowClose event;
            data.eventCallbackFn(event);
        }
    );

    glfwSetCursorPosCallback(mWindow,
        [](GLFWwindow* pWindow, double x, double y) 
        {
            types::JWindowData& data = *static_cast<types::JWindowData*>(glfwGetWindowUserPointer(pWindow));
            JEventMouseMoved event{ x, y };
            data.eventCallbackFn(event);
        }
    );

    glfwSetFramebufferSizeCallback(mWindow,
        [](GLFWwindow* pWindow, int width, int height)
        {
            glViewport(0, 0, width, height);
        }
    );

    mGui = std::make_unique<JGui>(mWindow, mData.sizeWindow);
    mGui->colorEdit4("Background color window", mBackgroundColor);

    mShaderProgram = std::make_unique<JShaderProgram>(resources::vertexShader, resources::fragmentShader);
    if(!mShaderProgram->isCompiled())
    {
        return 0;
    }

    JBufferLayout bufferLayout2Vec3
    {
        JShaderDataType::Float3,
        JShaderDataType::Float3
    };

    mVertexArray1Buffer = std::make_unique<JVertexArray>();
    mVertexBufferPositionsColors = std::make_unique<JVertexBuffer>(resources::positions_colors, sizeof(resources::positions_colors), bufferLayout2Vec3);
    mIndexBuffer = std::make_unique<JIndexBuffer>(resources::indices, sizeof(resources::indices) / sizeof(GLuint));
    
    mVertexArray1Buffer->addVertexBuffer(*mVertexBufferPositionsColors);
    mVertexArray1Buffer->setIndexBuffer(*mIndexBuffer);

    return 0;
}

void jengine::JWindow::shutdown()
{
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}
