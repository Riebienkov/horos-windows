#include "SettingsDialog.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
  setWindowTitle("Horos Settings");
  QVBoxLayout *layout = new QVBoxLayout(this);

  QGroupBox *pacsGroup = new QGroupBox("PACS Configuration", this);
  QFormLayout *pacsLayout = new QFormLayout(pacsGroup);
  m_localAETitle = new QLineEdit("HOROS_WIN", this);
  m_pacsIP = new QLineEdit("127.0.0.1", this);
  m_pacsPort = new QSpinBox(this);
  m_pacsPort->setRange(1, 65535);
  m_pacsPort->setValue(104);
  m_pacsAETitle = new QLineEdit("ANY_SCP", this);

  pacsLayout->addRow("Local AE Title:", m_localAETitle);
  pacsLayout->addRow("PACS IP Address:", m_pacsIP);
  pacsLayout->addRow("PACS Port:", m_pacsPort);
  pacsLayout->addRow("PACS AE Title:", m_pacsAETitle);
  layout->addWidget(pacsGroup);

  QGroupBox *autoGroup = new QGroupBox("Automation", this);
  QFormLayout *autoLayout = new QFormLayout(autoGroup);
  m_watchFolder = new QLineEdit("C:/DICOM_INBOX", this);
  autoLayout->addRow("Auto-Import Folder:", m_watchFolder);
  layout->addWidget(autoGroup);

  QPushButton *saveBtn = new QPushButton("Save Settings", this);
  layout->addWidget(saveBtn);
  connect(saveBtn, &QPushButton::clicked, this, &SettingsDialog::onSave);
}

void SettingsDialog::onSave() {
  // Save to QSettings logic here
  accept();
}
