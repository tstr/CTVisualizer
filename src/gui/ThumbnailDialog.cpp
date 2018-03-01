/*
	Volume thumbnail dialog
*/

#include <QVBoxLayout>
#include <QListWidget>

#include "ThumbnailDialog.h"

#include "gfx/VolumeSubimageRange.h"

enum Constants
{
	THUMBNAIL_WIDTH = 128,
	THUMBNAIL_HEIGHT = 128,
	DIALOG_WIDTH = THUMBNAIL_WIDTH * 8,
	DIALOG_HEIGHT = THUMBNAIL_HEIGHT * 5
};

///////////////////////////////////////////////////////////////////////////////////////////////////////

ThumbnailDialog::ThumbnailDialog(VolumeRender* render, VolumeAxis axis, QWidget* parent) :
	QDialog(parent)
{
	Q_ASSERT(render != nullptr);

	VolumeSubimageRange range(render->volume(), axis);

	/*
		Setup list widget
	*/
	m_tbList = new QListWidget(this);

	m_tbList->setViewMode(QListWidget::IconMode);
	m_tbList->setIconSize(QSize(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT));
	m_tbList->setResizeMode(QListWidget::Adjust);
	m_tbList->resize(DIALOG_WIDTH, DIALOG_HEIGHT);
	m_tbList->setDragEnabled(false);

	ImageBuffer image(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT);

	//For every subimage along axis
	for (const VolumeSubimage& subimage : range)
	{
		//Create list item with subimage information
		QListWidgetItem* item = new QListWidgetItem(m_tbList);

		//Draw slice and store result in QIcon
		render->drawSubimage(image, subimage.index(), axis);
		
		QIcon icon;
		icon.addPixmap(image.toPixmap());

		item->setIcon(icon);
		item->setText(QString::number(subimage.index()));

		m_tbList->addItem(item);
	}

	//Setup ListView widget and sizing
	QDialog::setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	QDialog::resize(DIALOG_WIDTH, DIALOG_HEIGHT);
	QDialog::setLayout(new QVBoxLayout(this));
	QDialog::layout()->addWidget(m_tbList);

	//Set title
	QString axisLabels[] = 
	{
		QStringLiteral("X Subimages"),
		QStringLiteral("Y Subimages"),
		QStringLiteral("Z Subimages")
	};

	QDialog::setWindowTitle(axisLabels[axis]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
