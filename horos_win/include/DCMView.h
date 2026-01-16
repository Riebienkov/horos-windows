#ifndef DCMVIEW_H
#define DCMVIEW_H

#include "ROI.h"
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>

enum class ToolMode { WindowLevel, Pan, Ruler, Ellipse, Angle };

class DCMView : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

public:
  explicit DCMView(QWidget *parent = nullptr);
  ~DCMView();

  void setPixelData(const QVector<uint16_t> &data, int width, int height);
  void setWindowLevel(float width, float level);
  void setToolMode(ToolMode mode) { m_toolMode = mode; }
  void setImageStack(const QStringList &files);
  void nextImage();
  void prevImage();

protected:
  void initializeGL() override;
  void paintGL() override;
  void paintEvent(QPaintEvent *event) override; // For ROI overlays
  void resizeGL(int w, int h) override;

  // Interaction Overrides
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

private:
  void setupShaders();
  void updateTexture();
  void loadCurrentImage();

  QOpenGLShaderProgram *m_program = nullptr;
  QOpenGLTexture *m_texture = nullptr;

  QVector<uint16_t> m_pixelData;
  int m_imgWidth = 0;
  int m_imgHeight = 0;

  float m_windowWidth = 400.0f;
  float m_windowLevel = 40.0f;

  // Zoom/Pan
  float m_scale = 1.0f;
  QPointF m_panOffset;
  QPoint m_lastMousePos;
  bool m_isPanning = false;
  bool m_isDrawing = false;

  ToolMode m_toolMode = ToolMode::WindowLevel;
  QList<ROI *> m_rois;
  RulerROI *m_currentRuler = nullptr;
  EllipseROI *m_currentEllipse = nullptr;
  AngleROI *m_currentAngle = nullptr;
  int m_clickCount = 0;

  QStringList m_imageFiles;
  int m_currentIndex = 0;

  void calculateROIStats(ROI *roi);

  GLuint m_vbo = 0;
};

#endif // DCMVIEW_H
