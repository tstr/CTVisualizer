/*
	Program entry point
*/

#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QStyleFactory>
#include <QSettings>

#include "MainWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	//Read config file
	QSettings config("config.ini", QSettings::IniFormat);

	//Set application style
	QApplication::setStyle(QStyleFactory::create("fusion"));

	//Get volume dimensions
	const int sizeX = config.value("Application/sizeX", 0).toInt();
	const int sizeY = config.value("Application/sizeY", 0).toInt();
	const int sizeZ = config.value("Application/sizeZ", 0).toInt();

	Q_ASSERT(sizeX != 0);
	Q_ASSERT(sizeY != 0);
	Q_ASSERT(sizeZ != 0);

	//Try read volume data file
	QFile file(config.value("Application/dataset").toString());

	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(nullptr, "Volume loader error", file.errorString());
		return -1;
	}

	//Construct Volume viewer
	MainWindow window(Volume(file, sizeX, sizeY, sizeZ));

	//Show main window
	window.show();

	return app.exec();
}
