/*
Nombre/Name: Libreria para desarrollar en multiTasKing PSP
Desarrollador/Developer: Carlosgs (carlosgarciasaura@gmail.com)

AL UTILIZAR ESTE CODIGO FUENTE ACEPTAS QUE:
- Tienes que notificar al autor (Carlosgs) de que vas a usar este codigo (contactar en: carlosgarciasaura@gmail.com).
- Carlosgs no se hace responsable de cualquier daño que este codigo pueda causar.
- No hay NINGUNA garantía de funcionalidad.

WHEN YOU USE THIS SOURCE CODE YOU ACCEPT THAT:
- You have to notify the author (Carlosgs) that you are going to use this source (contact trough: carlosgarciasaura@gmail.com).
- Carlosgs is NOT responsable of any damage that this source code can cause.
- There is no warranty of functionality.
*/

//PSP_MODULE_INFO(HomebrewName, 0, 1, 0);//"HomebrewName" definido antes
//PSP_MAIN_THREAD_ATTR (PSP_THREAD_ATTR_USER);

#ifdef HomebrewName//This is only needed when compiling a program for MTK
#include <oslib/oslib.h>
/*#define RGB (r,v,b)((r) | ((v)<<8) | ((b)<<16) | (0xff<<24))
#define RGBA (r,v,b,a)((r) | ((v)<<8) | ((b)<<16) | (a<<24))
typedef unsigned long MTcolor*/
#endif

#define PI M_PI

#define A(color) ((u8)(color >> 24 & 0xFF))
#define B(color) ((u8)(color >> 16 & 0xFF))
#define G(color) ((u8)(color >> 8 & 0xFF))
#define R(color) ((u8)(color & 0xFF))

//#define MTgetZeroAlphaColor(color) ((u8)(RGBA(R(color),G(color),B(color),0)))

#define MTimage OSL_IMAGE
#define MTcolor OSL_COLOR
#define MTfont OSL_FONT

#define MTid3Tags ID3TagSimple

//Threads
enum priority {
	priorityVeryLow = 0x40,
	priorityLow = 0x30,
	priorityNormal = 0x20,
	priorityHigh = 0x10,
	priorityMoreThanHigh = 0x10,
	priorityVeryHigh = 0x8
};

typedef struct
{
	bool select;
	bool start;
	bool up;
	bool right;
	bool down;
	bool left;
	bool l;
	bool r;
	bool triangle;
	bool circle;
	bool cross;
	bool square;
	bool home;
	bool hold;
	bool note;
	
	int ax;//analog
	int ay;
	
	int as;//analog sensitivity (minimum displacement to give us ax or ay)
} MTcontrols;

/*typedef struct
{
	int open;
	int x,y;
	int width,height;
	int active;
	int CloseButton;
	char* name;
	SceCtrlData controls;
	int mouseX;
	int mouseY;
	MTimage* writtingBuffer;
	MTimage* readingBuffer;
	MTelementsList elements;
} MTwindow;

typedef struct
{
	int x,y;//Position (when printed in a window or in the screen directly)
	int width,height;//If no width specified, it'll be the lenght of the text or "hasBeenPressedText" (if defined).
	char* text;
	char* hasBeenPressedText;
	int isPressed;
	int hasBeenPressed;
	int keepPressed;//Keeps the button colored dark
	int wasLastPressed;//Variable needed for the count
	int count;//Counts the number of times it has been pressed
	int show;
} MTbutton;*/

#define MTfontHorizontal INTRAFONT_ADVANCE_H//default: advance horizontaly from one char to the next
#define MTfontVertical INTRAFONT_ADVANCE_V
//#define MTalignLeft INTRAFONT_ALIGN_LEFT//default: left-align the text
//#define MTalignCenter INTRAFONT_ALIGN_CENTER
//#define MTalignRight INTRAFONT_ALIGN_RIGHT
#define MTfontWidthVar INTRAFONT_WIDTH_VAR//default: variable-width
#define MTfontWidthFix INTRAFONT_WIDTH_FIX//set your custom fixed witdh to 24 pixels: INTRAFONT_WIDTH_FIX | 24

#define MTalignNone 0//Just to make sure nothing else takes it
#define MTalignUp 1
#define MTalignCenter 2
#define MTalignDown 3
#define MTalignLeft 4
#define MTalignRight 5

#define MTapiTypeWindow 0
#define MTapiTypeText 1
//#define MTapiTypeScrollArea 2
#define MTapiTypeRectangle 3
#define MTapiTypeCircle 4
#define MTapiTypeLine 5
#define MTapiTypeDot 6
#define MTapiTypeArc 7
#define MTapiTypeImage 8
#define MTapiTypeCross 9
#define MTapiTypeX 10
#define MTapiTypeTriangle 11
#define MTapiTypeGroup 12
#define MTapiTypeCircleSection 13
#define MTapiTypePolygon 14


