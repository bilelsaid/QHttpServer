#ifndef SERVER_H
#define SERVER_H

#include <QHttpServer>
#include <QSslServer>

class Server : public QObject {
  Q_OBJECT

public:
  explicit Server(QObject *parent = nullptr);
  bool start();

private:
  QHttpServer httpServer;
  std::unique_ptr<QSslServer> sslServer;

  void setupRoutes();
};

#endif // SERVER_H
