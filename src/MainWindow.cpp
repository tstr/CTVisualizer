/*
	Main Application Window implementation
*/

#include <QtWidgets>

#include "MainWindow.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	setCentralWidget(createWidgets());
	setWindowTitle("title");
}

MainWindow::~MainWindow()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

QWidget* MainWindow::createWidgets()
{
	QImage i("test.png");
	
	QPixmap pix(QPixmap::fromImage(QImage(QStringLiteral("test.png"))));
	
	m_leftImage = new QLabel(this);
	m_leftImage->setPixmap(pix);
	m_leftImage->setAlignment(Qt::AlignCenter);
	//m_leftImage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	m_rightImage = new QLabel(this);
	m_rightImage->setPixmap(pix);
	m_rightImage->setAlignment(Qt::AlignCenter);
	//m_rightImage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	
	//Images
	QHBoxLayout* imageLayout = new QHBoxLayout(this);
	imageLayout->addWidget(m_leftImage);
	imageLayout->addWidget(m_rightImage);

	


	//Controls
	QHBoxLayout* controlLayout = new QHBoxLayout(this);
	controlLayout->setAlignment(Qt::AlignLeft);
	controlLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);

	QVBoxLayout* controlSliderLayout = new QVBoxLayout(this);
	controlSliderLayout->setMargin(5);

	m_zSlider = new QSlider(this);
	m_zSlider->setOrientation(Qt::Horizontal);
	m_zSlider->setMaximumWidth(200);
	controlSliderLayout->addWidget(new QLabel(QStringLiteral("Z slider")));
	controlSliderLayout->addWidget(m_zSlider);

	m_ySlider = new QSlider(this);
	m_ySlider->setOrientation(Qt::Horizontal);
	m_ySlider->setMaximumWidth(200);
	controlSliderLayout->addWidget(new QLabel(QStringLiteral("Y slider")));
	controlSliderLayout->addWidget(m_ySlider);

	QGroupBox* sliderGroup = new QGroupBox(QStringLiteral("options"));
	controlLayout->addWidget(sliderGroup);
	controlLayout->addLayout(controlSliderLayout);

	//Central widget layout
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addLayout(imageLayout);
	layout->addLayout(controlLayout);

	//Central widget
	QWidget* center = new QWidget(this);
	center->setLayout(layout);

	return center;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
