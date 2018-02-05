
#include <QApplication>
#include <QMessageBox>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	QMessageBox box(QMessageBox::Information, "title", "hello world");
	box.show();

	return app.exec();
}
