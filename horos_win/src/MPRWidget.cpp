#include "MPRWidget.h"
#include "MPRViewport.h"

MPRWidget::MPRWidget(QWidget *parent) : QWidget(parent) {
  m_layout = new QGridLayout(this);
  m_layout->setSpacing(2);
  m_layout->setContentsMargins(0, 0, 0, 0);

  m_axialView = new MPRViewport(MPRViewport::Plane::Axial, this);
  m_sagittalView = new MPRViewport(MPRViewport::Plane::Sagittal, this);
  m_coronalView = new MPRViewport(MPRViewport::Plane::Coronal, this);
  m_3dView = new MPRViewport(MPRViewport::Plane::Volume, this);

  m_layout->addWidget(m_axialView, 0, 0);
  m_layout->addWidget(m_coronalView, 0, 1);
  m_layout->addWidget(m_sagittalView, 1, 0);
  m_layout->addWidget(m_3dView, 1, 1);

  setStyleSheet("background-color: #121212;");
}

MPRWidget::~MPRWidget() {}

void MPRWidget::setSeries(const QStringList &imageFiles) {
  // Logic to initialize VTK volume from images
}
