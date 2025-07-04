
#ifndef DIALOG_TR_H
#define DIALOG_TR_H

#include <QWidget>
#include <QDialog>
#include <QLabel>

class dialog_tr : public QDialog
{
    Q_OBJECT
public:
    dialog_tr(QWidget *parent = nullptr);
    ~dialog_tr();

    void switch_languages();

signals:

protected:
private:
    QLabel *_lab_languages = nullptr;
    QLabel *_lab_help = nullptr;
};
#endif // DIALOG_TR_H
