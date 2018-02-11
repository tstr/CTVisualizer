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

	void sliderChanged(int value);

private:

	QWidget* createWidgets();
	QWidget* createControlArea();
	QWidget* createImageArea();

	Volume m_volume;

	QPixmap m_img;

	QSlider* m_zSlider; //front-back view
	QSlider* m_ySlider; //top-down view
	QSlider* m_xSlider; //top-down view

	QLabel* m_topImage;
	QLabel* m_sideImage;
	QLabel* m_frontImage;
};