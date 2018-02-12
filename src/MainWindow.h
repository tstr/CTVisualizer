/*
	Main Application Window class
*/
#pragma once

#include "Volume.h"

#include <QMainWindow>

class QSlider;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
	
public:
	
    explicit MainWindow(Volume& volume, QWidget *parent = 0);
    ~MainWindow();

private slots:

	/*
		Update image views
	*/
	void setImageFront(int value);
	void setImageSide(int value);
	void setImageTop(int value);

private:

	QWidget* createWidgets();
	QWidget* createControlArea();
	QWidget* createImageArea();

	//Volume data
	Volume m_volume;

	//front-back view
	QSlider* m_zSlider;
	//top-down view
	QSlider* m_ySlider;
	//top-down view
	QSlider* m_xSlider;

	//Image view widgets
	QLabel* m_topImage;
	QLabel* m_sideImage;
	QLabel* m_frontImage;
};
