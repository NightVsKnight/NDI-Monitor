#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "ndiwrapper.h"

CNdiWrapper *pNdi = nullptr;

CNdiWrapper &ndi()
{
    return *pNdi;
}

int main(int argc, char *argv[])
{
    qSetMessagePattern("[%{time hh:mm:ss.zzz} p%{pid} t%{threadid} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] %{function}:%{line} | %{message}");

    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ndi-monitor-qt6_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;

    pNdi = new CNdiWrapper(&w);

    // TODO: These two lines should be tied more closely together,
    // so that the UX can tell the user what to do if NDI cannot be initialized.
    w.show();
    pNdi->ndiInitialize();

    auto returnCode = a.exec();

    // TODO: This should then also be more closely tied in to the destruction of MainWindow
    pNdi->ndiDestroy();

    qDebug() << "returnCode" << returnCode;
    return returnCode;
}
