#include "maingui.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainGUI w;
    w.show();
    return a.exec();
}
