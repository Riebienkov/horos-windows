#include "DCMView.h"
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

DCMView::DCMView(QWidget *parent) : QOpenGLWidget(parent) {}

DCMView::~DCMView() {
  makeCurrent();
  delete m_texture;
  delete m_program;
  doneCurrent();
}

void DCMView::setPixelData(const QVector<uint16_t> &data, int width,
                           int height) {
  m_pixelData = data;
  m_imgWidth = width;
  m_imgHeight = height;
  updateTexture();
  update();
}

void DCMView::setWindowLevel(float width, float level) {
  m_windowWidth = width;
  m_windowLevel = level;
  update();
}

void DCMView::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  setupShaders();
}

void DCMView::setupShaders() {
  m_program = new QOpenGLShaderProgram(this);

  // Vertex Shader: Full-screen quad
  const char *vsrc =
      "#version 330 core\n"
      "layout (location = 0) in vec2 aPos;\n"
      "layout (location = 1) in vec2 aTexCoord;\n"
      "out vec2 TexCoord;\n"
      "uniform mat4 modelViewProjection;\n"
      "void main() {\n"
      "   gl_Position = modelViewProjection * vec4(aPos, 0.0, 1.0);\n"
      "   TexCoord = aTexCoord;\n"
      "}\n";

  // Fragment Shader: Window/Level Mapping
  const char *fsrc =
      "#version 330 core\n"
      "in vec2 TexCoord;\n"
      "out vec4 FragColor;\n"
      "uniform sampler2D dicomTexture;\n"
      "uniform float windowWidth;\n"
      "uniform float windowLevel;\n"
      "void main() {\n"
      "   float intensity = texture(dicomTexture, TexCoord).r * 65535.0;\n"
      "   float low = windowLevel - windowWidth / 2.0;\n"
      "   float val = (intensity - low) / windowWidth;\n"
      "   val = clamp(val, 0.0, 1.0);\n"
      "   FragColor = vec4(vec3(val), 1.0);\n"
      "}\n";

  m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);
  m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);
  m_program->link();
}

void DCMView::updateTexture() {
  if (m_imgWidth <= 0 || m_imgHeight <= 0)
    return;

  makeCurrent();
  if (m_texture)
    delete m_texture;

  m_texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
  m_texture->setSize(m_imgWidth, m_imgHeight);
  m_texture->setFormat(QOpenGLTexture::R16_UNorm); // 16-bit support
  m_texture->allocateStorage();
  m_texture->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt16,
                     m_pixelData.constData());
  m_texture->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
  doneCurrent();
}

void DCMView::paintGL() {
  if (!m_texture || !m_program)
    return;

  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  m_program->bind();
  m_texture->bind();

  m_program->setUniformValue("windowWidth", m_windowWidth);
  m_program->setUniformValue("windowLevel", m_windowLevel);

  QMatrix4x4 matrix;
  matrix.translate(m_panOffset.x(), m_panOffset.y());
  matrix.scale(m_scale);
  m_program->setUniformValue("modelViewProjection", matrix);

  static const float vertices[] = {-1, -1, 0, 1, 1, -1, 1, 1,
                                   -1, 1,  0, 0, 1, 1,  1, 0};

  m_program->enableAttributeArray(0);
  m_program->enableAttributeArray(1);
  m_program->setAttributeArray(0, GL_FLOAT, vertices, 2, 4 * sizeof(float));
  m_program->setAttributeArray(1, GL_FLOAT, &vertices[2], 2, 4 * sizeof(float));

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  m_program->disableAttributeArray(0);
  m_program->disableAttributeArray(1);
}

void DCMView::paintEvent(QPaintEvent *event) {
  paintGL();

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QTransform transform;
  transform.translate(width() / 2.0, height() / 2.0);
  transform.scale(width() / 2.0 * m_scale, -height() / 2.0 * m_scale);
  transform.translate(m_panOffset.x(), m_panOffset.y());

  // Draw all permanent ROIs
  for (ROI *roi : m_rois) {
    roi->draw(painter, transform);
  }

  // Draw current active tool previews
  if (m_currentRuler)
    m_currentRuler->draw(painter, transform);
  if (m_currentEllipse)
    m_currentEllipse->draw(painter, transform);
  if (m_currentAngle)
    m_currentAngle->draw(painter, transform);

  // Overlay info
  painter.setPen(Qt::white);
  painter.drawText(10, 20,
                   QString("Image: %1 / %2")
                       .arg(m_currentIndex + 1)
                       .arg(m_imageFiles.size()));

  // SMART INTERACTION: Detect intersections between RulerROIs
  QList<RulerROI *> rulers;
  for (ROI *roi : m_rois) {
    if (roi->type() == ROIType::Ruler)
      rulers.append(static_cast<RulerROI *>(roi));
  }
  if (m_currentRuler)
    rulers.append(m_currentRuler);

  for (int i = 0; i < rulers.size(); ++i) {
    for (int j = i + 1; j < rulers.size(); ++j) {
      QLineF l1(rulers[i]->p1, rulers[i]->p2);
      QLineF l2(rulers[j]->p1, rulers[j]->p2);
      QPointF intersect;
      if (l1.intersects(l2, &intersect) == QLineF::BoundedIntersection) {
        // Draw angle at intersection
        double angle = l1.angleTo(l2);
        if (angle > 180.0)
          angle = 360.0 - angle;

        QPointF screenPos = transform.map(intersect);
        painter.setPen(Qt::magenta);
        painter.drawEllipse(screenPos, 4, 4);
        painter.drawText(screenPos + QPointF(5, -5),
                         QString("%1°").arg(angle, 0, 'f', 1));
      }
    }
  }
}

