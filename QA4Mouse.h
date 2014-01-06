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
#ifndef QA4Tech_H
#define QA4Tech_H

#include <QtGui/QMainWindow>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include "A4Device.h"
#include "Profile.h"

class QA4Mouse : public QMainWindow
{
    Q_OBJECT
public:
    QA4Mouse();
    virtual ~QA4Mouse();
    void setVisible(bool visible);
protected:
    void closeEvent(QCloseEvent* event);
private:
    void initSystemTray();
    void createActions();
    void createSystemTrayMenu();
	void showSystemTrayMessage(QString message, QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information);
	void initGUI();
	void writeSettings();
	void readSettings();
	QString getInfoString();
	QString getInfoOneLineString();
	
	void setUpdateInterval(int updateSec);
	int getUpdateInterval();
	
    //DATA
    QSystemTrayIcon* systemTray;
    QMenu* systemTrayMenu;

    QAction* quitAction;

    QAction* initProfileAction;
    QAction* nextProfileAction;
    QAction* previousProfileAction;
	QAction* infoAction;
	QAction* showMainWindow;
	
// 	QSpinBox* maxProfile;

    A4Device* dev;
	
	QCheckBox* checkProfile1;
	QCheckBox* checkProfile2;
	QCheckBox* checkProfile3;
	QCheckBox* checkProfile4;
	QCheckBox* checkProfile5;
	
	QLineEdit* lineProfile1;
	QLineEdit* lineProfile2;
	QLineEdit* lineProfile3;
	QLineEdit* lineProfile4;
	QLineEdit* lineProfile5;
	
	QList<Profile> profiles;
	QLabel* statusBarLabel;
	
	QComboBox* wakeMod;
	QSpinBox* wakeTime;
	QPushButton* saveWakeUpButton;
	
	QTimer* timerCheck;
	bool showInfoOnClick;
	bool showedInfoAboutMinimaze;
	bool initAll;
	int updateInterval;

	

//SLOTS
private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void quitApp();
    void changeProfile(int i);
    void nextProfile();
    void previousProfile();
    void initProfile();
    void showInfo();
	void profileWasChanged(bool showMessage = true);
	
	void profile1Changed();
	void profile2Changed();
	void profile3Changed();
	void profile4Changed();
	void profile5Changed();
    void checkTime();
    void saveWakeUp();
};

#endif // QA4Tech_H
