#ifndef CATAPULTE_H
#define CATAPULTE_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>
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
    QTime runtime_;
    QTimer *f_timer_;
    QTimer *g_timer_;
    QTimer *t_timer_;
    bool capturing_;
    Mat templateImage_;

private slots:
    void Start_Button__pressed();
    void Capture_Button__pressed();
    void afficherImage();
    void afficherGlobalTime();
};

#endif // CATAPULTE_H
