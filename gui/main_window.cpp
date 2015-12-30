#include "main_window.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDockWidget>
#include <QFileDialog>
#include <QIcon>
#include <QMap>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPoint>
#include <QSettings>
#include <QSize>
#include <QStatusBar>
#include <QString>
#include <QTextEdit>
#include <QToolBar>

#include "gl_render_widget.h"

#include <QDebug>

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

    mFPSAction->trigger();
    mAxisAction->trigger();

    connect(this, &MainWindow::volumeDataFileOpened, mRenderWidget, &GLRenderWidget::onVolumeDataFileSelected);
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

void MainWindow::onSelectVolumeDataFile()
{
    QString selectedVolumeDataFileName = QFileDialog::getOpenFileName(this, tr("Open volume data file"),
                                                                      "./", tr("Raw volume data (*.raw)"),
                                                                      0, QFileDialog::ReadOnly);
    emit volumeDataFileOpened(selectedVolumeDataFileName);
}

void MainWindow::onApplicationAbout()
{
    QMessageBox::about(this, tr("About Application"),
                             tr("GPU Based Single Pass Ray Casting Engine, 1.0\n\nlubomir.duraj@gmail.com"));
}

void MainWindow::printOpenGLContextInfo()
{
    qDebug() << "sdsahsaifhsauifh";
    //QMap<QString, QString> currentContextInfo;
    //mRenderWidget->obtainOpenGLContextInfo(currentContextInfo);
    //qDebug() << currentContextInfo;
    /*QMap<QString, QString>::const_iterator it = currentContextInfo.cbegin();

    qDebug() << currentContextInfo["vendor"];

    mOpenGLContextInfoTextEdit->append(tr("*** Context information ***"));
    //mOpenGLContextInfoTextEdit->append(currentContextInfo["vendor"]);
    while (it != currentContextInfo.cend())
    {
        //mOpenGLContextInfoTextEdit->append(tr(it.key().toLatin1()) + ": " + it.value().toLatin1());
        mOpenGLContextInfoTextEdit->append(currentContextInfo["vendor"]);
    }

    mOpenGLContextInfoTextEdit->moveCursor(QTextCursor::Start);*/
}

void MainWindow::fpsChanged(bool checked)
{
    mRenderWidget->setFPSIsDisplayed(checked);
}

void MainWindow::axisChanged(bool checked)
{
    mRenderWidget->setAxisIsDrawn(checked);
}

void MainWindow::createActions()
{
    mOpenAction = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    mOpenAction->setShortcuts(QKeySequence::Open);
    mOpenAction->setStatusTip(tr("Open and select volume model"));
    connect(mOpenAction, &QAction::triggered, this, &onSelectVolumeDataFile);

    mFPSAction = new QAction(QIcon(":/images/F0.svg"), tr("&Enable/disable displaying FPS info"), this);
    mFPSAction->setCheckable(true);
    mFPSAction->setChecked(false);
    mFPSAction->setStatusTip(tr("Enable/disable displaying FPS info"));
    connect(mFPSAction, &QAction::triggered, this, &fpsChanged);

    mAxisAction = new QAction(QIcon(":/images/dmA.svg"), tr("&Enable/disable displaying axis"), this);
    mAxisAction->setCheckable(true);
    mAxisAction->setChecked(false);
    mAxisAction->setStatusTip(tr("Enable/disable displaying axis"));
    connect(mAxisAction, &QAction::triggered, this, &axisChanged);

    mAppAboutAction = new QAction(tr("&About"), this);
    mAppAboutAction->setStatusTip(tr("Show the application's About box"));
    connect(mAppAboutAction, &QAction::triggered, this, &onApplicationAbout);

    mQtAboutAction = new QAction(tr("About &Qt"), this);
    mQtAboutAction->setStatusTip(tr("Show the Qt library's About box"));
    connect(mQtAboutAction, &QAction::triggered, qApp, &QApplication::aboutQt);

    mExitAction = new QAction(tr("E&xit"), this);
    mExitAction->setShortcuts(QKeySequence::Quit);
    mExitAction->setStatusTip(tr("Exit the application"));
    connect(mExitAction, &QAction::triggered, this, &close);
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
    mSettingsToolBar = new QToolBar("Scene Settings", this);
    mSettingsToolBar->addAction(mOpenAction);
    mSettingsToolBar->addAction(mFPSAction);
    mSettingsToolBar->addAction(mAxisAction);
    addToolBar(Qt::TopToolBarArea, mSettingsToolBar);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWidgets()
{
    mRenderWidget = new GLRenderWidget(this);
    connect(mRenderWidget, &GLRenderWidget::viewerInitialized, this, &MainWindow::printOpenGLContextInfo);

    mOpenGLContextInfoDockWidget = new QDockWidget(tr("Context Information"), this);
    mOpenGLContextInfoDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
    mOpenGLContextInfoDockWidget->setFloating(false);

    mOpenGLContextInfoTextEdit = new QTextEdit(mOpenGLContextInfoDockWidget);
    mOpenGLContextInfoTextEdit->setReadOnly(true);
    mOpenGLContextInfoDockWidget->setWidget(mOpenGLContextInfoTextEdit);

    addDockWidget(Qt::BottomDockWidgetArea, mOpenGLContextInfoDockWidget);
    setCentralWidget(mRenderWidget);
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
