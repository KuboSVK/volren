#include <QApplication>

#include "main_window.h"
#include "gl_render_widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("KuboSVK");
    a.setApplicationName("volren");

    gui::MainWindow w;
    w.show();

    return a.exec();
}
