#include "setting.h"
#include "ui_setting.h"
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>


Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	setWindowTitle("Setting");

	setting = new QSettings("config.ini", QSettings::IniFormat);
	// ��ʼ��ȡ�����ļ�
	loadSetting();
}

Setting::~Setting()
{
    delete ui;
}

void Setting::loadSetting()
{
	config.backgroundMusicOn = setting->value("backgroundMusicOn").toBool();
	config.difficulty = setting->value("difficulty").toInt();

	ui->musicCheckBox->setChecked(config.backgroundMusicOn);
	// load difficulty setting
	if (config.difficulty == Pease) {
		ui->peace->setChecked(true);
	}
	else if (config.difficulty == Easy) {
		ui->easy->setChecked(true);
	}
	else if (config.difficulty == Normal) {
		ui->normal->setChecked(true);
	}
	else if (config.difficulty == Difficult) {
		ui->difficult->setChecked(true);
	}
}

void Setting::updateSetting()
{
	config.backgroundMusicOn = ui->musicCheckBox->isChecked();
	// update difficulty setting
	if (ui->peace->isChecked()){
		config.difficulty = Pease;
	}
	else if (ui->easy->isChecked()) {
		config.difficulty = Easy;
	}
	else if (ui->normal->isChecked()) {
		config.difficulty = Normal;
	}
	else if (ui->difficult->isChecked()) {
		config.difficulty = Difficult;
	}

	setting->setValue("difficulty", config.difficulty);
	setting->setValue("backgroundMusicOn", config.backgroundMusicOn);
}

void Setting::resetSetting()
{
	ui->musicCheckBox->setChecked(true);
	ui->normal->setChecked(true);

	updateSetting();
}

void Setting::on_okBtn_clicked()
{
	updateSetting();
	// QMessageBox::information(this, "Done", "OK");
	QDialog::close();

}

void Setting::on_resetBtn_clicked()
{
	resetSetting();
}

void Setting::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mouseMoveing = true;
		lastMousePosition = event->globalPos();
	}
}

void Setting::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons().testFlag(Qt::LeftButton) && mouseMoveing)
	{
		this->move(this->pos() + (event->globalPos() - lastMousePosition));
		lastMousePosition = event->globalPos();
	}
}

void Setting::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mouseMoveing = false;
	}
}
