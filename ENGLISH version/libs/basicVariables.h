bool MTcanUseKernel = 0;

int MTsystemLanguage = -1;

int screenWidth = 480;
int screenHeight = 272;

//MTfont* MTdefaultFont;
float MTdefaultFontHeight = 10;
//char MTdefaultFontPath[512];
MTcolor MTdefaultFontColor = RGB(128,128,128);

int MTapiMaxFontID = 15;
MTfont* MTapiFontList[16];

bool screenLocked = 0;
bool controlsLocked = 0;

int minimumCursorPadSensitivity = 60;
int aceleracionMaximaCursor = 8;

MTimage* MTwallpaperImage;
char MTmainWalpaperImagePath[512];

//MTimage* MTmainCursorImage;
//char MTmainCursorImagePath[512];
//MTvector* MTmainCursorCenter;

int MTapiMaxCursorID = 14;
MTcursor* MTapiCursorList[15];
int MTcurrentCursorType = 0;

//char test[1024*1024];

//int cursorX = 0;
//int cursorY = 0;

MTmouse mouse;

MTmouse realMouse;

int varX = 0;//Temp vars controlled with d-pad
int varY = 0;

//Window list:
MTelementsList MTapiMainWindowList;
MTelementsList MTapiMainWindowsBarList;

//Download queue:
MTelementsList MTdownloadQueue;

//Extract queue:
MTelementsList MTextractQueue;

//Keyboard queue:
MTelementsList MTkeyboardQueue;

MTdrawFX MTgeneralDrawFX;

//Controls
MTcontrols MTpressedControls;
MTcontrols MTrealPressedControls;
MTcontrols MTtapControls;
MTcontrols MThasTapControls;
//MTcontrols MTemptyControls;

int MTapiMaxIconID = 213;
MTimage* MTapiIconList[214];

char MTrootDir[255];

char MTwindowCloseButtonPath[512];
char MTwindowRestoreButtonPath[512];
char MTwindowMinimizeButtonPath[512];
char MTwindowMaximizeButtonPath[512];

MTimage* MTwindowCloseButton;
MTimage* MTwindowRestoreButton;
MTimage* MTwindowMinimizeButton;
MTimage* MTwindowMaximizeButton;

int MToverlayAlphaValue = 0;
int MToverlayAlphaSpeed = 2;
int MToverlayAlphaMaxValue = 0;
MTcolor MToverlayColor = RGB(51,68,85);
bool MTthereIsOverlay = 0;
void (*MToverlayDrawFunction)() = NULL;

char MTscreenshotPath[200];
MTimage* MTscreenshotImage;


MTcolor MTapiWindowFontColor = RGBA(255,255,255,255);
int MTapiWindowFontType = 4;
int MTapiWindowFontMargin = 3;

MTcolor MTapiWindowShadowColor = RGBA(0,0,0,100);
int MTapiWindowShadowSize = 20;

int MTapiWindowBarRadius = 5;

MTcolor MTapiWindowBarOutColor = RGBA(150,200,250,255);
MTcolor MTapiWindowBarInColor = RGBA(100,150,200,255);

int MTapiWindowsBarThick = 18;

bool MTapiDrawMouseOverArea = 0;

MTclipArea MTcurrentScreenClip;

int MTsceneryAnimationNum = -1;

bool isDrawing = 0;

bool usbConnectionIsActive = 0;

MTimage* mapamundi;
