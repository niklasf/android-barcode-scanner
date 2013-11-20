#include "mainwindow.h"

#define PLAY_STORE_URL "http://play.google.com/store/apps/details?id=de.tuc.barcodescanner"
#define SERVICE_URL "https://android-barcode-scanner.herokuapp.com/"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set window properties.
    setFixedSize(420, 380);
    setWindowTitle(tr("Android Barcode Scanner"));

    // Make background white.
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::white);
    setPalette(palette);

    // Create vertical layout.
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Create the QR-code label.
    qrCodeLabel = new QLabel();
    qrCodeLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(qrCodeLabel);

    // Create the status label.
    statusLabel = new QLabel();
    statusLabel->setText(tr("Please wait ..."));
    statusLabel->setAlignment(Qt::AlignBottom | Qt::AlignCenter);
    statusLabel->setMargin(25);
    layout->addWidget(statusLabel);

    // Create about actions.
    QAction *aboutQtAction = new QAction(tr("About Qt"), this);
    connect(aboutQtAction, SIGNAL(triggered()), this, SLOT(aboutQt()));
    QAction *aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    QAction *installAppAction = new QAction(tr("Install app"), this);
    connect(installAppAction, SIGNAL(triggered()), this, SLOT(installApp()));

    // Create terminator actions.
    terminatorActions = new QActionGroup(this);
    terminatorActions->setExclusive(true);

    QAction *noTerminator = terminatorActions->addAction(tr("No terminator"));
    noTerminator->setCheckable(true);
    noTerminator->setData(QVariant());

    QAction *returnTerminator = terminatorActions->addAction(tr("Carriage return"));
    returnTerminator->setCheckable(true);
    returnTerminator->setData(QChar('\n'));
    returnTerminator->setChecked(true);

    QAction *spaceTerminator = terminatorActions->addAction(tr("Space"));
    spaceTerminator->setCheckable(true);
    spaceTerminator->setData(QChar(' '));

    QAction *tabTerminator = terminatorActions->addAction(tr("Tab"));
    tabTerminator->setCheckable(true);
    tabTerminator->setData(QChar('\t'));

    QAction *escapeTerminator = terminatorActions->addAction(tr("Escape"));
    escapeTerminator->setCheckable(true);
    escapeTerminator->setData(QChar('\x1b'));

    QAction *killTerminator = terminatorActions->addAction(tr("Ctrl + C"));
    killTerminator->setCheckable(true);
    killTerminator->setData(QChar('\x03'));

    // Create quit action.
    QAction *quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    // Create menu.
    QMenu *menu = menuBar()->addMenu(tr("Scanner"));
    menu->addAction(aboutQtAction);
    menu->addAction(aboutAction);
    menu->addAction(installAppAction);
    menu->addSeparator();
    menu->addAction(noTerminator);
    menu->addAction(returnTerminator);
    menu->addAction(spaceTerminator);
    menu->addAction(tabTerminator);
    menu->addAction(escapeTerminator);
    menu->addAction(killTerminator);
    menu->addSeparator();
    menu->addAction(quitAction);

    // Create system tray icon.
    systemTrayIcon = new QSystemTrayIcon(this);
    systemTrayIcon->setIcon(QIcon(":/barcode-16.png"));
    systemTrayIcon->setContextMenu(menu);
    connect(systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(systemTrayIconActivated(QSystemTrayIcon::ActivationReason)));
    systemTrayIcon->show();

    // Connect to the internet.
    networkAccessManager = new QNetworkAccessManager(this);
    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(networkAccessManager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)), this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    // TODO: Handle proxies.

    // Generate secret.
    qsrand(QDateTime::currentMSecsSinceEpoch());
    const char hexChars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    for (int i = 0; i < 64; i++) {
        secret = secret.append(QChar(hexChars[qrand() % 16]));
    }

    // Download the QR-code.
    // TODO: Use random channel.
    QNetworkRequest request;
    request.setUrl(QUrl(QString(SERVICE_URL) + "channel/" + secret + "/qr.png"));
    networkAccessManager->get(request);
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    // Handle network replies.
    if (reply->error() != QNetworkReply::NoError) {
        qCritical() << reply->errorString();
    } else if (reply->url().toString().endsWith("/qr.png")) {
        // Fade in QR-code.
        QPixmap pixmap;
        pixmap.loadFromData(reply->readAll());
        qrCodeLabel->setPixmap(pixmap);

        QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(this);
        opacityEffect->setOpacity(0);
        qrCodeLabel->setGraphicsEffect(opacityEffect);

        QPropertyAnimation* animation = new QPropertyAnimation(this);
        animation->setTargetObject(opacityEffect);
        animation->setPropertyName("opacity");
        animation->setDuration(2000);
        animation->setStartValue(opacityEffect->opacity());
        animation->setEndValue(1.0);
        animation->setEasingCurve(QEasingCurve::OutQuad);
        animation->start(QAbstractAnimation::DeleteWhenStopped);

        // Update status label.
        statusLabel->setText(tr("Connect by scanning the code ..."));
    } else {
        // Read scanned input.
        while (reply->canReadLine()) {
            QByteArray line = reply->readLine().trimmed();

            if (line.isEmpty()) {
                statusLabel->setText("Connected.");
                systemTrayIcon->showMessage(tr("Barcode scanner connected"), tr("Use this tray icon for configuring and closing."));
                hide();
            } else {
                typeString(line);
                statusLabel->setText("Last scan: " + line);
            }
        }
    }

    // Keep polling.
    QNetworkRequest request;
    request.setUrl(QUrl(QString(SERVICE_URL) + "channel/" + secret + "/"));
    networkAccessManager->deleteResource(request);
}

