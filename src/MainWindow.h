/*
	Main Application Window class
*/
#pragma once

#include <QMainWindow>

#include "VolumeRender.h"

class QLabel;
class QCheckBox;
class LabelledSlider;

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

	/*
		Displayed images
	*/
	void redrawAll();

private:

	//Resizes targets
	void resizeTargets(float scaleFactor);

	//Create gui widgets
	QWidget* createWidgets();
	QWidget* createControlArea();
	QWidget* createImageArea();

	//Volume viewer
	VolumeRender m_render;

	//Image targets
	QImage m_xTarget;
	QImage m_yTarget;
	QImage m_zTarget;

	//front-back view
	LabelledSlider* m_zSlider;
	//top-down view
	LabelledSlider* m_ySlider;
	//top-down view
	LabelledSlider* m_xSlider;
	//image scale
	LabelledSlider* m_scaleSlider;

	//histogram equalization toggle
	QCheckBox* m_heToggle;
	//mip toggle
	QCheckBox* m_mipToggle;

	//Image view widgets
	QLabel* m_topImage;
	QLabel* m_sideImage;
	QLabel* m_frontImage;
};
