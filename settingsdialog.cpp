#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QColorDialog>
#include <QDebug>
#include <QFontDialog>


SettingsDialog::SettingsDialog(const QString &hostName,
    int port, const QFont &font, LogPallette *pallette, QWidget *parent) :
    QDialog(parent),m_font(font),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ui->lineEdit_hostName->setText(hostName);
    ui->lineEdit_port->setText(QString("%1").arg(port));

    ui->label_font_family->setText(m_font.family());
    ui->label_font_size->setText(QString("%1").arg(m_font.pointSize()));

    ui->filter_fg_color->setCurrentColor(pallette->filter_fg_color);
    ui->filter_bg_color->setCurrentColor(pallette->filter_bg_color);

    ui->search_fg_color->setCurrentColor(pallette->search_fg_color);
    ui->search_bg_color->setCurrentColor(pallette->search_bg_color);

    ui->notice_fg_color->setCurrentColor(pallette->notice_fg_color);
    ui->notice_bg_color->setCurrentColor(pallette->notice_bg_color);

    ui->info_fg_color->setCurrentColor(pallette->info_fg_color);
    ui->info_bg_color->setCurrentColor(pallette->info_bg_color);

    ui->debug_fg_color->setCurrentColor(pallette->debug_fg_color);
    ui->debug_bg_color->setCurrentColor(pallette->debug_bg_color);

    ui->error_fg_color->setCurrentColor(pallette->error_fg_color);
    ui->error_fg_color->setCurrentColor(pallette->error_fg_color);

//    ui->debug_bg_color->setCurrentColor(pallette->debug_bg_color);
//    ui->debug_bg_color->setCurrentColor(pallette->debug_bg_color);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

#if 0
void SettingsDialog::on_tb_cc_notice_clicked()
{
    changeFGPalletteShow(qobject_cast<QToolButton *>(sender()), QColorDialog::getColor());
}


void SettingsDialog::changeFGPalletteShow(QToolButton *btn, const QColor &c)
{
    QPalette palette;
    palette.setColor(QPalette::Button, c);
    btn->setPalette(palette);
    btn->setAutoFillBackground(true);

    qDebug() << c;
}
#endif

void SettingsDialog::on_font_chooser_clicked()
{
    bool ok;
    m_font = QFontDialog::getFont(&ok, m_font,  this);
    if (ok) {
        // font is set to the font the user selected
        ui->label_font_family->setText(m_font.family());
        ui->label_font_size->setText(QString("%1").arg(m_font.pointSize()));
    }
}

void SettingsDialog::on_buttonBox_accepted()
{
    QSettings settings(SETTINGS_INI_FILE, QSettings::IniFormat);

    settings.beginGroup("color_settings");

    settings.setValue("filter_fg_color",  ui->filter_fg_color->currentColor());
    settings.setValue("filter_bg_color",  ui->filter_bg_color->currentColor());

    settings.setValue("search_fg_color",  ui->search_fg_color->currentColor());
    settings.setValue("search_bg_color",  ui->search_bg_color->currentColor());


    settings.setValue("notice_fg_color",  ui->notice_fg_color->currentColor());
    settings.setValue("notice_bg_color",  ui->notice_bg_color->currentColor());

    settings.setValue("info_fg_color",  ui->info_fg_color->currentColor());
    settings.setValue("info_bg_color",  ui->info_bg_color->currentColor());

    settings.setValue("debug_fg_color",  ui->debug_fg_color->currentColor());
    settings.setValue("debug_bg_color",  ui->debug_bg_color->currentColor());

    settings.setValue("error_fg_color",  ui->error_fg_color->currentColor());
    settings.setValue("error_bg_color",  ui->error_bg_color->currentColor());
    settings.endGroup();

    // 保存化字体

    settings.beginGroup("font_settings");
    settings.setValue("font",  m_font);
    settings.endGroup();

    settings.beginGroup("network_settings");
    settings.setValue("hostname",  ui->lineEdit_hostName->text());
    settings.setValue("font",  ui->lineEdit_port->text().toInt());
    settings.endGroup();

    QDialog::accept();
}

void SettingsDialog::on_buttonBox_rejected()
{
    QDialog::reject();
}
