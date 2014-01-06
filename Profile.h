#ifndef Profile_H
#define Profile_H
#include <libgmouse/system_a4.h>
#include <QIcon>
#include "A4Device.h"

class Profile
{
public:
    Profile(int id, QString name, bool enabled);
    virtual ~Profile();
    int id;
	static Profile getProfile(int id);
	QIcon getIconWithBattery(A4Device& dev);
    QIcon getIcon();
    QIcon getLowIcon();
	QString name();
	void setName(QString name);
	bool isEnabled();
	void setEnabled(bool enabled);
private:
    QString profileName;
	bool enabled;
};

#endif // Profile_H
