#ifndef GAMESETTING_H
#define GAMESETTING_H

#include "enum_level.h"
#include <QDialog>
#include <QDialogButtonBox>

namespace Ui {
class GameSetting;
}

/*
 * This class is designed to generate a small dialog to let player enter his name and choose the game level
 * Created an instance when Start or Restart button clicked
 */

class GameSetting : public QDialog
{
    Q_OBJECT

public:
    explicit GameSetting(QWidget *parent = 0);
    ~GameSetting();

    QString getPlayerName();// get player's name from GUI
    LEVEL getLevel();// get game level from GUI and return LEVEL variable
    QString getLevelText();// get game level from GUI and return QString object
    QDialogButtonBox* getButtonBox();//get GUI buttonbox, used to connect it with signal and slot functions

private:
    Ui::GameSetting *ui;

};

#endif // GAMESETTING_H
