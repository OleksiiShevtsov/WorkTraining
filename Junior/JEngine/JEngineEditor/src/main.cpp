#include <memory>

#include "jengineCore/application.h"
#include "glm/mat3x3.hpp"

#include <iostream>

class EditorJApp : public jengine::JApplication{

    virtual void onUpdate() override {
        //std::cout << "update frame: " << m_frame++ << std::endl;
    }
};

int main(){

    auto editorJApp = std::make_unique< EditorJApp >();

    int returnCode = editorJApp->engage(1024, 786, "JEngine Editor");

    return returnCode;
}
