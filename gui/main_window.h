#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

// QT's forward declarations
QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QCloseEvent)
QT_FORWARD_DECLARE_CLASS(QDockWidget)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QTextEdit)
QT_FORWARD_DECLARE_CLASS(QToolBar)

namespace gui
{

// forward declarations
class GLRenderWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

private slots:
    void onOpenAction();
    void onApplicationAbout();
    void printOpenGLContextInfo();
    void fpsChanged(bool checked);
    void axisChanged(bool checked);

private:
    void createActions();
    void createMenus();
    void createToolbars();
    void createStatusBar();
    void createDockWidgets();

    void loadSettings();
    void storeSettings();

    QAction* mOpenAction;
    QAction* mFPSAction;
    QAction* mAxisAction;
    QAction* mAppAboutAction;
    QAction* mQtAboutAction;
    QAction* mExitAction;

    QMenu* mFileMenu;
    QMenu* mHelpMenu;

    QToolBar* mSettingsToolBar;

    QDockWidget* mOpenGLContextInfoDockWidget;
    QTextEdit* mOpenGLContextInfoTextEdit;

    GLRenderWidget* mRenderWidget;
};

} // namespace gui

#endif // MAIN_WINDOW_H
