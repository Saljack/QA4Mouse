#include <QtGui/QApplication>
#include "QA4Tech.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QA4Tech foo;
    foo.show();
    return app.exec();
}
