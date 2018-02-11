/*
	Main Application Window implementation
*/

#include <QtWidgets>

#include "MainWindow.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(Volume& volume, QWidget *parent) :
	QMainWindow(parent),
	m_volume(std::move(volume))
{
	m_img = QPixmap::fromImage(m_volume.getSlice(76));

	//m_img = QPixmap::fromImage(QImage((uchar*)array, 4*4, 1, QImage::Format_Grayscale8));

	setWindowTitle("title");
	setFixedSize(QSize(1280, 720));
	setCentralWidget(createWidgets());
}

MainWindow::~MainWindow()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::sliderChanged(int value)
{
	m_img = QPixmap::fromImage(m_volume.getSlice(value));
	m_topImage->setPixmap(m_img);
}

QWidget* MainWindow::createWidgets()
{
	//Central widget layout
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(createControlArea());
	layout->addWidget(createImageArea());

	//Central widget
	QWidget* center = new QWidget(this);
	center->setLayout(layout);

	//Slider event
	connect(m_ySlider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));

	return center;
}

QWidget* MainWindow::createImageArea()
{
	//Images
	m_topImage = new QLabel(this);
	m_topImage->setPixmap(m_img);
	m_topImage->setAlignment(Qt::AlignCenter);
	m_sideImage = new QLabel(this);
	m_sideImage->setPixmap(m_img);
	m_sideImage->setAlignment(Qt::AlignCenter);
	m_frontImage = new QLabel(this);
	m_frontImage->setPixmap(m_img);
	m_frontImage->setAlignment(Qt::AlignCenter);
	
	//Image grid
	QGridLayout* imageLayout = new QGridLayout(this);
	imageLayout->addWidget(m_topImage, 0, 0);
	imageLayout->addWidget(m_sideImage, 0, 1);
	imageLayout->addWidget(m_frontImage, 1, 0);

	QScrollArea* scrollArea = new QScrollArea(this);
	scrollArea->setLayout(imageLayout);
	return scrollArea;
}

QWidget* MainWindow::createControlArea()
{
	//QFormLayout* controlLayout = new QFormLayout(this);
	QGridLayout* ctrlLayout = new QGridLayout(this);
	ctrlLayout->setMargin(5);
	ctrlLayout->setAlignment(Qt::AlignTop);
	
	m_zSlider = new QSlider(Qt::Horizontal, this);
	m_zSlider->setRange(0, m_volume.getColumns() - 1);
	//controlLayout->addRow(QStringLiteral("Z slider"), m_zSlider);
	ctrlLayout->addWidget(new QLabel(QStringLiteral("Z slider"), this), 1, 0);
	ctrlLayout->addWidget(m_zSlider, 1, 1);

	m_ySlider = new QSlider(Qt::Horizontal, this);
	m_ySlider->setRange(0, m_volume.getSlices() - 1);
	//controlLayout->addRow(QStringLiteral("Y slider"), m_ySlider);
	ctrlLayout->addWidget(new QLabel(QStringLiteral("Y slider"), this), 2, 0);
	ctrlLayout->addWidget(m_ySlider, 2, 1);

	m_xSlider = new QSlider(Qt::Horizontal, this);
	m_xSlider->setRange(0, m_volume.getRows() - 1);
	//controlLayout->addRow(QStringLiteral("Z slider"), m_zSlider);
	ctrlLayout->addWidget(new QLabel(QStringLiteral("X slider"), this), 3, 0);
	ctrlLayout->addWidget(m_xSlider, 3, 1);

	//ctrlLayout->addItem(new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Expanding), 4, 0, 2);
	//ctrlLayout->addWidget(new QSplitter(this));

	QGroupBox* ctrlGroup = new QGroupBox(QStringLiteral("Options"), this);
	ctrlGroup->setLayout(ctrlLayout);
	ctrlGroup->setMaximumWidth(300);
	ctrlGroup->setMinimumWidth(200);

	return ctrlGroup;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
