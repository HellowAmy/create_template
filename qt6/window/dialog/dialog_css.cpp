
#include "dialog_css.h"

#include "load_tr.h"
#include "load_css.h"
#include "bhtools.h"

dialog_css::dialog_css(QWidget *parent) : QDialog(parent)
{
    _ls_css = new QListWidget(this);

    _but_load = new QPushButton(this);
    _but_load->setText("加载");

    _but_ok = new QPushButton(this);
    _but_ok->setText("确定");


    connect(_but_load,&QPushButton::clicked,[=](){
        QString path = QFileDialog::getExistingDirectory(this,"CSS Path","../file/css");
        if(path.isEmpty() == false)
        {
            QDir dir(path);
            QFileInfoList ls = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);

            QStringList ls_abs;
            for(auto &a:ls)
            {
                ls_abs.push_back(a.absoluteFilePath());
            }
            _ls_css->clear();
            _ls_css->addItems(ls_abs);
        }
    });

    connect(_but_ok,&QPushButton::clicked,[=](){
        QListWidgetItem *it = _ls_css->currentItem();
        if(it)
        {
            QFileInfo info(it->text());
            QString path = info.absoluteFilePath();
            bool ok = load_css::sin()->is_load_css(qApp,path);
            vlogd($(ok) $(path.toStdString()));
        }
    });

    QVBoxLayout *laym = new QVBoxLayout(this);
    {
        QHBoxLayout *lay = new QHBoxLayout;
        lay->addWidget(_ls_css);
        laym->addLayout(lay);
    }
    {
        QHBoxLayout *lay = new QHBoxLayout;
        lay->addWidget(_but_load);
        lay->addWidget(_but_ok);
        laym->addLayout(lay);
    }
}

dialog_css::~dialog_css()
{
}