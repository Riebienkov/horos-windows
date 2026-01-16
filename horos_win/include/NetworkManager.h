#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QString>

struct PACSNode {
  QString name;
  QString ip;
  int port;
  QString aeTitle;
};

struct DicomQueryResult {
  QString patientName;
  QString patientID;
  QString studyDate;
  QString studyDescription;
  QString studyInstanceUID;
};

class NetworkManager : public QObject {
  Q_OBJECT
public:
  static NetworkManager &instance();

  void addNode(const PACSNode &node);
  void echo(const PACSNode &node);                              // C-ECHO (Ping)
  void query(const PACSNode &node, const QString &patientName); // C-FIND
  void retrieve(const PACSNode &node, const QString &studyUID); // C-MOVE/C-GET

signals:
  void logMessage(const QString &msg);
  void queryFinished(const QList<DicomQueryResult> &results);
  void retrievalFinished(bool success, const QString &studyUID);

private:
  explicit NetworkManager(QObject *parent = nullptr);
  QList<PACSNode> m_nodes;
};

#endif // NETWORKMANAGER_H
