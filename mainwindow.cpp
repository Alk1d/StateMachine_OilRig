#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    OilRig_StateMachine = new QStateMachine(this);

    HasOil = new QState();
    NoOil = new QState();
    FuelCreation = new QState();
    NoMoney = new QState();

    OilRig_StateMachine->addState(HasOil);
    OilRig_StateMachine->addState(NoOil);
    OilRig_StateMachine->addState(FuelCreation);
    OilRig_StateMachine->addState(NoMoney); // не реализован функционал
    OilRig_StateMachine->setInitialState(NoOil);
    OilRig_StateMachine->start();

    NoOil->assignProperty(ui->State_label, "text", "No Oil");
    NoOil->addTransition(ui->Insert_button, &QPushButton::clicked, HasOil);

    HasOil->assignProperty(ui->State_label, "text", "Has Oil");
    HasOil->addTransition(ui->CreateFuel_button, &QPushButton::clicked, FuelCreation);
    HasOil->addTransition(ui->Return_button, &QPushButton::clicked, NoOil);

    FuelCreation->assignProperty(ui->State_label, "text", "Fuel created, click to continue"); // не работает, надо отловить мышку
    FuelCreation->addTransition(NoOil);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Insert_button_clicked()
{
    if (OilRig_StateMachine->configuration().contains(HasOil))
        ui->State_label->setText("Already full!");
   return;
}


void MainWindow::on_Return_button_clicked()
{
    if (OilRig_StateMachine->configuration().contains(NoOil))
        ui->State_label->setText("Nothing to return!");
    return;
}


void MainWindow::on_CreateFuel_button_clicked()
{
    if (OilRig_StateMachine->configuration().contains(NoOil))
        ui->State_label->setText("No oil to create fuel!");
    return;
}


