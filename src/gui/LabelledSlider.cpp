/*
	Labelled slider class
*/

#include <QStylePainter>
#include <QStyleOptionSlider>

#include "LabelledSlider.h"

void LabelledSlider::paintEvent(QPaintEvent* event)
{
	//Draw actual slider
	QSlider::paintEvent(event);

	QStylePainter painter(this);

	QStyleOptionSlider options;
	options.init(this);
	options.orientation = orientation();
	
	//Get default thickness of slider
	const int thickness = style()->pixelMetric(QStyle::PM_SliderThickness, &options, this);

	//Get height of text
	QRect textr;
	painter.drawText(rect(), Qt::TextDontPrint, QString::number(9), &textr);
	const int theight = textr.bottom();

	//Calculate text drawing rect
	QRect r(rect());
	r.setTop(thickness);
	r.setBottom(r.bottom() + theight);

	//Draw minimum/current/maximum slider values
	painter.drawText(r, QString::number(minimum()), QTextOption(Qt::AlignLeft));
	painter.drawText(r, QString::number(maximum()), QTextOption(Qt::AlignRight));
	painter.drawText(r, QString::number(value()), QTextOption(Qt::AlignHCenter));

	//Update minimum height so text labels aren't clipped
	setMinimumHeight(thickness + theight);
}
