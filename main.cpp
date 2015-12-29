#include <QApplication>

#include "gui/main_window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("KuboSVK");
    a.setApplicationName("GPU Based Single Pass Ray Casting Engine, 1.0");

    gui::MainWindow w;
    w.show();

    return a.exec();
}
