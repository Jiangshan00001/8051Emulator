#include "EmulatorUI.h"
#include "qmessagebox.h"
#include "qfiledialog.h"
#include "../cpu.h"
#include "qthread.h"
#include <QtConcurrent/qtconcurrentrun.h>
#include "qgraphicsitem.h"
#include "LEDsSequence.h"

void cycleCallback(void* client) {
	((Worker*)client)->callback();
}

void EmulatorUI::startEmulation()
{
	cpu::getInstance()->emulateCycle();
}

EmulatorUI::EmulatorUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.actionLoad->setEnabled(true);
	ui.actionRun->setEnabled(false);
	ui.actionStop->setEnabled(false);
	isInitialized = false;
	
	LEDsSequence* leds = new LEDsSequence(ui.centralWidget);
	leds->move(10, 10);

	worker = new Worker(this);
	connect(worker, SIGNAL(updateUI()), this, SLOT(displayCycle()));
	connect(worker, SIGNAL(updateUI()), leds, SLOT(update()));

	connect(ui.actionLoad, &QAction::triggered, [=]() {
		auto name = QFileDialog::getOpenFileName(this, "Open Hex File", "", "HEX | *.hex");
		isInitialized = cpu::getInstance()->initialize(name.toStdString(), cycleCallback, worker);
		if (isInitialized) {
			ui.statusBar->showMessage("Loaded " + name);
			ui.actionRun->setEnabled(true);
		}
		else {
			ui.statusBar->showMessage("Failed to load " + name);
			ui.actionRun->setEnabled(false);
		}
		});
	connect(ui.actionRun, &QAction::triggered, [=]() {
		ui.statusBar->showMessage("Running loaded program...");
		ui.actionRun->setEnabled(false);
		ui.actionStop->setEnabled(true);
		ui.actionLoad->setEnabled(false);

		worker->start();

		});
	connect(ui.actionStop, &QAction::triggered, [=]() {
		worker->stop();
		ui.statusBar->showMessage("Stop selected", 3000);
		ui.actionRun->setEnabled(true);
		ui.actionStop->setEnabled(false);
		ui.actionLoad->setEnabled(true);
		});
}

EmulatorUI::~EmulatorUI()
{
	worker->stop();
	delete worker;
}

void EmulatorUI::resizeEvent(QResizeEvent* e)
{
	auto w = e->size().width();
	ui.cpuRegisters->setMaximumWidth(w / 3.5);
	ui.cpuRegisters->setMinimumWidth(w / 3.5);
}

void EmulatorUI::closeEvent(QCloseEvent* event)
{
	if(worker->isRunning()) {
		worker->stop();
	}
}

void EmulatorUI::displayCycle()
{
	ui.p0->setText(QString("0x%1").arg(cpu::getInstance()->getP0(), 2, 16, QLatin1Char('0')));
	ui.p1->setText(QString("0x%1").arg(cpu::getInstance()->getP1(), 2, 16, QLatin1Char('0')));
	ui.p2->setText(QString("0x%1").arg(cpu::getInstance()->getP2(), 2, 16, QLatin1Char('0')));
	ui.p2->setText(QString("0x%1").arg(cpu::getInstance()->getP3(), 2, 16, QLatin1Char('0')));

	ui.r0->setText(QString("0x%1").arg(cpu::getInstance()->getR0(), 2, 16, QLatin1Char('0')));
	ui.r1->setText(QString("0x%1").arg(cpu::getInstance()->getR1(), 2, 16, QLatin1Char('0')));
	ui.r2->setText(QString("0x%1").arg(cpu::getInstance()->getR2(), 2, 16, QLatin1Char('0')));
	ui.r3->setText(QString("0x%1").arg(cpu::getInstance()->getR3(), 2, 16, QLatin1Char('0')));
	ui.r4->setText(QString("0x%1").arg(cpu::getInstance()->getR4(), 2, 16, QLatin1Char('0')));
	ui.r5->setText(QString("0x%1").arg(cpu::getInstance()->getR5(), 2, 16, QLatin1Char('0')));
	ui.r6->setText(QString("0x%1").arg(cpu::getInstance()->getR6(), 2, 16, QLatin1Char('0')));
	ui.r7->setText(QString("0x%1").arg(cpu::getInstance()->getR7(), 2, 16, QLatin1Char('0')));

	ui.b->setText(QString("0x%1").arg(cpu::getInstance()->getB(), 2, 16, QLatin1Char('0')));
	ui.acc->setText(QString("0x%1").arg(cpu::getInstance()->getACC(), 2, 16, QLatin1Char('0')));
	ui.ip->setText(QString("0x%1").arg(cpu::getInstance()->getIP(), 2, 16, QLatin1Char('0')));
	ui.ie->setText(QString("0x%1").arg(cpu::getInstance()->getIE(), 2, 16, QLatin1Char('0')));
	ui.tmod->setText(QString("0x%1").arg(cpu::getInstance()->getTMOD(), 2, 16, QLatin1Char('0')));
	ui.tcon->setText(QString("0x%1").arg(cpu::getInstance()->getTCON(), 2, 16, QLatin1Char('0')));
	ui.pcon->setText(QString("0x%1").arg(cpu::getInstance()->getPCON(), 2, 16, QLatin1Char('0')));
	ui.dph->setText(QString("0x%1").arg(cpu::getInstance()->getDPH(), 2, 16, QLatin1Char('0')));
	ui.dpl->setText(QString("0x%1").arg(cpu::getInstance()->getDPL(), 2, 16, QLatin1Char('0')));
	ui.sp->setText(QString("0x%1").arg(cpu::getInstance()->getSP(), 2, 16, QLatin1Char('0')));

	ui.pc->setText(QString("0x%1").arg(cpu::getInstance()->getPC(), 4, 16, QLatin1Char('0')));

	ui.th0->setText(QString("0x%1").arg(cpu::getInstance()->getTH0(), 2, 16, QLatin1Char('0')));
	ui.th1->setText(QString("0x%1").arg(cpu::getInstance()->getTH1(), 2, 16, QLatin1Char('0')));
	ui.tl0->setText(QString("0x%1").arg(cpu::getInstance()->getTL0(), 2, 16, QLatin1Char('0')));
	ui.tl1->setText(QString("0x%1").arg(cpu::getInstance()->getTL1(), 2, 16, QLatin1Char('0')));
	
	ui.c->setText(QString("%1").arg(cpu::getInstance()->PSW_C(), 1, 2));
	ui.ac->setText(QString("%1").arg(cpu::getInstance()->PSW_AC(), 1, 2));
	ui.f0->setText(QString("%1").arg(cpu::getInstance()->PSW_F0(), 1, 2));
	ui.rs1->setText(QString("%1").arg(cpu::getInstance()->PSW_RS1(), 1, 2));
	ui.rs0->setText(QString("%1").arg(cpu::getInstance()->PSW_RS0(), 1, 2));
	ui.ov->setText(QString("%1").arg(cpu::getInstance()->PSW_OV(), 1, 2));
	ui.p->setText(QString("%1").arg(cpu::getInstance()->PSW_P(), 1, 2));
}
