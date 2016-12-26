#include "widgetareaselect.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WidgetAreaSelect w;
    w.show();

    return a.exec();
}
