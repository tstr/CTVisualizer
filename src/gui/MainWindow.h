/*
	Main Application Window class

	Root of the application GUI.
*/
#pragma once

#include <QMainWindow>

#include "gfx/VolumeRender.h"

class QLabel;
class QCheckBox;
class QRadioButton;
class LabelledSlider;
class SubimageView;
class CameraView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
	Q_DISABLE_COPY(MainWindow)

public:
	
    explicit MainWindow(Volume& volume, QWidget *parent = 0);
    ~MainWindow();

private slots:

	/*
		Update image views
	*/
	void scaleImages(int value);

private:

	//Create gui widgets
	QWidget* createWidgets();
	QWidget* createControlArea();
	QWidget* createImageArea();
	QWidget* create3DArea();

	//Volume viewer
	VolumeRender m_render;

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

	//histogram equalization toggle
	QCheckBox* m_heToggle;
	//mip toggle
	QCheckBox* m_mipToggle;

	//Sampler options
	QRadioButton* m_samplerDefault;
	QRadioButton* m_samplerBilinear;
	QRadioButton* m_samplerBicubic;

	//3D
	CameraView* m_3DView;
	//3D - sampling frequency slider
	LabelledSlider* m_3DSampleSlider;
};
