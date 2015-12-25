#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

// QT's forward declarations
QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QCloseEvent)
QT_FORWARD_DECLARE_CLASS(QDockWidget)
QT_FORWARD_DECLARE_CLASS(QMenu)
QT_FORWARD_DECLARE_CLASS(QToolBar)
QT_FORWARD_DECLARE_CLASS(QWidget)

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

private:
    void createActions();
    void createMenus();
    void createToolbars();
    void createStatusBar();
    void createDockWidgets();

    void loadSettings();
    void storeSettings();

    QAction* mOpenAction;
    QAction* mAppAboutAction;
    QAction* mQtAboutAction;
    QAction* mExitAction;

    QMenu* mFileMenu;
    QMenu* mHelpMenu;

    QToolBar* mFileToolBar;

    QDockWidget* mSettingsDockWidget;
    QDockWidget* mRenderDockWidget;

    QWidget* mSettingsWidget;
    GLRenderWidget* mRenderWidget;
};

} // namespace gui

#endif // MAIN_WINDOW_H
