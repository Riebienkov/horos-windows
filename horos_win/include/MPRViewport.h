#ifndef MPRVIEWPORT_H
#define MPRVIEWPORT_H

#include <QWidget>

// In a real VTK integration, this would inherit from QVTKOpenGLNativeWidget
class MPRViewport : public QWidget {
  Q_OBJECT
public:
  enum class Plane { Axial, Sagittal, Coronal, Volume };

  explicit MPRViewport(Plane plane, QWidget *parent = nullptr);

  void updateSlice(int index);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  Plane m_plane;
};

#endif // MPRVIEWPORT_H
