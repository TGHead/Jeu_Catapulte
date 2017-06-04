#ifndef CATAPULTE_H
#define CATAPULTE_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include "opencv2/opencv.hpp"
#include "gameround.h"
#include "gamesetting.h"

using namespace cv;

/*
 * This class is used to control general GUI and using opencv to capture and compare template image.
 *
 */

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
    VideoCapture *webCam_;// camera pointer
    QTime runtime_;// record total play time, start by click capture button, and end by the last target fired
    QTimer *f_timer_;// camera timer, used to control the frequency of capture image
    QTimer *g_timer_;// global timer, used to control the play time
    bool captured_;// boolean variable, false by default, turn to true by clicking the Capture Button
    bool started_;// boolean variable, false by default, turn to true by accept the came setting while clicking the Start or Restart Button
    bool launched_;// boolean variable, false by default, turn to true by clicking the Fire Button
    Mat templateImage_;// cv::Mat used to store template image matrice
//    float captured_location_[2];

    GameRound *round_;// GameRound Object pointer, making instance while clicking Capture Button
    GameSetting *setting_;// GameSetting Object pointer, making instance while clicking Start or Restart Button

    Point last_Pos_;// Record last template position, used to emit "Fire" signal while moving template image downward

private slots:
    /*
     * Button clicked slot function
     */
    void Start_Button__clicked();
    void Capture_Button__clicked();
    void Restart_Button__clicked();
    void Fire_Button__clicked();
    void Next_Button__clicked();
    void Replay_Button__clicked();

    void GameSetting_Accepted();
    void GameSetting_Rejected();

    /*
     * Display camera image and timer
     */
    void afficherImage();
    void afficherGlobalTime();
    void afficherRoundTime();

signals:
    void launch();//"Fire" signal
};

#endif // CATAPULTE_H
