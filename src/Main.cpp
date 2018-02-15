
#include <QApplication>
#include <QMessageBox>
#include "MainWindow.h"

#include <QFile>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	//Try read volume data
	QFile file("CThead");

	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(nullptr, QStringLiteral("CThead error"), file.errorString());
		return -1;
	}

	//Load volume dataset from file
	Volume volume(file, 256, 256, 113);

	//Show main window
	MainWindow window(volume);
	window.show();

	return app.exec();
}
