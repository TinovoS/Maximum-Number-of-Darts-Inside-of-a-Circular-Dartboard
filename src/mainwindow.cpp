#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>


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
    darts.clear();
    solved = false;

    bool ok;
    radius = radiusInput->text().toDouble(&ok);
    if (!ok || radius <= 0) return;

    QStringList pairs = dartsInput->text().split(";", Qt::SkipEmptyParts);
    for (const QString &pair : pairs) {
        QStringList coords = pair.trimmed().split(",");
        if (coords.size() != 2) return;
        double x = coords[0].trimmed().toDouble();
        double y = coords[1].trimmed().toDouble();
        darts.push_back({x, y});
    }

    if (darts.empty()) return;

    result = bruteForce(darts, radius);
    solved = true;

    resultLabel->setText(QString("Max darts: %1 | Center: (%2, %3)")
        .arg(result.maxDarts)
        .arg(result.cx, 0, 'f', 2)
        .arg(result.cy, 0, 'f', 2));
}
