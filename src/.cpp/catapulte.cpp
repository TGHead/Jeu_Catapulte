#include "src/.h/catapulte.h"
#include "ui_catapulte.h"

#include <QGraphicsView>

#include <QDebug>

Catapulte::Catapulte(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Catapulte)
{
    ui->setupUi(this);

    ui->Score_Table_->setVisible(false);
    ui->ReStart_Button_->setVisible(false);
    ui->Capture_Button_->setVisible(false);
    ui->Fire_Button_->setVisible(false);
    ui->Next_Button_->setVisible(false);
    ui->Replay_Button_->setVisible(false);

    ui->s_player_name_->setVisible(false);
    ui->s_level_->setVisible(false);
    ui->s_runtime_->setVisible(false);
    ui->s_target_time_->setVisible(false);
    ui->s_target_left_->setVisible(false);
    ui->s_scores_->setVisible(false);

    ui->v_player_name_->setVisible(false);
    ui->v_level_->setVisible(false);
    ui->v_runtime_->setVisible(false);
    ui->v_target_time_->setVisible(false);
    ui->v_target_left_->setVisible(false);
    ui->v_scores_->setVisible(false);

    resize(width(), ui->SceneGL_->minimumHeight());

    captured_ = false;
    started_ = false;
    launched_ = false;

//    captured_location_[0] = 0.5;
//    captured_location_[1] = 0;

    f_timer_ = new QTimer(this);
    g_timer_ = new QTimer(this);
    round_ = NULL;
    setting_ = NULL;

    webCam_=new VideoCapture(0);
    int w=webCam_->get(CV_CAP_PROP_FRAME_WIDTH);
    int h=webCam_->get(CV_CAP_PROP_FRAME_HEIGHT);

    if(!webCam_->isOpened())  // check if we succeeded
    {
        ui->Frame_->setText("Error openning the default camera !");
    }
    else
    {
        ui->Frame_->setText(QString("Video ok, image size is %1x%2 pixels.\n"
                                    "(Click Start to show image)").arg(w).arg(h));
        ui->Start_Button_->setEnabled(true);
        connect(ui->Start_Button_, SIGNAL(clicked()), this, SLOT(Start_Button__clicked()));
    }
}

Catapulte::~Catapulte()
{
    delete ui;
    delete webCam_;
    delete f_timer_;
    delete g_timer_;
    if(round_ != NULL)
        delete round_;
    if(setting_ != NULL)
        delete setting_;
}

void Catapulte::Start_Button__clicked()
{
    setting_ = new GameSetting(this);
    connect(setting_->getButtonBox(),SIGNAL(accepted()),this,SLOT(GameSetting_Accepted()));
    connect(setting_->getButtonBox(),SIGNAL(rejected()),this,SLOT(GameSetting_Rejected()));
    setting_->exec();

    if(started_ == true)
    {
        ui->Start_Button_->setEnabled(false);
        ui->Start_Button_->setVisible(false);

        ui->Capture_Button_->setVisible(true);

        connect(ui->Capture_Button_, SIGNAL(clicked()), this, SLOT(Capture_Button__clicked()));
        connect(ui->ReStart_Button_, SIGNAL(clicked()), this, SLOT(Restart_Button__clicked()));
        connect(ui->Fire_Button_, SIGNAL(clicked()), this, SLOT(Fire_Button__clicked()));
        connect(this, SIGNAL(launch()), this, SLOT(Fire_Button__clicked()));
        connect(ui->Next_Button_, SIGNAL(clicked()), this, SLOT(Next_Button__clicked()));
        connect(ui->Replay_Button_, SIGNAL(clicked()), this, SLOT(Replay_Button__clicked()));

        connect(f_timer_, SIGNAL(timeout()),this, SLOT(afficherImage()));
        connect(g_timer_, SIGNAL(timeout()),this, SLOT(afficherGlobalTime()));
        connect(g_timer_, SIGNAL(timeout()),this, SLOT(afficherRoundTime()));

        f_timer_->start(20);

        started_ = false;
    }
}

