/*
	Main Application Window implementation
*/

#include <QtWidgets>

#include "MainWindow.h"
#include "VolumeRender.h"

#define IMAGE_SCALE_MIN 128
#define IMAGE_SCALE_MAX 512

//////////////////////////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(Volume& volume, QWidget *parent) :
	QMainWindow(parent),
	m_render(volume)
{
	m_imageBuffer = QImage(256, 256, QImage::Format_Grayscale8);

	setWindowTitle(QStringLiteral("title"));
	resize(QSize(1280, 720));
	setCentralWidget(createWidgets());
}

MainWindow::~MainWindow()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Image drawing
//////////////////////////////////////////////////////////////////////////////////////////////////////////

QPixmap MainWindow::updateImage(int value, VolumeAxis axis)
{
	//Debug time
	QElapsedTimer t;
	t.start();
	//Draw image
	m_render.drawView(m_imageBuffer, value, axis);
	quint64 e = t.elapsed();

	//Print debug
	qDebug() << "axis:" << axis << " -- elapsed: " << e << "ms";

	return QPixmap::fromImage(m_imageBuffer);
}

void MainWindow::updateImageSide(int value)
{
	m_sideImage->setPixmap(updateImage(value, VolumeAxis::XAxis));
}

void MainWindow::updateImageFront(int value)
{
	m_frontImage->setPixmap(updateImage(value, VolumeAxis::YAxis));
}

void MainWindow::updateImageTop(int value)
{
	m_topImage->setPixmap(updateImage(value, VolumeAxis::ZAxis));
}

void MainWindow::scaleImages(int value)
{
	//Update image buffer size
	m_imageBuffer = QImage(value, value, QImage::Format_Grayscale8);
	redrawAll();
}

void MainWindow::redrawAll()
{
	updateImageFront(m_xSlider->value());
	updateImageSide(m_ySlider->value());
	updateImageTop(m_zSlider->value());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

QWidget* MainWindow::createWidgets()
{
	//Central widget layout
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(createControlArea());
	layout->addWidget(createImageArea());

	/*
		Events
	*/

	//Connect sliders
	connect(m_xSlider, &QSlider::valueChanged, this, &MainWindow::updateImageFront);
	connect(m_ySlider, &QSlider::valueChanged, this, &MainWindow::updateImageSide);
	connect(m_zSlider, &QSlider::valueChanged, this, &MainWindow::updateImageTop);

	connect(m_scaleSlider, &QSlider::valueChanged, this, &MainWindow::scaleImages);

	//Connect draw options
	connect(m_mipToggle, &QCheckBox::toggled, &m_render, &VolumeRender::enableMip);
	connect(m_heToggle, &QCheckBox::toggled, &m_render, &VolumeRender::enableHist);

	//When MIP is enabled, disable the view sliders
	connect(m_mipToggle, &QCheckBox::toggled, m_xSlider, &QSlider::setDisabled);
	connect(m_mipToggle, &QCheckBox::toggled, m_ySlider, &QSlider::setDisabled);
	connect(m_mipToggle, &QCheckBox::toggled, m_zSlider, &QSlider::setDisabled);

	//Connect redraw signal
	connect(&m_render, &VolumeRender::redrawAll, this, &MainWindow::redrawAll);

	m_xSlider->setSliderPosition(m_render.volume()->rows() / 2);
	m_ySlider->setSliderPosition(m_render.volume()->columns() / 2);
	m_zSlider->setSliderPosition(m_render.volume()->slices() / 2);

	//Central widget
	QWidget* center = new QWidget(this);
	center->setLayout(layout);

	return center;
}

QWidget* MainWindow::createImageArea()
{
	//Images
	m_topImage = new QLabel(this);
	m_topImage->setAlignment(Qt::AlignCenter);
	m_sideImage = new QLabel(this);
	m_sideImage->setAlignment(Qt::AlignCenter);
	m_frontImage = new QLabel(this);
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
	QFormLayout* ctrlLayout = new QFormLayout(this);
	ctrlLayout->setMargin(5);
	ctrlLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	
	//X view
	m_xSlider = new QSlider(Qt::Horizontal, this);
	m_xSlider->setRange(0, (int)m_render.volume()->columns() - 1);
	//Y view
	m_ySlider = new QSlider(Qt::Horizontal, this);
	m_ySlider->setRange(0, (int)m_render.volume()->rows() - 1);
	//Z view
	m_zSlider = new QSlider(Qt::Horizontal, this);
	m_zSlider->setRange(0, (int)m_render.volume()->slices() - 1);
	//Scale
	m_scaleSlider = new QSlider(Qt::Horizontal, this);
	m_scaleSlider->setRange(IMAGE_SCALE_MIN, IMAGE_SCALE_MAX);
	m_scaleSlider->setValue((int)m_render.volume()->columns());

	m_mipToggle = new QCheckBox(QStringLiteral("Maximum Intensity Projection"), this);
	m_heToggle = new QCheckBox(QStringLiteral("Histogram Equalization"), this);

	ctrlLayout->addRow(QStringLiteral("Front"), m_xSlider);
	ctrlLayout->addRow(QStringLiteral("Side"), m_ySlider);
	ctrlLayout->addRow(QStringLiteral("Top"), m_zSlider);
	ctrlLayout->addWidget(new QSplitter(this));
	ctrlLayout->addRow(QStringLiteral("Scale"), m_scaleSlider);
	ctrlLayout->addWidget(m_mipToggle);
	ctrlLayout->addWidget(m_heToggle);

	QGroupBox* ctrlGroup = new QGroupBox(QStringLiteral("Options"), this);
	ctrlGroup->setLayout(ctrlLayout);
	ctrlGroup->setMaximumWidth(300);
	ctrlGroup->setMinimumWidth(200);

	return ctrlGroup;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
