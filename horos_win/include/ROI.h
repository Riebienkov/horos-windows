#ifndef ROI_H
#define ROI_H

#include <QLineF>
#include <QPainter>
#include <QPointF>
#include <QRectF>
#include <QString>
#include <QTransform>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <QPainter>
#include <QPointF>
#include <QString>

enum class ROIType { Ruler, Oval, Rectangle };

struct ROIStats {
  double mean = 0.0;
  double sd = 0.0;
  double area = 0.0; // in mm^2
  double min = 0.0;
  double max = 0.0;
};

class ROI {
public:
  virtual ~ROI() = default;
  virtual ROIType type() const = 0;
  virtual void draw(QPainter &painter, const QTransform &viewTransform) = 0;
  ROIStats stats;
};

class RulerROI : public ROI {
public:
  QPointF p1, p2;
  float pixelSpacingX = 1.0f;
  float pixelSpacingY = 1.0f;

  ROIType type() const override { return ROIType::Ruler; }

  void draw(QPainter &painter, const QTransform &viewTransform) override {
    QPointF screenP1 = viewTransform.map(p1);
    QPointF screenP2 = viewTransform.map(p2);

    painter.setPen(QPen(Qt::yellow, 2));
    painter.drawLine(screenP1, screenP2);

    // Calculate distance
    double dx = (p2.x() - p1.x()) * pixelSpacingX;
    double dy = (p2.y() - p1.y()) * pixelSpacingY;
    double dist = std::sqrt(dx * dx + dy * dy);

    painter.drawText(screenP2 + QPointF(5, -5),
                     QString("%1 mm").arg(dist, 0, 'f', 1));
  }
};

class EllipseROI : public ROI {
public:
  QRectF rect; // in image coordinates
  float pixelSpacingX = 1.0f;
  float pixelSpacingY = 1.0f;

  ROIType type() const override { return ROIType::Oval; }

  void draw(QPainter &painter, const QTransform &viewTransform) override {
    QRectF screenRect = viewTransform.mapRect(rect);

    painter.setPen(QPen(Qt::cyan, 2));
    painter.drawEllipse(screenRect);

    // Stats Display
    QString text = QString("Mean: %1\nArea: %2 mm²")
                       .arg(stats.mean, 0, 'f', 1)
                       .arg(stats.area, 0, 'f', 1);

    painter.setPen(Qt::white);
    painter.drawText(screenRect.bottomRight() + QPointF(5, 5), text);
  }
};

class AngleROI : public ROI {
public:
  QPointF p1, p2, p3; // p1: Vertex, p2/p3: sides

  ROIType type() const override { return ROIType::Angle; }

  void draw(QPainter &painter, const QTransform &viewTransform) override {
    QPointF s1 = viewTransform.map(p1);
    QPointF s2 = viewTransform.map(p2);
    QPointF s3 = viewTransform.map(p3);

    painter.setPen(QPen(Qt::green, 2));
    painter.drawLine(s1, s2);
    painter.drawLine(s1, s3);

    // Math for angle
    double angle1 = std::atan2(p2.y() - p1.y(), p2.x() - p1.x());
    double angle2 = std::atan2(p3.y() - p1.y(), p3.x() - p1.x());
    double degree = std::abs(angle1 - angle2) * 180.0 / M_PI;
    if (degree > 180.0)
      degree = 360.0 - degree;

    painter.drawText(s1 + QPointF(10, 10),
                     QString("%1°").arg(degree, 0, 'f', 1));
  }
};

#endif // ROI_H
