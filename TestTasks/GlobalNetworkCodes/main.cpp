#include "app/window_app.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    gnc::WindowApp window;
    window.show();

    return app.exec();
}
