#include "MPRViewport.h"
#include <QPainter>

MPRViewport::MPRViewport(Plane plane, QWidget *parent)
    : QWidget(parent), m_plane(plane) {
  setMinimumSize(200, 200);
}

void MPRViewport::updateSlice(int index) { update(); }

void MPRViewport::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setPen(Qt::gray);
  painter.drawRect(0, 0, width() - 1, height() - 1);

  QString label;
  switch (m_plane) {
  case Plane::Axial:
    label = "Axial";
    break;
  case Plane::Sagittal:
    label = "Sagittal";
    break;
  case Plane::Coronal:
    label = "Coronal";
    break;
  case Plane::Volume:
    label = "3D Volume";
    break;
  }

  painter.setPen(Qt::white);
  painter.drawText(10, 20, label);
}
