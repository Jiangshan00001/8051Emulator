#pragma once
#include "qwidget.h"
#include "qgraphicsview.h"
#include "qgraphicsitem.h"

#define NUMBER_OF_LEDS 8

class LEDsSequence : public QWidget
{
	Q_OBJECT
public:
	LEDsSequence(QWidget* parent);
	~LEDsSequence() {}

private:
	const qreal diameter = 20;
	const qreal padding = 5;
	QGraphicsView* view;
	QGraphicsEllipseItem* leds[NUMBER_OF_LEDS];

public slots:
	void update();
};

