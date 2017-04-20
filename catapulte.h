#ifndef CATAPULTE_H
#define CATAPULTE_H

#include <QMainWindow>

namespace Ui {
class Catapulte;
}

class Catapulte : public QMainWindow
{
    Q_OBJECT

public:
    explicit Catapulte(QWidget *parent = 0);
    ~Catapulte();

protected:


private slots:
    void on_Start__clicked();

private:
    Ui::Catapulte *ui;
};

#endif // CATAPULTE_H
