
#include <QApplication>
#include "MainWindow.h"
#include <QtWidgets>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	MainWindow window;
	window.show();

	return app.exec();
}
