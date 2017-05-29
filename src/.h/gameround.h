#ifndef GAMEROUND_H
#define GAMEROUND_H

#include "enum_level.h"
#include <QTime>
#include <time.h>


class GameRound
{

private:

    LEVEL level_;
    int r_left_;
    int scores_[10];
    QTime round_time_;
    QString player_name_;
    typedef struct {
        double x;
        double y;
    }Position;
    Position po ;

public:

    explicit GameRound(LEVEL level = EASY, QString player_name = "Null");
    ~GameRound();
    void setLevel(LEVEL level);
    void setR_left(int r_left);
    void setScores(int N, int score);
    void setPlayerName(QString player_name);

    void Init_Round_Time();
    void Round_Time_add1s();

    LEVEL getLevel() { return level_; }
    QString getLevelText();
    QString getPlayerName() { return player_name_; }
    int getR_left() { return r_left_; }
    int getScores(int N) { return scores_[N]; }
    int getSum_Scores();
    QTime getRound_Time(){ return round_time_; }
    void generatePostion();
    double getPositionX(){return po.x;}
    double getPositionY(){return po.y;}

};

#endif // GAMEROUND_H
