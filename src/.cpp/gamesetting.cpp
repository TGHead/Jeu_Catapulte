#include "src/.h/gamesetting.h"
#include "ui_gamesetting.h"

GameSetting::GameSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameSetting)
{
    ui->setupUi(this);
}

GameSetting::~GameSetting()
{
    delete ui;
}

QString GameSetting::getPlayerName()
{
    return ui->playernameedit_->displayText();
}

LEVEL GameSetting::getLevel()
{
//    return ui->lvlselect_->currentData();
    int idx = ui->lvlselect_->currentIndex();

    switch (idx) {
    case 0:
        return EASY;
        break;
    case 1:
        return NORMAL;
        break;
    case 2:
        return HARD;
        break;
    default:
        return EASY;
        break;
    }
}

QString GameSetting::getLevelText()
{
    return ui->lvlselect_->currentText();
}

QDialogButtonBox* GameSetting::getButtonBox()
{
    return ui->buttonBox_;
}

//void GameSetting::reject()
//{

//}

//void GameSetting::accept()
//{

//}

