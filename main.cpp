#include <QtGui/QApplication>
#include "QA4Mouse.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
	app.setApplicationName("QA4Mouse");
	app.setWindowIcon(QIcon(":/images/icon.png"));
    QA4Mouse foo;
    foo.show();
    return app.exec();
}