#define MTdrawFX OSL_ALPHA_PARAMS
//#define MTgetDrawFX oslGetAlphaEx
//#define MTsetDrawFX oslSetAlphaEx

#define MTdrawFXnone OSL_FX_NONE//: Blending is purely disabled.
#define MTdrawFXdefault OSL_FX_DEFAULT//: Reverts to default mode (RGBA).
#define MTdrawFXrgba OSL_FX_RGBA//: The alpha value of images or colors is taken in account for blending. For example, drawing a rectangle with the following color: RGBA(0, 0, 0, 128) will draw it semi-transparent.
#define MTdrawFXalpha OSL_FX_ALPHA//: Regular alpha blending: dstColor = dstColor = srcColor * coeff1 + dstColor * (1 - coeff1).
#define MTdrawFXadd OSL_FX_ADD//: Color addition: dstColor = dstColor * coeff2 + srcColor * coeff1. With oslSetAlpha, coeff2 is not supplied and set to full (255) by default.
#define MTdrawFXsub OSL_FX_SUB//: Color substraction: dstColor = dstColor * coeff2 - srcColor * coeff1. Please note that substracting colors produce the invert effect that you'd expect to see: substracting red (255, 0, 0) from a white image (255, 255, 255) for example gives cyan output (0, 255, 255). Furthermore, you can OR the value with the following:
#define MTdrawFXcolor OSL_FX_COLOR//: Coeff1 and coeff2 are treated as 32-bit colors instead of 8-bit alpha values. Multiplication between a color and another gives a tinted color.
#define MTdrawFXtint OSL_FX_TINT//it will do (OSL_FX_ALPHA | OSL_FX_COLOR)

//#define MTsound OSL_SOUND

typedef struct {
	bool paused;
	OSL_SOUND* snd;
	MTimage* coverImg;
	MTid3Tags* tags;
	char* data;
} MTsound;

typedef struct
{
	int x;
	int y;
	
} MTvector;

typedef struct {
	char* url;
	char* file;
	char* cookie;
	char* post;
	int size;
	bool knownsize;
	int receivedsize;
	char percent;
	bool done;
	bool error;
	int ret;
	int response;
} MTdownload;

typedef struct {
	char* title;
	char* text;
	char* initialText;
	int len;
	int maxLen;
	int lines;
	int language;
	bool cancel;
	bool done;
} MTkeyboard;

#define MTunpackTypeRar 0
#define MTunpackTypeZip 1

typedef struct {
	char* file;
	char* path;
	char* pass;
	int type;
	bool error;
	int ret;
	char percent;
	bool done;
} MTextract;
/*
Rar errors:
0: OK
1: File doesnt exists
2: (File corrupt/Not rar)?
3: Wrong password
8: Not enough memory?
*/


typedef struct {
	MTimage* frames;
	int frameCount;//counter
	int frameNum;//number
	int step;//1 will step 1 frame each 1 call - 2 will step 1 frame each 2 calls
	int currentFrame;//current
	int loop;//frame to loop the anim - 0 will loop the whole anim - -1 wont loop, it will stop with the last frame
	
	u16 sizeX, sizeY;						//!< Displayable size /auto generated with first frame
	int x, y;								//!< Image positions
	int stretchX, stretchY;					//!< Final image size when drawn (stretched)
	//Rotation
	int centerX, centerY;					//!< Rotation center
	int angle;								//!< Angle (rotation) in degrees
	bool exists;
} MTanimation;

typedef struct {
	MTvector pos;
	MTvector size;
	int use;
} MTclipArea;

MTcolor mouseOverImage;
MTcolor mouseOverText;
MTcolor mouseOverColor;

MTcolor shadowColor;
int shadowRadius;
MTcolor barColor;
MTcolor buttonColor;
int cornerRadius;//buttons, windows

typedef struct {
	MTvector pos;
	MTvector size;//if 0,0 with a image it will draw it with no scale, else with scale
	MTimage* img;
	int count;
	bool pressed;
	bool oldPressed;
	char* text;
	void (*function);//ejecuta cada tap
	bool use;
	bool exists;
} MTapiButton;

typedef struct {
	int x;
	int y;
	MTvector drag;//Stores the origin of the drag
	bool dragging;
	int tap;//count of taps in the same position
} MTmouse;

