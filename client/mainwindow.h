#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QLabel>
#include <QPropertyAnimation>
#include <QAbstractAnimation>
#include <QGraphicsOpacityEffect>
#include <QEasingCurve>
#include <QPalette>
#include <QWidget>
#include <QVBoxLayout>
#include <QSystemTrayIcon>
#include <QtGlobal>
#include <QDebug>
#include <QSslError>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDateTime>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QString secret;
    QLabel *qrCodeLabel;
    QLabel *statusLabel;
    QSystemTrayIcon *systemTrayIcon;
    QNetworkAccessManager *networkAccessManager;
    QActionGroup *terminatorActions;

    void typeString(QString string);

private slots:
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void replyFinished(QNetworkReply *reply);
    void systemTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void aboutQt();
    void about();
    void installApp();
};

#endif // MAINWINDOW_H
