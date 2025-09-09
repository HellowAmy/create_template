
#include "TranslatorNon.h"

TranslatorNon::TranslatorNon(QObject *parent) : QTranslator(parent)
{
}

TranslatorNon::~TranslatorNon()
{
}

QString TranslatorNon::translate(const char *context, const char *sourceText, const char *disambiguation, int n) const
{
    QString tr = QTranslator::translate(context, sourceText, disambiguation, n);
    if (tr.isEmpty())
    {
        return QString(sourceText);
    }
    return tr;
}
