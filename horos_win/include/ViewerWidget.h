#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

#include "DCMView.h"
#include <QWidget>

class ViewerWidget : public QWidget {
  Q_OBJECT

public:
  explicit ViewerWidget(const QString &studyUID, QWidget *parent = nullptr);
  DCMView *view() const { return m_view; }

private:
  void setupUI();
  DCMView *m_view;
  QString m_studyUID;
};

#endif // VIEWERWIDGET_H
