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

#include "Profile.h"
#include <QString>
#include "A4Tool.h"

Profile::Profile(int id, QString name, bool enabled): id(id), profileName(name), enabled(enabled)
{

}

Profile::~Profile()
{

}

Profile Profile::getProfile(int id)
{
	Profile prof(id, QString("Profile %1").arg(id), true);
	return prof;
}

QIcon Profile::getIconWithBattery(A4Device& dev)
{
	dev.openDevice();
	int battery = getBatteryMouse(dev.device);
	if(battery < A4Device::batteryWarningLevel){
		return getLowIcon();
	}
	return getIcon();
}



QIcon Profile::getIcon()
{
    if (id > 0 && id < 6) {
        QString iconPath = QString(":/images/profile-%1.png").arg(id);
        return QIcon(iconPath);
    }
    return QIcon(":/images/icon.png");
}

QIcon Profile::getLowIcon()
{
	if (id > 0 && id < 6) {
		QString iconPath = QString(":/images/profile-%1-low.png").arg(id);
		return QIcon(iconPath);
	}
    return QIcon(":/images/icon-low.png");
}

QString Profile::name()
{

    return profileName;
}

void Profile::setName(QString name)
{
    profileName = name;
}

bool Profile::isEnabled()
{
	return enabled;
}

void Profile::setEnabled(bool enabled)
{
	this->enabled = enabled;
}