void MainWindow::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    Q_UNUSED(reply);

    for (int i = 0; i < errors.size(); i++) {
        qCritical() << errors.at(i).errorString();
    }
}

void MainWindow::systemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick) {
        // Activate the window when double clicking the tray icon.
        showNormal();
        raise();
        activateWindow();
    } else if (reason == QSystemTrayIcon::Trigger) {
        // Show the context menu on left click.
        systemTrayIcon->contextMenu()->popup(QCursor::pos());
    }
}

void MainWindow::typeString(QString string)
{
#ifdef Q_OS_WIN
    // Send string via keyboard.
    for (int i = 0; i < string.size(); i++) {
        INPUT input;
        memset(&input, 0, sizeof(INPUT));

        input.type = INPUT_KEYBOARD;
        input.ki.wScan = string.at(i).unicode();
        input.ki.dwFlags = KEYEVENTF_UNICODE;
        SendInput(1, &input, sizeof(INPUT));

        input.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
    }

    // Send terminator.
    if (terminatorActions->checkedAction()->data().isValid()) {
        QChar c = terminatorActions->checkedAction()->data().toChar();

        INPUT input;
        memset(&input, 0, sizeof(INPUT));

        input.type = INPUT_KEYBOARD;

        if (c == QChar('\n')) {
            input.ki.wVk = VK_RETURN;
        } else if (c == QChar('\t')) {
            input.ki.wVk = VK_TAB;
        } else if (c == QChar(' ')) {
            input.ki.wVk = VK_SPACE;
        } else if (c == QChar('\x1b')) {
            input.ki.wVk = VK_ESCAPE;
        } else if (c == QChar('\x03')) {
            input.ki.wVk = VK_CANCEL;
        } else {
            input.ki.wScan = c.unicode();
            input.ki.dwFlags = KEYEVENTF_UNICODE;
        }

        SendInput(1, &input, sizeof(INPUT));

        input.ki.dwFlags |= KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
    }
#else
    qCritical() << tr("Platform not supported.");
#endif
}

void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Android Barcode Scanner"), tr("<h1>Android Barcode Scanner 0.0.1</h1><p>Allows you to use your Android smartphone as a barcode scanner, relayed over the internet, wireless and configurable.</p><p>&copy; 2013 Niklas Fiekas &lt;<a href=\"mailto:niklas.fiekas@tu-clausthal.de\">niklas.fiekas@tu-clausthal.de</a>&gt;<br>Licensed under the GNU General-Public-License 3.</p>"));
}

void MainWindow::installApp()
{
    QDesktopServices::openUrl(QUrl(PLAY_STORE_URL));
}

MainWindow::~MainWindow()
{

}
