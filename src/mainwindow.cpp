#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>

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

    result = angularSweep(darts, radius);
    solved = true;

    resultLabel->setText(QString("Max darts: %1 | Center: (%2, %3)")
        .arg(result.maxDarts)
        .arg(result.cx, 0, 'f', 2)
        .arg(result.cy, 0, 'f', 2));
        
    update();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    if (!solved || darts.empty())
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF drawArea(10, 60, width() - 20, height() - 80);
    painter.fillRect(drawArea, Qt::white);

    double minX = darts[0].x, maxX = darts[0].x;
    double minY = darts[0].y, maxY = darts[0].y;
    for (const auto &p : darts) {
        minX = std::min(minX, p.x);
        maxX = std::max(maxX, p.x);
        minY = std::min(minY, p.y);
        maxY = std::max(maxY, p.y);
    }
    minX = std::min(minX, result.cx - radius);
    maxX = std::max(maxX, result.cx + radius);
    minY = std::min(minY, result.cy - radius);
    maxY = std::max(maxY, result.cy + radius);

    double margin = 1.0;
    minX -= margin; maxX += margin;
    minY -= margin; maxY += margin;

    double rangeX = maxX - minX;
    double rangeY = maxY - minY;
    double scale = std::min(drawArea.width() / rangeX, drawArea.height() / rangeY);

    double offsetX = drawArea.left() + (drawArea.width() - rangeX * scale) / 2.0 - minX * scale;
    double offsetY = drawArea.top() + (drawArea.height() + rangeY * scale) / 2.0 + minY * scale;

    painter.setClipRect(drawArea);

    painter.setPen(QPen(QColor(30, 100, 220), 2));
    painter.setBrush(QBrush(QColor(30, 100, 220, 30)));
    double circleScreenR = radius * scale;
    double cx = offsetX + result.cx * scale;
    double cy = offsetY - result.cy * scale;
    painter.drawEllipse(QPointF(cx, cy), circleScreenR, circleScreenR);

    int n = static_cast<int>(darts.size());
    for (int i = 0; i < n; ++i) {
        double sx = offsetX + darts[i].x * scale;
        double sy = offsetY - darts[i].y * scale;

        if (result.inside[i]) {
            painter.setPen(QPen(QColor(0, 160, 0), 2));
            painter.setBrush(QColor(0, 160, 0));
        } else {
            painter.setPen(QPen(QColor(200, 0, 0), 2));
            painter.setBrush(QColor(200, 0, 0));
        }
        painter.drawEllipse(QPointF(sx, sy), 5, 5);
    }
}
