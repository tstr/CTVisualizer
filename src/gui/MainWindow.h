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
class SubimageView;

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
	void scaleImages(int value);
	void updateCamera(const QMatrix4x4& matrix);

private:

	//Create gui widgets
	QWidget* createWidgets();
	QWidget* createControlArea();
	QWidget* createImageArea();
	QWidget* create3DArea();

	//Volume viewer
	VolumeRender m_render;
	//3D camera
	ArcballCamera m_camera;

	//View sliders
	LabelledSlider* m_zSlider;
	LabelledSlider* m_ySlider;
	LabelledSlider* m_xSlider;
	//Image scale slider
	LabelledSlider* m_scaleSlider;
	
	//Subimage view widgets
	SubimageView* m_xSubimage;
	SubimageView* m_ySubimage;
	SubimageView* m_zSubimage;

	LabelledSlider* m_rotationXSlider;
	LabelledSlider* m_rotationYSlider;

	//histogram equalization toggle
	QCheckBox* m_heToggle;
	//mip toggle
	QCheckBox* m_mipToggle;

	//3D
	QLabel* m_3DView;
};
