
#include "load_css.h"

load_css::load_css(QObject *parent) : QObject(parent)
{
}

load_css::~load_css()
{
}

bool load_css::is_load_css(QApplication *app, QString file)
{
    if (app == nullptr)
    {
        return false;
    }

    QString ctx = load_css_ctx(file);
    if (ctx.isEmpty())
    {
        return false;
    }
    app->setStyleSheet(ctx);
    return true;
}

QString load_css::load_css_ctx(QString file)
{
    QString ctx;
    QFile f(file);
    if (f.open(QIODevice::ReadOnly))
    {
        ctx = f.readAll();
        f.close();
    }
    return ctx;
}
