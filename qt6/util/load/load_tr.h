
#ifndef LOAD_TR_H
#define LOAD_TR_H

#include <QObject>
#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <map>

#include "TranslatorNon.h"
#include "Tsin.h"

class load_tr : public QObject
{
    Q_OBJECT
public:
    load_tr(QObject *parent = nullptr);
    ~load_tr();

    void set_prefix(const QString &prefix);

    bool is_load_tr(QApplication *app = nullptr, QLocale::Language type = QLocale::Chinese);

    static load_tr *sin();

signals:

protected:
private:
    QString _prefix = ":/i18n/app_";
    std::map<QLocale::Language, QTranslator *> _mp_tr;
};

#endif // LOAD_TR_H
