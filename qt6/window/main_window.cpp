
#include "main_window.h"
#include "dialog_tr.h"
#include "dialog_css.h"

#include <QPushButton>
#include <QHBoxLayout>

main_window::main_window(QWidget *parent) : QWidget(parent)
{
    {
        this->resize(400, 400);
    
        QVBoxLayout *laym = new QVBoxLayout(this);
        laym->setSpacing(10);
        laym->setContentsMargins(20, 20, 20, 20);
        {
            QPushButton *but = new QPushButton(this);
            but->setText(tr("TR"));
            laym->addWidget(but);
    
            connect(but, &QPushButton::clicked, this, [=]() {
                dialog_tr *dia = new dialog_tr(this);
                dia->resize(400,400);
                dia->show(); 
            });
        }
        {
            QPushButton *but = new QPushButton(this);
            but->setText(tr("CSS"));
            laym->addWidget(but);

            connect(but, &QPushButton::clicked, this, [=]() {
                dialog_css *dia = new dialog_css(this);
                dia->resize(400,400);
                dia->show(); 
            });
        }
    }
}

main_window::~main_window()
{
}