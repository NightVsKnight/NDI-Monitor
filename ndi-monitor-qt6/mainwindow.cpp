#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTimer>

#include "main.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ndiReceiver(this)
{
    if (false)
    {
        setFullScreen(true);
    }

    ui->setupUi(this);

    pActionExit = new QAction(tr("E&xit"));
    connect(pActionExit, &QAction::triggered, this, &MainWindow::onActionExit);

    pActionFullScreenToggle = new QAction(tr("Full Screen Toggle"));
    connect(pActionFullScreenToggle, &QAction::triggered, this, &MainWindow::onActionFullScreenToggle);

    ndiReceiver.addVideoSink(ui->videoWidget->videoSink());
    ndiReceiver.start();
}

MainWindow::~MainWindow()
{
    ndiReceiver.stop();
    delete ui;
}

#ifndef QT_NO_CONTEXTMENU
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    contextMenuAddNdiSources(&menu);
    menu.addSeparator();
    menu.addAction(pActionFullScreenToggle);
    menu.addSeparator();
    menu.addAction(pActionExit);
    menu.exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU

void MainWindow::contextMenuAddNdiSources(QMenu *menu)
{
    auto ndiSources = ndi().ndiFindSources();
    uint32_t i = 0;
    for (QMap<QString, NDIlib_source_t>::iterator it = ndiSources.begin(); it != ndiSources.end(); ++it) {
        QString cNdiSourceName = it.key();
        QString cNdiSourceAddress  = QString::fromUtf8(it.value().p_url_address);
        qDebug().nospace() << "[" << i++ << "] ADDRESS: " << cNdiSourceAddress << ", NAME: " << cNdiSourceName;

        QString actionText = cNdiSourceName;
        if (true)
        {
            actionText += " {address:" + cNdiSourceAddress + "}";
        }
        auto action = new QAction(actionText);
        action->setData(cNdiSourceName);
        connect(action, &QAction::triggered, this, &MainWindow::onNdiSourceSelected);
        menu->addAction(action);
    }
}

void MainWindow::onActionExit()
{
    qDebug() << "onActionExit()";
    QApplication::exit(0);
}

void MainWindow::onActionFullScreenToggle()
{
    qDebug() << "onActionFullScreenToggle()";
    setFullScreen(windowState() != Qt::WindowFullScreen);
}

void MainWindow::setFullScreen(bool fullScreen)
{
    qDebug().nospace() << "setFullScreen(" << fullScreen << ")";
    if (fullScreen)
    {
        setWindowState(Qt::WindowFullScreen);
    }
    else
    {
        setWindowState(Qt::WindowNoState);
    }
}

void MainWindow::onNdiSourceSelected()
{
    auto pAction = qobject_cast<QAction*>(sender());
    if (!pAction) return;
    QString cNdiSourceName = pAction->data().toString();
    qDebug() << "cNdiSourceName" << cNdiSourceName;
    ndiReceiver.setNdiSourceName(cNdiSourceName);
}
