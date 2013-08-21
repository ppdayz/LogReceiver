#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include "netreceiver.h"

#define SHOW_LVL_NOTICE         (0x01)
#define SHOW_LVL_INFO             (0x01 << 1)
#define SHOW_LVL_DEBUG          (0x01 << 2)
#define SHOW_LVL_ERROR           (0x01 << 3)


class QLabel;
class QFile;
class LogPallette;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void dealWithComingLog(const QString &log);
    void netReceiverStateChanged(QAbstractSocket::SocketState state);

    void changeFilter(const QString &filter);

    void on_actionNotice_triggered();
    void on_actionInfo_triggered();
    void on_actionDebug_triggered();
    void on_actionError_triggered();
    void on_actionGoDowm_triggered();
    void on_actionSave_triggered();
    void on_actionSettings_triggered();
    void on_actionLink_triggered();

    void on_m_filter_casesensitivity_clicked();
    void on_m_filter_cb_activated(int index);

    void on_pb_search_clicked();
    void on_tb_findNext_clicked();
    void on_tb_findForward_clicked();

private:
    NetReceiver *netReceiver;
    int showLvL;

    QRegExp logFilter;
    QFile *savedFile;

    LogPallette *m_Pallette;

    QLabel *linkState;
    QLabel *linkStateBar;
    QLabel *statLabel;
    QLabel *showTimeLabel;

    QString m_hostName;
    int m_port;
    QFont m_font;

    int m_log_count;
    int m_time;
    int m_time_timerID;
    int m_auto_conn_timerID;
private:
    void calcShowLVL();
    void loadSettingFromIni();

};

#endif // MAINWINDOW_H
