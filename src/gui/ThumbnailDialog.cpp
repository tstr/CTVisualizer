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

	//For every subimage along axis
	for (const VolumeSubimage& subimage : range)
	{
		//Create list item with subimage information
		QListWidgetItem* item = new QListWidgetItem(m_tbList);

		//Draw slice and store result in QIcon
		QIcon icon;
		icon.addPixmap(
			render->drawSubimage(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, subimage.index(), axis)
		);

		item->setIcon(icon);
		item->setText(QString::number(subimage.index()));

		m_tbList->addItem(item);
	}

	QDialog::setLayout(new QVBoxLayout(this));
	QDialog::setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	QDialog::resize(DIALOG_WIDTH, DIALOG_HEIGHT);
	QDialog::layout()->addWidget(m_tbList);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
