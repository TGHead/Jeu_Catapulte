#ifndef GAMEROUND_H
#define GAMEROUND_H

#include "enum_level.h"
#include <QTime>

class GameRound
{

private:

    LEVEL level_;
    int r_left_;
    int scores_[10];
    QTime round_time_;
    QString player_name_;

public:

    explicit GameRound();
    ~GameRound();
    void setLevel(LEVEL level);
    void setR_left(int r_left);
    void setScores(int N, int score);

    void Init_Round_Time();
    void Round_Time_add1s();

    LEVEL getLevel() { return level_; }
    int getR_left() { return r_left_; }
    int getScores(int N) { return scores_[N]; }
    int getSum_Scores();
    QTime getRound_Time(){ return round_time_; }

};

#endif // GAMEROUND_H
