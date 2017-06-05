#ifndef GAMEROUND_H
#define GAMEROUND_H

#include "enum_level.h"
#include <QTime>
#include <time.h>

/*
 * This class is designed to store some basic game settings and status, like game level, total score, global time etc...
 * Created an instance when Capture button clicked
 */

class GameRound
{

private:

    LEVEL level_;// store game level in an enum variable LEVEL defined in file enum_level.h
    int r_left_;// store the number of targets left during 1 game round initialize to 10 at the beginning
    int scores_[10];// an array to store each target's score
    QTime round_time_;// store the a target aimming time of one game round, by the way the global time is stored in runtime_ variable of Catapulte class
    QString player_name_;// record the player's name
    /*
     * a struct to record target position
     */
    typedef struct {
        double x;
        double y;
    }Position;
    Position po ;

public:

    explicit GameRound(LEVEL level = EASY, QString player_name = "Null");
    ~GameRound();

    /*
     * Setters
     */

    void setLevel(LEVEL level);
    void setR_left(int r_left);
    void setScores(int N, int score);
    void setPlayerName(QString player_name);

    void Init_Round_Time();// Initialize the round_time_ to 00:00:00, this function is called in the constructor and while clicking next target button
    void Round_Time_add1s();// +1s to the round_time_

    void calculScore(double d);// calculate the target score according to the game level with d the distance from the center of target position to the projectile position

    /*
     * Getters
     */

    LEVEL getLevel() { return level_; }
    QString getLevelText();
    QString getPlayerName() { return player_name_; }
    int getR_left() { return r_left_; }
    int getScores(int N) { return scores_[N]; }
    int getSum_Scores();// calculate the total score.
    QTime getRound_Time(){ return round_time_; }

    void generatePostion();// generate randomly the target position according to the game level
    double getPositionX(){return po.x;}// get target x position
    double getPositionY(){return po.y;}// get target y position

};

#endif // GAMEROUND_H
