#ifndef GAMESETTING_H
#define GAMESETTING_H

#include "enum_level.h"
#include <QDialog>

namespace Ui {
class GameSetting;
}

class GameSetting : public QDialog
{
    Q_OBJECT

public:
    explicit GameSetting(QWidget *parent = 0);
    ~GameSetting();
    QString getPlayerName();
    LEVEL getLevel();

private:
    Ui::GameSetting *ui;
};

#endif // GAMESETTING_H
