#ifndef SERVER_H
#define SERVER_H

#include "api_controller.h"
#include <QHttpServer>
#include <QObject>
#include <QSslServer>

class Server : public QObject {
  Q_OBJECT

public:
  explicit Server(QObject *parent = nullptr);
  ~Server();
  bool start();

private:
  QHttpServer mi_httpServer;
  std::unique_ptr<QSslServer> m_sslServer;
  ApiController m_apiController;

  void setupRoutes();
  bool configureSsl();
};

#endif // SERVER_H
