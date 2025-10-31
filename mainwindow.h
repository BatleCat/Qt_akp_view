//-------------------------------------------------------------------
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//-------------------------------------------------------------------
#include <QMainWindow>
#include <QColor>
#include <QString>
#include <QVector>
#include <QtNetwork/QUdpSocket>
#include <QTimer>
#include <QTimerEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QLabel>
#include <QSettings>
#include <QButtonGroup>
#include <QThread>
//-------------------------------------------------------------------
#include "akp_check_state.h"
//-------------------------------------------------------------------
//-------------------------------------------------------------------
#include "vak_8.h"
#include "vak_8_2pc.h"
//#include "qt_vak_8u_vk.h"
//-------------------------------------------------------------------
#include "qt_vk.h"
#include "qt_bigreed.h"
#include "qt_time_line.h"
#include "qt_vak_32_fkd.h"
#include "qt_deptcol.h"
#include "qt_ml.h"
#include "qt_akp_file.h"
//-------------------------------------------------------------------
namespace Ui {
class MainWindow;
}
//-------------------------------------------------------------------
class CustomScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CustomScene(QObject *parent = nullptr) : QGraphicsScene(parent)
    {

    }
    explicit CustomScene(const QRectF &sceneRect, QObject *parent = nullptr) : QGraphicsScene(sceneRect, parent)
    {

    }
    explicit CustomScene(qreal x, qreal y, qreal width, qreal height, QObject *parent = nullptr) : QGraphicsScene(x, y, width, height, parent)
    {

    }
    ~CustomScene(){}

signals:
    // Сигнал для передачи координат положения курсора мыши
    void signalTargetCoordinate(QPointF point);

public slots:

private:
    // Функция, в которой производится отслеживание положения мыши
    void mousePressEvent(QGraphicsSceneMouseEvent *event){
        emit signalTargetCoordinate(event->scenePos());
    }
};
//-----------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

    void cmdSetDepth(const qint32 dept_cm);
    void cmdSetDeptStep(const qint32 dept_step_cm);
    void showDept(const qint32 dept);
    void showNewData(void);

    void changeFKDstep(int newStep);
    void changeDpsX(int newDpsX);
    void changeDpsY(int newDpsY);
    void changeDeptScale(int newScale);
    void changeFKDlevel(int newLevel);
    void changeVKmaxAmpl(int newMaxAmpl);

public slots:

private:
    Ui::MainWindow*     ui;

    QSettings*          app_settings;

//    QButtonGroup*       buttonGroup;

    //-------------------------------------------------------------------
//    akp_check_state     check_state;
    //-------------------------------------------------------------------
    qt_akp_file_read    akp_file;
    QThread*            file_thread;
    int                 index;
    //-------------------------------------------------------------------
    // QGraphicsScene*     scene_vk1;
    CustomScene*        scene_vk1;
    QGraphicsView*      view_vk1;
    Qt_VK*              vk1;
    Qt_biGREED*         vk1_greed;
    Qt_TIME_LINE*       time_line1;
    //-------------------------------------------------------------------
    // QGraphicsScene*     scene_vk2;
    CustomScene*        scene_vk2;
    QGraphicsView*      view_vk2;
    Qt_VK*              vk2;
    Qt_biGREED*         vk2_greed;
    Qt_TIME_LINE*       time_line2;
    //-------------------------------------------------------------------
    QGraphicsScene*     scene_vk1_fkd;
    QGraphicsView*      view_vk1_fkd;
    CVAK32_FKD*         vk1_fkd;
    //-------------------------------------------------------------------
    QGraphicsScene*     scene_vk2_fkd;
    QGraphicsView*      view_vk2_fkd;
    CVAK32_FKD*         vk2_fkd;
    //-------------------------------------------------------------------
    QGraphicsScene*     scene_dept_col;
    QGraphicsView*      view_dept_col;
    Qt_DEPTCOL*         deptCol;
    //-------------------------------------------------------------------
    QGraphicsScene*     scene_ml_col;
    QGraphicsView*      view_ml_col;
    Qt_ML*              mlCol;

    QLabel*             label_GoodBlk;
    QLabel*             label_BadBlk;

    QLabel*             label_Depth;
    QLabel*             label_Distance;
