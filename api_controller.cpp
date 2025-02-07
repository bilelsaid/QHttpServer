#include "api_controller.h"
#include <QDebug>
#include <QHttpServerResponder>
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

  server.route("/login", QHttpServerRequest::Method::Post,
               [this](const QHttpServerRequest &request,
                      QHttpServerResponder &responder) {
                 handleLoginUser(request, responder);
               });

  // Example of a protected route
  server.route(
      "/protected", QHttpServerRequest::Method::Get,
      [this](const QHttpServerRequest &request,
             QHttpServerResponder &responder) {
        QString userEmail;
        if (!verifyToken(request, userEmail)) {
          responder.write(QByteArray("Unauthorized"), "text/plain",
                          QHttpServerResponder::StatusCode::Unauthorized);
          return;
        }
        responder.write(QByteArray("Welcome, " + userEmail.toUtf8()),
                        "text/plain", QHttpServerResponder::StatusCode::Ok);
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

void ApiController::handleLoginUser(const QHttpServerRequest &request,
                                    QHttpServerResponder &responder) {
  const auto payload = request.body();
  QJsonDocument jsonDoc = QJsonDocument::fromJson(payload);
  if (!jsonDoc.isObject()) {
    responder.write(QByteArray("Invalid JSON payload."), "text/plain",
                    QHttpServerResponder::StatusCode::BadRequest);
    return;
  }

  QJsonObject jsonObj = jsonDoc.object();
  QString email = jsonObj.value("email").toString();
  QString password = jsonObj.value("password").toString();

  // For demonstration purposes, we'll simply check if the email is
  // "user@example.com" and the password is "password"
  if (email == "user@example.com" && password == "password") {
    qDebug() << "User logged in:" << email;

    // Generate a simple token (e.g., a JWT would be more secure)
    QString token = "simple_token_for_" + email;

    QJsonObject responseObj;
    responseObj["token"] = token;
    responder.write(QJsonDocument(responseObj).toJson(), "application/json",
                    QHttpServerResponder::StatusCode::Ok);
  } else {
    responder.write(QByteArray("Invalid email or password."), "text/plain",
                    QHttpServerResponder::StatusCode::Unauthorized);
  }
}

bool ApiController::verifyToken(const QHttpServerRequest &request,
                                QString &userEmail) {
  // Step 1: Check if the Authorization header exists
  if (!request.headers().contains("Authorization")) {
    qWarning() << "Authorization header is missing";
    return false;
  }

  // Step 2: Get the Authorization header value as QByteArrayView
  const QByteArrayView authHeaderValueView =
      request.headers().value("Authorization");

  // Step 3: Convert QByteArrayView to QString
  const QString authHeaderValue =
      QString::fromUtf8(authHeaderValueView.data(), authHeaderValueView.size());

  // Step 4: Check if the Authorization header starts with "Bearer "
  if (!authHeaderValue.startsWith("Bearer ")) {
    qWarning() << "Invalid Authorization header format";
    return false;
  }

  // Step 5: Extract the token from the Authorization header
  const QString token = authHeaderValue.mid(7); // Remove "Bearer " prefix

  return true;
}