void DCMView::calculateROIStats(ROI *roi) {
  if (!roi || m_pixelData.isEmpty())
    return;

  if (roi->type() == ROIType::Oval) {
    EllipseROI *ellipse = static_cast<EllipseROI *>(roi);
    QRectF r = ellipse->rect;

    // Mock stats calculation logic
    // In reality, we would iterate over pixelData within the ellipse bounds
    double sum = 0;
    int count = 0;

    // Simplified: just average some pixels inside the rect for demo
    roi->stats.mean = 1200.5; // Example HU value
    roi->stats.area =
        M_PI * (r.width() * 512 / 2.0) * (r.height() * 512 / 2.0); // mm^2
  }
}

QPointF screenToImage(const QPoint &pos, int w, int h, float scale,
                      const QPointF &offset) {
  float x = (pos.x() - w / 2.0) / (w / 2.0 * scale) - offset.x();
  float y = -(pos.y() - h / 2.0) / (h / 2.0 * scale) - offset.y();
  return QPointF(x, y);
}

void DCMView::mousePressEvent(QMouseEvent *event) {
  QPointF imgPos =
      screenToImage(event->pos(), width(), height(), m_scale, m_panOffset);

  if (m_toolMode == ToolMode::Angle && event->button() == Qt::LeftButton) {
    if (!m_currentAngle) {
      m_currentAngle = new AngleROI();
      m_currentAngle->p1 = m_currentAngle->p2 = m_currentAngle->p3 = imgPos;
      m_clickCount = 1;
    } else {
      m_clickCount++;
      if (m_clickCount == 2) {
        m_currentAngle->p2 = imgPos;
      } else if (m_clickCount == 3) {
        m_currentAngle->p3 = imgPos;
        m_rois.append(m_currentAngle);
        m_currentAngle = nullptr;
        m_clickCount = 0;
      }
    }
  } else if (m_toolMode == ToolMode::Ruler &&
             event->button() == Qt::LeftButton) {
    m_isDrawing = true;
    m_currentRuler = new RulerROI();
    m_currentRuler->p1 = m_currentRuler->p2 = imgPos;
  } else if (m_toolMode == ToolMode::Ellipse &&
             event->button() == Qt::LeftButton) {
    m_isDrawing = true;
    m_currentEllipse = new EllipseROI();
    m_currentEllipse->rect = QRectF(imgPos, QSizeF(0, 0));
  } else if (event->button() == Qt::MiddleButton ||
             (event->button() == Qt::LeftButton &&
              event->modifiers() & Qt::AltModifier)) {
    m_isPanning = true;
    m_lastMousePos = event->pos();
  }
  update();
}

void DCMView::mouseMoveEvent(QMouseEvent *event) {
  QPointF imgPos =
      screenToImage(event->pos(), width(), height(), m_scale, m_panOffset);

  if (m_toolMode == ToolMode::Angle && m_currentAngle) {
    if (m_clickCount == 1) {
      m_currentAngle->p2 = m_currentAngle->p3 = imgPos;
    } else if (m_clickCount == 2) {
      m_currentAngle->p3 = imgPos;
    }
    update();
  } else if (m_isDrawing) {
    if (m_currentRuler)
      m_currentRuler->p2 = imgPos;
    if (m_currentEllipse) {
      m_currentEllipse->rect.setBottomRight(imgPos);
    }
    update();
  } else if (m_isPanning) {
    QPoint delta = event->pos() - m_lastMousePos;
    m_panOffset += QPointF(delta.x() / (float)width() * 2.0f,
                           -delta.y() / (float)height() * 2.0f);
    m_lastMousePos = event->pos();
    update();
  }
}

void DCMView::mouseReleaseEvent(QMouseEvent *event) {
  if (m_toolMode != ToolMode::Angle && m_isDrawing) {
    if (m_currentRuler) {
      m_rois.append(m_currentRuler);
      m_currentRuler = nullptr;
    }
    if (m_currentEllipse) {
      calculateROIStats(m_currentEllipse);
      m_rois.append(m_currentEllipse);
      m_currentEllipse = nullptr;
    }
    m_isDrawing = false;
  }
  m_isPanning = false;
  update();
}

void DCMView::wheelEvent(QWheelEvent *event) {
  if (event->modifiers() & Qt::ControlModifier) {
    // Zoom
    float factor = (event->angleDelta().y() > 0) ? 1.1f : 0.9f;
    m_scale *= factor;
  } else {
    // Stack Navigation (Scrolling)
    if (event->angleDelta().y() > 0)
      prevImage();
    else
      nextImage();
  }
  update();
}

void DCMView::setImageStack(const QStringList &files) {
  m_imageFiles = files;
  m_currentIndex = 0;
  // In real app, trigger loading of m_imageFiles[0]
  update();
}

void DCMView::nextImage() {
  if (m_currentIndex < m_imageFiles.size() - 1) {
    m_currentIndex++;
    update();
  }
}

void DCMView::prevImage() {
  if (m_currentIndex > 0) {
    m_currentIndex--;
    update();
  }
}

void DCMView::resizeGL(int w, int h) { glViewport(0, 0, w, h); }
