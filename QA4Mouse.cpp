/*
 Copyright (c) 2014 Tomas Poledny
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.
 */
#include "QA4Mouse.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QApplication>
#include <qgridlayout.h>
#include <QDebug>
#include <QSettings>
#include <QStatusBar>
#include <unistd.h>
#include <libgmouse/system_a4.h>

#include "A4Tool.h"

QA4Mouse::QA4Mouse() : showedInfoAboutMinimaze(false), initAll(true)
{
    dev = new A4Device();
    profiles << Profile::getProfile(1);
    profiles << Profile::getProfile(2);
    profiles << Profile::getProfile(3);
    profiles << Profile::getProfile(4);
    profiles << Profile::getProfile(5);

    initGUI();
    createActions();
    initSystemTray();

    QMenu* fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction(infoAction);
    fileMenu->addAction(previousProfileAction);
    fileMenu->addAction(nextProfileAction);
    fileMenu->addAction(quitAction);

    readSettings();

    timerCheck = new QTimer(this);
    connect(timerCheck, SIGNAL(timeout()), SLOT(checkTime()));
    timerCheck->start(updateInterval);
	checkTime();
}

QA4Mouse::~QA4Mouse()
{
    delete dev;
}

void QA4Mouse::closeEvent(QCloseEvent* event)
{
    if (systemTray->isVisible()) {
        if (!showedInfoAboutMinimaze) {
            systemTray->showMessage(tr("QA4Mouse"), tr("The program will keep running in the system tray."), QSystemTrayIcon::Information, 5000);
            showedInfoAboutMinimaze = true;
        }
        hide();
        event->ignore();
    }
}

void QA4Mouse::showSystemTrayMessage(QString message, QSystemTrayIcon::MessageIcon icon)
{
    if (systemTray->isVisible()) {
        systemTray->showMessage(tr("QA4Mouse"), message, icon, 5000);
    }
}



void QA4Mouse::initSystemTray()
{
    systemTray = new QSystemTrayIcon(this);
    systemTray->setIcon(windowIcon());
    systemTray->setVisible(true);
    createSystemTrayMenu();
    systemTray->setToolTip(getInfoString());

    connect(systemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void QA4Mouse::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
        case QSystemTrayIcon::Trigger:
            if (showInfoOnClick) {
                showInfo();
                break;
            }
        case QSystemTrayIcon::DoubleClick:
            if (isVisible()) {
                setVisible(false);
            } else {
                setVisible(true);
            }
            break;
        case QSystemTrayIcon::MiddleClick:
            nextProfile();
            break;
    }
}

void QA4Mouse::setVisible(bool visible)
{
    QMainWindow::setVisible(visible);
}

void QA4Mouse::createActions()
{
    quitAction = new QAction(QIcon::fromTheme("application-exit"), tr("&Quit"), this);
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, SIGNAL(triggered()), SLOT(quitApp()));

    initProfileAction = new QAction(QIcon::fromTheme("media-playback-start"), tr("Init profile"), this);
    connect(initProfileAction, SIGNAL(triggered(bool)), SLOT(initProfile()));

    previousProfileAction = new QAction(QIcon::fromTheme("go-previous-view"), tr("Previous profile"), this);
    connect(previousProfileAction, SIGNAL(triggered(bool)), SLOT(previousProfile()));

    nextProfileAction = new QAction(QIcon::fromTheme("go-next-view"), tr("Next profile"), this);
    connect(nextProfileAction, SIGNAL(triggered(bool)), SLOT(nextProfile()));

    infoAction = new QAction(QIcon::fromTheme("help-about"), tr("Show info"), this);
    connect(infoAction, SIGNAL(triggered(bool)), SLOT(showInfo()));

    showMainWindow = new QAction(QIcon(":/images/icon.png"), tr("Show window"), this);
    connect(showMainWindow, SIGNAL(triggered(bool)), SLOT(show()));
}

void QA4Mouse::quitApp()
{
    writeSettings();
    qApp->quit();
}

