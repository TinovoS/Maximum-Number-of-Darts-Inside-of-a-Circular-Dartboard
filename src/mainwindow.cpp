#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <cmath>

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

    animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, &MainWindow::onAnimStep);

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

    sweepIndex = 0;
    sweepAngle = -M_PI;
    animating = true;
    animTimer->start(50);
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

    if (animating && sweepIndex < n) {
        double pivotX = offsetX + darts[sweepIndex].x * scale;
        double pivotY = offsetY - darts[sweepIndex].y * scale;

        painter.setPen(QPen(Qt::yellow, 3));
        painter.setBrush(Qt::yellow);
        painter.drawEllipse(QPointF(pivotX, pivotY), 7, 7);

        double animCx = darts[sweepIndex].x + radius * std::cos(sweepAngle);
        double animCy = darts[sweepIndex].y + radius * std::sin(sweepAngle);
        double asx = offsetX + animCx * scale;
        double asy = offsetY - animCy * scale;

        painter.setPen(QPen(QColor(255, 165, 0), 2, Qt::DashLine));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(QPointF(asx, asy), circleScreenR, circleScreenR);

        int animCount = 0;
        for (int j = 0; j < n; ++j) {
            double dx = darts[j].x - animCx;
            double dy = darts[j].y - animCy;
            if (dx * dx + dy * dy <= radius * radius + 1e-6) {
                double sx2 = offsetX + darts[j].x * scale;
                double sy2 = offsetY - darts[j].y * scale;
                painter.setPen(QPen(QColor(0, 100, 255), 2));
                painter.setBrush(QColor(0, 100, 255));
                painter.drawEllipse(QPointF(sx2, sy2), 5, 5);
                ++animCount;
            }
        }

        painter.setPen(Qt::black);
        QFont font = painter.font();
        font.setPointSize(12);
        font.setBold(true);
        painter.setFont(font);
        painter.drawText(drawArea.right() - 160, drawArea.top() + 25,
                         QString("Point: %1/%2").arg(sweepIndex + 1).arg(n));
        painter.drawText(drawArea.right() - 160, drawArea.top() + 50,
                         QString("Current: %1").arg(animCount));
        painter.drawText(drawArea.right() - 160, drawArea.top() + 75,
                         QString("Best: %1").arg(result.maxDarts));
    }
}

void MainWindow::onAnimStep()
{
    sweepAngle += 0.03;
    if (sweepAngle > M_PI) {
        sweepAngle = -M_PI;
        sweepIndex++;
        if (sweepIndex >= static_cast<int>(darts.size())) {
            animTimer->stop();
            animating = false;
        }
    }
    update();
}
