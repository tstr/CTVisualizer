/*
	Volume thumbnail dialog
*/

#pragma once

#include <QLabel>
#include <QDialog>

#include "gfx/VolumeRender.h"

class QListWidget;
class QListWidgetItem;

class ThumbnailDialog : public QDialog
{
	Q_OBJECT
	Q_DISABLE_COPY(ThumbnailDialog)

public:

	/*
		Construct a thumbnail dialog showing all the subimages of a given axis.
	*/
	explicit ThumbnailDialog(VolumeRender* render, VolumeAxis axis, QWidget* parent = nullptr);

private:

	//Thumbnail list
	QListWidget* m_tbList;
};
