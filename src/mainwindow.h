#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

#include "algorithm.h"

class MainWindow : public QWidget
{
    Q_OBJECT
private:
    QLineEdit *dartsInput;
    QLineEdit *radiusInput;
    QPushButton *solveButton;
    QLabel *resultLabel;

    std::vector<Point> darts;
    double radius = 2.0;
    Result result;
    bool solved = false;

    QTimer *animTimer;
    int sweepIndex = 0;       
    double sweepAngle = 0.0;  
    bool animating = false;

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onSolve();
    void onAnimStep();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
