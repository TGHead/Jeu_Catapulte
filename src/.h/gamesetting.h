#ifndef GAMESETTING_H
#define GAMESETTING_H

#include "enum_level.h"
#include <QDialog>
#include <QDialogButtonBox>

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
    QString getLevelText();
    QDialogButtonBox* getButtonBox();

private:
    Ui::GameSetting *ui;

//private slots:
//    void reject();
//    void accept();
};

#endif // GAMESETTING_H
