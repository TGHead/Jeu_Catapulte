#include "catapulte.h"
#include "ui_catapulte.h"
#include <QGraphicsView>

Catapulte::Catapulte(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Catapulte)
{
    ui->setupUi(this);
    ui->Score_Table_->setVisible(false);
    ui->ReStart_Button_->setVisible(false);
    ui->Capture_Button_->setVisible(false);

    ui->s_runtime_->setVisible(false);
    ui->s_target_time_->setVisible(false);
    ui->s_scores_->setVisible(false);

    ui->v_runtime_->setVisible(false);
    ui->v_target_time_->setVisible(false);
    ui->v_scores_->setVisible(false);

    ui->centralWidget->setGeometry(0,0,width(),ui->SceneGL_->height());

    connect(ui->Start_Button_, SIGNAL(clicked()), this, SLOT(on_Start__clicked()));
}

Catapulte::~Catapulte()
{
    delete ui;
}

void Catapulte::on_Start__clicked()
{

    ui->Start_Button_->setEnabled(false);
    ui->Start_Button_->setVisible(false);

    ui->Score_Table_->setVisible(true);
    ui->ReStart_Button_->setVisible(true);
    ui->Capture_Button_->setVisible(true);

    ui->s_runtime_->setVisible(true);
    ui->s_target_time_->setVisible(true);
    ui->s_scores_->setVisible(true);

    ui->v_runtime_->setVisible(true);
    ui->v_target_time_->setVisible(true);
    ui->v_scores_->setVisible(true);
}
