#ifndef MPRWIDGET_H
#define MPRWIDGET_H

#include <QGridLayout>
#include <QWidget>

class MPRViewport;

class MPRWidget : public QWidget {
  Q_OBJECT
public:
  explicit MPRWidget(QWidget *parent = nullptr);
  ~MPRWidget();

  void setSeries(const QStringList &imageFiles);

private:
  QGridLayout *m_layout;
  MPRViewport *m_axialView;
  MPRViewport *m_sagittalView;
  MPRViewport *m_coronalView;
  MPRViewport *m_3dView;
};

#endif // MPRWIDGET_H