void Catapulte::Capture_Button__clicked()
{
    ui->Capture_Button_->setEnabled(false);
    ui->Capture_Button_->setVisible(false);

    ui->Score_Table_->setVisible(true);

    ui->ReStart_Button_->setVisible(true);
    ui->Fire_Button_->setVisible(true);

    ui->s_player_name_->setVisible(true);
    ui->s_level_->setVisible(true);
    ui->s_runtime_->setVisible(true);
    ui->s_target_time_->setVisible(true);
    ui->s_target_left_->setVisible(true);
    ui->s_scores_->setVisible(true);

    ui->v_player_name_->setVisible(true);
    ui->v_level_->setVisible(true);
    ui->v_runtime_->setVisible(true);
    ui->v_target_time_->setVisible(true);
    ui->v_target_left_->setVisible(true);
    ui->v_scores_->setVisible(true);

    if(round_ != NULL)
        delete round_;

    round_ = new GameRound(setting_->getLevel(), setting_->getPlayerName());
    ui->SceneGL_->setRound(round_);
    ui->SceneGL_->updateGL();
    captured_ = true;
    launched_ = false;

    runtime_ = QTime(0, 0);
    g_timer_->start(1000);

    ui->v_player_name_->setText(round_->getPlayerName());
    ui->v_level_->setText(round_->getLevelText());
    ui->v_target_left_->setText(QString("%1").arg(round_->getR_left()));
    ui->v_scores_->setText(QString("%1").arg(round_->getSum_Scores()));
}

void Catapulte::Restart_Button__clicked()
{
    delete setting_;
    setting_ = new GameSetting(this);
    connect(setting_->getButtonBox(),SIGNAL(accepted()),this,SLOT(GameSetting_Accepted()));
    connect(setting_->getButtonBox(),SIGNAL(rejected()),this,SLOT(GameSetting_Rejected()));
    setting_->exec();

    if(started_ == true)
    {
        ui->Score_Table_->clearContents();
        ui->Score_Table_->setRowCount(0);

        ui->Capture_Button_->setEnabled(true);
        ui->Capture_Button_->setVisible(true);

    //    ui->Score_Table_->setVisible(false);
        ui->ReStart_Button_->setVisible(false);
        ui->Fire_Button_->setVisible(false);

        ui->Next_Button_->setVisible(false);
        ui->Replay_Button_->setVisible(false);

        ui->s_player_name_->setVisible(false);
        ui->s_level_->setVisible(false);
        ui->s_runtime_->setVisible(false);
        ui->s_target_time_->setVisible(false);
        ui->s_target_left_->setVisible(false);
        ui->s_scores_->setVisible(false);

        ui->v_player_name_->setVisible(false);
        ui->v_level_->setVisible(false);
        ui->v_runtime_->setVisible(false);
        ui->v_target_time_->setVisible(false);
        ui->v_target_left_->setVisible(false);
        ui->v_scores_->setVisible(false);

        resize(width(), minimumHeight());//?ISSUE 1 (Resize mainwindow failed when restart)?

        captured_ = false;

        g_timer_->stop();

        started_ = false;

        ui->SceneGL_->setCatapultAngle(0.5,1);
        ui->SceneGL_->setRound(NULL);
        ui->SceneGL_->updateGL();
    }
}

void Catapulte::Fire_Button__clicked()
{
    g_timer_->stop();
    launched_ = true;

    ui->Fire_Button_->setVisible(false);
    ui->Next_Button_->setVisible(true);
    ui->Next_Button_->setEnabled(false);
    ui->Replay_Button_->setVisible(true);
    ui->Replay_Button_->setEnabled(false);
    ui->ReStart_Button_->setEnabled(false);

    ui->SceneGL_->getCatapulteStatus()->AngleBackUp();
//    ui->SceneGL_->cameraBackup();
//    ui->SceneGL_->getCatapulteStatus()->setHSpeed();
    ui->SceneGL_->drawAnime();
//    ui->SceneGL_->cameraRecover();

    ui->Next_Button_->setEnabled(true);
    ui->Replay_Button_->setEnabled(true);
    ui->ReStart_Button_->setEnabled(true);

    ui->Score_Table_->setRowCount(10 - round_->getR_left() + 1);
    ui->Score_Table_->setItem(10 - round_->getR_left(), 0, new QTableWidgetItem(QString("%1").arg(10 - round_->getR_left() + 1)));
    ui->Score_Table_->setItem(10 - round_->getR_left(), 1, new QTableWidgetItem(round_->getPlayerName()));
    ui->Score_Table_->setItem(10 - round_->getR_left(), 2, new QTableWidgetItem(ui->v_runtime_->text()));
    ui->Score_Table_->setItem(10 - round_->getR_left(), 3, new QTableWidgetItem(ui->v_target_time_->text()));
    ui->Score_Table_->setItem(10 - round_->getR_left(), 4, new QTableWidgetItem(QString("%1").arg(round_->getScores(10 - round_->getR_left()))));
    for(int i = 0 ; i < 5 ; i++)
    {
        QTableWidgetItem *item = ui->Score_Table_->item(10 - round_->getR_left(), i);
        item -> setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }

    round_->setR_left(round_->getR_left() - 1);
//    round_->Init_Round_Time();

//    ui->v_target_time_->setText(round_->getRound_Time().toString());
    ui->v_target_left_->setText(QString("%1").arg(round_->getR_left()));
    ui->v_scores_->setText(QString("%1").arg(round_->getSum_Scores()));

    if(round_->getR_left() == 0)
    {
        g_timer_->stop();
        ui->Fire_Button_->setEnabled(false);
    }
}

