#pragma once

#include <QMainWindow>

#include "mywidget.h"

class QTimer;

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private Q_SLOTS:
    void reset();
    MyWidget::Result step();
    void animate();
    void solve();
    void timeout();

private:
    void solved();
    void notSolvable();

    Ui::MainWindow *ui;
    QTimer *m_timer;
};
