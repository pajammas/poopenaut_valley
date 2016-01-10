#include "mainwindow.h"
#include <QApplication>

// Launcher for the rest of the program
int main(int argc, char *argv[])
{
	// Start the Qt application
    QApplication a(argc, argv);
    // Give it our amazing MainWindow
    MainWindow w;
    w.show();
    // Run!
    return a.exec();
}