void Catapulte::Next_Button__clicked()
{
    round_->Init_Round_Time();
    ui->v_target_time_->setText(round_->getRound_Time().toString());
    g_timer_->start(1000);

    launched_ = false;
    ui->SceneGL_->setFiringFlag(false);
    ui->SceneGL_->updateGL();

    ui->Fire_Button_->setVisible(true);
    ui->Next_Button_->setVisible(false);
    ui->Replay_Button_->setVisible(false);
    round_->generatePostion();
}

void Catapulte::Replay_Button__clicked()
{
    ui->Next_Button_->setEnabled(false);
    ui->Replay_Button_->setEnabled(false);
    ui->ReStart_Button_->setEnabled(false);

    ui->SceneGL_->getCatapulteStatus()->AngleRecover();
    ui->SceneGL_->drawAnime();

    ui->Next_Button_->setEnabled(true);
    ui->Replay_Button_->setEnabled(true);
    ui->ReStart_Button_->setEnabled(true);
}

void Catapulte::GameSetting_Accepted()
{
    started_ = true;
}

void Catapulte::GameSetting_Rejected()
{
    started_ = false;
}

void Catapulte::afficherImage()
{
    Mat image;

    if (webCam_->read(image)) {   // Capture a frame

        flip(image,image,1);

        if(captured_ == false)
        {
            Rect templateRect(webCam_->get(CV_CAP_PROP_FRAME_WIDTH)/2 - 48,webCam_->get(CV_CAP_PROP_FRAME_HEIGHT)/2 - 48,96,96);
            last_Pos_.x = webCam_->get(CV_CAP_PROP_FRAME_WIDTH)/2;
            last_Pos_.y = webCam_->get(CV_CAP_PROP_FRAME_HEIGHT)/2;
            templateImage_=Mat(image,templateRect).clone();
            cvtColor(image,image,CV_BGR2RGB);
            rectangle(image,templateRect,Scalar( 255, 0, 0),2,8,0);
        }
        else
        {
            Mat resultImage;    // to store the matchTemplate result
            int result_cols = image.cols - templateImage_.cols + 1;
            int result_rows = image.rows - templateImage_.rows + 1;
            resultImage.create( result_cols, result_rows, CV_32FC1 );

            matchTemplate( image, templateImage_, resultImage, TM_CCORR_NORMED );

            double minVal; double maxVal; Point minLoc; Point maxLoc;
            minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
//            qDebug()<<maxLoc.y - last_Pos_.y;
            if(maxLoc.y - last_Pos_.y > 100 && !launched_) emit launch();
            last_Pos_ = maxLoc;
            if(!launched_)
            {
                ui->SceneGL_->setCatapultAngle((float)maxLoc.x/(float)webCam_->get(CV_CAP_PROP_FRAME_WIDTH),
                                               (float)maxLoc.y/(float)webCam_->get(CV_CAP_PROP_FRAME_HEIGHT));
                ui->SceneGL_->updateGL();
            }

            Rect resultRect=Rect(maxLoc.x,maxLoc.y,96,96);
            cvtColor(image,image,CV_BGR2RGB);
            rectangle(image,resultRect,Scalar( 0, 255, 0),2,8,0);
        }

        QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
        img = img.scaled(ui->Frame_->width(),ui->Frame_->height(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        ui->Frame_->setPixmap(QPixmap::fromImage(img));

    }
    else {
        ui->Frame_->setText("Error capturing the frame");
    }
}

void Catapulte::afficherGlobalTime()
{
    runtime_ = runtime_.addSecs(1);// +1s
    ui->v_runtime_->setText(runtime_.toString());
}

void Catapulte::afficherRoundTime()
{
    round_ -> Round_Time_add1s();
    ui->v_target_time_->setText(round_->getRound_Time().toString());
}
