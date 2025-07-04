#include <QApplication>
#include <QWidget>
#include <QTranslator>
#include <QDebug>
#include <QFile>

#include "main_window.h"
#include "Tlog.h"

#include "load_tr.h"
#include "load_css.h"

using namespace std;
using namespace bhtools;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    {
        vlogd("Run QApplication Version: " << qVersion());
    
        bool is_css = load_css::is_load_css(&a);
        vlogd("Load css result: " << $(is_css));
    
        bool is_tr = Tsin_load_tr()()->is_load_tr(&a, QLocale::English);
        vlogd("Load tr result: " << $(is_tr));
    }

    main_window w;
    w.show();

    return a.exec();
}