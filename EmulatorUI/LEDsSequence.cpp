#include "LEDsSequence.h"
#include "Constants.h"
#include "../cpu.h"

LEDsSequence::LEDsSequence(QWidget* parent)
	: QWidget(parent) {

	view = new QGraphicsView(this);
	view->setFixedWidth((diameter + padding) * NUMBER_OF_LEDS + (padding * 2));
	view->setFixedHeight(diameter + (padding * 2));
	QGraphicsScene* scene = new QGraphicsScene(view);
	view->setScene(scene);
	auto w = view->size().width();
	qreal x = (-1 * (w / 2)) + padding;
	for (auto i = 0; i < NUMBER_OF_LEDS; ++i) {
		leds[i] = new QGraphicsEllipseItem(x, 0, diameter, diameter);
		leds[i]->setBrush(QBrush(EMU_CONSTANTS::white));
		scene->addItem(leds[i]);
		x += diameter + padding;
	}
}

void LEDsSequence::update() {

	auto p1 = cpu::getInstance()->getP1();
	for (auto i = 0; i < NUMBER_OF_LEDS; ++i) {
		if ((p1 >> i) & 0x01) {
			((QGraphicsEllipseItem*)leds[i])->setBrush(QBrush(EMU_CONSTANTS::green));
		}
		else {
			((QGraphicsEllipseItem*)leds[i])->setBrush(QBrush(EMU_CONSTANTS::white));
		}
	}
}
