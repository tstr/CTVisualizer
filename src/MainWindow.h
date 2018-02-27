/*
	Main Application Window class
*/
#pragma once

#include <QMainWindow>

#include "gfx/Camera.h"
#include "gfx/VolumeRender.h"

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

	void updateCamera(const QMatrix4x4& matrix);

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
	QWidget* create3DArea();

	//Volume viewer
	VolumeRender m_render;
	//3D camera
	ArcballCamera m_camera;

	//Scaled volume dimensions
	Volume::SizeType m_xscaled;
	Volume::SizeType m_yscaled;
	Volume::SizeType m_zscaled;

	LabelledSlider* m_zSlider;
	LabelledSlider* m_ySlider;
	LabelledSlider* m_xSlider;
	//image scale
	LabelledSlider* m_scaleSlider;
	
	LabelledSlider* m_rotationXSlider;
	LabelledSlider* m_rotationYSlider;

	//histogram equalization toggle
	QCheckBox* m_heToggle;
	//mip toggle
	QCheckBox* m_mipToggle;

	//Image view widgets
	QLabel* m_topImage;
	QLabel* m_sideImage;
	QLabel* m_frontImage;
	
	//3D
	QLabel* m_3DView;
};