typedef struct MTwinData{
	MTvector pos;//actual
	MTvector pos0;//original-default-before it was fullscreen
	//MTvector newPos;//actual
	MTvector size;//actual
	MTvector size0;//original-default
	//MTvector newSize;//original-default
	MTvector minimSize;//minimum size of the window
	
	//char animPercent;//when set to 100 it will begin a countdown animating the size and pos of the window, from pos to newPos and from size to newSize
	
	bool minimized;
	bool maximized;
	bool wasMaximized;
	
	bool active;
	
	void* data;//Will give caracteristics to the window. Example: it stores an image for the image browser, or a path for the file browser
	//Better: it will store a structure defined by the user
	
	void (*freeData)(void*);//function that will draw the contents of the window each frame
	
	bool autoFree;
	
	bool autoFreeIcon;
	
	void (*function)(struct MTwinData*);//function that will draw the contents of the window each frame
	
	bool visible;
	
	bool hasNoBar;//with this on the window wont have any button or graphic, it will only draw the contents (back color included)
	//hasNoBar = 1 will ignore "allowClose"... etc
	
	bool allowClose;
	bool allowMinimize;
	bool allowMaximize;
	bool allowResize;
	
	int barThick;
	int nameVertAlign;
	
	MTimage* icon;//icon to be displayed on the left. it will be scaled to fit the bar width, and scaled to a square of that size
	
	MTcolor backColor;//background
	
	bool shadow;
	//bool bar;
	
	char* name;
	
	MTmouse mouse;
	MTcontrols controls;
	MTcontrols tapControls;
	bool mouseIsOver;
	
	bool mouseIsOverWholeWindow;
	
	int cursorType;
	
	int dragging;
	bool wasDragging;
	MTvector dragPos;
	
	bool on;
	bool exists;
} MTapiWindow;

typedef struct {
	MTimage* img;
	MTanimation* anim;
	bool animated;
	MTvector origin;
} MTcursor;

typedef struct {
	char* name;
	char* data;
	void* data2;
	void (*function)();
	MTimage* img;
} MTapiMenuElement;

typedef struct {
	bool direction;//1 = vertical
	MTvector pos;
	MTvector size;
	
	int dragPos;
	bool dragging;
	
	//bool mouseIsOver;
	
	bool allowDrag;
	
	int newVal;
	
	int maxVal;
	int currVal;
	int shownNum;
	int totalNum;
	
} MTapiScrollBar;

typedef struct {
	char* name;
	char* prefix;
	MTvector pos;
	MTvector size;
	MTvector realSize;
	MTvector prevSize;
	/*int height;
	int realHeight;
	int prevHeight;*/
	
	MTelementsList list;
	
	int font;
	int textHeight;
	int textJumpSize;
	int prevTextHeight;
	int prevTextJumpSize;
	MTcolor textColor;
	
	int align;//0 = left 2=right 1 = middle
	
	int count;
	int prevCount;
	int firstNum;
	int maxFirstNum;
	int prevFirstNum;
	MTpositionInList firstShownElementPosition;
	MTapiMenuElement* selectedElement;
	char menuReadyToPrint[1024];
	
	//bool barIsActive;
	
	bool active;
	
	MTapiScrollBar scrollBarH;
	MTapiScrollBar scrollBarV;
	//bool needRefresh;
} MTapiMenu;

typedef struct {
	MTvector pos;
	MTvector size;
	MTvector realSize;
	MTvector prevSize;
	
	char* text;
	
	int font;
	int textHeight;
	int textJumpSize;
	int prevTextHeight;
	int prevTextJumpSize;
	MTcolor textColor;
	
	int len;
	int prevLen;
	
	int lines;
	
	bool autoLineBreak;
	bool prevAutoLineBreak;
	
	int adjustSpaces;
	int prevAdjustSpaces;
	
	int align;//0 = left 2=right 1 = middle
	
	int firstLine;
	int firstLinePos;
	int prevFirstLine;
	
	int maxFirstLine;
	
	int cursorPos;//selectedElement (NOT WORKING NOW)
	char* textReadyToPrint;
	
	int drawnLen;
	
	//bool barIsActive;
	
	bool active;
	
	MTapiScrollBar scrollBarH;
	MTapiScrollBar scrollBarV;
	//bool needRefresh;
} MTapiTextBox;

typedef struct {
	char* ip;
	char* countryCode;
	char* countryName;
	int regionCode;
	char* regionName;
	char* cityName;
	float latitude;
	float longitude;
	MTimage* flag;
} MTipInfo;

//Functions from MultiTasKing:
void MTdelay(int time);
//ETC////////////////////////////////////////////////

void MTapiSetWallpaperImage(char* path);
void MTapiSetCursorImage(char* path,int centerX, int centerY);

void MTdrawSmoothFillRoundRectangle(int x0, int y0, int x1, int y1, MTcolor color0, int radius, int sections);


