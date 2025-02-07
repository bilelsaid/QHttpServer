#include "server.h"
#include <QCoreApplication>

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  Server server;
  if (!server.start()) {
    return -1;
  }

  return app.exec();
}
