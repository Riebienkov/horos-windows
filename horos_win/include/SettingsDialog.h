#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

class SettingsDialog : public QDialog {
  Q_OBJECT
public:
  explicit SettingsDialog(QWidget *parent = nullptr);

private slots:
  void onSave();

private:
  QLineEdit *m_localAETitle;
  QLineEdit *m_pacsIP;
  QSpinBox *m_pacsPort;
  QLineEdit *m_pacsAETitle;
  QLineEdit *m_watchFolder;
};

#endif // SETTINGSDIALOG_H
