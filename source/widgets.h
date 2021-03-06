#include <vector>
#include <string>
#include <map>
#include <3ds.h>
#include "images.h"
#include "font.h"

#ifndef __WIDGETSH__
#define __WIDGETSH__

class CBaseWindow;

typedef int (*LPEVENTFUNC)(CBaseWindow *,u32);
typedef struct{
	u8 *screen;
	u32 index;
	LPRECT prcItem;
	char *value;
} DRAWITEM,*LPDRAWITEM;

//---------------------------------------------------------------------------
class CBaseWindow {
public:
	CBaseWindow();
	CBaseWindow(gfxScreen_t s);
	virtual ~CBaseWindow();
	virtual int draw(u8 *screen);
	int set_BkColor(u32 c);
	int set_TextColor(u32 c);
	gfxScreen_t get_Screen(){return scr;}
	virtual CBaseWindow *onTouchEvent(touchPosition *p,u32 flags=0);
	virtual CBaseWindow *onKeysPressEvent(u32 press,u32 flags=0);
	virtual CBaseWindow *onKeysUpEvent(u32 press,u32 flags=0);
	virtual int onCharEvent(u8 c);
	virtual int onActivate(int v);	
	virtual int create(u32 x,u32 y,u32 w,u32 h,u32 id);
	virtual int set_Alpha(u8 val);
	int set_Parent(CBaseWindow *w){parent = w;return 0;};
	CBaseWindow *get_Parent(){return parent;};
	virtual int Invalidate(int flags=0);
	virtual int set_Pos(int x, int y);
	int get_WindowRect(LPRECT prc);
	virtual int get_ClientRect(LPRECT prc,u32 flags=0);
	virtual int set_Text(char *s);	
	int get_Text(char *s,u32 len);
	u32 get_ID(){return ID;};
	int set_Events(char *key,LPEVENTFUNC value);
	virtual int Show();
	virtual int Hide();
	virtual int is_Visible(){return (status & 4) == 0;};
private:
	void Init();
protected:
	virtual int is_invalidate();
	virtual int destroy();
	virtual int fire_event(const char *key,u32 param = 0);
	int has_event(const char *key);
	CBaseWindow *get_Desktop();
	virtual u32 adjust_AlphaColor(u32 col);
	
