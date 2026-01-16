#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>

class ExportDialog : public QDialog {
  Q_OBJECT
public:
  explicit ExportDialog(const QString &studyUID, QWidget *parent = nullptr);

  struct ExportSettings {
    QString format;
    bool anonymize;
    QString outputDir;
  };

  ExportSettings settings() const;

private slots:
  void onBrowse();

private:
  QString m_studyUID;
  QComboBox *m_formatCombo;
  QCheckBox *m_anonymizeCheck;
  QLineEdit *m_pathEdit;
};

#endif // EXPORTDIALOG_H
