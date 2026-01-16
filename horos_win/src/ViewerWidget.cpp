#include "ViewerWidget.h"
#include <QButtonGroup> // Added
#include <QHBoxLayout>  // Ensure QHBoxLayout is included for ctlLayout
#include <QLabel>
#include <QPushButton> // Added
#include <QSlider>
#include <QVBoxLayout>

ViewerWidget::ViewerWidget(const QString &studyUID, QWidget *parent)
    : QWidget(parent), m_studyUID(studyUID) {
  setupUI();

  // Mock image data for testing (512x512 gradient)
  QVector<uint16_t> mockData(512 * 512);
  for (int i = 0; i < 512 * 512; ++i)
    mockData[i] = (i % 512) * 100;

  m_view->setPixelData(mockData, 512, 512);
}

void ViewerWidget::setupUI() {
  QVBoxLayout *layout = new QVBoxLayout(this);

  // Viewer
  m_view = new DCMView(this);
  layout->addWidget(m_view, 1);

  // Controls Layout (will contain both tool selector and basic controls)
  QHBoxLayout *ctlLayout = new QHBoxLayout();

  // Tool Selector
  QButtonGroup *tools = new QButtonGroup(this);
  QPushButton *btnWL = new QPushButton("Contrast", this);
  QPushButton *btnRuler = new QPushButton("Ruler", this);
  QPushButton *btnEllipse = new QPushButton("Ellipse", this);
  QPushButton *btnAngle = new QPushButton("Angle", this);
  btnWL->setCheckable(true);
  btnRuler->setCheckable(true);
  btnEllipse->setCheckable(true);
  btnAngle->setCheckable(true);
  btnWL->setChecked(true); // Default to Window/Level tool

  tools->addButton(btnWL);
  tools->addButton(btnRuler);
  tools->addButton(btnEllipse);
  tools->addButton(btnAngle);

  ctlLayout->addWidget(btnWL);
  ctlLayout->addWidget(btnRuler);
  ctlLayout->addWidget(btnEllipse);
  ctlLayout->addWidget(btnAngle);

  // Basic Controls (Window/Level slider)
  QSlider *wwSlider = new QSlider(Qt::Horizontal, this);
  wwSlider->setRange(1, 4000);
  wwSlider->setValue(400);

  QLabel *wwLabel = new QLabel("WW:", this);
  ctlLayout->addWidget(wwLabel);
  ctlLayout->addWidget(wwSlider);

  layout->addLayout(
      ctlLayout); // Add the combined controls layout to the main layout

  // Connect tool buttons
  connect(btnWL, &QPushButton::clicked,
          [this]() { m_view->setToolMode(ToolMode::WindowLevel); });
  connect(btnRuler, &QPushButton::clicked,
          [this]() { m_view->setToolMode(ToolMode::Ruler); });
  connect(btnEllipse, &QPushButton::clicked,
          [this]() { m_view->setToolMode(ToolMode::Ellipse); });
  connect(btnAngle, &QPushButton::clicked,
          [this]() { m_view->setToolMode(ToolMode::Angle); });

  // Connect WW slider
  connect(wwSlider, &QSlider::valueChanged,
          [this](int val) { m_view->setWindowLevel(val, 40); });
}