void QA4Mouse::createSystemTrayMenu()
{
    systemTrayMenu = new QMenu(this);
    systemTrayMenu->setTitle(qAppName());
    systemTrayMenu->setIcon(windowIcon());

    systemTrayMenu->addAction(showMainWindow);
    systemTrayMenu->addAction(infoAction);
    systemTrayMenu->addAction(initProfileAction);
    systemTrayMenu->addAction(previousProfileAction);
    systemTrayMenu->addAction(nextProfileAction);
    systemTrayMenu->addSeparator();
    systemTrayMenu->addAction(quitAction);


    systemTray->setContextMenu(systemTrayMenu);

    showInfo();
}

void QA4Mouse::profileWasChanged(bool showMessage)
{
    dev->openDevice();
    int i = getProfile(dev->device);
    if (i >= 0 && i < 5) {
        Profile& p = profiles[i];
        if (showMessage) {
            showSystemTrayMessage(tr("Profile is %1 %2").arg(p.id).arg(p.name()));
        }
        systemTray->setIcon(p.getIconWithBattery(*dev));
    }
    systemTray->setToolTip(getInfoString());
    statusBarLabel->setText(getInfoOneLineString());
}



void QA4Mouse::changeProfile(int i)
{
    dev->openDevice();
    setProfile(dev->device, i);
}

void QA4Mouse::initProfile()
{
    dev->openDevice();
    if (isInit(dev->device)) {
        if (initMultifunc(dev->device)) {
            showSystemTrayMessage(tr("Init profile succes"));
        } else {
            showSystemTrayMessage(tr("Init profile failed"));
        }
    }
}

void QA4Mouse::nextProfile()
{
    dev->openDevice();
    int i = getProfile(dev->device);
    for (int y = 0; y < 5; ++y) {
        i = (i + 1) % 5;
        if (profiles[i].isEnabled()) {
            break;
        }
    }

    if (setProfile(dev->device, i)) {
        profileWasChanged();
    } else {
        showSystemTrayMessage(tr("Action next profile failed"));
    }
}

void QA4Mouse::previousProfile()
{
    dev->openDevice();
    int i = getProfile(dev->device);
    for (int y = 0; y < 5; ++y) {
        i = (i - 1) % 5;
        if (i < 0) {
            i = 5 - (i * -1);
        }
        if (profiles[i].isEnabled()) {
            break;
        }
    }

    if (setProfile(dev->device, i)) {
        profileWasChanged();
    } else {
        showSystemTrayMessage(tr("Action previous profile failed"));
    }
}

QString QA4Mouse::getInfoString()
{
    dev->openDevice();
    int battery = getBatteryMouse(dev->device);
    int profile = getProfile(dev->device);
    bool init = isInit(dev->device);
    QString msg = tr("Battery state: %1 %\n"
                     "Profile: %2 %4\n"
                     "Init: %3").arg(battery).arg(profiles[profile].id).arg(QString::number(init)).arg(profiles[profile].name());
    return msg;
}

QString QA4Mouse::getInfoOneLineString()
{
    dev->openDevice();
    int battery = getBatteryMouse(dev->device);
    int profile = getProfile(dev->device);
    bool init = isInit(dev->device);
    QString msg = tr("Battery state: %1 %\t"
                     "Profile: %2 %4 \t"
                     "Init: %3").arg(battery).arg(profiles[profile].id).arg(QString::number(init)).arg(profiles[profile].name());
    return msg;
}



void QA4Mouse::showInfo()
{
    profileWasChanged(false);
    showSystemTrayMessage(getInfoString());
}


