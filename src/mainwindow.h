#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class MainWindow : public QWidget
{
    Q_OBJECT
private:
    QLineEdit *dartsInput;
    QLineEdit *radiusInput;
    QPushButton *solveButton;
    QLabel *resultLabel;

private slots:
    void onSolve();
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
