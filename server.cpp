#include "server.h"
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QSslCertificate>
#include <QSslConfiguration>
#include <QSslKey>

Server::~Server() = default;

Server::Server(QObject *parent)
    : QObject(parent), sslServer(std::make_unique<QSslServer>()) {}

bool Server::start() {
  // Load SSL certificate and key
  QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
  const auto sslCertificateChain =
      QSslCertificate::fromPath("assets/certificate.crt");
  if (sslCertificateChain.isEmpty()) {
    qWarning() << "Couldn't retrieve SSL certificate from file.";
    return false;
  }

  QFile privateKeyFile("assets/private.key");
  if (!privateKeyFile.open(QIODevice::ReadOnly)) {
    qWarning() << "Couldn't open file for reading:"
               << privateKeyFile.errorString();
    return false;
  }

  sslConfig.setLocalCertificate(sslCertificateChain.front());
  sslConfig.setPrivateKey(QSslKey(&privateKeyFile, QSsl::Rsa));
  privateKeyFile.close();

  sslServer->setSslConfiguration(sslConfig);

  setupRoutes();

  if (!sslServer->listen(QHostAddress::AnyIPv4, 8443) ||
      !httpServer.bind(sslServer.get())) {
    qWarning() << "Server failed to listen on port 8443.";
    return false;
  }

  return true;
}

void Server::setupRoutes() { apiController.registerRoutes(httpServer); }
