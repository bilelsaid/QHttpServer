#include "server.h"
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QSslCertificate>
#include <QSslConfiguration>
#include <QSslKey>

Server::~Server() = default;

Server::Server(QObject *parent)
    : QObject(parent), m_sslServer(std::make_unique<QSslServer>()) {}

bool Server::start() {
  if (!configureSsl()) {
    return false;
  }

  setupRoutes();

  if (!m_sslServer->listen(QHostAddress::AnyIPv4, 8443) ||
      !mi_httpServer.bind(m_sslServer.get())) {
    qWarning() << "Server failed to listen on port 8443.";
    return false;
  }

  return true;
}

void Server::setupRoutes() { m_apiController.registerRoutes(mi_httpServer); }

bool Server::configureSsl() {
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

  m_sslServer->setSslConfiguration(sslConfig);

  return true;
}
