#include "mainwindow.h"
#include <QDebug>
#include <QLabel>
#include <QString>
#include <QScrollBar>
#include <QFile>
#include <QDateTime>

#include "settingsdialog.h"

#define SHOW_REG_INFO ("[info]")
#define SHOW_REG_NOTICE ("[notice]")
#define SHOW_REG_DEBUG ("[debug]")
#define SHOW_REG_ERROR ("[err]")

#define NO_PIPEI ("^(?!.*%1).*$")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),showLvL(0), savedFile(new QFile),
    m_Pallette(new LogPallette), m_log_count(0),
    m_time_timerID(0), m_auto_conn_timerID(0)
{
    // 初始化UI
    setupUi(this);

    //  ----初始化状态栏图标//

    linkState = new QLabel(this);
    statusbar->addWidget(linkState);
    linkState->setFixedSize(16,16);
    linkState->setPixmap(QPixmap("://res/unlinked.png").scaled(linkState->size(),
                                                               Qt::IgnoreAspectRatio, Qt::SmoothTransformation));


    // ---初始化状态栏 //
    linkStateBar =  new QLabel(this);
    statusbar->addWidget(linkStateBar);

    // --统计label
    statLabel = new QLabel(this);
    statusbar->addPermanentWidget(statLabel);

    showTimeLabel = new QLabel(this);
    statusbar->addPermanentWidget(showTimeLabel);

    // 读取配置文件//
    loadSettingFromIni();

    // 初始化接收器//
    netReceiver = new NetReceiver(this);
    netReceiver->connectToLogForwarder(m_hostName, m_port);
    connect(netReceiver, SIGNAL(singleLogComing(QString)), this, SLOT(dealWithComingLog(QString)));

    // 初始化日志级别//
    calcShowLVL();

    //连接信号槽//
    connect(m_Filter, SIGNAL(filterChanged(QString)), this, SLOT(changeFilter(QString)));
    connect(netReceiver, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(netReceiverStateChanged(QAbstractSocket::SocketState)));

    m_Pallette->filterColor = QColor(Qt::red);

    m_filter_cb->setCurrentIndex(0);
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(m_time_timerID == event->timerId()){
        QTime t = QTime(0, 0, 0);
        m_time ++;
        showTimeLabel->setText(t.addSecs(m_time).toString("hh:mm:ss"));
    }

    if(m_auto_conn_timerID == event->timerId()){
        on_actionLink_triggered();
    }
}


void MainWindow::dealWithComingLog(const QString &log)
{
    if(actionSave->isChecked() && savedFile->isOpen()){
        savedFile->write(log.toUtf8().data());
        savedFile->write("\n");
    }

    m_log_count ++;
    statLabel->setText(QString("%2 %1 %3").arg(m_log_count)
                       .arg(QString::fromUtf8("\xE6\x80\xBB\xE5\x85\xB1\xE6\x94\xB6\xE5\x88\xB0"))
                       .arg(QString::fromUtf8("\xE4\xB8\xAA\xE6\x97\xA5\xE5\xBF\x97    ")));

    QColor lvlColor = QColor(Qt::black);
    QColor BGColor = QColor(Qt::transparent);

    if(!actionPause->isChecked()){
        if(showLvL & SHOW_LVL_INFO){
            if(log.contains(SHOW_REG_INFO)){
                lvlColor = m_Pallette->info_fg_color;
                BGColor = m_Pallette->info_bg_color;
                goto FITER_REGEXP;
            }
        }

        if(showLvL & SHOW_LVL_NOTICE){
            if(log.contains(SHOW_REG_NOTICE)){
                lvlColor = m_Pallette->notice_fg_color;
                goto FITER_REGEXP;
            }
        }

        if(showLvL & SHOW_LVL_DEBUG){
            if(log.contains(SHOW_REG_DEBUG)){
                lvlColor = m_Pallette->debug_fg_color;
                BGColor = m_Pallette->debug_bg_color;
                goto FITER_REGEXP;
            }
        }

        if(showLvL & SHOW_LVL_ERROR){
            if(log.contains(SHOW_REG_ERROR)){
                lvlColor = m_Pallette->error_fg_color;
                BGColor = m_Pallette->error_bg_color;
                goto FITER_REGEXP;
            }
        }

        return;

FITER_REGEXP:

        if(log.contains(logFilter)){
            textBrowser->setTextColor(lvlColor);
            textBrowser->setTextBackgroundColor(BGColor);

            if(m_filter_cb->currentIndex() == 2 && !m_Filter->text().isEmpty()){
                textBrowser->setTextColor(m_Pallette->filter_fg_color);
                textBrowser->setTextBackgroundColor(m_Pallette->filter_bg_color);
            }

            textBrowser->append(log);
        }else  if(m_filter_cb->currentIndex() == 2 && !m_Filter->text().isEmpty()){
            textBrowser->setTextColor(lvlColor);
            textBrowser->setTextBackgroundColor(BGColor);

            textBrowser->append(log);
        }
    }
}

