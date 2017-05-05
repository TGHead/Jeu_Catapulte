#include <src/.h/catapulte.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Catapulte w;
    w.show();

    return a.exec();
}
