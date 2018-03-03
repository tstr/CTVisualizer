/*
	Main Application Window implementation
*/

#include <QtWidgets>

#include "MainWindow.h"

#include "gfx/VolumeRender.h"

#include "SubimageView.h"
#include "CameraView.h"
#include "LabelledSlider.h"

#include "gl/GLVolumeScene.h"

enum Constants
{
	IMAGE_SCALE_MIN = 10,
	IMAGE_SCALE_MAX = 200,
	CTRL_WIDGET_WIDTH_MIN = 400,
	CTRL_WIDGET_WIDTH_MAX = 400,
	RAYCAST_FREQUENCY_MIN = 10,
	RAYCAST_FREQUENCY_MAX = 400,
	WINDOW_DEFAULT_WIDTH = 1064,
	WINDOW_DEFAULT_HEIGHT = 720
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(Volume& volume, QWidget *parent) :
	QMainWindow(parent),
	m_render(volume)
{
	setWindowTitle(QStringLiteral("CT Viewer"));
	resize(QSize(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT));

	QTabWidget* tab = new QTabWidget(this);

	QWidget* standardView = createWidgets();
	m_glView = new GLVolumeScene(m_render.volume(), tab);

	tab->addTab(standardView, QStringLiteral("Standard View"));
	tab->addTab(m_glView, QStringLiteral("GL View"));

	setCentralWidget(tab);
}

MainWindow::~MainWindow()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Slots
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::scaleImages(int value)
{
	//Scale percentage value to normalized value
	float scaleFactor = std::max((float)value / 100.0f, 0.0f);

	m_xSubimage->setScale(scaleFactor);
	m_ySubimage->setScale(scaleFactor);
	m_zSubimage->setScale(scaleFactor);

	m_render.redraw2D();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

QWidget* MainWindow::createWidgets()
{
	//Central widget layout
	QWidget* center = new QWidget(this);
	center->setLayout(new QHBoxLayout(center));
	center->layout()->addWidget(createControlArea());
	center->layout()->addWidget(createImageArea());

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Setup signals/slots
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Connect sliders
	connect(m_xSlider, &QSlider::valueChanged, m_xSubimage, &SubimageView::setIndex);
	connect(m_ySlider, &QSlider::valueChanged, m_ySubimage, &SubimageView::setIndex);
	connect(m_zSlider, &QSlider::valueChanged, m_zSubimage, &SubimageView::setIndex);

	connect(m_scaleSlider, &QSlider::valueChanged, this, &MainWindow::scaleImages);

	//Connect rendering options
	connect(m_heToggle, &QCheckBox::toggled, &m_render, &VolumeRender::enableHist);
	connect(m_mipToggle, &QCheckBox::toggled, m_xSubimage, &SubimageView::useMIP);
	connect(m_mipToggle, &QCheckBox::toggled, m_ySubimage, &SubimageView::useMIP);
	connect(m_mipToggle, &QCheckBox::toggled, m_zSubimage, &SubimageView::useMIP);

	//When MIP is enabled, disable the view sliders
	connect(m_mipToggle, &QCheckBox::toggled, m_xSlider, &QSlider::setDisabled);
	connect(m_mipToggle, &QCheckBox::toggled, m_ySlider, &QSlider::setDisabled);
	connect(m_mipToggle, &QCheckBox::toggled, m_zSlider, &QSlider::setDisabled);

	//Sampling functions
	connect(m_samplerBasic,  &QRadioButton::clicked, &m_render, &VolumeRender::setSamplingTypeBasic);
	connect(m_samplerBilinear, &QRadioButton::clicked, &m_render, &VolumeRender::setSamplingTypeBilinear);
	connect(m_samplerBicubic,  &QRadioButton::clicked, &m_render, &VolumeRender::setSamplingTypeBicubic);

	connect(m_samplerBasic3D, &QRadioButton::clicked, &m_render, &VolumeRender::setSamplingType3DBasic);
	connect(m_samplerTrilinear, &QRadioButton::clicked, &m_render, &VolumeRender::setSamplingTypeTrilinear);

	//3D sample frequency slider
	connect(m_3DSampleSlider, &LabelledSlider::valueChanged, &m_render, &VolumeRender::setSampleFrequency);

	//Connect redraw signals - subimage widgets are redrawn when renderer is updated
	connect(&m_render, &VolumeRender::redraw2D, m_xSubimage, &SubimageView::redraw);
	connect(&m_render, &VolumeRender::redraw2D, m_ySubimage, &SubimageView::redraw);
	connect(&m_render, &VolumeRender::redraw2D, m_zSubimage, &SubimageView::redraw);

	connect(&m_render, &VolumeRender::redraw3D, m_3DView, &CameraView::redraw);

	m_xSlider->setSliderPosition((int)m_render.volume()->sizeX() / 2);
	m_ySlider->setSliderPosition((int)m_render.volume()->sizeY() / 2);
	m_zSlider->setSliderPosition((int)m_render.volume()->sizeZ() / 2);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return center;
}

QWidget* MainWindow::createImageArea()
{
	//Images
	m_xSubimage = new SubimageView(&m_render, VolumeAxis::XAxis, 0, this);
	m_ySubimage = new SubimageView(&m_render, VolumeAxis::YAxis, 0, this);
	m_zSubimage = new SubimageView(&m_render, VolumeAxis::ZAxis, 0, this);

	//3D view
	m_3DView = new CameraView(&m_render, this);

	//Image grid
	QGridLayout* imageLayout = new QGridLayout(this);
	imageLayout->addWidget(m_zSubimage, 0, 0);
	imageLayout->addWidget(m_ySubimage, 0, 1);
	imageLayout->addWidget(m_xSubimage, 1, 0);
	imageLayout->addWidget(m_3DView,    1, 1);

	QWidget* viewport = new QWidget(this);
	viewport->setLayout(imageLayout);

	QScrollArea* scroll = new QScrollArea(this);
	//scroll->setWidget(scrollWidget);
	scroll->setWidgetResizable(true);
	scroll->setWidget(viewport);

	return scroll;
}

QWidget* MainWindow::createControlArea()
{
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// Slider widgets
	///////////////////////////////////////////////////////////////////////////////////////////////////

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

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// Render state widgets
	///////////////////////////////////////////////////////////////////////////////////////////////////

	m_mipToggle = new QCheckBox(QStringLiteral("Maximum Intensity Projection"), this);
	m_heToggle = new QCheckBox(QStringLiteral("Histogram Equalization"), this);

	//2D sampler functions
	QGroupBox* samplerGroup2D = new QGroupBox(QStringLiteral("2D Sampler Function:"), this);

	m_samplerBasic = new QRadioButton(QStringLiteral("Nearest-Neighbour"), samplerGroup2D);
	m_samplerBilinear = new QRadioButton(QStringLiteral("Bilinear"), samplerGroup2D);
	m_samplerBicubic = new QRadioButton(QStringLiteral("Bicubic"), samplerGroup2D);
	m_samplerBilinear->setChecked(true);

	samplerGroup2D->setLayout(new QVBoxLayout(samplerGroup2D));
	samplerGroup2D->layout()->addWidget(m_samplerBasic);
	samplerGroup2D->layout()->addWidget(m_samplerBilinear);
	samplerGroup2D->layout()->addWidget(m_samplerBicubic);


	//3D sampler functions
	QGroupBox* samplerGroup3D = new QGroupBox(QStringLiteral("3D Sampler Function:"), this);

	m_samplerBasic3D = new QRadioButton(QStringLiteral("Nearest-Neighbour"), samplerGroup3D);
	m_samplerTrilinear = new QRadioButton(QStringLiteral("Trilinear"), samplerGroup3D);
	m_samplerTrilinear->setChecked(true);

	samplerGroup3D->setLayout(new QVBoxLayout(samplerGroup3D));
	samplerGroup3D->layout()->addWidget(m_samplerBasic3D);
	samplerGroup3D->layout()->addWidget(m_samplerTrilinear);

	///////////////////////////////////////////////////////////////////////////////////////////////////

	QVBoxLayout* ctrlLayout = new QVBoxLayout(this);
	ctrlLayout->setMargin(5);
	ctrlLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	QFormLayout* ctrlSliders = new QFormLayout(this);
	ctrlSliders->addRow(QStringLiteral("X: Side"), m_xSlider);
	ctrlSliders->addRow(QStringLiteral("Y: Front"), m_ySlider);
	ctrlSliders->addRow(QStringLiteral("Z: Top"), m_zSlider);
	ctrlSliders->addWidget(new QSplitter(this));
	ctrlSliders->addRow(QStringLiteral("Scale(%)"), m_scaleSlider);

	ctrlLayout->addLayout(ctrlSliders);
	ctrlLayout->addWidget(new QSplitter(this));
	ctrlLayout->addWidget(m_mipToggle);
	ctrlLayout->addWidget(m_heToggle);
	ctrlLayout->addWidget(new QSplitter(this));
	ctrlLayout->addWidget(samplerGroup2D);
	ctrlLayout->addWidget(new QSplitter(this));

	//3D options
	ctrlLayout->addWidget(samplerGroup3D);
	ctrlLayout->addWidget(new QLabel(QStringLiteral("Raycast Sample Frequency:")));
	m_3DSampleSlider = new LabelledSlider(this);
	m_3DSampleSlider->setRange(RAYCAST_FREQUENCY_MIN, RAYCAST_FREQUENCY_MAX);
	m_3DSampleSlider->setSliderPosition(125);
	ctrlLayout->addWidget(m_3DSampleSlider);

	QGroupBox* ctrlArea = new QGroupBox(QStringLiteral("Options:"), this);
	ctrlArea->setLayout(ctrlLayout);
	ctrlArea->setMaximumWidth(CTRL_WIDGET_WIDTH_MAX);
	ctrlArea->setMinimumWidth(CTRL_WIDGET_WIDTH_MIN);

	return ctrlArea;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
