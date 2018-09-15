#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->widget->reset(ui->spinBoxColumns->value(), ui->spinBoxRows->value());

    m_timer = new QTimer(this);
    m_timer->setInterval(1000/30);

    connect(ui->pushButtonReset, &QAbstractButton::pressed, this, &MainWindow::reset);
    connect(ui->pushButtonStep, &QAbstractButton::pressed, this, &MainWindow::step);
    connect(ui->pushButtonAnimate, &QAbstractButton::pressed, this, &MainWindow::animate);
    connect(ui->pushButtonSolve, &QAbstractButton::pressed, this, &MainWindow::solve);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::timeout);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reset()
{
    ui->widget->reset(ui->spinBoxColumns->value(), ui->spinBoxRows->value());
    ui->widget->repaint();

    ui->pushButtonStep->setEnabled(true);
    ui->pushButtonAnimate->setEnabled(true);
    ui->pushButtonSolve->setEnabled(true);
}

MyWidget::Result MainWindow::step()
{
    auto result = ui->widget->compute();
    ui->widget->repaint();
    switch(result)
    {
    case MyWidget::Result::Solved:
        solved();
        break;
    case MyWidget::Result::NotSolvable:
        notSolvable();
        break;
    }
    return result;
}

void MainWindow::animate()
{
    ui->spinBoxColumns->setEnabled(false);
    ui->spinBoxRows->setEnabled(false);
    ui->pushButtonReset->setEnabled(false);
    ui->pushButtonStep->setEnabled(false);
    ui->pushButtonAnimate->setEnabled(false);
    ui->pushButtonSolve->setEnabled(false);

    m_timer->start();
}

void MainWindow::solve()
{
    MyWidget::Result result;
    while((result = ui->widget->compute()) == MyWidget::Result::NotFinished);
    ui->widget->repaint();
    switch (result) {
    case MyWidget::Result::Solved:
        solved();
        break;
    case MyWidget::Result::NotSolvable:
        notSolvable();
        break;
    default:
        Q_UNREACHABLE();
    }
}

void MainWindow::timeout()
{
    auto result = step();
    switch(result)
    {
    case MyWidget::Result::Solved:
    case MyWidget::Result::NotSolvable:
        m_timer->stop();
        ui->spinBoxColumns->setEnabled(true);
        ui->spinBoxRows->setEnabled(true);
        ui->pushButtonReset->setEnabled(true);
        break;
    }
}

void MainWindow::solved()
{
    QMessageBox::information(this, tr("Found the optimal solution"), tr("The optimal solution was found and will be drawn in darker green."));
    ui->pushButtonStep->setEnabled(false);
    ui->pushButtonAnimate->setEnabled(false);
    ui->pushButtonSolve->setEnabled(false);
}

void MainWindow::notSolvable()
{
    QMessageBox::warning(this, tr("No solution found"), tr("There is no solution!"));
    ui->pushButtonStep->setEnabled(false);
    ui->pushButtonAnimate->setEnabled(false);
    ui->pushButtonSolve->setEnabled(false);
}
