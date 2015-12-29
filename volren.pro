#-------------------------------------------------
#
# Project created by QtCreator 2015-12-08T22:10:18
#
#-------------------------------------------------

QT       += core gui opengl xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = volren
TEMPLATE = app


SOURCES += main.cpp \
           gui/main_window.cpp \
           gui/gl_render_widget.cpp \
           core/shader_object.cpp \
           core/program_object.cpp \
           core/ray_casting_engine.cpp \
           gui/settings_widget.cpp

HEADERS  += gui/main_window.h \
            gui/gl_render_widget.h \
            core/shader_object.h \
            core/program_object.h \
            core/irenderer.h \
            core/ray_casting_engine.h \
            gui/settings_widget.h

RESOURCES += resources.qrc

INCLUDEPATH += D:/libQGLViewer-2.6.3/libQGLViewer-2.6.3
LIBS += -LD:/libQGLViewer-2.6.3/libQGLViewer-2.6.3/QGLViewer -lQGLViewer2
