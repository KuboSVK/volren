#ifndef GL_RENDER_WIDGET_H
#define GL_RENDER_WIDGET_H

#include <QGLViewer/qglviewer.h>
#include <QMap>
#include <QOpenGLFunctions>
#include <QString>

QT_FORWARD_DECLARE_CLASS(QGLContext)
QT_FORWARD_DECLARE_CLASS(QGLFormat)

namespace core
{
//forward declaration(s)
class IRenderer;
}

namespace gui
{

class GLRenderWidget : public QGLViewer, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLRenderWidget(QWidget* parent);
    ~GLRenderWidget();

    virtual void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    virtual void initializeGL() Q_DECL_OVERRIDE;
    virtual void init() Q_DECL_OVERRIDE;
    virtual void draw() Q_DECL_OVERRIDE;
    void obtainOpenGLContextInfo(QMap<QString, QString>& openGLContextInfo);
    virtual QString helpString() const;

public slots:
    void onVolumeDataFileSelected(QString selectedVolumeDataFilePath);

private:
    QGLContext* mOpenGLContext;
    QGLFormat* mOpenGLFormat;
    core::IRenderer* mRayCastingEngine;

    Q_DISABLE_COPY(GLRenderWidget)
};

} // namespace gui

#endif // GL_RENDER_WIDGET_H
