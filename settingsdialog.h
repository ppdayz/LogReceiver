#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

#define SETTINGS_INI_FILE ("./LogReceiver.ini")

class QToolButton;

namespace Ui {
class SettingsDialog;
}


class LogPallette
{
public:
    /** 配色 */
    QColor filterColor;

    QColor filter_fg_color;
    QColor filter_bg_color;

    QColor search_fg_color;
    QColor search_bg_color;

    /*notice*/
    QColor notice_fg_color;
    QColor notice_bg_color;

    /*info*/
    QColor info_fg_color;
    QColor info_bg_color;

    /*debug*/
    QColor debug_fg_color;
    QColor debug_bg_color;

    /*error*/
    QColor error_fg_color;
    QColor error_bg_color;

    explicit LogPallette(){}

};
class SettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingsDialog(const QString &hostName, int port, const QFont &font,
                            LogPallette *pallette, QWidget *parent = 0);
    ~SettingsDialog();
    
private slots:
//    void on_tb_cc_notice_clicked();
    void on_font_chooser_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::SettingsDialog *ui;

    QFont m_font;
//    void changeFGPalletteShow(QToolButton *btn, const QColor &c);


};

#endif // SETTINGSDIALOG_H
