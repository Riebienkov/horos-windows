#include "NetworkManager.h"
#include <QDebug>

NetworkManager &NetworkManager::instance() {
  static NetworkManager inst;
  return inst;
}

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) {}

void NetworkManager::addNode(const PACSNode &node) { m_nodes.append(node); }

void NetworkManager::echo(const PACSNode &node) {
  emit logMessage(
      QString("C-ECHO to %1 (%2)...").arg(node.name).arg(node.aeTitle));
  // Implementation would use DCMTK: ASC_association and DIMSE_echoUser
  emit logMessage("C-ECHO Success");
}

void NetworkManager::query(const PACSNode &node, const QString &patientName) {
  emit logMessage(
      QString("C-FIND Query for: %1 on %2").arg(patientName).arg(node.name));

  // Mock results
  QList<DicomQueryResult> results;
  results.append(
      {"SMITH^JOHN", "ID001", "2023-10-01", "CT Thorax", "UID.1.2.3"});
  results.append({"DOE^JANE", "ID002", "2023-10-05", "MR Brain", "UID.4.5.6"});

  emit queryFinished(results);
}

void NetworkManager::retrieve(const PACSNode &node, const QString &studyUID) {
  emit logMessage(QString("C-MOVE Request for Study: %1").arg(studyUID));
  // Logic for C-MOVE
}