	u32 color,bkcolor,status,ID,redraw,text_len;
	gfxScreen_t scr;
	RECT rcWin;
	CBaseWindow *parent;
	char *text;
	font_s *font;
	u8 alpha;
	std::map<std::string,void *>events;
};
//---------------------------------------------------------------------------
class CContainerWindow : public CBaseWindow{
public:
	CContainerWindow();
	CContainerWindow(gfxScreen_t s);
	virtual ~CContainerWindow(){};
	int add(CBaseWindow *w);
	int remove(CBaseWindow *w);
	int remove(u32 id);
	virtual int draw(u8 *screen);
	virtual int Invalidate(int flags=0);
	CBaseWindow *get_Window(u32 id);
	virtual int recalc_layout();
	virtual int set_Alpha(u8 val);
	virtual CBaseWindow *onTouchEvent(touchPosition *p,u32 flags=0);
	virtual CBaseWindow *onKeysPressEvent(u32 press,u32 flags=0);
	virtual CBaseWindow *onKeysUpEvent(u32 press,u32 flags=0);
	virtual int set_Pos(int x, int y);
protected:
	virtual int EraseBkgnd(u8 *screen);
	std::vector<CBaseWindow *>wins;
	u32 bktcolor;
};
//---------------------------------------------------------------------------
class CCursor : public CAnimation {
public:
	CCursor(CContainerWindow *w);
	virtual ~CCursor(){};
	int Show(CBaseWindow *w,int x,int y);
	int Hide();
	int onTimer();
	int set_Pos(int x,int y);	
protected:
	int draw();
	CBaseWindow *win;
	CContainerWindow *desk;
	POINT pos;
	SIZE sz;
};
//---------------------------------------------------------------------------
class CDialog : public CContainerWindow {
public:
	CDialog();
	virtual ~CDialog();
};
//---------------------------------------------------------------------------
class CDesktop : public CContainerWindow {
public:
	CDesktop(gfxScreen_t s);
	virtual ~CDesktop(){};
	virtual int ShowCursor(CBaseWindow *w,int x,int y);
	virtual int HideCursor();
	virtual int SetCursorPos(int x,int y);
	u8 *get_Buffer();
	int ShowDialog(CDialog *w);
	int HideDialog();
	virtual int draw(u8 *screen);
	virtual int Invalidate(int flags=0);
	virtual int init(){return -1;};
	int BringWinTop(CBaseWindow *w);
	int ActiveWindow(CBaseWindow *w);
	virtual CBaseWindow *onTouchEvent(touchPosition *p,u32 flags=0);
	virtual CBaseWindow *onKeysPressEvent(u32 press,u32 flags=0);
	CBaseWindow *get_ActiveWindow(){return a_win;};
protected:
	virtual int onActivateWindow(CBaseWindow *win);
	CBaseWindow *a_win,*dlg_win;
	CCursor *cursor;
};
//---------------------------------------------------------------------------
class CWindow : public CContainerWindow {
public:	
	CWindow();
	virtual ~CWindow();
	virtual int get_ClientRect(LPRECT prc,u32 flags=0);
protected:
	virtual int EraseBkgnd(u8 *screen);
	SIZE szCaption;
};
//---------------------------------------------------------------------------
class CLabel : public CBaseWindow{	
public:
	CLabel(char *c);
	virtual ~CLabel(){};
	int draw(u8 *screen);
};
//---------------------------------------------------------------------------
class CImageWindow : public CBaseWindow{
public:
	CImageWindow();
	virtual int draw(u8 *screen);
	virtual int load(u8 *src,int w=-1,int h=-1);
	virtual int load(CImage *img);
protected:
	virtual int destroy();
	CImage *pImage;
};
//---------------------------------------------------------------------------
class CToolButton : public CImageWindow{
public:
	CToolButton();
	virtual int draw(u8 *screen);
	virtual int load(CImage *img,int idx);
protected:
	RECT rcImage;
};
//---------------------------------------------------------------------------
class CButton : public CBaseWindow{
public:
	CButton(char *c);
	CButton();
	virtual ~CButton();
	virtual int draw(u8 *screen);
	CBaseWindow *onKeysPressEvent(u32 press,u32 flags = 0);
	int set_Accelerator(int v){accel = v;return 0;};
protected:
	int accel;
};
//---------------------------------------------------------------------------
class CStatusBar  : public CContainerWindow{
public:
	CStatusBar();
	virtual ~CStatusBar();
protected:	
	int EraseBkgnd(u8 *screen);
};
//---------------------------------------------------------------------------
class CMenuBar : public CContainerWindow{
public:
	CMenuBar();
	virtual ~CMenuBar(){};
	int draw(u8 *screen);
};
//---------------------------------------------------------------------------
class CScrollBar : public CBaseWindow{
public:
	CScrollBar(int m=0);
	virtual ~CScrollBar(){};
	int draw(u8 *screen);
	virtual int create(u32 x,u32 y,u32 w,u32 h,u32 id);
	virtual CBaseWindow *onTouchEvent(touchPosition *p,u32 flags=0);
	int set_ScrollInfo(u32 mn,u32 mx,u32 pg);
	int get_ScrollInfo(u32 *mn,u32 *mx,u32 *pg);
	int set_ScrollPos(u32 p);
	int get_ScrollPos(u32 *p);
protected:
	int recalc_layout();
	u32 pos,min,max,page;
	RECT rcThumb;
};
//---------------------------------------------------------------------------
class CEditText : public CBaseWindow{
public:
	CEditText();
	virtual ~CEditText();
	int draw(u8 *screen);
	int onActivate(int v);
	virtual int create(u32 x,u32 y,u32 w,u32 h,u32 id);
	virtual int onCharEvent(u8 c);
	virtual CBaseWindow *onKeysPressEvent(u32 press,u32 flags = 0);
protected:
	int HideCursor();
	int ShowCursor(int x,int y);
	u32 char_pos;
	POINT ptCursor;
};
//---------------------------------------------------------------------------
class CEditBox : public CContainerWindow
{
public:
	CEditBox();
	virtual ~CEditBox(){};
	int create(u32 x,u32 y,u32 w,u32 h,u32 id);
	int onActivate(int v);
protected:
	int EraseBkgnd(u8 *screen);
	CScrollBar *vbar;
	POINT ptCursor;
};
//---------------------------------------------------------------------------
class CListBox : public CContainerWindow{
public:
	CListBox();
	virtual ~CListBox(){};
	virtual int create(u32 x,u32 y,u32 w,u32 h,u32 id);
	virtual int draw(u8 *screen);
	static int onScroll(CBaseWindow *sb,u32 param);
	static int onClicked(CBaseWindow *sb,u32 param);
	int onClicked(u32 param);
	int onScroll(u32 param);
	int add_item(char *val);
	int remove_item(u32 idx);
	int set_ItemHeight(u32 val);
	u32 get_ItemHeight(){return item_height;};
protected:
	CScrollBar *vbar;
	u32 first_visible_item,item_height;
	std::vector<std::string> items;
};
//---------------------------------------------------------------------------
class CToolBar : public CContainerWindow{
public:
	CToolBar();
	virtual ~CToolBar();
	virtual int recalc_layout();
	virtual CBaseWindow *onTouchEvent(touchPosition *p,u32 flags=0);	
protected:
	int EraseBkgnd(u8 *screen);
};

extern int getCursorPos(LPPOINT p);

#endif