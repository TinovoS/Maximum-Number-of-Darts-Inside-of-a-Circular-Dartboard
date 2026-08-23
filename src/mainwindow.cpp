#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "algorithm.h"


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Maximum Number of Darts Inside of a Circular Dartboard");
    resize(800, 600);

    auto *mainLayout = new QVBoxLayout(this);
    auto *inputLayout = new QHBoxLayout();
    
    inputLayout->addWidget(new QLabel("Darts (x1,y1;x2,y2;....)"));
    dartsInput = new QLineEdit("-2,0;2,0;0,2;0,-2");
    inputLayout->addWidget(dartsInput);

    inputLayout->addWidget(new QLabel("r:"));
    radiusInput = new QLineEdit("2");
    radiusInput->setMaximumWidth(60);
    inputLayout->addWidget(radiusInput);

    solveButton = new QPushButton("Solve");
    inputLayout->addWidget(solveButton);
    
    mainLayout->addLayout(inputLayout);

    resultLabel = new QLabel("Enter darts and radius, then click Solve.");
    mainLayout->addWidget(resultLabel);

    connect(solveButton, &QPushButton::clicked, this, &MainWindow::onSolve);
}

MainWindow::~MainWindow() {}

void MainWindow::onSolve()
{
    std::vector<Point> darts;
    double radius;
    Result result;
    bool solved = false;
}
