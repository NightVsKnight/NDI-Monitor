#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QContextMenuEvent>

#include "ndireceiver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

private:
    Ui::MainWindow *ui;
    NdiReceiver ndiReceiver;

    void contextMenuAddNdiSources(QMenu *menu);
    void setFullScreen(bool fullScreen);

    QAction *pActionFullScreenToggle;
    QAction *pActionExit;

private slots:
    void onActionExit();
    void onActionFullScreenToggle();
    void onNdiSourceSelected();
};
#endif // MAINWINDOW_H
