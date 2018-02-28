/*
	Main Application Window implementation
*/

#include <QtWidgets>

#include "gfx/VolumeRender.h"
#include "gui/LabelledSlider.h"

#include "MainWindow.h"
#include "SubimageView.h"

enum Constants
{
	IMAGE_SCALE_MIN = 10,
	IMAGE_SCALE_MAX = 200,
	CTRL_WIDGET_WIDTH_MIN = 400,
	CTRL_WIDGET_WIDTH_MAX = 400,
	CAMERA_WIDTH = 256,
	CAMERA_HEIGHT = 256
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(Volume& volume, QWidget *parent) :
	QMainWindow(parent),
	m_render(volume)
{
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

void MainWindow::scaleImages(int value)
{
	//Scale percentage value to normalized value
	float scaleFactor = std::max((float)value / 100.0f, 0.0f);

	m_xSubimage->setScale(scaleFactor);
	m_ySubimage->setScale(scaleFactor);
	m_zSubimage->setScale(scaleFactor);

	m_render.redrawAll();
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
	connect(m_xSlider, &QSlider::valueChanged, m_xSubimage, &SubimageView::setIndex);
	connect(m_ySlider, &QSlider::valueChanged, m_ySubimage, &SubimageView::setIndex);
	connect(m_zSlider, &QSlider::valueChanged, m_zSubimage, &SubimageView::setIndex);

	connect(m_scaleSlider, &QSlider::valueChanged, this, &MainWindow::scaleImages);

	//Connect draw options
	connect(m_mipToggle, &QCheckBox::toggled, &m_render, &VolumeRender::enableMip);
	connect(m_heToggle, &QCheckBox::toggled, &m_render, &VolumeRender::enableHist);

	//When MIP is enabled, disable the view sliders
	connect(m_mipToggle, &QCheckBox::toggled, m_xSlider, &QSlider::setDisabled);
	connect(m_mipToggle, &QCheckBox::toggled, m_ySlider, &QSlider::setDisabled);
	connect(m_mipToggle, &QCheckBox::toggled, m_zSlider, &QSlider::setDisabled);

	//Connect redraw signal - subimage widgets are redrawn when renderer is updated
	connect(&m_render, &VolumeRender::redrawAll, m_xSubimage, &SubimageView::redraw);
	connect(&m_render, &VolumeRender::redrawAll, m_ySubimage, &SubimageView::redraw);
	connect(&m_render, &VolumeRender::redrawAll, m_zSubimage, &SubimageView::redraw);

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
	m_xSubimage = new SubimageView(&m_render, VolumeAxis::XAxis, 0, this);
	m_ySubimage = new SubimageView(&m_render, VolumeAxis::YAxis, 0, this);
	m_zSubimage = new SubimageView(&m_render, VolumeAxis::ZAxis, 0, this);

	//Image grid
	QGridLayout* imageLayout = new QGridLayout(this);
	imageLayout->addWidget(m_zSubimage, 0, 0);
	imageLayout->addWidget(m_ySubimage, 0, 1);
	imageLayout->addWidget(m_xSubimage, 1, 0);

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
