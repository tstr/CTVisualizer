/*
	Program entry point
*/

#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QStyleFactory>

#include "MainWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	//Set application style
	QApplication::setStyle(QStyleFactory::create("fusion"));

	//Try read volume data
	QFile file("CThead");

	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(nullptr, "CThead error", file.errorString());
		return -1;
	}

	//Load volume dataset from file
	Volume volume(file, 256, 256, 113);

	//Show main window
	MainWindow window(volume);
	window.show();

	return app.exec();
}
