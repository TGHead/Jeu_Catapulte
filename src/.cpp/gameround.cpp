#include "src/.h/gameround.h"


GameRound::GameRound(LEVEL level, QString player_name)
{
    level_ = level;
    r_left_ = 10;
    for(int i=0;i<10;i++)
    {
        scores_[i] = 0;
    }
    Init_Round_Time();
    player_name_ = player_name;
    generatePostion();
}

GameRound::~GameRound()
{
}

void GameRound::setLevel(LEVEL level)
{
    level_ = level;
}

void GameRound::setR_left(int r_left)
{
    r_left_ = r_left;
}

void GameRound::setScores(int N, int score)
{
    scores_[N] = score;
}

void GameRound::setPlayerName(QString player_name)
{
    player_name_ = player_name;
}

void GameRound::Init_Round_Time()
{
    round_time_ = QTime(0, 0);
}

void GameRound::Round_Time_add1s()
{
    round_time_ = round_time_.addSecs(1);
}

void GameRound::calculScore(double d)
{
//    qDebug()<<"d:"<<d;
    switch (level_) {
    case EASY:
        if(d <= 10.0)
        {
            setScores(10 - getR_left(), 10 - ((int)d % 10));
        }
        break;
    case NORMAL:
        if(d <= 7.5)
        {
            setScores(10 - getR_left(), 10 - ((int)(d / 7.5 * 10) % 10));
        }
        break;
    case HARD:
        if(d <= 5.0)
        {
            setScores(10 - getR_left(), 10 - ((int)(d * 2) % 10));
        }
        break;
    }
}

QString GameRound::getLevelText()
{
    switch (level_) {
    case EASY:
        return "Easy";
    case NORMAL:
        return "Normal";
    case HARD:
        return "Hard";
    default:
        return "Easy";
    }
}

int GameRound::getSum_Scores()
{
    int sum = 0;
    for(int i = 0 ; i < 10 ; i++)
    {
        sum += scores_[i];
    }
    return sum;
}
/*
 * Generate the random position of the target with 80<x<135 and -35<y<35,only define the centre of the target
 */
void GameRound::generatePostion(){
    srand((unsigned)time(NULL));
    po.x=rand()%70-35;
    po.y=rand()%55+80;
}

