/*
	Main Application Window class
*/
#pragma once

#include "Volume.h"
#include "VolumeEqualization.h"

#include <QMainWindow>

class QSlider;
class QLabel;

class VolumeSubimage;

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
	void setImageSide(int value);
	void setImageFront(int value);
	void setImageTop(int value);

private:

	void writeSubimage(QImage& target, const VolumeSubimage& view);

	QWidget* createWidgets();
	QWidget* createControlArea();
	QWidget* createImageArea();

	//Volume data
	Volume m_volume;
	Equalizer m_equalizer;

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
