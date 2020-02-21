#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

int main()
{
	Display *dis;
	int screen;
	Window win;
	GC gc;
	unsigned long black, white;
	XEvent event;
	KeySym key;
	char text[256] = "hello, world!";

	dis = XOpenDisplay((char *)0);
	screen = DefaultScreen(dis);
	black = BlackPixel(dis, screen);
	white = WhitePixel(dis, screen);

	win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, 300, 200, 5, white, black);
	XSetStandardProperties(dis, win, "My Window", "HI!", None, NULL, 0, NULL);
	XSelectInput(dis, win, ExposureMask | ButtonPressMask | KeyPressMask);
	gc = XCreateGC(dis, win, 0, 0);
	XSetBackground(dis, gc, white);
	XSetForeground(dis, gc, black);
	XClearWindow(dis, win);
	XMapRaised(dis, win);

	for (;;) {
		XNextEvent(dis, &event);
		if (event.type == Expose && event.xexpose.count == 0) {
			XDrawString(dis, win, gc, 0, 0, text, strlen(text));
		}
		if (event.type == KeyPress && XLookupString(&event.xkey, text, sizeof(text), &key, 0) == 1) {
			if (text[0] == 'q') break;
			printf("You pressed the '%c' key!\n", text[0]);
		}
		if (event.type == ButtonPress) {
			printf("You pressed a button at (%i,%i)\n", event.xbutton.x, event.xbutton.y);
			XDrawString(dis, win, gc, 0, 0, text, strlen(text));
		}
	}

	XFreeGC(dis, gc);
	XDestroyWindow(dis, win);
	XCloseDisplay(dis);
	return 0;
}
