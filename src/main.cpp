#include "app.hpp"
#include <QApplication>
#include <iostream>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Controller controller;
    cout << "Application started." << endl;
    return app.exec();
}