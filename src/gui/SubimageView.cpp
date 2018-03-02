/*
	Subimage View widget source
*/

#include "SubimageView.h"
#include "ThumbnailDialog.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SubimageView::SubimageView(VolumeRender* render, VolumeAxis axis, Volume::IndexType index, QWidget* parent) :
	m_render(render),
	m_axis(axis),
	m_index(index),
	QWidget(parent)
{
	Q_ASSERT(m_render != nullptr);

	//Setup widget
	m_layout.addWidget(&m_image);
	m_layout.addWidget(&m_imageLabel);
	QWidget::setLayout(&m_layout);

	m_image.setAlignment(Qt::AlignCenter);
	m_imageLabel.setAlignment(Qt::AlignTop | Qt::AlignHCenter);

	//Figure out scaled width/height for given axis
	auto v = m_render->volume();
	auto xscaled = v->sizeX() * v->scaleX();
	auto yscaled = v->sizeY() * v->scaleY();
	auto zscaled = v->sizeZ() * v->scaleZ();

	Volume::SizeType dim[3][2] =
	{
		{ yscaled, zscaled }, //x
		{ xscaled, zscaled }, //y
		{ xscaled, yscaled }  //z
	};

	m_scaledWidth = dim[m_axis][0];
	m_scaledHeight = dim[m_axis][1];

	const char* labels[] =
	{
		"X: ",
		"Y: ",
		"Z: "
	};

	m_text = labels[m_axis];

	redraw();
}

void SubimageView::redraw()
{
	//Reserve space in buffer
	const Volume::SizeType w = m_scaleFactor * m_scaledWidth;
	const Volume::SizeType h = m_scaleFactor * m_scaledHeight;
	m_buffer.realloc(w, h);

	//If using maximum intensity projection
	//Render view
	if (m_useMip)
	{
		m_render->drawSubimageMIP(m_buffer, m_axis);
	}
	else
	{
		m_render->drawSubimage(m_buffer, m_index, m_axis);
	}

	//Present view
	m_image.setPixmap(m_buffer.toPixmap());

	//Update size label
	m_imageLabel.setText(m_text + QString::number(w) + "x" + QString::number(h));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SubimageView::mousePressEvent(QMouseEvent* event)
{
	//When image is clicked bring up the window
	ThumbnailDialog* dialog = new ThumbnailDialog(m_render, m_axis, this);
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->show();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
