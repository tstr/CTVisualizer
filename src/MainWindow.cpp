/*
	Main Application Window implementation
*/

#include <QtWidgets>

#include "MainWindow.h"
#include "VolumeRender.h"
#include "util/LabelledSlider.h"

#define IMAGE_SCALE_MIN 10
#define IMAGE_SCALE_MAX 200
#define CTRL_WIDGET_WIDTH_MIN 400
#define CTRL_WIDGET_WIDTH_MAX 400

//////////////////////////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(Volume& volume, QWidget *parent) :
	QMainWindow(parent),
	m_render(volume)
{
	resizeTargets(1.0f);

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

void MainWindow::updateImageFront(int value)
{
	//Draw X
	m_render.drawSubimage(m_xTarget, value, VolumeAxis::XAxis);
	m_frontImage->setPixmap(QPixmap::fromImage(m_xTarget));
}

void MainWindow::updateImageSide(int value)
{
	//Draw Y
	m_render.drawSubimage(m_yTarget, value, VolumeAxis::YAxis);
	m_sideImage->setPixmap(QPixmap::fromImage(m_yTarget));
}

void MainWindow::updateImageTop(int value)
{
	//Draw Z
	m_render.drawSubimage(m_zTarget, value, VolumeAxis::ZAxis);
	m_topImage->setPixmap(QPixmap::fromImage(m_zTarget));
}

void MainWindow::scaleImages(int value)
{
	//Scale percentage value to normalized value
	float scaleFactor = (float)value / 100.0f;

	resizeTargets(scaleFactor);

	redrawAll();
}

void MainWindow::redrawAll()
{
	updateImageFront(m_xSlider->value());
	updateImageSide(m_ySlider->value());
	updateImageTop(m_zSlider->value());
}

void MainWindow::resizeTargets(float scaleFactor)
{
	scaleFactor = std::max(scaleFactor, 0.0f);

	const Volume* v = m_render.volume();
	const auto xscaled = (Volume::SizeType)(scaleFactor * v->sizeX() * v->scaleX());
	const auto yscaled = (Volume::SizeType)(scaleFactor * v->sizeY() * v->scaleY());
	const auto zscaled = (Volume::SizeType)(scaleFactor * v->sizeZ() * v->scaleZ());

	//Scale target images
	m_xTarget = QImage(yscaled, zscaled, QImage::Format_Grayscale8);
	m_yTarget = QImage(xscaled, zscaled, QImage::Format_Grayscale8);
	m_zTarget = QImage(xscaled, yscaled, QImage::Format_Grayscale8);
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

	m_xSlider->setSliderPosition((int)m_render.volume()->sizeX() / 2);
	m_ySlider->setSliderPosition((int)m_render.volume()->sizeY() / 2);
	m_zSlider->setSliderPosition((int)m_render.volume()->sizeZ() / 2);

	
	//3D
	m_3DTarget = QImage(256, 256, QImage::Format_Grayscale8);
	m_3DView = new QLabel(this);
	m_3DView->setText("test label");
	m_render.draw3D(m_3DTarget, QVector3D(0.0f, 0.0f, 1.0f));
	m_3DView->setPixmap(QPixmap::fromImage(m_3DTarget));
	layout->addWidget(m_3DView);

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

	//Set background colour
	QPalette pal(palette());
	pal.setColor(QPalette::Background, Qt::gray);
	scrollArea->setAutoFillBackground(true);
	scrollArea->setPalette(pal);

	return scrollArea;
}

QWidget* MainWindow::createControlArea()
{
	QFormLayout* ctrlLayout = new QFormLayout(this);
	ctrlLayout->setMargin(5);
	ctrlLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	
	//X view
	m_xSlider = new LabelledSlider(this);
	m_xSlider->setRange(0, (int)m_render.volume()->sizeX() - 1);
	//Y view
	m_ySlider = new LabelledSlider(this);
	m_ySlider->setRange(0, (int)m_render.volume()->sizeY() - 1);
	//Z view
	m_zSlider = new LabelledSlider(this);
	m_zSlider->setRange(0, (int)m_render.volume()->sizeZ() - 1);
	//Scale
	m_scaleSlider = new LabelledSlider(this);
	m_scaleSlider->setRange(IMAGE_SCALE_MIN, IMAGE_SCALE_MAX);
	m_scaleSlider->setValue(100);

	m_mipToggle = new QCheckBox(QStringLiteral("Maximum Intensity Projection"), this);
	m_heToggle = new QCheckBox(QStringLiteral("Histogram Equalization"), this);

	ctrlLayout->addRow(QStringLiteral("X: Side"), m_xSlider);
	ctrlLayout->addRow(QStringLiteral("Y: Front"), m_ySlider);
	ctrlLayout->addRow(QStringLiteral("Z: Top"), m_zSlider);
	ctrlLayout->addWidget(new QSplitter(this));
	ctrlLayout->addRow(QStringLiteral("Scale(%)"), m_scaleSlider);
	ctrlLayout->addWidget(m_mipToggle);
	ctrlLayout->addWidget(m_heToggle);

	QGroupBox* ctrlGroup = new QGroupBox(QStringLiteral("Options"), this);
	ctrlGroup->setLayout(ctrlLayout);
	ctrlGroup->setMaximumWidth(CTRL_WIDGET_WIDTH_MAX);
	ctrlGroup->setMinimumWidth(CTRL_WIDGET_WIDTH_MIN);

	return ctrlGroup;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