void QA4Mouse::initGUI()
{
    QWidget* w = new QWidget(this);

    QGridLayout* l = new QGridLayout(w);
    lineProfile1 = new QLineEdit(tr("Profile 1"), w);
    lineProfile2 = new QLineEdit(tr("Profile 2"), w);
    lineProfile3 = new QLineEdit(tr("Profile 3"), w);
    lineProfile4 = new QLineEdit(tr("Profile 4"), w);
    lineProfile5 = new QLineEdit(tr("Profile 5"), w);

    //Profile 1
    l->addWidget(new QLabel(tr("1"), w), 0, 0);
    checkProfile1 = new QCheckBox(w);
    checkProfile1->setChecked(true);
    l->addWidget(lineProfile1, 0, 1);
    l->addWidget(checkProfile1, 0, 2);
    connect(checkProfile1, SIGNAL(stateChanged(int)), SLOT(profile1Changed()));
    connect(lineProfile1, SIGNAL(editingFinished()), SLOT(profile1Changed()));

    //Profile 2
    l->addWidget(new QLabel(tr("2"), w), 1, 0);
    checkProfile2 = new QCheckBox(w);
    checkProfile2->setChecked(true);
    l->addWidget(lineProfile2, 1, 1);
    l->addWidget(checkProfile2, 1, 2);
    connect(checkProfile2, SIGNAL(stateChanged(int)), SLOT(profile2Changed()));
    connect(lineProfile2, SIGNAL(editingFinished()), SLOT(profile2Changed()));

    //Profile 3
    l->addWidget(new QLabel(tr("3"), w), 2, 0);
    checkProfile3 = new QCheckBox(w);
    checkProfile3->setChecked(true);
    l->addWidget(lineProfile3, 2, 1);
    l->addWidget(checkProfile3, 2, 2);
    connect(checkProfile3, SIGNAL(stateChanged(int)), SLOT(profile3Changed()));
    connect(lineProfile3, SIGNAL(editingFinished()), SLOT(profile3Changed()));

    //Profile 4
    l->addWidget(new QLabel(tr("4"), w), 3, 0);
    checkProfile4 = new QCheckBox(w);
    checkProfile4->setChecked(true);
    l->addWidget(lineProfile4, 3, 1);
    l->addWidget(checkProfile4, 3, 2);
    connect(checkProfile4, SIGNAL(stateChanged(int)), SLOT(profile4Changed()));
    connect(lineProfile4, SIGNAL(editingFinished()), SLOT(profile4Changed()));

    //Profile 5
    l->addWidget(new QLabel(tr("5"), w), 4, 0);
    checkProfile5 = new QCheckBox(w);
    checkProfile5->setChecked(true);
    l->addWidget(lineProfile5, 4, 1);
    l->addWidget(checkProfile5, 4, 2);
    connect(checkProfile5, SIGNAL(stateChanged(int)), SLOT(profile5Changed()));
    connect(lineProfile5, SIGNAL(editingFinished()), SLOT(profile5Changed()));

    //Wake up
    l->addWidget(new QLabel(tr("Wake up:"), w), 5, 0);
    wakeMod = new QComboBox(w);
    wakeMod->addItem(tr("Click"), CLICK);
    wakeMod->addItem(tr("Move"), MOVE);
    wakeTime = new QSpinBox(w);
    wakeTime->setMinimum(3);
    wakeTime->setMaximum(10);
    wakeTime->setSuffix(tr(" min"));
    l->addWidget(wakeMod, 5, 1);
    l->addWidget(wakeTime, 5, 2);

    saveWakeUpButton = new QPushButton(QIcon::fromTheme("document-save"), tr("Save"), w);
    l->addWidget(saveWakeUpButton, 5, 3);
    connect(saveWakeUpButton, SIGNAL(clicked(bool)), SLOT(saveWakeUp()));

    setCentralWidget(w);

    statusBarLabel = new QLabel(getInfoOneLineString(), statusBar());
    statusBar()->addWidget(statusBarLabel);

}

void QA4Mouse::readSettings()
{
    QSettings settings;
    checkProfile1->setChecked(settings.value("enableProfile1", true).toBool());
    lineProfile1->setText(settings.value("profile1Name", "Profile 1").toString());
    profile1Changed();
    checkProfile2->setChecked(settings.value("enableProfile2", true).toBool());
    lineProfile2->setText(settings.value("profile2Name", "Profile 2").toString());
    profile2Changed();
    checkProfile3->setChecked(settings.value("enableProfile3", true).toBool());
    lineProfile3->setText(settings.value("profile3Name", "Profile 3").toString());
    profile3Changed();
    checkProfile4->setChecked(settings.value("enableProfile4", true).toBool());
    lineProfile4->setText(settings.value("profile4Name", "Profile 4").toString());
    profile4Changed();
    checkProfile5->setChecked(settings.value("enableProfile5", true).toBool());
    lineProfile5->setText(settings.value("profile5Name", "Profile 5").toString());
    profile5Changed();
    showedInfoAboutMinimaze = settings.value("showedInfoAboutMinimaze", false).toBool();
    updateInterval = settings.value("updateInterval",  3600000).toInt();

    dev->openDevice();
    WakeAfter wakeModMouse = getWakeUpBy(dev->device);
    int wakeTimeMouse = getWakeUpTime(dev->device);
    wakeMod->setCurrentIndex(wakeMod->findData(wakeModMouse));
    wakeTime->setValue(wakeTimeMouse);
}


