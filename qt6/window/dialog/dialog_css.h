
#ifndef DIALOG_CSS_H
#define DIALOG_CSS_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>
#include <QVBoxLayout>

class dialog_css : public QDialog
{
    Q_OBJECT
public:
    dialog_css(QWidget *parent = nullptr);
    ~dialog_css();

signals:

protected:

private:
    QListWidget *_ls_css = nullptr;
    QPushButton *_but_load = nullptr;
    QPushButton *_but_ok = nullptr;
};
#endif // DIALOG_CSS_H
