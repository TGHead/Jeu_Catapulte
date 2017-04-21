#ifndef CATAPULTE_H
#define CATAPULTE_H

#include <QMainWindow>
#include "opencv2/opencv.hpp"

using namespace cv;

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

private:
    Ui::Catapulte *ui;
    VideoCapture *webCam_;
    QTimer *f_timer_;
    bool capturing_;
    Mat templateImage_;

private slots:
    void on_Start_Button__clicked();
    void on_Capture_Button__clicked();
    void afficherImage();
};

#endif // CATAPULTE_H
