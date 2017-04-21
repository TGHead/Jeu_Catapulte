#ifndef CATAPULTE_H
#define CATAPULTE_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include "opencv2/opencv.hpp"
#include "gameround.h"

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
    bool capturing_;
    Mat templateImage_;

    GameRound *round_;

private slots:
    void Start_Button__clicked();
    void Capture_Button__clicked();
    void Restart_Button__clicked();
    void Fire_Button__clicked();
    void Replay_Button__clicked();

    void afficherImage();
    void afficherGlobalTime();
    void afficherRoundTime();
};

#endif // CATAPULTE_H
