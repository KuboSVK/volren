#include "main_window.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDockWidget>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPoint>
#include <QSettings>
#include <QSize>
#include <QStatusBar>
#include <QToolBar>
#include <QWidget>

#include "gl_render_widget.h"

namespace gui
{

MainWindow::MainWindow()
{
    loadSettings();

    createActions();
    createMenus();
    createToolbars();
    createStatusBar();
    createDockWidgets();
}

MainWindow::~MainWindow()
{
    delete mRenderWidget;
    mRenderWidget = 0;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    storeSettings();
    event->accept();
}

void MainWindow::onOpenAction()
{

}

void MainWindow::onApplicationAbout()
{
    QMessageBox::about(this, tr("About Application"), tr("Volumetric based rendering application."));
}

void MainWindow::createActions()
{
    mOpenAction = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    mOpenAction->setShortcuts(QKeySequence::Open);
    mOpenAction->setStatusTip(tr("Open and select volume model"));
    connect(mOpenAction, SIGNAL(triggered()), this, SLOT(onOpenAction()));

    mAppAboutAction = new QAction(tr("&About"), this);
    mAppAboutAction->setStatusTip(tr("Show the application's About box"));
    connect(mAppAboutAction, SIGNAL(triggered()), this, SLOT(onApplicationAbout()));

    mQtAboutAction = new QAction(tr("About &Qt"), this);
    mQtAboutAction->setStatusTip(tr("Show the Qt library's About box"));
    connect(mQtAboutAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    mExitAction = new QAction(tr("E&xit"), this);
    mExitAction->setShortcuts(QKeySequence::Quit);
    mExitAction->setStatusTip(tr("Exit the application"));
    connect(mExitAction, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::createMenus()
{
    mFileMenu = menuBar()->addMenu(tr("&File"));
    mFileMenu->addAction(mOpenAction);
    mFileMenu->addSeparator();
    mFileMenu->addAction(mExitAction);

    mHelpMenu = menuBar()->addMenu(tr("&Help"));
    mHelpMenu->addAction(mQtAboutAction);
    mHelpMenu->addAction(mAppAboutAction);
}

void MainWindow::createToolbars()
{
    mFileToolBar = addToolBar(tr("File"));
    mFileToolBar->addAction(mOpenAction);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWidgets()
{
    mSettingsDockWidget = new QDockWidget(tr("Settings"), this);
    mSettingsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    mSettingsWidget = new QWidget(mSettingsDockWidget);
    mSettingsDockWidget->setWidget(mSettingsWidget);

    mRenderDockWidget = new QDockWidget(tr("Render Output"), this);
    mRenderDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    mRenderWidget = new GLRenderWidget(mRenderDockWidget);
    mRenderDockWidget->setWidget(mRenderWidget);

    addDockWidget(Qt::LeftDockWidgetArea, mSettingsDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, mRenderDockWidget);
}

void MainWindow::loadSettings()
{
    QSettings settings;
    QPoint position = settings.value("main_window/position", QPoint(200, 200)).toPoint();
    QSize size = settings.value("main_window/size", QSize(600, 400)).toSize();

    resize(size);
    move(position);
}

void MainWindow::storeSettings()
{
    QSettings settings;
    settings.setValue("main_window/position", pos());
    settings.setValue("main_window/size", size());
}

} // namespace gui
