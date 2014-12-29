#include <vector>
#include <3ds.h>
#include "widgets.h"
#include "keyboard.h"

#ifndef __GUIH__
#define __GUIH__

class CTopDesktop : public CDesktop{
public:
	CTopDesktop();
	virtual ~CTopDesktop(){};
};

class CBottomDesktop : public CDesktop{
public:
	CBottomDesktop();
	virtual ~CBottomDesktop(){};
	virtual int draw(u8 *screen);
	int onTouchEvent(touchPosition *p);
protected:
	int EraseBkgnd(u8 *screen);
};

class CConsoleWindow : public CWindow{
public:
	CConsoleWindow();
	int printf(char *fmt,...);
};

class CLoaderWindow : public CImageWindow{
public:
	CLoaderWindow();
};

int gui_init();
int gui_destroy();

extern CDesktop *top,*bottom;
extern CConsoleWindow *console;
#define print console->printf

#endif