#include <QApplication>
#include"widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //création de la fenetre principale
    Widget w;
    w.show();

    return a.exec();
}
