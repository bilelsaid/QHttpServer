#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QHttpServer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSslCertificate>
#include <QSslConfiguration>
#include <QSslKey>
#include <QSslServer>

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  // Load SSL certificate and key
  QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
  const auto sslCertificateChain =
      QSslCertificate::fromPath("assets/certificate.crt");
  if (sslCertificateChain.isEmpty()) {
    qWarning() << "Couldn't retrieve SSL certificate from file.";
    return -1;
  }

  QFile privateKeyFile("assets/private.key");
  if (!privateKeyFile.open(QIODevice::ReadOnly)) {
    qWarning() << "Couldn't open file for reading:"
               << privateKeyFile.errorString();
    return -1;
  }

  sslConfig.setLocalCertificate(sslCertificateChain.front());
  sslConfig.setPrivateKey(QSslKey(&privateKeyFile, QSsl::Rsa));
  privateKeyFile.close();

  // Set up SSL server
  QHttpServer httpServer;
  auto sslServer = std::make_unique<QSslServer>();
  sslServer->setSslConfiguration(sslConfig);

  // Define the route to handle POST requests for user registration
  httpServer.route(
      "/register", QHttpServerRequest::Method::Post,
      [](const QHttpServerRequest &request) {
        // Parse JSON payload
        const auto payload = request.body();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(payload);
        if (!jsonDoc.isObject()) {
          return QHttpServerResponse(
              "Invalid JSON payload.",
              QHttpServerResponder::StatusCode::BadRequest);
        }

        QJsonObject jsonObj = jsonDoc.object();
        QString name = jsonObj.value("name").toString();
        QString email = jsonObj.value("email").toString();
        QString password = jsonObj.value("password").toString();
        QString role = jsonObj.value("role").toString();
        QString phoneNumber = jsonObj.value("phone_number").toString();
        QString homeAddress = jsonObj.value("home_address").toString();
        QString dateOfBirth = jsonObj.value("date_of_birth").toString();
        bool isActive = jsonObj.value("is_active").toBool();
        QJsonArray specialtiesArray = jsonObj.value("specialties").toArray();
        QStringList specialties;
        for (const auto &specialty : specialtiesArray) {
          specialties.append(specialty.toString());
        }

        // Perform registration logic here (e.g., save to a database)
        // For now, we'll just print the information to the console
        qDebug() << "Registering user:" << name << email << password << role
                 << phoneNumber << homeAddress << dateOfBirth << isActive
                 << specialties;

        return QHttpServerResponse("User registered successfully.",
                                   QHttpServerResponder::StatusCode::Ok);
      });

  if (!sslServer->listen(QHostAddress::AnyIPv4, 8443) ||
      !httpServer.bind(sslServer.get())) {
    qWarning() << "Server failed to listen on port 8443.";
    return -1;
  }

  return app.exec();
}
