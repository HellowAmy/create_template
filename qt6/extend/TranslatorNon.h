
#ifndef TRANSLATORNON_H
#define TRANSLATORNON_H

#include <QTranslator>

class TranslatorNon : public QTranslator
{
    Q_OBJECT
public:
    TranslatorNon(QObject *parent = nullptr);
    ~TranslatorNon();

    QString translate(const char *context, const char *sourceText, const char *disambiguation = nullptr, int n = -1) const override;

signals:

protected:
private:
};
#endif // TRANSLATORNON_H
