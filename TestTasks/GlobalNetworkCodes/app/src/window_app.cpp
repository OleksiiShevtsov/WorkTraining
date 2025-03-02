#include "app/window_app.h"

#include <QListWidget>

gnc::WindowApp::WindowApp()
{
    setWindowTitle("Global Network Codes");
    resize(640, 480);

    m_operatorModelTree = std::make_unique<OperatorModelTree>(this);
    m_layout = std::make_unique<QVBoxLayout>(this);

    m_operatorModelTree->loadData();
    m_operatorModelTree->showTree();

    m_layout->addWidget(m_operatorModelTree.get());
    setLayout(m_layout.get());
}
