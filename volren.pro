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
           main_window.cpp \
           gl_render_widget.cpp \
           shader_object.cpp \
           program_object.cpp \
           ray_casting_engine.cpp

HEADERS  += main_window.h \
            gl_render_widget.h \
            shader_object.h \
            program_object.h \
            irenderer.h \
            ray_casting_engine.h

RESOURCES += resources.qrc

INCLUDEPATH += D:/libQGLViewer-2.6.3/libQGLViewer-2.6.3
LIBS += -LD:/libQGLViewer-2.6.3/libQGLViewer-2.6.3/QGLViewer -lQGLViewer2
