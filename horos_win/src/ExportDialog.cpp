#include "ExportDialog.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

ExportDialog::ExportDialog(const QString &studyUID, QWidget *parent)
    : QDialog(parent), m_studyUID(studyUID) {
  setWindowTitle("Export Study - " + studyUID);
  QVBoxLayout *layout = new QVBoxLayout(this);

  layout->addWidget(new QLabel("Export Format:"));
  m_formatCombo = new QComboBox(this);
  m_formatCombo->addItems(
      {"DICOM (Original)", "DICOM (Anonymized)", "JPEG", "TIFF"});
  layout->addWidget(m_formatCombo);

  m_anonymizeCheck = new QCheckBox("Remove Patient Identity (Anonymize)", this);
  layout->addWidget(m_anonymizeCheck);

  layout->addWidget(new QLabel("Output Directory:"));
  QHBoxLayout *pathLayout = new QHBoxLayout();
  m_pathEdit = new QLineEdit(this);
  QPushButton *browseBtn = new QPushButton("Browse...", this);
  pathLayout->addWidget(m_pathEdit);
  pathLayout->addWidget(browseBtn);
  layout->addLayout(pathLayout);

  QHBoxLayout *btnLayout = new QHBoxLayout();
  QPushButton *okBtn = new QPushButton("Export", this);
  QPushButton *cancelBtn = new QPushButton("Cancel", this);
  btnLayout->addStretch();
  btnLayout->addWidget(okBtn);
  btnLayout->addWidget(cancelBtn);
  layout->addLayout(btnLayout);

  connect(browseBtn, &QPushButton::clicked, this, &ExportDialog::onBrowse);
  connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
  connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
}

void ExportDialog::onBrowse() {
  QString dir =
      QFileDialog::getExistingDirectory(this, "Select Export Directory");
  if (!dir.isEmpty())
    m_pathEdit->setText(dir);
}

ExportDialog::ExportSettings ExportDialog::settings() const {
  return {m_formatCombo->currentText(), m_anonymizeCheck->isChecked(),
          m_pathEdit->text()};
}
