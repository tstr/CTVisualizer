/*
	Main Application Window class
*/
#pragma once

#include <QMainWindow>

class QSlider;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
	
public:
	
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

	QWidget* createWidgets();

	QSlider* m_zSlider; //front-back view
	QSlider* m_ySlider; //top-down view

	QLabel* m_rightImage;
	QLabel* m_leftImage;
};