#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EmulatorUI.h"
#include <qevent.h>
#include <qthread.h>
#include "qmutex.h"
#include "../cpu.h"

class Worker : public QThread {
	Q_OBJECT
public:
	explicit Worker(QObject* parent) : QThread(parent), m_isRunning(false){}
	void run() {
		m_isRunning = true;
		cpu::getInstance()->emulateCycle();
	}

	void callback() {
		emit updateUI();
	}

	void stop() {
		cpu::getInstance()->stopEmulation();
		m_isRunning = false;
	}

	bool isRunning() { return m_isRunning; }

signals:
	void updateUI();

private:
	bool m_isRunning;
};

class EmulatorUI : public QMainWindow
{
	Q_OBJECT

public:
	EmulatorUI(QWidget *parent = Q_NULLPTR);
	~EmulatorUI();
	
	void resizeEvent(QResizeEvent* e) override;
	void closeEvent(QCloseEvent* event) override;

public slots:
	void displayCycle();
private:
	Ui::EmulatorUIClass ui;
	bool isInitialized;

	Worker* worker;
	QThread thread;
	void startEmulation();
};
