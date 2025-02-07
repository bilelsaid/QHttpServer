#ifndef API_CONTROLLER_H
#define API_CONTROLLER_H

#include <QHttpServer>
#include <QObject>

class ApiController : public QObject {
  Q_OBJECT

public:
  explicit ApiController(QObject *parent = nullptr);
  void registerRoutes(QHttpServer &server);

private:
  void handleRegisterUser(const QHttpServerRequest &request,
                          QHttpServerResponder &responder);
  void handleLoginUser(const QHttpServerRequest &request,
                       QHttpServerResponder &responder);
  bool verifyToken(const QHttpServerRequest &request, QString &userEmail);
};

#endif // API_CONTROLLER_H
