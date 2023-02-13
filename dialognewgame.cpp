#include "dialognewgame.h"
#include "ui_dialognewgame.h"

DialogNewGame::DialogNewGame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewGame)
{
    ui->setupUi(this);
    ui->spinBox->setValue(3);
    ui->spinBox->setMinimum(2);
    ui->spinBox->setMaximum(10);
}

DialogNewGame::~DialogNewGame()
{
    delete ui;
}

void DialogNewGame::on_buttonBox_accepted()
{
    _largeur = ui->spinBox->value();
}
