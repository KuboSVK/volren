#include "gl_render_widget.h"

#include <QGLContext>
#include <QDebug>
#include <QGLFormat>
#include <QGLFunctions>

#include "irenderer.h"
#include "ray_casting_engine.h"

namespace gui
{

GLRenderWidget::GLRenderWidget(QWidget *parent)
: QGLViewer(parent)
{
    mRayCastingEngine = new core::RayCastingEngine();
}

GLRenderWidget::~GLRenderWidget()
{

}

void GLRenderWidget::resizeGL(int width, int height)
{
    qDebug() << "resizeGL";

    QGLViewer::resizeGL(width, height);

    mRayCastingEngine->resize(width, height);
}

void GLRenderWidget::initializeGL()
{
    qDebug() << "initializeGL";

    QGLViewer::initializeGL();

    mOpenGLFormat = new QGLFormat;
    mOpenGLFormat->setVersion(4, 3);
    mOpenGLFormat->setProfile(QGLFormat::CoreProfile);

    mOpenGLContext = new QGLContext(*mOpenGLFormat, this);
    mOpenGLContext->create();
    mOpenGLContext->makeCurrent();

    initializeOpenGLFunctions();
}

void GLRenderWidget::init()
{
    qDebug() << "init";

    QGLViewer::init();

    camera()->showEntireScene();

    // Restore previous viewer state.
    restoreStateFromFile();

    // Opens help window
    help();

    mRayCastingEngine->init();
}

void GLRenderWidget::draw()
{
    qDebug() << "draw";

    mRayCastingEngine->draw();
}

void GLRenderWidget::obtainOpenGLContextInfo(QMap<QString, QString>& openGLContextInfo)
{
    qDebug() << "obtainOpenGLContextInfo";

    const GLubyte *p;
    if ((p = glGetString(GL_VENDOR)))
        openGLContextInfo["vendor"] = QString::fromLatin1(reinterpret_cast<const char *>(p));
    if ((p = glGetString(GL_RENDERER)))
        openGLContextInfo["renderer"] = QString::fromLatin1(reinterpret_cast<const char *>(p));
    if ((p = glGetString(GL_VERSION)))
        openGLContextInfo["version"] = QString::fromLatin1(reinterpret_cast<const char *>(p));
    if ((p = glGetString(GL_SHADING_LANGUAGE_VERSION)))
        openGLContextInfo["glslVersion"] = QString::fromLatin1(reinterpret_cast<const char *>(p));}

QString GLRenderWidget::helpString() const
{
    QString text("<h2>S i m p l e V i e w e r</h2>");
    text += "Use the mouse to move the camera around the object. ";
    text += "You can respectively revolve around, zoom and translate with the three mouse buttons. ";
    text += "Left and middle buttons pressed together rotate around the camera view direction axis<br><br>";
    text += "Pressing <b>Alt</b> and one of the function keys (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
    text += "Simply press the function key again to restore it. Several keyFrames define a ";
    text += "camera path. Paths are saved when you quit the application and restored at next start.<br><br>";
    text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
    text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save a snapshot. ";
    text += "See the <b>Keyboard</b> tab in this window for a complete shortcut list.<br><br>";
    text += "Double clicks automates single click actions: A left button double click aligns the closer axis with the camera (if close enough). ";
    text += "A middle button double click fits the zoom of the camera and the right button re-centers the scene.<br><br>";
    text += "A left button double click while holding right button pressed defines the camera <i>Revolve Around Point</i>. ";
    text += "See the <b>Mouse</b> tab and the documentation web pages for details.<br><br>";
    text += "Press <b>Escape</b> to exit the viewer.";
    return text;
}

} // namespace gui
