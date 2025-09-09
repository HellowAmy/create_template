
#ifndef LOAD_CSS_H
#define LOAD_CSS_H

#include <QObject>
#include <QApplication>
#include <QFile>
#include <QFileInfo>

#include "Tsin.h"

class load_css : public QObject
{
    Q_OBJECT
public:
    load_css(QObject *parent = nullptr);
    ~load_css();

    static bool is_load_css(QApplication *app = nullptr, QString file = ":/file/css/common.css");

    static QString load_css_ctx(QString file);

    static load_css *sin();

signals:

protected:
private:
};
#endif // LOAD_CSS_H
