//-----------------------------------------------------------------------------
#include <QString>
#include <QColor>
#include <QDialog>
#include <QVector>
#include <QtNetwork/QUdpSocket>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QStatusBar>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QDesktopWidget>
#include <QSettings>
#include <QRgb>
#include <QButtonGroup>
#include <QEvent>
#include <QResizeEvent>
#include <QScrollBar>
//-----------------------------------------------------------------------------
#include <memory.h>
#if defined(Q_OS_LINUX)
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
#elif defined(Q_OS_WIN)
    #include <winsock.h>
#endif
//-----------------------------------------------------------------------------
#include "mainwindow.h"
#include "ui_mainwindow.h"
//-----------------------------------------------------------------------------
#include "vak_8.h"
#include "vak_8_2pc.h"
#include "qt_vak_32_fkd.h"
#include "qt_deptcol.h"
#include "qt_ml.h"
#include "qt_vk.h"
//-----------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    app_settings(new QSettings("TNG-Group", "qt_akp_reg")),
//    check_state(parent),
    akp_file(parent)
{
    //-------------------------------------------------------------------------
    qRegisterMetaType<TDataPocket>("TDataPocket");
    qRegisterMetaType<TCtrlPocket>("TCtrlPocket");
    //-------------------------------------------------------------------
//    qRegisterMetaType<TVAK_8_DATA>("TVAK_8_DATA");
//    qRegisterMetaType<TVAK8_WAVE>("TVAK8_WAVE");
    //-------------------------------------------------------------------------
    file_thread = new QThread();
    akp_file.moveToThread(file_thread);
    connect(file_thread, &QThread::started,           &akp_file,     &qt_akp_file_read::start );
    connect(&akp_file,   &qt_akp_file_read::finished, file_thread,   &QThread::quit           );
    connect(file_thread, &QThread::finished,          file_thread,   &QThread::deleteLater    );
    //-------------------------------------------------------------------------
    ui->setupUi(this);
    // setWindowIcon(QIcon(":/icons/images/TNG.ico"));

    ui->pushButton_Up->setDisabled(true);
    ui->pushButton_Down->setDisabled(true);

    QCoreApplication::setOrganizationName(QString::fromUtf8("TNG-Group"));
    QCoreApplication::setApplicationName(QString::fromUtf8("qt_akp_view"));
    //-------------------------------------------------------------------------
    load_settings();
    //-------------------------------------------------------------------------
    label_Depth = new QLabel(QString::fromUtf8("Глубина: 99999.99 м"));
    label_Depth->setAlignment(Qt::AlignLeft);
    label_Depth->setMinimumSize(label_Depth->sizeHint());
    statusBar()->addWidget(label_Depth);
    label_Depth->setText(QString::fromUtf8("Глубина: %1 м").arg( Depth / 100.0, 0, 'f', 2));

    label_Distance = new QLabel(QString::fromUtf8("Пройдено: 9999.99 м"));
    label_Distance->setAlignment(Qt::AlignLeft);
    label_Distance->setMinimumSize(label_Distance->sizeHint());
    statusBar()->addWidget(label_Distance);
    label_Distance->setText(QString::fromUtf8("Пройдено: 0.00 м"));

//    label_Velocity = new QLabel(QString::fromUtf8("Скорость: 9999 м/ч"));
//    label_Velocity->setAlignment(Qt::AlignLeft);
//    label_Velocity->setMinimumSize(label_Velocity->sizeHint());
//    statusBar()->addWidget(label_Velocity);
//    label_Velocity->setText(QString::fromUtf8("Скорость: 0 м/ч"));

    label_GoodBlk = new QLabel(QString::fromUtf8("Хорошие блоки: 999999999"));
    label_GoodBlk->setAlignment(Qt::AlignLeft);
    label_GoodBlk->setMinimumSize(label_GoodBlk->sizeHint());
    statusBar()->addWidget(label_GoodBlk);
    label_GoodBlk->setText(QString::fromUtf8("Хорошие блоки: 0"));

    label_BadBlk = new QLabel(QString::fromUtf8("Плохие блоки: 999999999"));
    label_BadBlk->setAlignment(Qt::AlignLeft);
    label_BadBlk->setMinimumSize(label_BadBlk->sizeHint());
    statusBar()->addWidget(label_BadBlk);
    label_BadBlk->setText(QString::fromUtf8("Плохие блоки: 0"));

    label_CRC_check = new QLabel(QString::fromUtf8("CRC1: ОШИБКА!!!"));
    label_CRC_check->setAlignment(Qt::AlignLeft);
    label_CRC_check->setMinimumSize(label_CRC_check->sizeHint());
    statusBar()->addWidget(label_CRC_check);
    label_CRC_check->setText(QString::fromUtf8(""));

//    label_CRC2_check = new QLabel(QString::fromUtf8("CRC2: ОШИБКА!!!"));
//    label_CRC2_check->setAlignment(Qt::AlignLeft);
//    label_CRC2_check->setMinimumSize(label_CRC2_check->sizeHint());
//    statusBar()->addWidget(label_CRC2_check);
//    label_CRC2_check->setText(QString::fromUtf8(""));

//    label_CRC3_check = new QLabel(QString::fromUtf8("CRC3: ОШИБКА!!!"));
//    label_CRC3_check->setAlignment(Qt::AlignLeft);
//    label_CRC3_check->setMinimumSize(label_CRC3_check->sizeHint());
//    statusBar()->addWidget(label_CRC3_check);
//    label_CRC3_check->setText(QString::fromUtf8(""));

//    label_CRC4_check = new QLabel(QString::fromUtf8("CRC4: ОШИБКА!!!"));
//    label_CRC4_check->setAlignment(Qt::AlignLeft);
//    label_CRC4_check->setMinimumSize(label_CRC4_check->sizeHint());
//    statusBar()->addWidget(label_CRC4_check);
//    label_CRC4_check->setText(QString::fromUtf8(""));

//    label_CRC5_check = new QLabel(QString::fromUtf8("CRC5: ОШИБКА!!!"));
//    label_CRC5_check->setAlignment(Qt::AlignLeft);
//    label_CRC5_check->setMinimumSize(label_CRC5_check->sizeHint());
//    statusBar()->addWidget(label_CRC5_check);
//    label_CRC5_check->setText(QString::fromUtf8(""));

//    label_CRC6_check = new QLabel(QString::fromUtf8("CRC6: ОШИБКА!!!"));
//    label_CRC6_check->setAlignment(Qt::AlignLeft);
//    label_CRC6_check->setMinimumSize(label_CRC6_check->sizeHint());
//    statusBar()->addWidget(label_CRC6_check);
//    label_CRC6_check->setText(QString::fromUtf8(""));

//    label_CRC7_check = new QLabel(QString::fromUtf8("CRC7: ОШИБКА!!!"));
//    label_CRC7_check->setAlignment(Qt::AlignLeft);
//    label_CRC7_check->setMinimumSize(label_CRC7_check->sizeHint());
//    statusBar()->addWidget(label_CRC7_check);
//    label_CRC7_check->setText(QString::fromUtf8(""));

//    label_CRC8_check = new QLabel(QString::fromUtf8("CRC8: ОШИБКА!!!"));
//    label_CRC8_check->setAlignment(Qt::AlignLeft);
//    label_CRC8_check->setMinimumSize(label_CRC8_check->sizeHint());
//    statusBar()->addWidget(label_CRC8_check);
//    label_CRC8_check->setText(QString::fromUtf8(""));

    label_VK1time = new QLabel(QString::fromUtf8("Т1: 1024 мкс "));
    label_VK1time->setAlignment(Qt::AlignLeft);
    label_VK1time->setMinimumSize(label_VK1time->sizeHint());
    statusBar()->addWidget(label_VK1time);
    label_VK1time->setText(QString::fromUtf8(""));

    label_VK1ampl = new QLabel(QString::fromUtf8("А1: 65536 "));
    label_VK1ampl->setAlignment(Qt::AlignLeft);
    label_VK1ampl->setMinimumSize(label_VK1ampl->sizeHint());
    statusBar()->addWidget(label_VK1ampl);
    label_VK1ampl->setText(QString::fromUtf8(""));

    label_VK2time = new QLabel(QString::fromUtf8("Т2: 1024 мкс "));
    label_VK2time->setAlignment(Qt::AlignLeft);
    label_VK2time->setMinimumSize(label_VK2time->sizeHint());
    statusBar()->addWidget(label_VK2time);
    label_VK2time->setText(QString::fromUtf8(""));

    label_VK2ampl = new QLabel(QString::fromUtf8("А2: 65536 "));
    label_VK2ampl->setAlignment(Qt::AlignLeft);
    label_VK2ampl->setMinimumSize(label_VK2ampl->sizeHint());
    statusBar()->addWidget(label_VK2ampl);
    label_VK2ampl->setText(QString::fromUtf8(""));

    label_ML = new QLabel(QString::fromUtf8("MM"));
    label_ML->setAlignment(Qt::AlignCenter);

    QSize ML_Size = label_ML->sizeHint();
    ML_Size.setWidth(ML_Size.width() * 2);

    label_ML->setMinimumSize(ML_Size);
//    label_ML->setFrameStyle(QFrame::Box);
    QColor color(Qt::red);
    color = palette().window().color();
    label_ML->setStyleSheet(QString::fromUtf8("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
    statusBar()->addPermanentWidget(label_ML);
    //-------------------------------------------------------------------------
//    connect(&check_state,  &akp_check_state::VK_update,             this, &MainWindow::on_showNewData       );
//    connect(&check_state,  &akp_check_state::good_blk_cnt_update,   this, &MainWindow::on_showPocketCount   );
//    connect(&check_state,  &akp_check_state::bad_blk_cnt_update,    this, &MainWindow::on_showBadPocketCount);

//    connect(&check_state,  &akp_check_state::izl_type_update,       this, &MainWindow::on_showIZLtype       );
//    connect(&check_state,  &akp_check_state::Fsig_update,           this, &MainWindow::on_showIZLfreq       );
//    connect(&check_state,  &akp_check_state::izl_periods_update,    this, &MainWindow::on_showIZLnum        );

//    connect(&check_state,  &akp_check_state::rx_type_update,        this, &MainWindow::on_showRXtype        );
//    connect(&check_state,  &akp_check_state::rx_delay_update,       this, &MainWindow::on_showRXdelay       );
//    connect(&check_state,  &akp_check_state::Td_update,             this, &MainWindow::on_showRXtd          );
//    connect(&check_state,  &akp_check_state::Ku_update,             this, &MainWindow::on_showRXku          );

//    connect(&check_state,  &akp_check_state::tool_no_update,        this, &MainWindow::on_showToolNo        );
//    connect(&check_state,  &akp_check_state::soft_version_update,   this, &MainWindow::on_showSoftVer       );

//    connect(&check_state,  &akp_check_state::time_meserment_update, this, &MainWindow::on_showTimeMeserment );

//    connect(&check_state,  &akp_check_state::CRC1_update,           this, &MainWindow::on_showCRC1          );
//    connect(&check_state,  &akp_check_state::CRC2_update,           this, &MainWindow::on_showCRC2          );
//    connect(&check_state,  &akp_check_state::CRC3_update,           this, &MainWindow::on_showCRC3          );
//    connect(&check_state,  &akp_check_state::CRC4_update,           this, &MainWindow::on_showCRC4          );
//    connect(&check_state,  &akp_check_state::CRC5_update,           this, &MainWindow::on_showCRC5          );
//    connect(&check_state,  &akp_check_state::CRC6_update,           this, &MainWindow::on_showCRC6          );
//    connect(&check_state,  &akp_check_state::CRC7_update,           this, &MainWindow::on_showCRC7          );
//    connect(&check_state,  &akp_check_state::CRC8_update,           this, &MainWindow::on_showCRC8          );
    //-------------------------------------------------------------------------
    connect(ui->pushButton_FileOpen, &QPushButton::pressed,         this, &MainWindow::on_pushButtonFileOpen);
    connect(ui->pushButton_Down,     &QPushButton::clicked,         this, &MainWindow::on_pushButtonDown    );
    connect(ui->pushButton_Up,       &QPushButton::clicked,         this, &MainWindow::on_pushButtonUp      );
//    connect(ui->pushButton_Down,     &QPushButton::pressed,         this, &MainWindow::on_pushButtonDown    );
//    connect(ui->pushButton_Up,       &QPushButton::pressed,         this, &MainWindow::on_pushButtonUp      );

    connect(ui->pushButton_incAmpl,  &QPushButton::pressed,         this, &MainWindow::on_cmdIncAmpl        );
    connect(ui->pushButton_decAmpl,  &QPushButton::pressed,         this, &MainWindow::on_cmdDecAmpl        );
    connect(ui->pushButton_upFKD,    &QPushButton::pressed,         this, &MainWindow::on_cmdIncLevel       );
    connect(ui->pushButton_downFKD,  &QPushButton::pressed,         this, &MainWindow::on_cmdDecLevel       );

    connect(ui->verticalScrollBar,   &QScrollBar::valueChanged,     this, &MainWindow::on_show              );
    //-------------------------------------------------------------------------

    connect(this,                   &MainWindow::cmdSetDepth,       this,           &MainWindow::on_showDept            );

//    connect(&akp,           &akp_class::dataUpdate,         &akp_file,      &qt_akp_file_save::onDataUpdate );

//    connect(&check_state,   &akp_check_state::deptUpdate,   this,           &MainWindow::on_showDept        );
//    connect(&check_state,   &akp_check_state::mlUpdate,     this,           &MainWindow::on_showML          );

//    connect(&check_state,   SIGNAL( timer_clk_update           (const bool, const quint32) ), this, SLOT(on_timerClkUpdate(const bool, const quint32)          ) );
//    connect(&check_state,   SIGNAL( time_start_meserment_update(const bool, const quint32) ), this, SLOT(on_timeStartMesermentUpdate(const bool, const quint32)) );
//    connect(&check_state,   SIGNAL( time_stop_meserment_update (const bool, const quint32) ), this, SLOT(on_timeStopMesermentUpdate (const bool, const quint32)) );

//    connect(&check_state,   SIGNAL( CRC1_update(const bool) ), this, SLOT(on_CRC1update(const bool)) );
//    connect(&check_state,   SIGNAL( CRC2_update(const bool) ), this, SLOT(on_CRC2update(const bool)) );
//    connect(&check_state,   SIGNAL( CRC3_update(const bool) ), this, SLOT(on_CRC3update(const bool)) );
//    connect(&check_state,   SIGNAL( CRC4_update(const bool) ), this, SLOT(on_CRC4update(const bool)) );
//    connect(&check_state,   SIGNAL( CRC5_update(const bool) ), this, SLOT(on_CRC5update(const bool)) );
//    connect(&check_state,   SIGNAL( CRC6_update(const bool) ), this, SLOT(on_CRC6update(const bool)) );
//    connect(&check_state,   SIGNAL( CRC7_update(const bool) ), this, SLOT(on_CRC7update(const bool)) );
//    connect(&check_state,   SIGNAL( CRC8_update(const bool) ), this, SLOT(on_CRC8update(const bool)) );

    //-------------------------------------------------------------------------
    QRect  rect;
    QRectF rectf;
    rectf.setRect(0.0, 0.0, 100.0, 100.0);

    // scene_vk1 = new QGraphicsScene(rectf);
    scene_vk1 = new CustomScene(rectf);

    view_vk1 = new QGraphicsView(scene_vk1);
    view_vk1->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    view_vk1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_vk1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_vk1->scale(1.0, 1.0);

    // rect.setRect(0, 0, VAK_8_NUM_POINTS * FKDstep, 380);
    if (FKDstep > 0)
    {
        rect.setRect(0, 0, VAK_8_NUM_POINTS * FKDstep, ui->widget_VK1->height() );
    }
    else
    {
        rect.setRect(0, 0, VAK_8_NUM_POINTS, ui->widget_VK1->height() );
    }

    vk1 = new Qt_VK(rect, scene_vk1);
    vk1->setColorLine(VKColor);
    vk1->setColorBack(FonColor);
    vk1->setColorText(TextColor);
    vk1->setMaxAmpl(max_ampl);
    vk1->setXScale(FKDstep);
    vk1->setColorLevelBack(LevelColor);
    vk1->setCaption(QString::fromUtf8("ВК-1 "));

    connect(this, &MainWindow::changeFKDstep,   vk1,        &Qt_VK::on_changeTimeScale  );
    connect(this, &MainWindow::changeFKDlevel,  vk1,        &Qt_VK::on_changeLevel      );
    connect(this, &MainWindow::changeVKmaxAmpl, vk1,        &Qt_VK::on_changeMaxAmpl    );
    connect(vk1,  SIGNAL( update() ),           scene_vk1,  SLOT( update()            ) );

    vk1_greed = new Qt_biGREED(rect, scene_vk1);
    vk1_greed->setColorLine(GreedColor);
    vk1_greed->setColorBack(Qt::transparent);
    vk1_greed->setLineWidth(1);
    vk1_greed->set_step_x_line(128);
    vk1_greed->set_num_y_line(4);

    time_line1 = new Qt_TIME_LINE(rect, scene_vk1);
    time_line1->setColorText(TextColor);
    time_line1->setColorBack(Qt::transparent);
    time_line1->set_step_x_line(128);
    time_line1->set_time_zero(0);
    time_line1->set_time_step(2);       // Td 2mks
    time_line1->set_x_scale(FKDstep);

    connect(this, &MainWindow::changeFKDstep,   time_line1, &Qt_TIME_LINE::on_changeTimeScale );

    // view_vk1->setMouseTracking(true);
    // connect (scene_vk1, &CustomScene::signalTargetCoordinate, this, &MainWindow::on_changeVK1pos);

    ui->horizontalLayout_VK1->addWidget(view_vk1);
    //-------------------------------------------------------------------------
    rectf.setRect(0.0, 0.0, 100.0, 100.0);
    scene_vk1_fkd = new QGraphicsScene(rectf);

    view_vk1_fkd = new QGraphicsView(scene_vk1_fkd);
    view_vk1_fkd->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    view_vk1_fkd->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_vk1_fkd->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_vk1_fkd->scale(1.0, 1.0);

//    rect.setRect(0, 0, VAK_8_NUM_POINTS * FKDstep, 200);
    if (FKDstep > 0)
    {
        rect.setRect(0, 0, VAK_8_NUM_POINTS * FKDstep, ui->widget_VK1_FKD->height() );
    }
    else
    {
        rect.setRect(0, 0, VAK_8_NUM_POINTS, ui->widget_VK1_FKD->height() );
    }
    vk1_fkd = new CVAK32_FKD(rect, scene_vk1_fkd);
    vk1_fkd->setColorBack(FonColor);
    vk1_fkd->setColorGreed(GreedColor);
    vk1_fkd->setColorLine(FKDColor);
    vk1_fkd->on_changeGreedStepX(128);
    vk1_fkd->on_changeDpsX(dpsX);
    vk1_fkd->on_changeDpsY(dpsY);
    vk1_fkd->on_changeDeptScale(DepthScale);
    vk1_fkd->on_setDirectionUp();
    vk1_fkd->on_changeTimeScale(FKDstep);

    ui->horizontalLayout_VK1_FKD->addWidget(view_vk1_fkd);

    connect(this,           &MainWindow::cmdSetDepth,       vk1_fkd,         &CVAK32_FKD::on_changeDept         );
    connect(this,           &MainWindow::changeDpsX,        vk1_fkd,         &CVAK32_FKD::on_changeDpsX         );
    connect(this,           &MainWindow::changeDpsY,        vk1_fkd,         &CVAK32_FKD::on_changeDpsY         );
    connect(this,           &MainWindow::changeDeptScale,   vk1_fkd,         &CVAK32_FKD::on_changeDeptScale    );
    connect(this,           &MainWindow::changeFKDstep,     vk1_fkd,         &CVAK32_FKD::on_changeTimeScale    );
    connect(this,           &MainWindow::changeFKDlevel,    vk1_fkd,         &CVAK32_FKD::on_changeLevel        );
    connect(vk1_fkd,        SIGNAL( update() ),             scene_vk1_fkd,   SLOT( update() )                   );

    //-------------------------------------------------------------------------
//    QRect  rect;
//    QRectF rectf;
    rectf.setRect(0.0, 0.0, 100.0, 100.0);

    // scene_vk2 = new QGraphicsScene(rectf);
    scene_vk2 = new CustomScene(rectf);

    view_vk2 = new QGraphicsView(scene_vk2);
    view_vk2->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    view_vk2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_vk2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_vk2->scale(1.0, 1.0);

    // rect.setRect(0, 0, VAK_8_NUM_POINTS * FKDstep, 380);
    if (FKDstep > 0)
    {
        rect.setRect(0, 0, VAK_8_NUM_POINTS * FKDstep, ui->widget_VK2->height() );
    }
    else
    {
        rect.setRect(0, 0, VAK_8_NUM_POINTS, ui->widget_VK2->height() );
    }

    vk2 = new Qt_VK(rect, scene_vk2);
    vk2->setColorLine(VKColor);
    vk2->setColorBack(FonColor);
    vk2->setColorText(TextColor);
    vk2->setMaxAmpl(max_ampl);
    vk2->setXScale(FKDstep);
    vk2->setColorLevelBack(LevelColor);
    vk2->setCaption(QString::fromUtf8("ВК-2"));

    connect(this, &MainWindow::changeFKDstep,   vk2,        &Qt_VK::on_changeTimeScale  );
    connect(this, &MainWindow::changeFKDlevel,  vk2,        &Qt_VK::on_changeLevel      );
    connect(this, &MainWindow::changeVKmaxAmpl, vk2,        &Qt_VK::on_changeMaxAmpl    );
    connect(vk2,  SIGNAL( update() ),           scene_vk2,  SLOT( update()            ) );

    vk2_greed = new Qt_biGREED(rect, scene_vk2);
    vk2_greed->setColorLine(GreedColor);
    vk2_greed->setColorBack(Qt::transparent);
    vk2_greed->setLineWidth(1);
    vk2_greed->set_step_x_line(128);
    vk2_greed->set_num_y_line(4);

    time_line2 = new Qt_TIME_LINE(rect, scene_vk2);
    time_line2->setColorText(TextColor);
    time_line2->setColorBack(Qt::transparent);
    time_line2->set_step_x_line(128);
    time_line2->set_time_zero(0);
    time_line2->set_time_step(2);       // Td 2mks
    time_line2->set_x_scale(FKDstep);

    connect(this, &MainWindow::changeFKDstep,   time_line2, &Qt_TIME_LINE::on_changeTimeScale );

    // view_vk2->setMouseTracking(true);
    // connect (scene_vk2, &CustomScene::signalTargetCoordinate, this, &MainWindow::on_changeVK2pos);

    ui->horizontalLayout_VK2->addWidget(view_vk2);
    //-------------------------------------------------------------------------
    rectf.setRect(0.0, 0.0, 100.0, 100.0);
    scene_vk2_fkd = new QGraphicsScene(rectf);

    view_vk2_fkd = new QGraphicsView(scene_vk2_fkd);
    view_vk2_fkd->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    view_vk2_fkd->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_vk2_fkd->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_vk2_fkd->scale(1.0, 1.0);

    // rect.setRect(0, 0, VAK_8_NUM_POINTS * FKDstep, 200);
    if (FKDstep > 0)
    {
        rect.setRect(0, 0, VAK_8_NUM_POINTS * FKDstep, ui->widget_VK2_FKD->height() );
    }
    else
    {
        rect.setRect(0, 0, VAK_8_NUM_POINTS, ui->widget_VK2_FKD->height() );
    }
    vk2_fkd = new CVAK32_FKD(rect, scene_vk2_fkd);
    vk2_fkd->setColorBack(FonColor);
    vk2_fkd->setColorGreed(GreedColor);
    vk2_fkd->setColorLine(FKDColor);
    vk2_fkd->on_changeGreedStepX(128);
    vk2_fkd->on_changeDpsX(dpsX);
    vk2_fkd->on_changeDpsY(dpsY);
    vk2_fkd->on_changeDeptScale(DepthScale);
    vk2_fkd->on_setDirectionUp();
    vk2_fkd->on_changeTimeScale(FKDstep);

    ui->horizontalLayout_VK2_FKD->addWidget(view_vk2_fkd);

    connect(this,           &MainWindow::cmdSetDepth,       vk2_fkd,         &CVAK32_FKD::on_changeDept         );
    connect(this,           &MainWindow::changeDpsX,        vk2_fkd,         &CVAK32_FKD::on_changeDpsX         );
    connect(this,           &MainWindow::changeDpsY,        vk2_fkd,         &CVAK32_FKD::on_changeDpsY         );
    connect(this,           &MainWindow::changeDeptScale,   vk2_fkd,         &CVAK32_FKD::on_changeDeptScale    );
    connect(this,           &MainWindow::changeFKDstep,     vk2_fkd,         &CVAK32_FKD::on_changeTimeScale    );
    connect(this,           &MainWindow::changeFKDlevel,    vk2_fkd,         &CVAK32_FKD::on_changeLevel        );
    connect(vk2_fkd,        SIGNAL( update() ),             scene_vk2_fkd,   SLOT( update() )                   );
    // connect(vk2_fkd,        &CVAK32_FKD::update,            scene_vk2_fkd,   &QGraphicsScene::update            );

    //-------------------------------------------------------------------------
    rectf.setRect(0.0, 0.0, 80.0, 150.0);
    scene_dept_col = new QGraphicsScene(rectf);

    view_dept_col = new QGraphicsView(scene_dept_col);
    view_dept_col->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    view_dept_col->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_dept_col->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    rect.setRect(0, 0, 80, 150);
    deptCol = new Qt_DEPTCOL(rect, scene_dept_col);
    deptCol->setColorBack(FonColor);
    deptCol->setColorLine(GreedColor);
    deptCol->setColorText(TextColor);
    deptCol->on_changeDpsY(dpsY);
    deptCol->on_changeDeptScale(DepthScale);
    deptCol->on_setDirectionUp();

    ui->verticalLayout_dept_col->addWidget(view_dept_col);

    connect(this,           &MainWindow::cmdSetDepth,           deptCol,         &Qt_DEPTCOL::on_changeDept             );
    connect(this,           &MainWindow::changeDpsY,            deptCol,         &Qt_DEPTCOL::on_changeDpsY             );
    connect(this,           &MainWindow::changeDeptScale,       deptCol,         &Qt_DEPTCOL::on_changeDeptScale        );
    connect(this,           &MainWindow::cmdSetDepth,           deptCol,         &Qt_DEPTCOL::on_changeDept             );
    connect(deptCol,        SIGNAL( update() ),                 scene_dept_col,  SLOT( update()                       ) );
    connect(deptCol,        &Qt_DEPTCOL::changeBaseLinesShift,  vk1_fkd,         &CVAK32_FKD::on_changeBaseLinesShift   );
    connect(deptCol,        &Qt_DEPTCOL::changeBaseLinesShift,  vk2_fkd,         &CVAK32_FKD::on_changeBaseLinesShift   );

    //-------------------------------------------------------------------------
    rectf.setRect(0.0, 0.0, 10.0, 150.0);
    scene_ml_col = new QGraphicsScene(rectf);

    view_ml_col = new QGraphicsView(scene_ml_col);
    view_ml_col->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    view_ml_col->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_ml_col->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    rect.setRect(0, 0, 5, 150);
    mlCol = new Qt_ML(rect, scene_ml_col);
    mlCol->setColorBack(FonColor);
    mlCol->setColorML(MMColor);
    mlCol->on_changeDpsY(dpsY);
    mlCol->on_changeDeptScale(DepthScale);
    mlCol->on_setDirectionUp();
    mlCol->setLineWidth(2);

    ui->verticalLayout_ML->addWidget(view_ml_col);

    connect(this,           &MainWindow::cmdSetDepth,           mlCol,        &Qt_ML::on_changeDept         );
    connect(this,           &MainWindow::changeDpsY,            mlCol,        &Qt_ML::on_changeDpsY         );
    connect(this,           &MainWindow::changeDeptScale,       mlCol,        &Qt_ML::on_changeDeptScale    );
    connect(mlCol,          SIGNAL( update() ),                 scene_ml_col, SLOT( update()              ) );
    //-------------------------------------------------------------------------
    file_thread->start();
    //-------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    //-------------------------------------------------------------------------
    // save_settings();
    //-------------------------------------------------------------------------
    delete mlCol;
    delete view_ml_col;
    delete scene_ml_col;
    //-------------------------------------------------------------------------
    delete deptCol;
    delete view_dept_col;
    delete scene_dept_col;
    //-------------------------------------------------------------------------
    delete vk2_fkd;
    delete view_vk2_fkd;
    delete scene_vk2_fkd;

    delete time_line2;
    delete vk2_greed;
    delete vk2;
    delete view_vk2;
    delete scene_vk2;
    //-------------------------------------------------------------------------
    delete vk1_fkd;
    delete view_vk1_fkd;
    delete scene_vk1_fkd;

    delete time_line1;
    delete vk1_greed;
    delete vk1;
    delete view_vk1;
    delete scene_vk1;
    //-------------------------------------------------------------------------
    file_thread->exit();
    while ( file_thread->isRunning() );
    delete file_thread;
    //-------------------------------------------------------------------------
    delete ui;
    //-------------------------------------------------------------------------
}
//-------------------------------------------------------------------
void MainWindow::on_pushButtonFileOpen(void)
{
    akp_file.clear();

    FileName = QFileDialog::getOpenFileName(this, QString::fromUtf8("Открыть файл АКП"), FileName, QString::fromUtf8("Файлы АКП (*.gis)"));
    if (!FileName.isEmpty())
    {
        akp_file.load(FileName);
        index = 0;
    }

    // qDebug() << QString::fromUtf8("Файл %1 считан в память").arg(FileName);

    vk2_fkd->clearData();
    vk1_fkd->clearData();
    mlCol->delPoints();

    TVAK8_WAVE vk;
    int i;
    // for(i = 0; i < akp_file.count(); i++)
    for(i = 2; i < akp_file.count(); i++)
    {
        // qDebug() << QString::fromUtf8("Читаю кадр %1").arg(i);
        akp_file.read_ch1(i, vk);
        // qDebug() << QString::fromUtf8("Читаю ВК1");
        vk1_fkd->addData(akp_file.read_dept(i), vk);
        // qDebug() << QString::fromUtf8("Получены данные ФКД1");
        akp_file.read_ch2(i, vk);
        // qDebug() << QString::fromUtf8("Читаю ВК2");
        vk2_fkd->addData(akp_file.read_dept(i), vk);
        // qDebug() << QString::fromUtf8("Получены данные ФКД2");
    }

    // qDebug() << QString::fromUtf8("Получены данные ФКД1 и ФКД2");

    ui->pushButton_Down->setDisabled(false);
    ui->pushButton_Up->setDisabled(false);

//    Depth = akp_file.read_dept(0);
//    startDepth = akp_file.start_dept().toInt();
    startDepth = akp_file.read_dept(0);
//    lastDepth = Depth;

    blk_count = 0;
    bad_blk   = 0;

    ui->verticalScrollBar->setMinimum(0);
    ui->verticalScrollBar->setMaximum(akp_file.count() - 1);

    on_pushButtonDown();

    view_vk1->setMouseTracking(true);
    connect (scene_vk1, &CustomScene::signalTargetCoordinate, this, &MainWindow::on_changeVK1pos);

    view_vk2->setMouseTracking(true);
    connect (scene_vk2, &CustomScene::signalTargetCoordinate, this, &MainWindow::on_changeVK2pos);
}
//-------------------------------------------------------------------
void MainWindow::on_cmdIncAmpl(void)
{
    int new_max;
    int new_step;

    new_max = vk1->maxAmpl();
    new_step = new_max / vk1_greed->get_num_y_line();

    switch(new_step)
    {
//        case 10000:
//        {
//            new_step =  5000;
//            break;
//        }
        case 5000:
        {
            new_step =  2000;
            break;
        }
        case 2000:
        {
            new_step =  1000;
            break;
        }
        case 1000:
        {
            new_step =  500;
            break;
        }
        case 500:
        {
            new_step =  200;
            break;
        }
        case 200:
        {
            new_step =  100;
            break;
        }
        case 100:
        {
            new_step =  50;
            break;
        }
        case 50:
        {
//            new_step =  20;
            break;
        }
//        case 20:
//        {
//            new_step =  10;
//            break;
//        }
        default:
        {
            new_step =  50;
            break;
        }
    }

    new_max = new_step * vk1_greed->get_num_y_line();
    max_ampl = new_max;
    emit changeVKmaxAmpl(max_ampl);

    ui->groupBox_Ampl->setTitle(QString::fromUtf8("Амплитуда (%1 / div)").arg(new_step));
}
//-------------------------------------------------------------------
void MainWindow::on_cmdDecAmpl(void)
{
    int new_max;
    int new_step;

    new_max = vk1->maxAmpl();
    new_step = new_max / vk1_greed->get_num_y_line();
    switch(new_step)
    {
        case 5000:
        {
//            new_step =  10000;
            break;
        }
        case 2000:
        {
            new_step =  5000;
            break;
        }
        case 1000:
        {
            new_step =  2000;
            break;
        }
        case 500:
        {
            new_step =  1000;
            break;
        }
        case 200:
        {
            new_step =  500;
            break;
        }
        case 100:
        {
            new_step =  200;
            break;
        }
        case 50:
        {
            new_step =  100;
            break;
        }
//        case 20:
//        {
//            new_step =  50;
//            break;
//        }
//        case 10:
//        {
//            new_step =  20;
//            break;
//        }
        default:
        {
            new_step =  5000;
            break;
        }
    }

    new_max = new_step * vk1_greed->get_num_y_line();
    max_ampl = new_max;
    emit changeVKmaxAmpl(max_ampl);

    ui->groupBox_Ampl->setTitle(QString::fromUtf8("Амплитуда (%1 / div)").arg(new_step));
}
//-------------------------------------------------------------------
void MainWindow::on_cmdIncLevel(void)
{
    int max_ampl = vk1->maxAmpl();
    fkd_level += max_ampl / 16;
    if (fkd_level > max_ampl) fkd_level = max_ampl;
    emit changeFKDlevel(fkd_level);
}
//-------------------------------------------------------------------
void MainWindow::on_cmdDecLevel(void)
{
    int max_ampl = vk1->maxAmpl();
    fkd_level -= max_ampl / 16;
    if (fkd_level < 0) fkd_level = 0;
    emit changeFKDlevel(fkd_level);
}
//-------------------------------------------------------------------
void MainWindow::on_pushButtonDown(void)
{
    if (index > 0)
    {
        index--;
        ui->verticalScrollBar->setSliderPosition(index);

        if (akp_file.is_frame_CRC_OK(index))
        {
            blk_count--;
        }
        else
        {
            bad_blk--;
        }
    }

    on_show();
}
//-------------------------------------------------------------------
void MainWindow::on_pushButtonUp(void)
{
    if (index < akp_file.count())
    {
        index++;
        ui->verticalScrollBar->setSliderPosition(index);

        if (akp_file.is_frame_CRC_OK(index))
        {
            blk_count++;
        }
        else
        {
            bad_blk++;
        }
    }

    on_show();
}
//-----------------------------------------------------------------------------
//#pragma pack(1)
//void MainWindow::on_cmdSetDepth(void)
//{
//    akp.on_cmdSetDepth(Depth);
//}
//#pragma pack()
//-------------------------------------------------------------------
void MainWindow::on_show(void)
{
    TVAK8_WAVE vk;

    index = ui->verticalScrollBar->maximum() - ui->verticalScrollBar->value();

    on_showPocketCount(blk_count);
    on_showBadPocketCount(bad_blk);

    //    void on_showDept    (const qint32 dept);
        on_showML(akp_file.read_ml(index));

        akp_file.read_ch1(index, vk);
        on_showNewData (akp_file.read_ch1_vk_number(index), vk);
//        vk1_fkd->addData(akp_file.read_dept(index), vk);
        if ( (VK1_pos >= 0) && (VK1_pos <  VAK_8_NUM_POINTS) )
        {
            int t;
            t = VK1_pos * time_line1->get_time_step() + time_line1->get_time_zero();
            on_showVK1time(t);
            on_showVK1ampl(vk[VK1_pos]);
        }

        akp_file.read_ch2(index, vk);
        on_showNewData (akp_file.read_ch2_vk_number(index), vk);
//        vk2_fkd->addData(akp_file.read_dept(index), vk);
        if ( (VK2_pos >= 0) && (VK2_pos <  VAK_8_NUM_POINTS) )
        {
            int t;
            t = VK2_pos * time_line2->get_time_step() + time_line2->get_time_zero();
            on_showVK2time(t);
            on_showVK2ampl(vk[VK2_pos]);
        }

    //    akp_file.read_izl_ampl                               (const int index);
    //    akp_file.read_tool_type                              (const int index);

        on_showIZLtype (akp_file.is_frame_CRC_OK_for_izl_type(index), akp_file.read_izl_type(index));
        on_showIZLfreq (akp_file.is_frame_CRC_OK_for_izl_freq(index), akp_file.read_izl_freq(index));
        on_showIZLnum  (akp_file.is_frame_CRC_OK_for_izl_periods(index), akp_file.read_izl_periods(index));

        on_showRXtype  (akp_file.is_frame_CRC_OK_for_rx_type(index), akp_file.read_rx_type(index));
        on_showRXdelay (akp_file.is_frame_CRC_OK_for_rx_delay(index), akp_file.read_rx_delay(index));
        on_showRXtd    (akp_file.is_frame_CRC_OK_for_rx_Td(index), akp_file.read_rx_Td(index));
        on_showRXku    (akp_file.is_frame_CRC_OK_for_rx_Ku(index), akp_file.read_rx_Ku(index));

        on_showToolNo  (akp_file.is_frame_CRC_OK_for_tool_no(index), akp_file.read_tool_no(index));
        on_showSoftVer (akp_file.is_frame_CRC_OK_for_soft_version(index), akp_file.read_soft_version_major(index), akp_file.read_soft_version_minor(index));

        on_showTimeMeserment(akp_file.is_frame_CRC_OK_for_time_meserment(index), akp_file.read_time_meserment(index));

    //    akp_file.is_frame_CRC_OK_for_tool_type(index);

        on_showCRC (akp_file.is_frame_CRC_OK(index));
//        on_showCRC (akp_file.is_frame_CRC_OK_for_ch2_frame_label(index));
//        on_showCRC2 (akp_file.is_frame_CRC_OK_for_mode_number(index));
//        on_showCRC3 (akp_file.is_frame_CRC_OK_for_mode_count(index));
    //    on_showCRC4 (const bool crc);
    //    on_showCRC5 (const bool crc);
//        on_showCRC6 (akp_file.is_frame_CRC_OK_for_vk_calibration_amplitude(index));
//        on_showCRC7 (akp_file.is_frame_CRC_OK_for_timer_clk(index));
//        on_showCRC8 (akp_file.is_frame_CRC_OK_for_time_stop_meserment(index));

        emit cmdSetDepth(akp_file.read_dept(index));
}
    //-------------------------------------------------------------------
void MainWindow::on_showPocketCount(const int count)
{
    label_GoodBlk->setText(QString::fromUtf8("Хорошие блоки: %1").arg(count));
}
//-------------------------------------------------------------------
void MainWindow::on_showBadPocketCount(const int count)
{
    label_BadBlk->setText(QString::fromUtf8("Плохие блоки: %1").arg(count));
}
//-------------------------------------------------------------------
void MainWindow::on_showDept(const qint32 dept)
{
    QColor color = palette().window().color();//.background().color();

    statusBar()->setStyleSheet(QString::fromUtf8("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

    lastDepth = dept;

    label_Depth->setText(QString::fromUtf8("Глубина: %1 м")
                                .arg( dept / 100.0, 0, 'f', 2)
                         );

    label_Distance->setText(QString::fromUtf8("Пройдено: %1 м")
                                   .arg( abs(startDepth - dept) / 100.0, 0, 'f', 2)
                            );
}
//-------------------------------------------------------------------
void MainWindow::on_showML(const bool ml)
{
    QColor  color;

    if (ml)
    {
        color = MMColor;
        mlCol->setML(Depth);    //==========>>??????
    }
    else    color = palette().window().color();

    label_ML->setStyleSheet(QString::fromUtf8("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
}
//-------------------------------------------------------------------
void MainWindow::on_showNewData(const quint16 vk_no, const TVAK8_WAVE &vk)
{
    if (vk_no == 0)
    {
        vk1->AddData(vk);
        bool crc;
        crc = akp_file.is_frame_CRC_OK_for_ch1(index);
        QColor  color = get_color_on_CRC(crc);
        vk1->setColorText(color);
        if (crc)
        {
            vk1->setCaption(QString::fromUtf8("ВК-1 (CRC Ok)"));
        }
        else
        {
            vk1->setCaption(QString::fromUtf8("ВК-1 (CRC Ошибка!!!)"));
        }
//        vk1_fkd->addData(akp_file.read_dept(index), vk);
    }
    else if(vk_no == 4)
    {
        vk2->AddData(vk);
        bool crc;
        crc = akp_file.is_frame_CRC_OK_for_ch2(index);
        QColor  color = get_color_on_CRC(crc);
        vk2->setColorText(color);
        if (crc)
        {
            vk2->setCaption(QString::fromUtf8("ВК-2 (CRC Ok)"));
        }
        else
        {
            vk2->setCaption(QString::fromUtf8("ВК-2 (CRC Ошибка!!!)"));
        }
//        vk2_fkd->addData(akp_file.read_dept(index), vk);
    }
}
//-------------------------------------------------------------------
void MainWindow::on_showIZLtype(const bool crc, const quint16 value)
{
    QColor  color = get_color_on_CRC(crc);
    ui->label_IZLtype->setStyleSheet(QString::fromUtf8("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

    switch (value)
    {
        case 0: //IZL_MONOPOL:
            ui->label_IZLtype->setText("Монополь");
            break;
//        case IZL_DIPOL_1:
//            ui->label_IZLtype->setText("Диполь-1");
//            break;
//        case IZL_DIPOL_2:
//            ui->label_IZLtype->setText("Диполь-2");
//            break;
//        case IZL_QUADROPOL:
//            ui->label_IZLtype->setText("Квадруполь");
//            break;
//        case IZL_SFERA_1:
//            ui->label_IZLtype->setText("Сфера-1");
//            break;
//        case IZL_SFERA_2:
//            ui->label_IZLtype->setText("Сфера-2");
//            break;
//        case IZL_SFERA_3:
//            ui->label_IZLtype->setText("Сфера-3");
//            break;
//        case IZL_SFERA_4:
//            ui->label_IZLtype->setText("Сфера-4");
//            break;
        default:
            ui->label_IZLtype->setText("НЕИЗВЕСТНО");
            break;
    }
}
//-------------------------------------------------------------------
void MainWindow::on_showIZLfreq(const bool crc, const quint16 value)
{
    QColor  color = get_color_on_CRC(crc);
    ui->label_IZLfreq->setStyleSheet(QString::fromUtf8("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
    ui->label_IZLfreq->setText(QString::fromUtf8("%1 кГц").arg(value));
}
//-------------------------------------------------------------------
void MainWindow::on_showIZLnum(const bool crc, const quint16 value)
{
    QColor  color = get_color_on_CRC(crc);
    ui->label_IZLnum->setStyleSheet(QString::fromUtf8("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

    switch (value)
    {
        case 1:
            ui->label_IZLnum->setText(QString::fromUtf8("1 период"));
            break;
        case 2:
            ui->label_IZLnum->setText(QString::fromUtf8("2 периода"));
            break;
        case 3:
            ui->label_IZLnum->setText(QString::fromUtf8("3 периода"));
            break;
        case 4:
            ui->label_IZLnum->setText(QString::fromUtf8("4 периода"));
            break;
        case 5:
            ui->label_IZLnum->setText(QString::fromUtf8("5 периодов"));
            break;
        case 6:
            ui->label_IZLnum->setText(QString::fromUtf8("6 периодов"));
            break;
        case 7:
            ui->label_IZLnum->setText(QString::fromUtf8("7 периодов"));
            break;
        case 8:
            ui->label_IZLnum->setText(QString::fromUtf8("8 периодов"));
            break;
        default:
            ui->label_IZLnum->setText(QString::fromUtf8("НЕИЗВЕСТНО"));
            break;
    }
}
//-------------------------------------------------------------------
void MainWindow::on_showRXtype  (const bool crc, const quint16 value)
{
    QColor  color = get_color_on_CRC(crc);
    ui->label_RXtype->setStyleSheet(QString::fromUtf8("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
    if (0 == value)
    {
        ui->label_RXtype->setText(QString::fromUtf8("Монополь"));
    }
    else
    {
        ui->label_RXtype->setText(QString::fromUtf8("Тип неизвестен"));
    }
}
//-------------------------------------------------------------------
void MainWindow::on_showRXdelay(const bool crc, const quint16 value)
{
    qint16 delay = 2 * value;
    time_line1->set_time_zero(delay / time_line1->get_time_step());

    QColor  color = get_color_on_CRC(crc);
    ui->label_RXdelay->setStyleSheet(QString::fromUtf8("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
    ui->label_RXdelay->setText(QString::fromUtf8("Задержка %1 мкс").arg(delay));
}
//-------------------------------------------------------------------
void MainWindow::on_showRXtd(const bool crc, const quint16 value)
{
    Q_UNUSED(crc);
    Q_UNUSED(value);

    Td = 2;
    time_line1->set_time_step(Td);
    ui->label_RXtd->setText(QString::fromUtf8("Тд = %1 мкс").arg(Td));

//    switch (value)
//    {
//    case TD_4_MKS:
//        Td = 4;
//        time_line1->set_time_step(Td);
//        ui->label_RXtd->setText(QString::fromUtf8("Тд = %1 мкс").arg(Td));
//        break;
//    case TD_8_MKS:
//        Td = 8;
//        time_line1->set_time_step(Td);
//        ui->label_RXtd->setText(QString::fromUtf8("Тд = %1 мкс").arg(Td));
//        break;
//    case TD_16_MKS:
//        Td = 16;
//        time_line1->set_time_step(Td);
//        ui->label_RXtd->setText(QString::fromUtf8("Тд = %1 мкс").arg(Td));
//        break;
//    case TD_32_MKS:
//        Td = 32;
//        time_line1->set_time_step(Td);
//        ui->label_RXtd->setText(QString::fromUtf8("Тд = %1 мкс").arg(Td));
//        break;
//    default:
//        ui->label_RXtd->setText(QString::fromUtf8("Тд - НЕИЗВЕСТНО"));
//        //time_line1->set_time_step(?);
//        break;
//    }
}
//-------------------------------------------------------------------
void MainWindow::on_showRXku(const bool crc, const quint16 value)
{
    QColor  color = get_color_on_CRC(crc);
    ui->label_RXku->setStyleSheet(QString::fromUtf8("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

    if (value < 0)
        ui->label_RXku->setText(QString::fromUtf8("КУ = нет данных"));
    else
        ui->label_RXku->setText(QString::fromUtf8("КУ = %1").arg(value + 1));
}
//-------------------------------------------------------------------
void MainWindow::on_showToolNo  (const bool crc, const quint16 value)
{
    QColor  color = get_color_on_CRC(crc);
    ui->label_ToolNo->setStyleSheet(QString::fromUtf8("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

    ui->label_ToolNo->setText(QString::fromUtf8("Прибор № %1").arg(value));
}
//-------------------------------------------------------------------
void MainWindow::on_showSoftVer (const bool crc, const quint16 soft_version_major, const quint16 soft_version_minor)
{
    QColor  color = get_color_on_CRC(crc);
    ui->label_SoftVer->setStyleSheet(QString::fromUtf8("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

    ui->label_SoftVer->setText(QString::fromUtf8("Версия прошивки: %1.%2").arg(soft_version_major).arg(soft_version_minor));
}
//-------------------------------------------------------------------
void MainWindow::on_showTimeMeserment(const bool crc, const quint32 time_meserment)
{
    QColor  color = get_color_on_CRC(crc);
    ui->label_TimeOfMeserment->setStyleSheet(QString::fromUtf8("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

    ui->label_TimeOfMeserment->setText(QString::fromUtf8("Интервал = %1 мкс").arg(time_meserment));
}
//-------------------------------------------------------------------
void MainWindow::on_showCRC (const bool crc)
{
    QColor  color = get_color_on_CRC(crc);
    label_CRC_check->setStyleSheet(QString::fromUtf8("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

    if (crc)
        label_CRC_check->setText(QString::fromUtf8("CRC: Ok"));
    else
        label_CRC_check->setText(QString::fromUtf8("CRC: ОШИБКА!!!"));
}
//-------------------------------------------------------------------
void MainWindow::on_showVK1time (const int time)
{
    label_VK1time->setText( QString::fromUtf8("T1: %1мкс").arg(time) );
}
//-------------------------------------------------------------------
void MainWindow::on_showVK1ampl (const int ampl)
{
    label_VK1ampl->setText( QString::fromUtf8("A1: %1").arg(ampl) );
}
//-------------------------------------------------------------------
void MainWindow::on_showVK2time (const int time)
{
    label_VK2time->setText( QString::fromUtf8("T2: %1мкс").arg(time) );
}
//-------------------------------------------------------------------
void MainWindow::on_showVK2ampl (const int ampl)
{
    label_VK2ampl->setText( QString::fromUtf8("A2: %1").arg(ampl) );
}
//-------------------------------------------------------------------
void MainWindow::on_changeVK1pos (const QPointF pos)
{
    VK1_pos = pos.x() / time_line1->get_x_scale();
    on_show();
}
//-------------------------------------------------------------------
void MainWindow::on_changeVK2pos (const QPointF pos)
{
    VK2_pos = pos.x() / time_line2->get_x_scale();
    on_show();
}
//-------------------------------------------------------------------
//void MainWindow::on_showCRC2 (const bool crc)
//{
//    QColor  color = get_color_on_CRC(crc);
//    label_CRC2_check->setStyleSheet(QString::fromUtf8("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

//    if (crc)
//        label_CRC2_check->setText(QString::fromUtf8("CRC2: Ok"));
//    else
//        label_CRC2_check->setText(QString::fromUtf8("CRC2: ОШИБКА!!!"));
//}
//-------------------------------------------------------------------
//void MainWindow::on_showCRC3 (const bool crc)
//{
//    QColor  color = get_color_on_CRC(crc);
//    label_CRC3_check->setStyleSheet(QString::fromUtf8("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

//    if (crc)
//        label_CRC3_check->setText(QString::fromUtf8("CRC3: Ok"));
//    else
//        label_CRC3_check->setText(QString::fromUtf8("CRC3: ОШИБКА!!!"));
//}
//-------------------------------------------------------------------
//void MainWindow::on_showCRC4 (const bool crc)
//{
//    QColor  color = get_color_on_CRC(crc);
//    label_CRC4_check->setStyleSheet(QString::fromUtf8("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

//    if (crc)
//        label_CRC4_check->setText(QString::fromUtf8("CRC4: Ok"));
//    else
//        label_CRC4_check->setText(QString::fromUtf8("CRC4: ОШИБКА!!!"));
//}
//-------------------------------------------------------------------
//void MainWindow::on_showCRC5 (const bool crc)
//{
//    QColor  color = get_color_on_CRC(crc);
//    label_CRC5_check->setStyleSheet(QString::fromUtf8("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

//    if (crc)
//        label_CRC5_check->setText(QString::fromUtf8("CRC5: Ok"));
//    else
//        label_CRC5_check->setText(QString::fromUtf8("CRC5: ОШИБКА!!!"));
//}
//-------------------------------------------------------------------
//void MainWindow::on_showCRC6 (const bool crc)
//{
//    QColor  color = get_color_on_CRC(crc);
//    label_CRC6_check->setStyleSheet(QString::fromUtf8("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

//    if (crc)
//        label_CRC6_check->setText(QString::fromUtf8("CRC6: Ok"));
//    else
//        label_CRC6_check->setText(QString::fromUtf8("CRC6: ОШИБКА!!!"));
//}
//-------------------------------------------------------------------
//void MainWindow::on_showCRC7 (const bool crc)
//{
//    QColor  color = get_color_on_CRC(crc);
//    label_CRC7_check->setStyleSheet(QString::fromUtf8("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

//    if (crc)
//        label_CRC7_check->setText(QString::fromUtf8("CRC7: Ok"));
//    else
//        label_CRC7_check->setText(QString::fromUtf8("CRC7: ОШИБКА!!!"));
//}
//-------------------------------------------------------------------
//void MainWindow::on_showCRC8 (const bool crc)
//{
//    QColor  color = get_color_on_CRC(crc);
//    label_CRC8_check->setStyleSheet(QString::fromUtf8("color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));

//    if (crc)
//        label_CRC8_check->setText(QString::fromUtf8("CRC8: Ok"));
//    else
//        label_CRC8_check->setText(QString::fromUtf8("CRC8: ОШИБКА!!!"));
//}
//-------------------------------------------------------------------
QColor MainWindow::get_color_on_CRC(const bool crc)
{
    if (crc)
    {
        return QColor(Qt::black);
    }
    else
    {
        return QColor(Qt::red);
    }
}
//-------------------------------------------------------------------
void MainWindow::load_settings(void)
{
    app_settings->beginGroup(QString::fromUtf8("/Settings"));

    OperatorName    = app_settings->value(QString::fromUtf8("/OperatorName"),  QString::fromUtf8("Я")      ).toString();
    WellNo          = app_settings->value(QString::fromUtf8("/WellNo"),        QString::fromUtf8("0")      ).toString();
    FildName        = app_settings->value(QString::fromUtf8("/FildName"),      QString::fromUtf8("None")   ).toString();
    Depth           = app_settings->value(QString::fromUtf8("/Depth"),         0                           ).toInt();
    startDepth      = Depth;
    lastDepth       = Depth;
    DepthStep       = app_settings->value(QString::fromUtf8("/DepthStep"),     10                           ).toInt();

#if defined(Q_OS_LINUX)
    FolderName      = QDir::toNativeSeparators(app_settings->value(QString::fromUtf8("/FolderName"),    QString::fromUtf8("~/Скважины")     ).toString());
#elif defined(Q_OS_WIN)
    FolderName      = app_settings->value(QString::fromUtf8("/FolderName"),    QString::fromUtf8("D:\\Скважины")     ).toString();
#endif

    FileName        = QString::fromUtf8("%1/%2a.4sd").arg(FolderName).arg(WellNo);

    QColor color = QColor(Qt::red);
    MMColor.setRgba(  app_settings->value(QString::fromUtf8("/MMColor"),       color.rgba()                ).toInt());

    color = palette().window().color();
    FonColor.setRgba( app_settings->value(QString::fromUtf8("/FonColor"),      color.rgba()                ).toInt());

    color = QColor(Qt::green);
    GreedColor.setRgba(app_settings->value(QString::fromUtf8("/GreedColor"),   color.rgba()                ).toInt());

    color = QColor(Qt::black);
    TextColor.setRgba(app_settings->value(QString::fromUtf8("/TextColor"),     color.rgba()                ).toInt());

    color = QColor(Qt::gray);
    LevelColor.setRgba(app_settings->value(QString::fromUtf8("/LevelColor"),   color.rgba()                ).toInt());

    color = QColor(Qt::gray);
    FKDColor.setRgba(app_settings->value(QString::fromUtf8("/FKDColor"),       color.rgba()                ).toInt());

    FKDstep         = app_settings->value(QString::fromUtf8("/FKDstep"),       2                           ).toInt();

    color = QColor(Qt::blue);
    VKColor.setRgba(app_settings->value(QString::fromUtf8("/VKColor"),         color.rgba()                ).toInt());

    VKlineSize      = app_settings->value(QString::fromUtf8("/VKlineSize"),    1                           ).toInt();
    DepthScale      = app_settings->value(QString::fromUtf8("/DepthScale"),    100                         ).toInt();
    dpsX            = app_settings->value(QString::fromUtf8("/dpsX"),          26                          ).toInt();
    dpsY            = app_settings->value(QString::fromUtf8("/dpsY"),          26                          ).toInt();

    max_ampl        = app_settings->value(QString::fromUtf8("/MaxAmpl"),       8000                        ).toInt();
    fkd_level       = app_settings->value(QString::fromUtf8("/FkdLevel"),      0                           ).toInt();

    app_settings->endGroup();

    VK1_pos = VAK_8_NUM_POINTS;
    VK2_pos = VAK_8_NUM_POINTS;
}
//-----------------------------------------------------------------------------
void MainWindow::save_settings(void)
{
    app_settings->beginGroup(QString::fromUtf8("/Settings"));

    app_settings->setValue(QString::fromUtf8("/OperatorName"), OperatorName         );
    app_settings->setValue(QString::fromUtf8("/WellNo"),       WellNo               );
    app_settings->setValue(QString::fromUtf8("/FildName"),     FildName             );

    app_settings->setValue(QString::fromUtf8("/Depth"),        Depth                );
    app_settings->setValue(QString::fromUtf8("/DepthStep"),    DepthStep            );

    app_settings->setValue(QString::fromUtf8("/FolderName"),   FolderName           );

    app_settings->setValue(QString::fromUtf8("/MMColor"),      MMColor.rgba()       );
    app_settings->setValue(QString::fromUtf8("/FonColor"),     FonColor.rgba()      );
    app_settings->setValue(QString::fromUtf8("/GreedColor"),   GreedColor.rgba()    );
    app_settings->setValue(QString::fromUtf8("/TextColor"),    TextColor.rgba()     );
    app_settings->setValue(QString::fromUtf8("/LevelColor"),   LevelColor.rgba()    );
    app_settings->setValue(QString::fromUtf8("/FKDColor"),     FKDColor.rgba()      );

    app_settings->setValue(QString::fromUtf8("/FKDstep"),      FKDstep              );

    app_settings->setValue(QString::fromUtf8("/VKColor"),      VKColor.rgba()       );

    app_settings->setValue(QString::fromUtf8("/VKlineSize"),   VKlineSize           );
    app_settings->setValue(QString::fromUtf8("/DepthScale"),   DepthScale           );
    app_settings->setValue(QString::fromUtf8("/dpsX"),         dpsX                 );
    app_settings->setValue(QString::fromUtf8("/dpsY"),         dpsY                 );

    app_settings->setValue(QString::fromUtf8("/MaxAmpl"),      max_ampl             );
    app_settings->setValue(QString::fromUtf8("/FkdLevel"),     fkd_level            );

    app_settings->endGroup();

    app_settings->sync();
/*
    app_settings->beginGroup(QString::fromUtf8("/Settings"));

    app_settings->setValue(QString::fromUtf8("/OperatorName"), OperatorName         );
    app_settings->setValue(QString::fromUtf8("/WellNo"),       WellNo               );
    app_settings->setValue(QString::fromUtf8("/FildName"),     FildName             );

    app_settings->setValue(QString::fromUtf8("/Depth"),        Depth                );
    app_settings->setValue(QString::fromUtf8("/DepthStep"),    DepthStep            );

    app_settings->setValue(QString::fromUtf8("/FolderName"),   FolderName           );
    app_settings->setValue(QString::fromUtf8("/ExtFolderCtl"), bExtFolderCtl        );

    app_settings->setValue(QString::fromUtf8("/MMColor"),      MMColor.rgba()       );
    app_settings->setValue(QString::fromUtf8("/FonColor"),     FonColor.rgba()      );
    app_settings->setValue(QString::fromUtf8("/GreedColor"),   GreedColor.rgba()    );
    app_settings->setValue(QString::fromUtf8("/TextColor"),    TextColor.rgba()     );
    app_settings->setValue(QString::fromUtf8("/LevelColor"),   LevelColor.rgba()    );
    app_settings->setValue(QString::fromUtf8("/FKDColor"),     FKDColor.rgba()      );

    app_settings->setValue(QString::fromUtf8("/FKDstep"),      FKDstep              );

    app_settings->setValue(QString::fromUtf8("/VKColor"),      VKColor.rgba()       );

    app_settings->setValue(QString::fromUtf8("/VKlineSize"),   VKlineSize           );
    app_settings->setValue(QString::fromUtf8("/DepthScale"),   DepthScale           );
    app_settings->setValue(QString::fromUtf8("/dpsX"),         dpsX                 );
    app_settings->setValue(QString::fromUtf8("/dpsY"),         dpsY                 );

    app_settings->setValue(QString::fromUtf8("/MaxAmpl"),      max_ampl             );
    app_settings->setValue(QString::fromUtf8("/FkdLevel"),     fkd_level            );

    app_settings->setValue(QString::fromUtf8("/ModeCount"),    mode_list.size()     );

    int i = 0;
    mode = mode_list.begin();
    vak32_ctrl_command_class* modeX;

    app_settings->beginGroup(QString::fromUtf8("Mode_%1").arg(i+1));
        modeX = *mode;
        app_settings->setValue(QString::fromUtf8("/IzlType"),        modeX->get_izl_type()       );
        app_settings->setValue(QString::fromUtf8("/IzlFreq"),        modeX->get_Fsig()           );
        app_settings->setValue(QString::fromUtf8("/PeriodNum"),      modeX->get_period_number()  );
        app_settings->setValue(QString::fromUtf8("/Td"),             modeX->get_Td()             );
        app_settings->setValue(QString::fromUtf8("/Delay"),          modeX->get_rx_delay()       );
        app_settings->setValue(QString::fromUtf8("/KU_win_start"),   modeX->get_KU_window_start());
        app_settings->setValue(QString::fromUtf8("/KU_win_width"),   modeX->get_KU_window_width());
    app_settings->endGroup();


    app_settings->endGroup();

    app_settings->sync();
*/
}
//-----------------------------------------------------------------------------
/*
bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if (target == ui->widgetAllVK)
    {
        if (event->type() == QEvent::Resize)
        {
            QResizeEvent* rsizeEvent = static_cast<QResizeEvent*>(event);
            QSize new_size = rsizeEvent->size();

            QRect rect;
            int new_top = 0;
            int new_height = new_size.height();
            new_height -=1;
            new_height /= 4;
            new_height /= 4;
            new_height *= 4;

            rect.setRect(0, new_top, VAK32_WAVE_NUM_POINTS, new_height);
            int x = rect.x();
            int y = rect.y();
            dop_vk1->setPos(x, y);
            dop_vk1->set_height(new_height);
            dop_vk1_greed->setPos(x, y);
            dop_vk1_greed->set_num_y_line(4);
            dop_vk1_greed->set_height(new_height);

            new_top += new_height;
            rect.setRect(0, new_top, VAK32_WAVE_NUM_POINTS, new_height);
            x = rect.x();
            y = rect.y();
            dop_vk2->setPos(x, y);
            dop_vk2->set_height(new_height);
            dop_vk2_greed->setPos(x, y);
            dop_vk2_greed->set_num_y_line(4);
            dop_vk2_greed->set_height(new_height);

            new_top += new_height;
            rect.setRect(0, new_top, VAK32_WAVE_NUM_POINTS, new_height);
            x = rect.x();
            y = rect.y();
            dop_vk3->setPos(x, y);
            dop_vk3->set_height(new_height);
            dop_vk3_greed->setPos(x, y);
            dop_vk3_greed->set_num_y_line(4);
            dop_vk3_greed->set_height(new_height);

            new_top += new_height;
            rect.setRect(0, new_top, VAK32_WAVE_NUM_POINTS, new_height);
            x = rect.x();
            y = rect.y();
            dop_vk4->setPos(x, y);
            dop_vk4->set_height(new_height);
            dop_vk4_greed->setPos(x, y);
            dop_vk4_greed->set_num_y_line(4);
            dop_vk4_greed->set_height(new_height);
        }
    }
    return QMainWindow::eventFilter(target, event);

}
//-----------------------------------------------------------------------------
*/
