/*
	Main Application Window implementation
*/

#include <QtWidgets>

#include "MainWindow.h"
#include "gfx/VolumeRender.h"
#include "util/LabelledSlider.h"

enum Constants
{
	IMAGE_SCALE_MIN = 10,
	IMAGE_SCALE_MAX = 200,
	CTRL_WIDGET_WIDTH_MIN = 400,
	CTRL_WIDGET_WIDTH_MAX = 400,
	CAMERA_WIDTH = 300,
	CAMERA_HEIGHT = 300
};

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
	m_frontImage->setPixmap(
		m_render.drawSubimage(m_yscaled, m_zscaled, value, VolumeAxis::XAxis)
	);
}

void MainWindow::updateImageSide(int value)
{
	//Draw Y
	m_sideImage->setPixmap(
		m_render.drawSubimage(m_xscaled, m_zscaled, value, VolumeAxis::YAxis)
	);
}

void MainWindow::updateImageTop(int value)
{
	//Draw Z
	m_topImage->setPixmap(
		m_render.drawSubimage(m_xscaled, m_yscaled, value, VolumeAxis::ZAxis)
	);
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
	m_xscaled = (Volume::SizeType)(scaleFactor * v->sizeX() * v->scaleX());
	m_yscaled = (Volume::SizeType)(scaleFactor * v->sizeY() * v->scaleY());
	m_zscaled = (Volume::SizeType)(scaleFactor * v->sizeZ() * v->scaleZ());
}

void MainWindow::updateCamera(const QMatrix4x4& matrix)
{
	m_3DView->setPixmap(
		m_render.draw3D(CAMERA_WIDTH, CAMERA_HEIGHT, matrix)
	);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

QWidget* MainWindow::createWidgets()
{
	//Central widget layout
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(createControlArea());
	layout->addWidget(createImageArea());
	layout->addWidget(create3DArea());

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		2D Events
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
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		3D Events
	*/

	m_rotationXSlider->setSliderPosition(0);
	m_rotationYSlider->setSliderPosition(0);

	connect(m_rotationXSlider, &LabelledSlider::valueChanged, &m_camera, &ArcballCamera::setRotationX);
	connect(m_rotationYSlider, &LabelledSlider::valueChanged, &m_camera, &ArcballCamera::setRotationY);
	connect(&m_camera, &ArcballCamera::cameraUpdated, this, &MainWindow::updateCamera);
	
	updateCamera(QMatrix4x4());

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Central widget
	QWidget* center = new QWidget(this);
	center->setLayout(layout);

	return center;
}


QWidget* MainWindow::create3DArea()
{
	m_3DView = new QLabel(this);
	m_3DView->setText("test label");
	m_3DView->setAlignment(Qt::AlignCenter);

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(m_3DView);
	
	QFrame* area = new QFrame(this);
	area->setLayout(layout);

	//Set background colour
	QPalette pal(palette());
	pal.setColor(QPalette::Background, Qt::gray);
	area->setAutoFillBackground(true);
	area->setPalette(pal);

	return area;
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

	QFrame* area = new QFrame(this);
	area->setLayout(imageLayout);

	//Set background colour
	QPalette pal(palette());
	pal.setColor(QPalette::Background, Qt::gray);
	area->setAutoFillBackground(true);
	area->setPalette(pal);

	return area;
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

	m_rotationXSlider = new LabelledSlider(this);
	m_rotationXSlider->setRange(0, 360);
	m_rotationYSlider = new LabelledSlider(this);
	m_rotationYSlider->setRange(0, 360);

	m_mipToggle = new QCheckBox(QStringLiteral("Maximum Intensity Projection"), this);
	m_heToggle = new QCheckBox(QStringLiteral("Histogram Equalization"), this);

	ctrlLayout->addRow(QStringLiteral("X: Side"), m_xSlider);
	ctrlLayout->addRow(QStringLiteral("Y: Front"), m_ySlider);
	ctrlLayout->addRow(QStringLiteral("Z: Top"), m_zSlider);
	ctrlLayout->addWidget(new QSplitter(this));
	ctrlLayout->addRow(QStringLiteral("Scale(%)"), m_scaleSlider);
	ctrlLayout->addWidget(m_mipToggle);
	ctrlLayout->addWidget(m_heToggle);
	ctrlLayout->addWidget(new QSplitter(this));
	ctrlLayout->addRow(QStringLiteral("X rotation:"), m_rotationXSlider);
	ctrlLayout->addRow(QStringLiteral("Y rotation:"), m_rotationYSlider);

	QGroupBox* ctrlGroup = new QGroupBox(QStringLiteral("CT Viewer"), this);
	ctrlGroup->setLayout(ctrlLayout);
	ctrlGroup->setMaximumWidth(CTRL_WIDGET_WIDTH_MAX);
	ctrlGroup->setMinimumWidth(CTRL_WIDGET_WIDTH_MIN);

	return ctrlGroup;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
