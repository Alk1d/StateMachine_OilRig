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

    Oil_timer = new QTimer();
    Fuel_timer = new QTimer();
    connect(Oil_timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
    connect(Fuel_timer, SIGNAL(timeout()), this, SLOT(onFuelTimer()));
    Oil_timer->start(10000);

    OilRig_StateMachine->addState(HasOil);
    OilRig_StateMachine->addState(NoOil);
    OilRig_StateMachine->addState(FuelCreation);
    OilRig_StateMachine->addState(NoMoney);
    OilRig_StateMachine->setInitialState(NoOil);
    OilRig_StateMachine->start();

    NoOil->assignProperty(ui->State_label, "text", "No Oil");
    NoOil->assignProperty(ui->CurrentState_label, "text", "No Oil");
    NoOil->addTransition(ui->Insert_button, &QPushButton::clicked, HasOil);

    HasOil->assignProperty(ui->State_label, "text", "Has Oil");
    HasOil->assignProperty(ui->CurrentState_label, "text", "Has Oil");
    HasOil->addTransition(ui->CreateFuel_button, &QPushButton::clicked, FuelCreation);
    HasOil->addTransition(ui->Return_button, &QPushButton::clicked, NoOil);

    FuelCreation->assignProperty(ui->State_label, "text", "Fuel creation");
    FuelCreation->assignProperty(ui->CurrentState_label, "text", "Fuel creation");
    FuelCreation->addTransition(ui->Fuel_spinBox, &QSpinBox::valueChanged, NoOil);

    NoMoney->assignProperty(ui->State_label, "text", "No money");
    NoMoney->assignProperty(ui->CurrentState_label, "text", "No money");

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Insert_button_clicked()
{
    if (OilRig_StateMachine->configuration().contains(HasOil))
        ui->State_label->setText("Already full!");
    if (OilRig_StateMachine->configuration().contains(FuelCreation))
        ui->State_label->setText("Creating fuel, wait!");
   return;
}


void MainWindow::on_Return_button_clicked()
{
    if (OilRig_StateMachine->configuration().contains(NoOil))
        ui->State_label->setText("Nothing to return!");
    if (OilRig_StateMachine->configuration().contains(FuelCreation))
        ui->State_label->setText("Creating fuel, wait!");
    return;
}


void MainWindow::on_CreateFuel_button_clicked()
{
    if (OilRig_StateMachine->configuration().contains(NoOil))
        ui->State_label->setText("No oil to create fuel!");
    if (OilRig_StateMachine->configuration().contains(NoMoney))
        ui->State_label->setText("No money to create fuel!");
    if (OilRig_StateMachine->configuration().contains(HasOil))
        FuelProduction();
    return;
}





void MainWindow::slotTimerAlarm()
{
    ui->Oil_spinBox->setValue(ui->Oil_spinBox->value()+1);
}

void MainWindow::onFuelTimer()
{
    if (ui->progressBar->value() > 99)
    {
        ui->progressBar->setValue(1);
        Fuel_timer->stop();
        ui->Fuel_spinBox->setValue(ui->Fuel_spinBox->value()+1);
        return;
    }
    ui->progressBar->setValue(ui->progressBar->value()+1);
}

void MainWindow::FuelProduction()
{
        Fuel_timer->start(10);
        ui->Money_spinBox->setValue(ui->Money_spinBox->value()-1);
        ui->Oil_spinBox->setValue(ui->Oil_spinBox->value()-1);
}



void MainWindow::on_Money_spinBox_valueChanged(int arg1)
{
    if (arg1 == 0)
        NoOil->addTransition(NoMoney);
    else NoMoney->addTransition(NoOil);
    return;
}


void MainWindow::on_SellFuel_button_clicked()
{
    if (OilRig_StateMachine->configuration().contains(FuelCreation))
    {
        ui->State_label->setText("Creating fuel, wait!");
        return;
    }
    int Fuel_amount = ui->Fuel_spinBox->value();
    if (Fuel_amount > 0)
    {
        ui->Fuel_spinBox->setValue(Fuel_amount - 1);
        ui->Money_spinBox->setValue(ui->Money_spinBox->value() +1);
    }
    else ui->State_label->setText("No Fuel to sell!");
    return;
}

