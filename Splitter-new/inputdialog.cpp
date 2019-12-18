#include "inputdialog.h"
#include "ui_inputdialog.h"

InputDialog::InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDialog)
{
    ui->setupUi(this);

    ui->spinBox->setRange(3,21);
    ui->spinBox->setSingleStep(2);
    ui->spinBox->setValue(3);
    ui->lineEdit->setText(QString::number(3));
    ui->lineEdit->setEnabled(false);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &InputDialog::reject);

}

InputDialog::~InputDialog()
{
    delete ui;
}

void InputDialog::on_pushButton_clicked()
{
    emit emitRadius(ui->spinBox->value(),ui->lineEdit->text().toDouble());
    accept();
}

void InputDialog::setSpinEnable(bool isEnable){
    ui->lineEdit->setEnabled(isEnable);
}
