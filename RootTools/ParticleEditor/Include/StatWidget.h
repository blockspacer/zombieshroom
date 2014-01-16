#pragma once

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtGui/QCloseEvent>

struct StatStruct
{
	unsigned int m_numEmitters;
	unsigned int m_numTotalParticles;
};

class StatWidget : public QDockWidget
{
	Q_OBJECT

public:
	StatWidget(QWidget *parent = 0);
	~StatWidget();
	//SETTERS
	void SetStatStruct( StatStruct* p_statStruct);

	//METHODS
	void Update(float p_dt);

private:
	//METHODS
	void closeEvent(QCloseEvent *event);
	//MEMBERS
	StatStruct*		m_statStruct;
	QWidget*		m_widget;
	QLCDNumber*		m_fpsLCD;
	QLabel*			m_labelLCD;
};

