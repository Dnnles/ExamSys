#include "logindialog.h"
#include "examdialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //loginDialog w;
    //w.show();
    examdialog e;
    e.show();

    return a.exec();
}
