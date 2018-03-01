/*
	Labelled slider class

	Identical to QSlider but min/max/current values are labelled.
	Can only be used in a horizontal orientation.
*/

#pragma once

#include <QSlider>

class LabelledSlider : public QSlider
{
	Q_OBJECT
	Q_DISABLE_COPY(LabelledSlider)

public:

	explicit LabelledSlider(QWidget* parent = nullptr) :
		QSlider(Qt::Horizontal, parent) //Horizontal orientation
	{}

private:

	void paintEvent(QPaintEvent *e) override;
};
