
#include "load_tr.h"

load_tr::load_tr(QObject *parent) : QObject(parent)
{
}

load_tr::~load_tr()
{
}

void load_tr::set_prefix(const QString &prefix)
{
    _prefix = prefix;
}

bool load_tr::is_load_tr(QApplication *app, QLocale::Language type)
{
    if (app == nullptr)
    {
        return false;
    }

    auto it = _mp_tr.find(type);
    if (it != _mp_tr.end())
    {
        return app->installTranslator(it->second);
    }
    else
    {
        QTranslator *trl = new QTranslator(this);
        // QString name = _prefix + QLocale(QLocale::languageToCode(type)).name();
        QString name = _prefix + QLocale::languageToString(type);
        if (trl->load(name))
        {
            return app->installTranslator(trl);
        }
    }
    return false;
}
