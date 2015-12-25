#ifndef GL_RENDER_WIDGET_H
#define GL_RENDER_WIDGET_H

#include <QGLViewer/qglviewer.h>

#include <QString>

namespace gui
{

class GLRenderWidget : public QGLViewer
{
public:
    GLRenderWidget(QWidget* parent);
    ~GLRenderWidget();

    virtual void init() Q_DECL_OVERRIDE;
    virtual void draw() Q_DECL_OVERRIDE;
    virtual QString helpString() const;
};

} // namespace gui

#endif // GL_RENDER_WIDGET_H
