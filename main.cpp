#include "widgetareaselect.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WidgetAreaSelect w;
    w.show();
    w.setActualSize(1000,1000);

    return a.exec();
}