//    QLabel*             label_Velocity;

    QLabel*             label_CRC_check;
//    QLabel*             label_CRC2_check;
//    QLabel*             label_CRC3_check;
//    QLabel*             label_CRC4_check;
//    QLabel*             label_CRC5_check;
//    QLabel*             label_CRC6_check;
//    QLabel*             label_CRC7_check;
//    QLabel*             label_CRC8_check;

    QLabel*             label_ML;

    QLabel*             label_VK1time;
    QLabel*             label_VK1ampl;

    QLabel*             label_VK2time;
    QLabel*             label_VK2ampl;

    QString OperatorName;
    QString WellNo;
    QString FildName;
    qint32  Depth;
    qint32  startDepth;
    qint32  lastDepth;
    qint32  DepthStep;
//    qint32  curentDepthStep;
    QString FolderName;
    QString FileName;

    QColor  MMColor;
    QColor  FonColor;
    QColor  GreedColor;
    QColor  TextColor;
    QColor  LevelColor;
    QColor  FKDColor;
    int     FKDstep;
    int     Td;
    QColor  VKColor;
    int     VKlineSize;
    int     DepthScale;
    int     dpsX;
    int     dpsY;

    int     max_ampl;
    int     fkd_level;

    qint32  blk_count;
    qint32  bad_blk;

    int     VK1_pos;
    int     VK2_pos;

    QColor  get_color_on_CRC(const bool crc);
    void    load_settings(void);
    void    save_settings(void);

private slots:

    void on_pushButtonFileOpen(void);
    void on_pushButtonUp(void);
    void on_pushButtonDown(void);

    void on_cmdIncAmpl(void);
    void on_cmdDecAmpl(void);

    void on_cmdIncLevel(void);
    void on_cmdDecLevel(void);

    void on_show(void);

    void on_showPocketCount     (const int count);
    void on_showBadPocketCount  (const int count);
    void on_showDept    (const qint32 dept);
    void on_showML      (const bool ml);
    void on_showNewData (const quint16 vk_no, const TVAK8_WAVE &vk);

    void on_showIZLtype (const bool crc, const quint16 value);
    void on_showIZLfreq (const bool crc, const quint16 value);
    void on_showIZLnum  (const bool crc, const quint16 value);

    void on_showRXtype  (const bool crc, const quint16 value);
    void on_showRXdelay (const bool crc, const quint16 value);
    void on_showRXtd    (const bool crc, const quint16 value);
    void on_showRXku    (const bool crc, const quint16 value);

    void on_showToolNo  (const bool crc, const quint16 value);
    void on_showSoftVer (const bool crc, const quint16 soft_version_major, const quint16 soft_version_minor);

    void on_showTimeMeserment(const bool crc, const quint32 time_meserment);

    void on_showCRC (const bool crc);
//    void on_showCRC2 (const bool crc);
//    void on_showCRC3 (const bool crc);
//    void on_showCRC4 (const bool crc);
//    void on_showCRC5 (const bool crc);
//    void on_showCRC6 (const bool crc);
//    void on_showCRC7 (const bool crc);
//    void on_showCRC8 (const bool crc);

    void on_showVK1time (const int time);
    void on_showVK1ampl (const int ampl);
    void on_showVK2time (const int time);
    void on_showVK2ampl (const int ampl);

    void on_changeVK1pos (const QPointF pos);
    void on_changeVK2pos (const QPointF pos);
};
//-------------------------------------------------------------------
#endif // MAINWINDOW_H
//-------------------------------------------------------------------