void MainWindow::netReceiverStateChanged(QAbstractSocket::SocketState state)
{
    switch(state){
    case QAbstractSocket::ConnectedState:
        linkState->setPixmap(QPixmap("://res/linked.png").scaled(linkState->size(),
                                                                 Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        actionLink->setEnabled(false);
        linkStateBar->setText(QString("%1 : %2:%3")
                              .arg(QString::fromUtf8("\xE5\xB7\xB2\xE7\xBB\x8F\xE8\xBF\x9E\xE6\x8E\xA5\xE4\xB8\x8A\xE6\x9C\x8D\xE5\x8A\xA1\xE5\x99\xA8"))
                              .arg(m_hostName).arg(m_port));

        textBrowser->append(QString("%1 %2").arg(QDateTime::currentDateTime().toString("yyyyMMdd hh:mm:ss"))
                                                 .arg(linkStateBar->text()));

        if(m_time_timerID == 0){
            m_time = 0;
            m_time_timerID =  startTimer(1000);
        }

        if(m_auto_conn_timerID != 0){
            killTimer(m_auto_conn_timerID);
            m_auto_conn_timerID = 0;
        }

        break;
    case QAbstractSocket::UnconnectedState:
        if(m_auto_conn_timerID == 0){
            m_auto_conn_timerID = startTimer(60*1000);
        }
        textBrowser->append(QString("%1 Disconnect from the server, will try to link again!")
                            .arg(QDateTime::currentDateTime().toString("yyyyMMdd hh:mm:ss")));
    case QAbstractSocket::ConnectingState:
        linkStateBar->setText("Connection ... Please wait");
    default:
        linkState->setPixmap(QPixmap("://res/unlinked.png").scaled(linkState->size(),
                                                                   Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        actionLink->setEnabled(true);
        linkStateBar->setText(QString::fromUtf8("\xE4\xBB\x8E\xE6\x9C\x8D\xE5\x8A\xA1\xE5\x99\xA8\xE6\x96\xAD\xE5\xBC\x80 ..."));
        if(m_time_timerID != 0){
            killTimer(m_time_timerID);
            m_time_timerID = 0;
        }

        break;
    }
}

void MainWindow::changeFilter(const QString &filter)
{
    Qt::CaseSensitivity c = m_filter_casesensitivity->isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive;
    QString f = filter;
    if(m_filter_cb->currentIndex() == 1){
        f = QString(NO_PIPEI).arg(filter);
    }

    logFilter = QRegExp(f, c);
}

void MainWindow::calcShowLVL()
{
    if(actionNotice->isChecked()){
        showLvL |= SHOW_LVL_NOTICE;
        //        levelRegexp.append()
    }

    if(actionInfo->isChecked()){
        showLvL |= SHOW_LVL_INFO;
    }

    if(actionDebug->isChecked()){
        showLvL |= SHOW_LVL_DEBUG;
    }

    if(actionError->isChecked()){
        showLvL |= SHOW_LVL_ERROR;
    }
}

void MainWindow::loadSettingFromIni()
{
    QSettings settings(SETTINGS_INI_FILE, QSettings::IniFormat);

    // 初始化配色设置

    settings.beginGroup("color_settings");

    m_Pallette->filter_fg_color = settings.value("filter_fg_color", QColor(Qt::red)).value<QColor>();
    m_Pallette->filter_bg_color = settings.value("filter_bg_color", QColor(Qt::white)).value<QColor>();

    m_Pallette->search_fg_color = settings.value("search_fg_color", QColor(Qt::black)).value<QColor>();
    m_Pallette->search_bg_color = settings.value("search_bg_color", QColor(255, 247, 100)).value<QColor>();

    /*notice*/
    m_Pallette->notice_fg_color = settings.value("notice_fg_color", QColor(Qt::black)).value<QColor>();
    m_Pallette->notice_bg_color = settings.value("notice_bg_color", QColor(Qt::white)).value<QColor>();

    /*info*/
    m_Pallette->info_fg_color = settings.value("info_fg_color", QColor(Qt::black)).value<QColor>();
    m_Pallette->info_bg_color = settings.value("info_bg_color", QColor(Qt::white)).value<QColor>();

    /*debug*/
    m_Pallette->debug_fg_color = settings.value("debug_fg_color", QColor(Qt::black)).value<QColor>();
    m_Pallette->debug_bg_color = settings.value("debug_bg_color", QColor(Qt::white)).value<QColor>();

    /*error*/
    m_Pallette->error_fg_color = settings.value("error_fg_color", QColor(Qt::black)).value<QColor>();
    m_Pallette->error_bg_color = settings.value("error_bg_color", QColor(Qt::white)).value<QColor>();
    settings.endGroup();

    // 初始化字体

    settings.beginGroup("font_settings");
    QFont defaultFont = textBrowser->font();
    m_font = settings.value("font", defaultFont).value<QFont >();
    settings.endGroup();

    settings.beginGroup("network_settings");
    m_hostName = settings.value("hostname", "leadon-pub.vicp.cc").value<QString>();
    m_port = settings.value("port", 12345).value<int>();
    settings.endGroup();

    textBrowser->setFont(m_font);

}


void MainWindow::on_actionNotice_triggered()
{

}

void MainWindow::on_actionInfo_triggered()
{
    if(actionInfo->isChecked()){
        showLvL |= SHOW_LVL_INFO;
    }else{
        showLvL &= ~SHOW_LVL_INFO;
    }
}

void MainWindow::on_actionDebug_triggered()
{

}

void MainWindow::on_actionError_triggered()
{

}

void MainWindow::on_actionLink_triggered()
{
    if(netReceiver->state() != QAbstractSocket::ConnectedState){
        netReceiver->connectToLogForwarder(m_hostName, m_port);
    }
}

void MainWindow::on_m_filter_casesensitivity_clicked()
{
    Qt::CaseSensitivity c = m_filter_casesensitivity->isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive;

    logFilter.setCaseSensitivity(c);
}


void MainWindow::on_m_filter_cb_activated(int index)
{
    QString filter = m_Filter->text();

    if(filter.isEmpty() || index == 2){
        return;
    }

    Qt::CaseSensitivity c = m_filter_casesensitivity->isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive;
    if(index == 1){
        filter = QString(NO_PIPEI).arg(filter);
    }

    logFilter = QRegExp(filter, c);
}

void MainWindow::on_actionGoDowm_triggered()
{
    QScrollBar *bar = textBrowser->verticalScrollBar();
    bar->setSliderPosition(bar->maximum());
}

void MainWindow::on_actionSave_triggered()
{
    if(!savedFile->isOpen()){
        savedFile->setFileName(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")+".log");
        if(!savedFile->open(QIODevice::Append)){

        }
    }else{
        savedFile->close();
    }
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog *dia = new SettingsDialog(m_hostName, m_port, m_font, m_Pallette, this);
    if(dia->exec()){
        loadSettingFromIni();
    }
}

void MainWindow::on_pb_search_clicked()
{
    QTextDocument *document = textBrowser->document();
    QString filter = m_search->text();

    if (!filter.isEmpty()) {
        QTextCursor highlightCursor(document);
        QTextCursor cursor(document);

        //***************开始***************
        cursor.beginEditBlock();

        QTextCharFormat plainFormat(highlightCursor.charFormat());
        QTextCharFormat colorFormat = plainFormat;
        colorFormat.setBackground(m_Pallette->search_bg_color);
        colorFormat.setForeground(m_Pallette->error_fg_color);

        QFlag flag = 0;

        if(cb_search_case->isChecked()){
            flag = flag | QTextDocument::FindCaseSensitively;
        }

        if(cb_wholeWord->isChecked()){
            flag = flag | QTextDocument::FindWholeWords;
        }


        while (!highlightCursor.isNull() && !highlightCursor.atEnd()) {
            highlightCursor = document->find(filter, highlightCursor, flag);

            if (!highlightCursor.isNull()) {
                highlightCursor.mergeCharFormat(colorFormat);
            }
        }

        cursor.endEditBlock();
        //***************结束***************
    }

}

void MainWindow::on_tb_findNext_clicked()
{
    QString finder = m_search->text();

    if(!finder.isEmpty()){
        QFlag flag = 0;

        if(cb_search_case->isChecked()){
            flag = flag | QTextDocument::FindCaseSensitively;
        }

        if(cb_wholeWord->isChecked()){
            flag = flag | QTextDocument::FindWholeWords;
        }

        textBrowser->find( finder, flag );
    }
}

void MainWindow::on_tb_findForward_clicked()
{
    QString finder = m_search->text();

    if(!finder.isEmpty()){
        QFlag flag = QTextDocument::FindBackward;

        if(cb_search_case->isChecked()){
            flag = flag | QTextDocument::FindCaseSensitively;
        }

        if(cb_wholeWord->isChecked()){
            flag = flag |  QTextDocument::FindWholeWords;

        }

        textBrowser->find( finder, flag );
    }
}
