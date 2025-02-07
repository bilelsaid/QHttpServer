#include "api_controller.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

ApiController::ApiController(QObject *parent) : QObject(parent) {}

void ApiController::registerRoutes(QHttpServer &server) {
  server.route("/register", QHttpServerRequest::Method::Post,
               [this](const QHttpServerRequest &request,
                      QHttpServerResponder &responder) {
                 handleRegisterUser(request, responder);
               });
}

void ApiController::handleRegisterUser(const QHttpServerRequest &request,
                                       QHttpServerResponder &responder) {
  const auto payload = request.body();
  QJsonDocument jsonDoc = QJsonDocument::fromJson(payload);
  if (!jsonDoc.isObject()) {
    responder.write(QByteArray("Invalid JSON payload."), "text/plain",
                    QHttpServerResponder::StatusCode::BadRequest);
    return;
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

  qDebug() << "Registering user:" << name << email << password << role
           << phoneNumber << homeAddress << dateOfBirth << isActive
           << specialties;

  responder.write(QByteArray("User registered successfully."), "text/plain",
                  QHttpServerResponder::StatusCode::Ok);
}
