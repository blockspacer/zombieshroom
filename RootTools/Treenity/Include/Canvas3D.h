#pragma once

#include <QtWidgets/QWidget>
#include <QtGui/QKeyEvent>
#include <memory>
#include <SDL2/SDL.h>
#include <qpiemenu.h>

class Canvas3D : public QWidget
{
	Q_OBJECT
		
public:
	Canvas3D(QWidget* p_parent = 0);
	~Canvas3D();

	void CreateOpenGLContext();

protected:
	void wheelEvent(QWheelEvent* event);
	//void keyPressEvent( QKeyEvent *k );
	//void keyReleaseEvent( QKeyEvent *k );
	void resizeEvent( QResizeEvent * event );
	//void enterEvent( QEvent * event );

	
private:
	std::shared_ptr<SDL_Window> m_window;

};