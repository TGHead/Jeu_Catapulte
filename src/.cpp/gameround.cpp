#include "gameround.h"

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

QString GameRound::getLevelText()
{
    switch (level_) {
    case EASY:
        return "Easy";
        break;
    case NORMAL:
        return "Normal";
        break;
    case HARD:
        return "Hard";
        break;
    default:
        return "Easy";
        break;
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
