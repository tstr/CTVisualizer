/*
	Program entry point
*/

#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QStyleFactory>
#include <QDesktopWidget>
#include <QSettings>

#include "gui/MainWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	//Read config file
	QSettings config("config.ini", QSettings::IniFormat);

	//Set application style
	QApplication::setStyle(QStyleFactory::create("fusion"));

	//Get volume dimensions from config
	Volume::Dimensions dimensions;

	dimensions.sizeX = config.value("Application/sizeX", 0).toInt();
	dimensions.sizeY = config.value("Application/sizeY", 0).toInt();
	dimensions.sizeZ = config.value("Application/sizeZ", 0).toInt();

	Q_ASSERT(dimensions.sizeX != 0);
	Q_ASSERT(dimensions.sizeY != 0);
	Q_ASSERT(dimensions.sizeZ != 0);

	dimensions.scaleX = config.value("Application/scaleX", 1).toInt();
	dimensions.scaleY = config.value("Application/scaleY", 1).toInt();
	dimensions.scaleZ = config.value("Application/scaleZ", 1).toInt();

	//Try read volume data file
	QFile file(config.value("Application/dataset").toString());

	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(nullptr, "Volume loader error", file.errorString());
		return -1;
	}
	
	//Construct Volume
	Volume v(file, dimensions);
	
	//Construct Volume viewer
	MainWindow window(v);

	//Move to center
	QRect r = window.geometry();
	r.moveCenter(QApplication::desktop()->availableGeometry().center());
	window.setGeometry(r);
	
	//Show main window
	window.show();

	return app.exec();
}
