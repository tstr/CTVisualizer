/*
	Main Application Window class
*/
#pragma once

#include <QMainWindow>

#include "VolumeRender.h"

class QSlider;
class QLabel;
class QCheckBox;

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
	void updateImageSide(int value);
	void updateImageFront(int value);
	void updateImageTop(int value);
	void scaleImages(int value);

	//Redraw images
	void redrawAll();

private:

	QPixmap updateImage(int value, VolumeAxis axis);

	QWidget* createWidgets();
	QWidget* createControlArea();
	QWidget* createImageArea();

	//Volume viewer
	VolumeRender m_render;

	QImage m_imageBuffer;

	//front-back view
	QSlider* m_zSlider;
	//top-down view
	QSlider* m_ySlider;
	//top-down view
	QSlider* m_xSlider;
	//image scale
	QSlider* m_scaleSlider;

	//histogram equalization toggle
	QCheckBox* m_heToggle;
	//mip toggle
	QCheckBox* m_mipToggle;

	//Image view widgets
	QLabel* m_topImage;
	QLabel* m_sideImage;
	QLabel* m_frontImage;
};
