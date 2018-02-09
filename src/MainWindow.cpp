/*
	Main Application Window implementation
*/

#include <QtWidgets>

#include "MainWindow.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{
	setWindowTitle("title");
	setFixedSize(QSize(1280, 720));
	setCentralWidget(createWidgets());
}

MainWindow::~MainWindow()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

QWidget* MainWindow::createWidgets()
{
	//qApp->setStyleSheet("QGroupBox {  border: 1px solid gray;}");
	
	//Central widget layout
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addWidget(createControlArea());
	layout->addWidget(createImageArea());

	//Central widget
	QWidget* center = new QWidget(this);
	center->setLayout(layout);

	return center;
}

QWidget* MainWindow::createImageArea()
{
	QPixmap pix(QPixmap::fromImage(QImage(QStringLiteral("test.png"))));

	m_leftImage = new QLabel(QStringLiteral("top"), this);
	m_leftImage->setPixmap(pix);
	m_leftImage->setAlignment(Qt::AlignCenter);
	m_rightImage = new QLabel(QStringLiteral("top"), this);
	m_rightImage->setPixmap(pix);
	m_rightImage->setAlignment(Qt::AlignCenter);

	//Images
	QGridLayout* imageLayout = new QGridLayout(this);
	imageLayout->addWidget(m_leftImage, 0, 0);
	imageLayout->addWidget(m_rightImage, 0, 1);

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
	//controlLayout->addRow(QStringLiteral("Z slider"), m_zSlider);
	ctrlLayout->addWidget(new QLabel(QStringLiteral("Z slider"), this), 1, 0);
	ctrlLayout->addWidget(m_zSlider, 1, 1);

	m_ySlider = new QSlider(Qt::Horizontal, this);
	//controlLayout->addRow(QStringLiteral("Y slider"), m_ySlider);
	ctrlLayout->addWidget(new QLabel(QStringLiteral("Y slider"), this), 2, 0);
	ctrlLayout->addWidget(m_ySlider, 2, 1);

	m_xSlider = new QSlider(Qt::Horizontal, this);
	//controlLayout->addRow(QStringLiteral("Z slider"), m_zSlider);
	ctrlLayout->addWidget(new QLabel(QStringLiteral("X slider"), this), 3, 0);
	ctrlLayout->addWidget(m_xSlider, 3, 1);

	ctrlLayout->addItem(new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Expanding), 4, 0, 2);

	QGroupBox* ctrlGroup = new QGroupBox(QStringLiteral("options"), this);
	ctrlGroup->setLayout(ctrlLayout);
	ctrlGroup->setMaximumWidth(300);
	ctrlGroup->setMinimumWidth(200);

	return ctrlGroup;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