void QA4Mouse::writeSettings()
{
    QSettings settings;
    settings.setValue("enableProfile1", checkProfile1->isChecked());
    settings.setValue("profile1Name", lineProfile1->text());
    settings.setValue("enableProfile2", checkProfile2->isChecked());
    settings.setValue("profile2Name", lineProfile2->text());
    settings.setValue("enableProfile3", checkProfile3->isChecked());
    settings.setValue("profile3Name", lineProfile3->text());
    settings.setValue("enableProfile4", checkProfile4->isChecked());
    settings.setValue("profile4Name", lineProfile4->text());
    settings.setValue("enableProfile5", checkProfile5->isChecked());
    settings.setValue("profile5Name", lineProfile5->text());

    settings.setValue("updateInterval", updateInterval);
    settings.setValue("showedInfoAboutMinimaze", showedInfoAboutMinimaze);
}

void QA4Mouse::profile1Changed()
{
    profiles[0].setEnabled(checkProfile1->isChecked());
    profiles[0].setName(lineProfile1->text());
    if (!checkProfile1->isChecked()) {
        lineProfile1->setEnabled(false);
    } else {
        lineProfile1->setEnabled(true);
    }
}

void QA4Mouse::profile2Changed()
{
    profiles[1].setEnabled(checkProfile2->isChecked());
    profiles[1].setName(lineProfile2->text());
    if (!checkProfile2->isChecked()) {
        lineProfile2->setEnabled(false);
    } else {
        lineProfile2->setEnabled(true);
    }
}

void QA4Mouse::profile3Changed()
{
    profiles[2].setEnabled(checkProfile3->isChecked());
    profiles[2].setName(lineProfile3->text());
    if (!checkProfile3->isChecked()) {
        lineProfile3->setEnabled(false);
    } else {
        lineProfile3->setEnabled(true);
    }
}

void QA4Mouse::profile4Changed()
{
    profiles[3].setEnabled(checkProfile4->isChecked());
    profiles[3].setName(lineProfile4->text());
    if (!checkProfile4->isChecked()) {
        lineProfile4->setEnabled(false);
    } else {
        lineProfile4->setEnabled(true);
    }
}

void QA4Mouse::profile5Changed()
{
    profiles[4].setEnabled(checkProfile5->isChecked());
    profiles[4].setName(lineProfile5->text());
    if (!checkProfile5->isChecked()) {
        lineProfile5->setEnabled(false);
    } else {
        lineProfile5->setEnabled(true);
    }
}

void QA4Mouse::checkTime()
{
    qDebug() << "Updated";
    profileWasChanged(false);
    int battery = getBatteryMouse(dev->device);
    if (battery < A4Device::batteryWarningLevel) {
        showSystemTrayMessage(tr("Battery level is low %1%").arg(battery), QSystemTrayIcon::Warning);
    }

    if (initAll) {
        initProfile();
    }
}


int QA4Mouse::getUpdateInterval()
{
    return updateInterval;
}

void QA4Mouse::setUpdateInterval(int updateSec)
{
    updateInterval = updateSec * 1000;
    timerCheck->start(updateInterval);
}

void QA4Mouse::saveWakeUp()
{
    WakeAfter mod = (WakeAfter)wakeMod->itemData(wakeMod->currentIndex()).toInt();
    dev->openDevice();
    if (!setWakeUp(dev->device, wakeTime->value(), mod)) {
        showSystemTrayMessage("Settings wake up failed!");
    }
}


#include "QA4Mouse.moc"
