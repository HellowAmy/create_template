
#include "dialog_tr.h"
#include "load_tr.h"
#include "Tlog.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QTranslator>
#include <QApplication>

using namespace bhtools;

dialog_tr::dialog_tr(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *laym = new QVBoxLayout(this);

    _lab_help = new QLabel(this);
    _lab_help->setAlignment(Qt::AlignCenter);
    laym->addWidget(_lab_languages);

    _lab_languages = new QLabel(this);
    _lab_languages->setAlignment(Qt::AlignCenter);
    laym->addWidget(_lab_languages);

    {
        QVBoxLayout *lay = new QVBoxLayout;
        laym->addLayout(lay);
        {

            QPushButton *but = new QPushButton(this);
            but->setText("中文");
            lay->addWidget(but);

            connect(but, &QPushButton::clicked, [=]() {
                bool is_tr = Tsin_load_tr()()->is_load_tr(qApp,QLocale::Chinese);
                vlogd("Load tr result: " << $(is_tr));

                switch_languages(); 
            });
        }
        {
            QPushButton *but = new QPushButton(this);
            but->setText("English");
            lay->addWidget(but);

            connect(but, &QPushButton::clicked, [=]() {
                bool is_tr = Tsin_load_tr()()->is_load_tr(qApp,QLocale::English);
                vlogd("Load tr result: " << $(is_tr));
    
                switch_languages(); 
            });
        }
        {
            QPushButton *but = new QPushButton(this);
            but->setText("Japan");
            lay->addWidget(but);

            connect(but, &QPushButton::clicked, [=]() {
                bool is_tr = Tsin_load_tr()()->is_load_tr(qApp,QLocale::Japanese);
                vlogd("Load tr result: " << $(is_tr));

                switch_languages(); 
            });
        }
    }

    switch_languages();
}

dialog_tr::~dialog_tr()
{
}

void dialog_tr::switch_languages()
{
    _lab_help->setText(tr("Help"));
    _lab_languages->setText(tr("Switch languages"));
}
