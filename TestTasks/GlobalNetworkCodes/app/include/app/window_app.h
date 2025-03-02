#pragma once

#include "app/operator_model_tree.h"

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <memory>

namespace gnc
{
    class WindowApp : public QWidget
    {
    public:
        WindowApp();
    private:
        std::unique_ptr<OperatorModelTree> m_operatorModelTree;
        std::unique_ptr<QVBoxLayout> m_layout;
    };
};
