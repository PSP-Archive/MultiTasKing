int message(char* text) {
	
	MTlockScreen();
	
	if(text == NULL) asprintf(&text,"That was a NULL string...");
	
	MTresetScreenClip();
	oslInitMessageDialog(text, 1);
	int end = 0;
	int skip1 = 0;
	int res = -1;
	while(1){
		MTstartDrawing();
		MTclearScreen (RGB(255,255,255));
		if (!skip1){
			oslDrawDialog();
			if (oslGetDialogStatus() == PSP_UTILITY_DIALOG_NONE){
			
                    if (oslDialogGetResult() == OSL_DIALOG_CANCEL)
                        res = -1;
                    else {
                        int button = oslGetDialogButtonPressed();
                        if (button == PSP_UTILITY_MSGDIALOG_RESULT_YES)
                            res = 1;
                        else if (button == PSP_UTILITY_MSGDIALOG_RESULT_NO)
                            res = 0;
                    }
                    
		oslEndDialog();
		end = 1;
			}
		oslEndDrawing();
		}
		oslEndFrame();
		skip1 = oslSyncFrame();
		if(end == 1) {
			MTunLockScreen();
			return res;
		}
		MTdelay(10000);
	}
	
	MTunLockScreen();
	return res;
}


void MTapiSetWallpaperImage(char* path) {
	
	/*int len = MTstrlen(MTrootDir);
	int len2 = MTstrlen(path);
	if(len2 > len) {
		strcpy(MTmainWalpaperImagePath,path + len - 1);
		MTmainWalpaperImagePath[0] = '.';
		//message("1");
	} else strcpy(MTmainWalpaperImagePath,path);*/
	
	//message(MTmainWalpaperImagePath);
	
	MTimage* newImage = MTloadImageFile(path);
	if(newImage != NULL) {
		if(MTwallpaperImage != NULL) MTfreeImage(MTwallpaperImage);
		MTwallpaperImage = newImage;
		MTwallpaperImage->stretchX = 480;
		MTwallpaperImage->stretchY = 272;
		
		//MTmainWalpaperImagePath MTrootDir
		//MTiniSetString("./Data/Configuration/General.ini", "Wallpaper","MTmainWalpaperImagePath", MTmainWalpaperImagePath);
	}
	return;
}

void MTapiSetNewWallpaperImage(char* path) {
	
	int len = MTstrlen(MTrootDir);
	int len2 = MTstrlen(path);
	if(len2 > len) {
		strcpy(MTmainWalpaperImagePath,path + len - 1);
		MTmainWalpaperImagePath[0] = '.';
		//message("1");
	} else strcpy(MTmainWalpaperImagePath,path);
	
	//message(MTmainWalpaperImagePath);
	
	MTimage* newImage = MTloadImageFile(MTmainWalpaperImagePath);
	if(newImage != NULL) {
		if(MTwallpaperImage != NULL) MTfreeImage(MTwallpaperImage);
		MTwallpaperImage = newImage;
		MTwallpaperImage->stretchX = 480;
		MTwallpaperImage->stretchY = 272;
		
		//MTmainWalpaperImagePath MTrootDir
		MTiniSetString("./Data/Configuration/General.ini", "Wallpaper","MTmainWalpaperImagePath", MTmainWalpaperImagePath);
	}
	return;
}

/*void MTapiSetCursorImage(char* path,int centerX, int centerY) {
	MTimage* newImage = MTloadImageFile(path);
	if(newImage != NULL) {
		if(MTmainCursorImage != NULL) MTfreeImage(MTmainCursorImage);
		MTmainCursorImage = newImage;
		MTmainCursorImage->centerX = centerX;
		MTmainCursorImage->centerY = centerY;
	}
	return;
}*/

//int MTapiMaxCursorID = 15;
//MTcursor* MTapiCursorList[16];

/*typedef struct {
	MTimage* img;
	MTanimation* anim;
	bool animated;
	MTvector origin;
} MTcursor;*/

MTcursor* MTapiGetCursor(int id) {
	if(id < 0 || id > MTapiMaxCursorID) return NULL;
	if(MTapiCursorList[id] == NULL) {//We load the icon
		
		MTapiCursorList[id] = (MTcursor*)malloc(sizeof(MTcursor));
		
		if(MTapiCursorList[id] == NULL) return NULL;
		
		MTapiCursorList[id]->img = NULL;
		MTapiCursorList[id]->anim = NULL;
		MTapiCursorList[id]->animated = 0;
		MTapiCursorList[id]->origin.x = 0;
		MTapiCursorList[id]->origin.y = 0;
		
		char buff[255];
		sprintf(buff,"./Data/Cursors/%d.png",id);
		
		MTanimation* anim = NULL;
		MTimage* img = NULL;
		
		if(MTdoesFileExists(buff) != 1) {
			int frame = 0;
			sprintf(buff,"./Data/Cursors/%d_%d.png",id,frame);
			
			if(MTdoesFileExists(buff) == 1) {
				anim = MTcreateAnimation(5, 1);
				
				while(MTdoesFileExists(buff) == 1) {
					
					MTloadAddFrameToAnimation(anim, buff);
					
					frame++;
					sprintf(buff,"./Data/Cursors/%d_%d.png",id,frame);
				}
				MTapiCursorList[id]->animated = 1;
			}
		} else {
			img = MTloadImageFile(buff);
		}
		
		if(id == 0) {
			MTapiCursorList[id]->origin.x = 11;
			MTapiCursorList[id]->origin.y = 8;
		} else if(id == 1) {
			MTapiCursorList[id]->origin.x = 8;
			MTapiCursorList[id]->origin.y = 7;
		} else if(id == 2) {
			MTapiCursorList[id]->origin.x = 15;
			MTapiCursorList[id]->origin.y = 15;
		} else if(id == 3) {
			MTapiCursorList[id]->origin.x = 12;
			MTapiCursorList[id]->origin.y = 9;
		} else if(id == 4) {
			MTapiCursorList[id]->origin.x = 15;
			MTapiCursorList[id]->origin.y = 13;
		} else if(id == 5) {
			MTapiCursorList[id]->origin.x = 16;
			MTapiCursorList[id]->origin.y = 17;
		} else if(id == 6) {
			MTapiCursorList[id]->origin.x = 16;
			MTapiCursorList[id]->origin.y = 16;
		} else if(id == 7) {
			MTapiCursorList[id]->origin.x = 16;
			MTapiCursorList[id]->origin.y = 16;
		} else if(id == 8) {
			MTapiCursorList[id]->origin.x = 16;
			MTapiCursorList[id]->origin.y = 16;
		} else if(id == 9) {
			MTapiCursorList[id]->origin.x = 16;
			MTapiCursorList[id]->origin.y = 16;
		} else if(id == 10) {
			MTapiCursorList[id]->origin.x = 11;
			MTapiCursorList[id]->origin.y = 25;
		} else if(id == 11) {
			MTapiCursorList[id]->origin.x = 15;
			MTapiCursorList[id]->origin.y = 16;
		} else if(id == 12) {
			MTapiCursorList[id]->origin.x = 20;
			MTapiCursorList[id]->origin.y = 8;
		} else if(id == 13) {
			MTapiCursorList[id]->origin.x = 16;
			MTapiCursorList[id]->origin.y = 24;
		} else if(id == 14) {
			MTapiCursorList[id]->origin.x = 15;
			MTapiCursorList[id]->origin.y = 15;
		}/* else {
			MTapiCursorList[id]->origin.x = 0;
			MTapiCursorList[id]->origin.y = 0;
		}*/
		
		MTapiCursorList[id]->img = img;
		MTapiCursorList[id]->anim = anim;
	}
	
	return MTapiCursorList[id];
}

void MTapiDrawCursor(MTcursor* cursor, int x, int y) {
	if(cursor == NULL) return;
	if(cursor->animated == 0) {
		if(cursor->img != NULL) {
			cursor->img->x = x-cursor->origin.x;
			cursor->img->y = y-cursor->origin.y;
			//cursor->img->centerX = cursor->origin.x;
			//cursor->img->centerY = cursor->origin.y;
			MTdrawImage(cursor->img);
		}
	} else {
		if(cursor->anim != NULL) {
			cursor->anim->x = x-cursor->origin.x;
			cursor->anim->y = y-cursor->origin.y;
			//cursor->anim->centerX = cursor->origin.x;
			//cursor->anim->centerY = cursor->origin.y;
			MTdrawAnimation(cursor->anim);
		}
	}
	return;
}

void MTapiGetDrawCursor(int id, int x, int y) {
	MTapiDrawCursor(MTapiGetCursor(id), x, y);
	return;
}

void MTapiSetCursorType(int id) {
	MTcurrentCursorType = id;
	return;
}

float MTapiSmoothAnimateFloat(float value, float endValue, int frame, int endFrame) {
	return (MTpercent((endValue - value), frame, endFrame) + value);
}

int MTapiSmoothAnimateInt(int value, int endValue, int frame, int endFrame) {
	return (MTround(MTpercent((endValue - value), frame, endFrame)) + value);
}

MTvector MTapiSmoothAnimateVector(MTvector value, MTvector endValue, int frame, int endFrame) {
	MTvector newVector;
	newVector.x = MTapiSmoothAnimateInt(value.x, endValue.x, frame, endFrame);
	newVector.y = MTapiSmoothAnimateInt(value.y, endValue.y, frame, endFrame);
	return newVector;
}

MTimage* MTapiGetIcon(int id) {
	if(id < 0 || id > MTapiMaxIconID) return NULL;
	if(MTapiIconList[id] == NULL) {//We load the icon
		char buff[255];
		sprintf(buff,"./Data/Icons/%d.png",id);
		MTapiIconList[id] = MTloadImageFile(buff);
	}
	
	return MTapiIconList[id];
}

void MTapiDrawIcon(int id, int posx, int posy, int size) {
	MTimage* icon = MTapiGetIcon(id);
	if(icon != NULL) {
		icon->x = posx;
		icon->y = posy;
		icon->stretchX = size;
		icon->stretchY = size;
		MTdrawImage(icon);
		/*char buff[255];
		sprintf(buff,"%d",id);
		MTdrawString(posx,posy,buff);*/
	}
	
	return;
}

/*
FONT TYPES
Latin Sans-Serif:
0: regular
2: italic
4: bold
6: both

Latin Sans-Serif small:
8: regular
10: italic
12: bold
14: both

Latin With Serif:
1: regular
3: italic
5: bold
7: both

Latin With Serif small:
9: regular
11: italic
13: bold
15: both*/

/*int MTapiMaxFontID = 15;
MTfont* MTapiFontList[16];*/

MTfont* MTapiGetFont(int id);

MTfont* MTapiGetFont(int id) {
	if(id < 0 || id > MTapiMaxFontID) {
		if(id != 0) return MTapiGetFont(0);//In case of problem, return the default
		return NULL;
	}
	
	if(MTapiFontList[id] == NULL) {//We load the icon
		char buff[255];
		sprintf(buff,"flash0:/font/ltn%d.pgf",id);
		MTapiFontList[id] = MTloadFontFile(buff);
		
		int linebreak = MTgetFontLineBreakSize(MTapiFontList[id]);
		if(linebreak > 0) MTsetFontLineBreakSize(MTapiFontList[id], linebreak);
	}
	if(MTapiFontList[id] == NULL && id != 0) return MTapiGetFont(0);//In case of problem, return the default
	
	return MTapiFontList[id];
}

int MTapiGetFontTextWidth(int id, int height, const char *str) {
	MTfont* font = MTapiGetFont(id);
	MTsetFontHeight(font, height);
	return MTgetFontTextWidth(font, str);
}

void MTapiSetFontHeight(int id, int height) {
	MTfont* font = MTapiGetFont(id);
	MTsetFontHeight(font, height);
	return;
}

void MTapiSetFontStyle(int id, int height, MTcolor color, MTcolor shadowColor, int prop) {
	MTfont* font = MTapiGetFont(id);
	MTsetFontStyle(font, 1.0f, color, shadowColor, prop);
	MTsetFontHeight(font, height);
	return;
}

void MTapiSetFontAndStyle(int id, int height, MTcolor color, MTcolor shadowColor, int prop) {
	MTfont* font = MTapiGetFont(id);
	MTsetFontStyle(font, 1.0f, color, shadowColor, prop);
	MTsetFontHeight(font, height);
	MTsetFont(font);
	return;
}

void MTapiSetFontLineBreak(int id, int height) {
	MTsetFontLineBreakSize(MTapiGetFont(id), height);
	return;
}

int MTapiGetFontLineBreak(int id) {
	return MTgetFontLineBreakSize(MTapiGetFont(id));
}

/*void MTapiResetFontLineBreak(int id) {
	MTfont* font = MTapiGetFont(id);
	if(font == NULL || font->intra == NULL) return;
	font->intra->advancey = 0;
	return;
}*/

void MTapiSetFont(int id) {
	MTsetFont(MTapiGetFont(id));
	return;
}

void MTapiResetDefaultFont() {
	MTapiSetFontAndStyle(0, MTdefaultFontHeight, MTdefaultFontColor, 0, INTRAFONT_ALIGN_LEFT);
	return;
}

void MTapiCopyMouse(MTmouse* dest, MTmouse* orig) {
	if(dest == NULL || orig == NULL) return;
	dest->x = orig->x;
	dest->y = orig->y;
	dest->drag.x = orig->drag.x;
	dest->drag.y = orig->drag.y;
	dest->dragging = orig->dragging;
	dest->tap = orig->tap;
	return;
}

void MTapiCopyMouseWithOffset(MTmouse* dest, MTmouse* orig, int x, int y) {
	if(dest == NULL || orig == NULL) return;
	dest->x = orig->x - x;
	dest->y = orig->y - y;
	dest->drag.x = orig->drag.x - x;
	dest->drag.y = orig->drag.y - y;
	dest->dragging = orig->dragging;
	dest->tap = orig->tap;
	return;
}

/*typedef struct {
	MTvector pos;//actual
	MTvector pos0;//original-default-before it was fullscreen
	MTvector newPos;//actual
	MTvector size;//actual
	MTvector size0;//original-default
	MTvector newSize;//original-default
	MTvector minimSize;//minimum size of the window
	
	char animPercent;//when set to 100 it will begin a countdown animating the size and pos of the window, from pos to newPos and from size to newSize
	
	bool minimized;
	
	void (*draw);//function that will draw the contents of the window each frame
	
	bool hasNoBar;//with this on the window wont have any button or graphic, it will only draw the contents (back color included)
	//hasNoBar = 1 will ignore "allowClose"... etc
	
	bool allowClose;
	bool allowMinimize;
	bool allowResize;
	
	bool maximized;
	int barThick;
	
	MTimage* icon;//icon to be displayed on the left. it will be scaled to fit the bar width, and scaled to a square of that size
	
	MTcolor backColor;//background
	
	bool shadow;
	bool bar;
	
	char* name;
	
	bool on;
	bool exists;
} MTapiWindow;*/

MTapiWindow* MTapiCreateWindow(char* winName, int sizeX, int sizeY, void* winDraw, MTcolor winBackColor) {
	//MTapiAddWindow(MTapiWindow* window)
	MTapiWindow* window = (MTapiWindow*)malloc(sizeof(MTapiWindow));
	
	window->barThick = 18;//default
	window->nameVertAlign = 2;//default
	
	if(sizeX < 0) {
		if(sizeX == -1) {
			if(winName != NULL) {
				window->size.x = MTapiGetFontTextWidth(MTapiWindowFontType, window->barThick-MTapiWindowFontMargin*2, winName) + window->barThick*6;
			} else window->size.x = window->barThick*6;
		} else {
			window->size.x = window->barThick*6;
		}
	} else {
		window->size.x = sizeX;
	}
	
	if(sizeY < 0) {
		window->size.y = 0;
	} else {
		window->size.y = sizeY;
	}
	
	window->minimSize.x = 120;//window->size.x;
	window->minimSize.y = 40;//window->size.y;
	
			if(window->size.y < window->minimSize.y) {
				window->size.y = window->minimSize.y;
			}
			if(window->size.x < window->minimSize.x) {
				window->size.x = window->minimSize.x;
			}
	
	window->pos.x = screenWidth/2 - window->size.x/2;
	window->pos.y = screenHeight/2 - window->size.y/2;
	window->pos0.x = window->pos.x;
	window->pos0.y = window->pos.y;
	
	window->size0.x = window->size.x;
	window->size0.y = window->size.y;
	
	window->minimized = 0;
	window->maximized = 0;
	window->wasMaximized = 0;
	
	window->active = 0;
	
	window->data = NULL;
	
	window->freeData = NULL;
	
	window->autoFree = 0;
	
	window->autoFreeIcon = 0;
	
	window->function = winDraw;
	
	window->visible = 1;
	
	window->hasNoBar = 0;
	window->allowClose = 1;
	window->allowMinimize = 1;
	window->allowMaximize = 1;
	window->allowResize = 1;
	
	window->icon = NULL;
	
	window->backColor = winBackColor;
	
	window->shadow = 1;
	
	window->name = winName;
	
	if(winName != NULL) {
		window->name = (char*) malloc ((MTstrlen(winName) + 1 ) * sizeof(char));
		strcpy(window->name,winName);
	} else window->name = NULL;
	
	window->cursorType = 0;
	
	window->dragging = 0;
	window->wasDragging = 0;
	window->dragPos.x = 0;
	window->dragPos.y = 0;
	
	window->mouse.x = 0;
	window->mouse.y = 0;
	
	window->on = 1;
	window->exists = 1;
	return window;
}

void MTapiWindowCheckMaximize(MTapiWindow* window) {
	if(window == NULL || window->exists == 0) return;
	if(window->maximized == 1) {
		if(window->wasMaximized == 0) {
			window->wasMaximized = 1;
			window->pos0.x = window->pos.x;
			window->pos0.y = window->pos.y;
			window->size0.x = window->size.x;
			window->size0.y = window->size.y;
			
			window->pos.x = 0;
			window->pos.y = window->barThick;
			window->size.x = screenWidth;
			window->size.y = screenHeight - window->barThick - MTapiWindowsBarThick;
		}
	} else if(window->wasMaximized == 1) {
		window->wasMaximized = 0;
		window->pos.x = window->pos0.x;
		window->pos.y = window->pos0.y;
		window->size.x = window->size0.x;
		window->size.y = window->size0.y;
	}
	return;
}

/*if(MTisOverArea(mouse.x, mouse.y, window->pos.x, window->pos.y-window->barThick, window->size.x, window->barThick)) {
	if(window->dragging == 1) {
		MTapiSetCursorType(4);
	} else MTapiSetCursorType(0);
} else if(MTisOverArea(mouse.x, mouse.y, window->pos.x+window->size.x, window->pos.y-window->barThick, MTapiWindowBarRadius, window->size.y+window->barThick) || MTisOverArea(mouse.x, mouse.y, window->pos.x-MTapiWindowBarRadius, window->pos.y-window->barThick, MTapiWindowBarRadius, window->size.y+window->barThick)) {
	MTapiSetCursorType(6);//horizontal
} else if(MTisOverArea(mouse.x, mouse.y, window->pos.x, window->pos.y+window->size.y, window->size.x, MTapiWindowBarRadius) || MTisOverArea(mouse.x, mouse.y, window->pos.x, window->pos.y-window->barThick - MTapiWindowBarRadius, window->size.x, MTapiWindowBarRadius)) {
	MTapiSetCursorType(7);//vertical
} else if(MTisOverArea(mouse.x, mouse.y, window->pos.x-MTapiWindowBarRadius, window->pos.y+window->size.y, MTapiWindowBarRadius, MTapiWindowBarRadius) || MTisOverArea(mouse.x, mouse.y, window->pos.x+window->size.x, window->pos.y-window->barThick-MTapiWindowBarRadius, MTapiWindowBarRadius, MTapiWindowBarRadius)) {
	MTapiSetCursorType(8);//diagonal right
} else if(MTisOverArea(mouse.x, mouse.y, window->pos.x-MTapiWindowBarRadius, window->pos.y-MTapiWindowBarRadius-window->barThick, MTapiWindowBarRadius, MTapiWindowBarRadius) || MTisOverArea(mouse.x, mouse.y, window->pos.x+window->size.x, window->pos.y+window->size.y, MTapiWindowBarRadius, MTapiWindowBarRadius)) {
	MTapiSetCursorType(9);//diagonal left
}*/

/*
window->dragging
0: no
1: dragging bar (or whole window with R + X)
2: horiz left
3: horiz right
4: vertical up
5: vertical down
6: top left resize
7: top right resize
8: down left resize
9: down right resize
*/

bool MTapiCursorIsOverWindowsBar() {
	return MTisOverArea(mouse.x, mouse.y, 0, screenHeight-MTapiWindowsBarThick, screenWidth, MTapiWindowsBarThick);
}

bool MTapiCursorIsOverWindow(MTapiWindow* window) {
	if(window == NULL || window->exists == 0 || window->visible == 0) return 0;
	//if(MTapiCursorIsOverWindowsBar() == 1) return 0;
	return MTisOverArea(mouse.x, mouse.y, window->pos.x-MTapiWindowBarRadius, window->pos.y-MTapiWindowBarRadius-window->barThick, window->size.x + 2*MTapiWindowBarRadius, window->size.y + 2*MTapiWindowBarRadius + window->barThick);
}

void MTapiDragWindow(MTapiWindow* window) {
	if(window == NULL || window->exists == 0) return;
	if(window->dragging != 0 && MTpressedControls.cross && window->active == 1 && window->maximized == 0) {
		int MTapiWindowBarRadiusHalf = MTapiWindowBarRadius/2;
		if(window->dragging == 1) {
			/*if(window->maximized == 1) {
				window->wasDragging = 1;
				window->maximized = 0;
				MTapiWindowCheckMaximize(window);
				window->dragPos.x = window->size.x/2;
				window->dragPos.y = -window->barThick/2;
			}*/
			if(window->wasDragging == 0) {
				window->wasDragging = 1;
				window->dragPos.x = mouse.x - window->pos.x;
				window->dragPos.y = mouse.y - window->pos.y;
			}
			window->pos.x = mouse.x - window->dragPos.x;
			window->pos.y = mouse.y - window->dragPos.y;
		} else if(window->dragging == 2) {
			if(window->wasDragging == 0) {
				window->wasDragging = 1;
				window->dragPos.x = window->pos.x + window->size.x;
			}
			window->pos.x = mouse.x + MTapiWindowBarRadiusHalf;
			window->size.x = window->dragPos.x - window->pos.x;
			if(window->size.x < window->minimSize.x) {
				window->size.x = window->minimSize.x;
				window->pos.x = window->dragPos.x - window->size.x;
			}
		} else if(window->dragging == 3) {
			if(window->wasDragging == 0) {
				window->wasDragging = 1;
			}
			window->size.x = mouse.x - window->pos.x - MTapiWindowBarRadiusHalf;
			if(window->size.x < window->minimSize.x) window->size.x = window->minimSize.x;
		} else if(window->dragging == 4) {
			if(window->wasDragging == 0) {
				window->wasDragging = 1;
				window->dragPos.y = window->pos.y + window->size.y;
			}
			window->pos.y = mouse.y + window->barThick + MTapiWindowBarRadiusHalf;
			window->size.y = window->dragPos.y - window->pos.y;
			if(window->size.y < window->minimSize.y) {
				window->size.y = window->minimSize.y;
				window->pos.y = window->dragPos.y - window->size.y;
			}
		} else if(window->dragging == 5) {
			if(window->wasDragging == 0) window->wasDragging = 1;
			window->size.y = mouse.y - window->pos.y - MTapiWindowBarRadiusHalf;
			if(window->size.y < window->minimSize.y) {
				window->size.y = window->minimSize.y;
			}
		} else if(window->dragging == 6) {
			if(window->wasDragging == 0) {
				window->wasDragging = 1;
				window->dragPos.x = window->pos.x + window->size.x;
				window->dragPos.y = window->pos.y + window->size.y;
			}
			window->pos.y = mouse.y + window->barThick + MTapiWindowBarRadiusHalf;
			window->pos.x = mouse.x + MTapiWindowBarRadiusHalf;
			window->size.y = window->dragPos.y - window->pos.y;
			window->size.x = window->dragPos.x - window->pos.x;
			if(window->size.y < window->minimSize.y) {
				window->size.y = window->minimSize.y;
				window->pos.y = window->dragPos.y - window->size.y;
			}
			if(window->size.x < window->minimSize.x) {
				window->size.x = window->minimSize.x;
				window->pos.x = window->dragPos.x - window->size.x;
			}
		} else if(window->dragging == 7) {
			if(window->wasDragging == 0) {
				window->wasDragging = 1;
				window->dragPos.y = window->pos.y + window->size.y;
			}
			window->pos.y = mouse.y + window->barThick + MTapiWindowBarRadiusHalf;
			window->size.y = window->dragPos.y - window->pos.y;
			window->size.x = mouse.x - window->pos.x - MTapiWindowBarRadiusHalf;
			if(window->size.y < window->minimSize.y) {
				window->size.y = window->minimSize.y;
				window->pos.y = window->dragPos.y - window->size.y;
			}
			if(window->size.x < window->minimSize.x) window->size.x = window->minimSize.x;
		} else if(window->dragging == 8) {
			if(window->wasDragging == 0) {
				window->wasDragging = 1;
				window->dragPos.x = window->pos.x + window->size.x;
			}
			window->pos.x = mouse.x + MTapiWindowBarRadiusHalf;
			window->size.x = window->dragPos.x - window->pos.x;
			window->size.y = mouse.y - window->pos.y - MTapiWindowBarRadiusHalf;
			if(window->size.x < window->minimSize.x) {
				window->size.x = window->minimSize.x;
				window->pos.x = window->dragPos.x - window->size.x;
			}
			if(window->size.y < window->minimSize.y) window->size.y = window->minimSize.y;
		} else if(window->dragging == 9) {
			window->size.x = mouse.x - window->pos.x - MTapiWindowBarRadiusHalf;
			window->size.y = mouse.y - window->pos.y - MTapiWindowBarRadiusHalf;
			if(window->size.x < window->minimSize.x) window->size.x = window->minimSize.x;
			if(window->size.y < window->minimSize.y) window->size.y = window->minimSize.y;
		}
	} else {
		window->dragging = 0;
		window->wasDragging = 0;
	}
	return;
}

void MTapiMoveWindowLast(MTapiWindow* window) {
	if(window == NULL) return;
	 //Comprobamos si la ventana ya esta primera
	 if(MTgetElementFromList(MTlastPositionInList(MTapiMainWindowList),MTapiMainWindowList) != window) {
			//Movemos la ventana de su posicion actual al primer puesto
			//while(printingWindows == 1) delay(100);//"semaforo"
			MTpositionInList WindowPositionInList = MTelementPositionInList(window, MTapiMainWindowList);
			MTeraseElementFromList(&WindowPositionInList,MTapiMainWindowList);
			MTinsertElementInList(window, MTlastPositionInList(MTapiMainWindowList), MTapiMainWindowList);
			
	 }
	 if(window->active == 1) window->active = 0;
}

void MTapiFreeWindow(MTapiWindow* window) {
	if(window == NULL) return;
	if(window->name != NULL) free(window->name);
	if(window->freeData != NULL && window->data != NULL) window->freeData(window->data);
	if(window->autoFreeIcon == 1 && window->icon != NULL) MTfreeImage(window->icon);
	window = NULL;
	return;
}

void MTapiRemoveWindow(MTapiWindow* window) {
	if(window == NULL) return;
			MTpositionInList WindowPositionInList = MTelementPositionInList(window, MTapiMainWindowList);
			MTeraseElementFromList(&WindowPositionInList,MTapiMainWindowList);
			
			MTpositionInList WindowPositionInList2 = MTelementPositionInList(window, MTapiMainWindowsBarList);
			MTeraseElementFromList(&WindowPositionInList2,MTapiMainWindowsBarList);
	if(window->exists == 1) window->exists = 0;
	
	if(window->autoFree == 1) MTapiFreeWindow(window);
	
	return;
}

void MTapiDrawWindow(MTapiWindow* window) {
	if(window == NULL || window->exists == 0) return;
	
	bool MTcursorIsOverWindowBarAreaNow = 0;
	bool windowNextMaximizedState = window->maximized;
	
	int winPosX = 0;
	int winPosY = 0;
	int posX = 0;
	int posY = 0;
	
	if(window->visible == 1) {
	
	MTapiWindowCheckMaximize(window);

	winPosX = window->pos.x;//varX;
	winPosY = window->pos.y;//-varY;

	posX = winPosX + window->size.x;
	posY = winPosY + window->size.y;

	//int margin = 3;

	/*MTcolor MTapiWindowFontColor = RGBA(255,255,255,255);
	int MTapiWindowFontType = 4;
	int MTapiWindowFontMargin = 3;

	MTcolor MTapiWindowShadowColor = RGBA(0,0,0,100);
	int MTapiWindowShadowSize = 20;

	int MTapiWindowBarRadius = 8;

	MTcolor MTapiWindowBarOutColor = RGBA(150,200,250,255);
	MTcolor MTapiWindowBarInColor = RGBA(100,150,200,255);*/

	int barThick = window->barThick;//MTgetTextHeight()+margin*2;
	//window->barThick = barThick;

	////MTsetTextHeight(barThick-MTapiWindowFontMargin*2);

	//float winNameSize = MTgetFontSize();//1.0f*varX*0.1f;
	//int shadowRadius = 20;
	//int barRadius = 8;
	int winNameVertAlign = window->nameVertAlign;//-1;
	
	MTcursorIsOverWindowBarAreaNow = 0;
	if(window->dragging == 0 && window->mouseIsOverWholeWindow == 1 && MTisOverArea(mouse.x, mouse.y, window->pos.x, window->pos.y-window->barThick, window->size.x, window->barThick)) {
	MTcursorIsOverWindowBarAreaNow = 1;
	}
	
	windowNextMaximizedState = window->maximized;
	
	/*window->mouseIsOver = 0;
	if(window->mouseIsOverWholeWindow == 1 && MTcursorIsOverWindowBarAreaNow == 0 && window->dragging == 0) {
		if(MTisOverArea(mouse.x, mouse.y, window->pos.x, window->pos.y, window->size.x, window->size.y)) {
			MTcopyControls(&window->controls,&MTpressedControls);
			MTcopyControls(&window->tapControls,&MTtapControls);
			MTapiCopyMouseWithOffset(&window->mouse, &mouse, window->pos.x, window->pos.y);
			window->mouseIsOver = 1;
		}
	} else {
		if(window->mouseIsOverWholeWindow == 0) {
			window->mouse.tap = 0;
			window->mouse.dragging = 0;
		}
		MTemptyControls(&window->controls);
		MTemptyControls(&window->tapControls);
	}*/
	
	if(window->hasNoBar == 0) {
	
		//MTsetFontStyle(4, winNameSize, RGBA(255,255,255,255), 0, INTRAFONT_ALIGN_LEFT);
		
		MTapiSetFontAndStyle(MTapiWindowFontType, barThick-MTapiWindowFontMargin*2, MTapiWindowFontColor, 0, INTRAFONT_ALIGN_LEFT);
		
		MTcolor MTapiWindowBarInColorTWO = MTapiWindowBarInColor;
		MTcolor MTapiWindowBarOutColorTWO = MTapiWindowBarOutColor;
		int MTapiWindowShadowSizeTWO = MTapiWindowShadowSize;
		
		if(window->active == 1) {
			MTapiWindowBarInColorTWO = MTchangeColorBright(MTapiWindowBarInColor,40);
			MTapiWindowBarOutColorTWO = MTchangeColorBright(MTapiWindowBarOutColor,40);
			MTapiWindowShadowSizeTWO = 2*MTapiWindowShadowSize;
		}
		
		if(window->shadow == 1) MTdrawSmoothRoundRectangle(winPosX-MTapiWindowShadowSizeTWO, winPosY-MTapiWindowShadowSizeTWO-barThick, posX+MTapiWindowShadowSizeTWO, posY+MTapiWindowShadowSizeTWO, MTapiWindowShadowColor, MTapiWindowShadowSizeTWO, 4);
		
		//int marg = -3;
		MTdrawFillRoundRectangleWithGradient(winPosX-MTapiWindowBarRadius, winPosY-MTapiWindowBarRadius-barThick, posX+MTapiWindowBarRadius, posY+MTapiWindowBarRadius, MTapiWindowBarOutColorTWO, MTgetZeroAlphaColor(MTapiWindowBarOutColorTWO), MTapiWindowBarRadius, 4);
		
		MTsetGlobalScreenClip(winPosX, winPosY-barThick, posX, winPosY);
		
		MTdrawFillRoundRectangleWithGradient(winPosX, winPosY-barThick, posX, winPosY+barThick-barThick, MTapiWindowBarInColorTWO, MTapiWindowBarOutColorTWO, MTapiWindowBarRadius, 4);
		
		int textLimitButtons = 0;
		if(window->allowClose == 1) {
			//Adaptamos la altura al ancho de la barra
			MTwindowCloseButton->stretchY = barThick;
		
			float proportion = ((float)MTwindowCloseButton->stretchY)/((float)MTwindowCloseButton->sizeY);
			MTwindowCloseButton->stretchX = proportion*((float)MTwindowCloseButton->sizeX);
		
			MTwindowCloseButton->x = posX-MTwindowCloseButton->stretchX-textLimitButtons;
			MTwindowCloseButton->y = winPosY+(barThick/2)-(MTwindowCloseButton->stretchY/2)-barThick;
		
			textLimitButtons += MTwindowCloseButton->stretchX;
		
			MTdrawImage(MTwindowCloseButton);
			
			if(MTcursorIsOverWindowBarAreaNow == 1 && window->dragging == 0 && window->mouseIsOverWholeWindow == 1 && MTisOverImage(mouse.x, mouse.y, MTwindowCloseButton)) {
				if(window->active == 1 && MTtapControls.cross) {
					//MTapiMoveWindowLast(window);
					//window->exists = 0;
					MTapiRemoveWindow(window);
					return;
				}
				//MTsetDrawFX(MTdrawFXtint,RGBA(128,128,128,128),0);
				MTdrawSmoothFillRoundRectangle(MTwindowCloseButton->x, MTwindowCloseButton->y, MTwindowCloseButton->x + MTwindowCloseButton->stretchX, MTwindowCloseButton->y + MTwindowCloseButton->stretchY, RGBA(255,255,255,128), 3, 4);
			}
		}
		if(window->allowMaximize == 1 && window->maximized == 0 && window->allowResize == 1) {
			//Adaptamos la altura al ancho de la barra
			MTwindowMaximizeButton->stretchY = barThick;
		
			float proportion = ((float)MTwindowMaximizeButton->stretchY)/((float)MTwindowMaximizeButton->sizeY);
			MTwindowMaximizeButton->stretchX = proportion*((float)MTwindowMaximizeButton->sizeX);
		
			MTwindowMaximizeButton->x = posX-MTwindowMaximizeButton->stretchX-textLimitButtons;
			MTwindowMaximizeButton->y = winPosY+(barThick/2)-(MTwindowMaximizeButton->stretchY/2)-barThick;
		
			textLimitButtons += MTwindowMaximizeButton->stretchX;
		
			MTdrawImage(MTwindowMaximizeButton);
			
			if(MTcursorIsOverWindowBarAreaNow == 1 && window->dragging == 0 && window->mouseIsOverWholeWindow == 1 && MTisOverImage(mouse.x, mouse.y, MTwindowMaximizeButton)) {
				if(window->active == 1 && MTtapControls.cross) {
					//MTapiMoveWindowLast(window);
					//window->maximized = 1;
					//MTapiWindowCheckMaximize(window);
					windowNextMaximizedState = 1;
				}
				//MTsetDrawFX(MTdrawFXtint,RGBA(128,128,128,128),0);
				MTdrawSmoothFillRoundRectangle(MTwindowMaximizeButton->x, MTwindowMaximizeButton->y, MTwindowMaximizeButton->x + MTwindowMaximizeButton->stretchX, MTwindowMaximizeButton->y + MTwindowMaximizeButton->stretchY, RGBA(255,255,255,128), 3, 4);
			}
		}
		if(window->maximized == 1 && window->allowResize == 1) {
			//Adaptamos la altura al ancho de la barra
			MTwindowRestoreButton->stretchY = barThick;
		
			float proportion = ((float)MTwindowRestoreButton->stretchY)/((float)MTwindowRestoreButton->sizeY);
			MTwindowRestoreButton->stretchX = proportion*((float)MTwindowRestoreButton->sizeX);
		
			MTwindowRestoreButton->x = posX-MTwindowRestoreButton->stretchX-textLimitButtons;
			MTwindowRestoreButton->y = winPosY+(barThick/2)-(MTwindowRestoreButton->stretchY/2)-barThick;
		
			textLimitButtons += MTwindowRestoreButton->stretchX;
		
			MTdrawImage(MTwindowRestoreButton);
			
			if(MTcursorIsOverWindowBarAreaNow == 1 && window->dragging == 0 && window->mouseIsOverWholeWindow == 1 && MTisOverImage(mouse.x, mouse.y, MTwindowRestoreButton)) {
				if(window->active == 1 && MTtapControls.cross) {
					//MTapiMoveWindowLast(window);
					//window->maximized = 0;
					//MTapiWindowCheckMaximize(window);
					windowNextMaximizedState = 0;
				}
				//MTsetDrawFX(MTdrawFXtint,RGBA(128,128,128,128),0);
				MTdrawSmoothFillRoundRectangle(MTwindowRestoreButton->x, MTwindowRestoreButton->y, MTwindowRestoreButton->x + MTwindowRestoreButton->stretchX, MTwindowRestoreButton->y + MTwindowRestoreButton->stretchY, RGBA(255,255,255,128), 3, 4);
			}
		}
		if(window->allowMinimize == 1) {
			//Adaptamos la altura al ancho de la barra
			MTwindowMinimizeButton->stretchY = barThick;
		
			float proportion = ((float)MTwindowMinimizeButton->stretchY)/((float)MTwindowMinimizeButton->sizeY);
			MTwindowMinimizeButton->stretchX = proportion*((float)MTwindowMinimizeButton->sizeX);
		
			MTwindowMinimizeButton->x = posX-MTwindowMinimizeButton->stretchX-textLimitButtons;
			MTwindowMinimizeButton->y = winPosY+(barThick/2)-(MTwindowMinimizeButton->stretchY/2)-barThick;
			
			textLimitButtons += MTwindowMinimizeButton->stretchX;
			
			MTdrawImage(MTwindowMinimizeButton);
			
			////CHECK THIS PART (MAKE FUNCTION THAT CHECKS AND MODIFIES THIS)
			if(MTcursorIsOverWindowBarAreaNow == 1 && window->dragging == 0 && window->mouseIsOverWholeWindow == 1 && MTisOverImage(mouse.x, mouse.y, MTwindowMinimizeButton)) {
				if(window->active == 1 && MTtapControls.cross) {
					MTapiMoveWindowLast(window);
					window->minimized = 1;
					window->visible = 0;
					return;
				}
				//MTsetDrawFX(MTdrawFXtint,RGBA(128,128,128,128),0);
				MTdrawSmoothFillRoundRectangle(MTwindowMinimizeButton->x, MTwindowMinimizeButton->y, MTwindowMinimizeButton->x + MTwindowMinimizeButton->stretchX, MTwindowMinimizeButton->y + MTwindowMinimizeButton->stretchY, RGBA(255,255,255,128), 3, 4);
			}
			
			//MTresetDrawFX();
		}
	
		int textLimitIcon = 0;
		if(window->icon != NULL) {
			//Adaptamos la altura al ancho de la barra
			window->icon->stretchY = barThick;
			window->icon->stretchX = barThick;
		
			window->icon->x = winPosX;
			window->icon->y = winPosY+(barThick/2)-(window->icon->stretchY/2)-barThick;
		
			textLimitIcon = window->icon->stretchX;
			MTdrawImage(window->icon);
			
			if(MTcursorIsOverWindowBarAreaNow == 1 && window->dragging == 0 && window->mouseIsOverWholeWindow == 1 && MTisOverImage(mouse.x, mouse.y, window->icon)) {
				if(window->active == 1 && MTtapControls.cross) {
					//MTapiMoveWindowLast(window);
					//window->minimized = 1;
				}
				//MTsetDrawFX(MTdrawFXtint,RGBA(128,128,128,128),0);
				MTdrawSmoothFillRoundRectangle(window->icon->x, window->icon->y, window->icon->x + window->icon->stretchX, window->icon->y + window->icon->stretchY, RGBA(255,255,255,128), 3, 4);
			}
		}
	
		//MTdrawString(winPosX+2,winPosY/*+(barThick/2)-(MTgetTextHeight()/2)+winNameVertAlign*/,"Ventana de prueba");
		if(window->name != NULL) {
			MTdrawStringLimited(winPosX+MTapiWindowFontMargin+textLimitIcon, winPosY+winNameVertAlign+MTapiWindowFontMargin-barThick, window->name,posX-winPosX-2*MTapiWindowFontMargin-textLimitButtons-textLimitIcon);
		}
	}
	
	}


	//if(window->function != NULL) {
		
		/*
		window->dragging
		0: no
		1: dragging bar (or whole window with R + X)
		2: horiz left resize
		3: horiz right resize
		4: vertical up resize
		5: vertical down resize
		6: top left resize
		7: top right resize
		8: down left resize
		9: down right resize
		*/
		
		if(window->active == 0 || window->maximized == 1){
			window->dragging = 0;
			window->wasDragging = 0;
		}
		
		window->mouseIsOver = 0;
		
		//if(MTapiDrawMouseOverArea) MTresetScreenClip();
		if(window->mouseIsOverWholeWindow == 1) {
		
		if(window->dragging == 0) {
			if(window->active == 1 && MTpressedControls.r && MTpressedControls.cross/* && MTapiCursorIsOverWindow(window) == 1*/) {
				window->dragging = 1;
				MTapiSetCursorType(4);//bar drag
			} else if(MTcursorIsOverWindowBarAreaNow == 1) {
				if(mouse.tap == 2 && window->active == 1) {
					//window->maximized = MTflipBool(window->maximized);
					//MTapiWindowCheckMaximize(window);
					if(windowNextMaximizedState == window->maximized) windowNextMaximizedState = MTflipBool(window->maximized);
					realMouse.tap = 0;
				} else if(MTtapControls.cross) window->dragging = 1;
				MTapiSetCursorType(0);//bar drag
			} else if(MTisOverArea(mouse.x, mouse.y, window->pos.x, window->pos.y, window->size.x, window->size.y)) {
				MTapiSetCursorType(window->cursorType);//no drag, window defined cursor
				window->mouseIsOver = 1;
			} else if(window->allowResize == 1 && MTisOverArea(mouse.x, mouse.y, window->pos.x-MTapiWindowBarRadius, window->pos.y-window->barThick, MTapiWindowBarRadius, window->size.y+window->barThick)) {
				if(MTtapControls.cross) window->dragging = 2;
				MTapiSetCursorType(6);//horizontal left resize
			} else if(window->allowResize == 1 && MTisOverArea(mouse.x, mouse.y, window->pos.x+window->size.x, window->pos.y-window->barThick, MTapiWindowBarRadius, window->size.y+window->barThick)) {
				if(MTtapControls.cross) window->dragging = 3;
				MTapiSetCursorType(6);//horizontal right resize
			} else if(window->allowResize == 1 && MTisOverArea(mouse.x, mouse.y, window->pos.x, window->pos.y-window->barThick - MTapiWindowBarRadius, window->size.x, MTapiWindowBarRadius)) {
				if(MTtapControls.cross) window->dragging = 4;
				MTapiSetCursorType(7);//vertical up resize
			} else if(window->allowResize == 1 && MTisOverArea(mouse.x, mouse.y, window->pos.x, window->pos.y+window->size.y, window->size.x, MTapiWindowBarRadius)) {
				if(MTtapControls.cross) window->dragging = 5;
				MTapiSetCursorType(7);//vertical down resize
			} else if(window->allowResize == 1 && MTisOverArea(mouse.x, mouse.y, window->pos.x-MTapiWindowBarRadius, window->pos.y-MTapiWindowBarRadius-window->barThick, MTapiWindowBarRadius, MTapiWindowBarRadius)) {
				if(MTtapControls.cross) window->dragging = 6;
				MTapiSetCursorType(9);//top left resize
			} else if(window->allowResize == 1 && MTisOverArea(mouse.x, mouse.y, window->pos.x+window->size.x, window->pos.y-window->barThick-MTapiWindowBarRadius, MTapiWindowBarRadius, MTapiWindowBarRadius)) {
				if(MTtapControls.cross) window->dragging = 7;
				MTapiSetCursorType(8);//top right resize
			} else if(window->allowResize == 1 && MTisOverArea(mouse.x, mouse.y, window->pos.x-MTapiWindowBarRadius, window->pos.y+window->size.y, MTapiWindowBarRadius, MTapiWindowBarRadius)) {
				if(MTtapControls.cross) window->dragging = 8;
				MTapiSetCursorType(8);//down left resize
			} else if(window->allowResize == 1 && MTisOverArea(mouse.x, mouse.y, window->pos.x+window->size.x, window->pos.y+window->size.y, MTapiWindowBarRadius, MTapiWindowBarRadius)) {
				if(MTtapControls.cross) window->dragging = 9;
				MTapiSetCursorType(9);//down right resize
			}
		} else if(window->dragging == 1) {
			MTapiSetCursorType(4);//Hand
		} else if(window->dragging == 2) {
			MTapiSetCursorType(6);//horizontal left
		} else if(window->dragging == 3) {
			MTapiSetCursorType(6);//horizontal right
		} else if(window->dragging == 4) {
			MTapiSetCursorType(7);//vertical left
		} else if(window->dragging == 5) {
			MTapiSetCursorType(7);//vertical right
		} else if(window->dragging == 6) {
			MTapiSetCursorType(9);//top left resize
		} else if(window->dragging == 7) {
			MTapiSetCursorType(8);//top right resize
		} else if(window->dragging == 8) {
			MTapiSetCursorType(8);//down left resize
		} else if(window->dragging == 9) {
			MTapiSetCursorType(9);//down right resize
		}
		
		MTapiDragWindow(window);
		}
		
	
	//window->mouseIsOver = 0;
	//if(window->mouseIsOverWholeWindow == 1 && MTcursorIsOverWindowBarAreaNow == 0 && window->dragging == 0) {
		if(window->mouseIsOver == 1) {
			MTcopyControls(&window->controls,&MTpressedControls);
			MTcopyControls(&window->tapControls,&MTtapControls);
			MTapiCopyMouseWithOffset(&window->mouse, &mouse, window->pos.x, window->pos.y);
			//window->mouseIsOver = 1;
		//}
	} else {
		//if(window->mouseIsOverWholeWindow == 0) {
			window->mouse.tap = 0;
			window->mouse.dragging = 0;
		//}
		MTemptyControls(&window->controls);
		MTemptyControls(&window->tapControls);
	}
	
	if(window->function != NULL) {
		if(window->visible == 1) {
			MTsetGlobalScreenClip(winPosX, winPosY, posX, posY);
			MTapiResetDefaultFont();
			MTdrawFillRectangle(winPosX, winPosY, posX, posY, window->backColor);
		} else MTsetGlobalScreenClip(0, 0, 0, 0);
		MTsetGlobalDrawingOrigin(winPosX, winPosY);
		window->function(window);
		MTsetGlobalDrawingOrigin(0, 0);
	}
	//}
	
	window->maximized = windowNextMaximizedState;
	
	MTresetScreenClip();
	
	return;
}

void MTapiMoveWindowFirst(MTapiWindow* window) {
	 if(window == NULL) return;
	 //Comprobamos si la ventana ya esta primera
	 if(MTgetElementFromList(MTfirstPositionInList(MTapiMainWindowList),MTapiMainWindowList) != window) {
			//Movemos la ventana de su posicion actual al primer puesto
			//while(printingWindows == 1) delay(100);//"semaforo"
			MTpositionInList WindowPositionInList = MTelementPositionInList(window, MTapiMainWindowList);
			MTeraseElementFromList(&WindowPositionInList,MTapiMainWindowList);
			MTinsertElementInList(window, MTfirstPositionInList(MTapiMainWindowList), MTapiMainWindowList);
			
	 }
	 if(window->active == 0) window->active = 1;
}

void MTapiAddWindow(MTapiWindow* window) {
	if(window == NULL) return;
	MTinsertElementInList (window, MTfirstPositionInList (MTapiMainWindowList), MTapiMainWindowList);
	MTinsertElementInList (window, MTlastPositionInList (MTapiMainWindowsBarList), MTapiMainWindowsBarList);
	return;
}

int MTapiWindowCount() {
	return MTnumberOfElementsInList(MTapiMainWindowList);
}

bool MTapiCursorIsOverWindowsBarNow = 0;

void MTapiCheckAllWindowsSelectionAndOrder() {
	MTapiWindow* window;
	MTpositionInList WindowPositionInList;
	
	int windowCount = MTapiWindowCount();
	if( windowCount > 0) {
		WindowPositionInList = MTfirstPositionInList (MTapiMainWindowList);
		//WindowPositionInList = MTnextPositionInList(WindowPositionInList,MTapiMainWindowList);
		window = MTgetElementFromList(WindowPositionInList, MTapiMainWindowList);
		
		MTapiCursorIsOverWindowsBarNow = MTapiCursorIsOverWindowsBar();
		
		if(window != NULL && window->exists == 1) {
			if(window->dragging != 0) {
				if(MTapiCursorIsOverWindowsBarNow) {
					window->mouseIsOverWholeWindow = 0;
				} else {
					window->mouseIsOverWholeWindow = 1;
				}
				return;
			}/* else {
				if(MTtapControls.cross) {
					if(MTapiCursorIsOverWindow(window)) {
						window->active = 1;
						return;
					} else window->active = 0;
				} 
			}*/
		}
		
		//WindowPositionInList = MTnextPositionInList(WindowPositionInList,MTapiMainWindowList);
		bool mouseOverSomeWindow = 0;
		
		while ((WindowPositionInList != MTlastPositionInList(MTapiMainWindowList))) {
			if(window != NULL && window->exists == 1) {
				
				if(mouseOverSomeWindow == 0) {
					if(MTapiCursorIsOverWindowsBarNow == 0 && MTapiCursorIsOverWindow(window)) {
						window->mouseIsOverWholeWindow = 1;
						mouseOverSomeWindow = 1;
						if(MTtapControls.cross) {
							MTapiMoveWindowFirst(window);
							return;
						}
					} else {
						window->mouseIsOverWholeWindow = 0;
						if(MTtapControls.cross) window->active = 0;
					}
				} else {
					window->mouseIsOverWholeWindow = 0;
					window->active = 0;//aqui nunca llega la 1ª
				}
				
				WindowPositionInList = MTnextPositionInList(WindowPositionInList,MTapiMainWindowList);
				
			} else break; //Si algo falla, puede que la lista de ventanas haya cambiado :-S por lo que salimos antes de que haya algun error
			window = MTgetElementFromList(WindowPositionInList, MTapiMainWindowList);
		}
		
	}
	return;
}

void MTapiDrawAllWindows() {
	
	MTapiCheckAllWindowsSelectionAndOrder();
	
	MTapiWindow* window;
	MTpositionInList WindowPositionInList;
	
	if(MTapiWindowCount() > 0) {
		MTapiWindow* firstWindow = MTgetElementFromList(MTfirstPositionInList (MTapiMainWindowList), MTapiMainWindowList);;
		/*if(firstWindow != NULL && firstWindow->exists == 1) {
			if(firstWindow->maximized == 1 && firstWindow->hasNoBar == 0) {
				//firstWindow->wasMaximized = 1;
				//firstWindow->pos.x = 0;
				//firstWindow->pos.y = firstWindow->barThick;
				//firstWindow->size.x = screenWidth;
				//firstWindow->size.y = screenHeight - firstWindow->barThick;
				MTapiDrawWindow(firstWindow);
				return;
			}
		}*/
		
		WindowPositionInList = MTlastPositionInList (MTapiMainWindowList);
		WindowPositionInList = MTpreviousPositionInList(WindowPositionInList,MTapiMainWindowList);
		window = MTgetElementFromList(WindowPositionInList, MTapiMainWindowList);
		while ((WindowPositionInList != MTfirstPositionInList(MTapiMainWindowList))) {
			if(window != NULL && window->exists == 1) {
				window->active = 0;
				MTapiDrawWindow(window);
				WindowPositionInList = MTpreviousPositionInList(WindowPositionInList,MTapiMainWindowList);
			} else break; //Si algo falla, puede que la lista de ventanas haya cambiado :-S por lo que salimos antes de que haya algun error
			window = MTgetElementFromList(WindowPositionInList, MTapiMainWindowList);
		}
		//imprimimos el primero
		if(firstWindow != NULL && firstWindow->exists == 1) {
			/*window = MTgetElementFromList(WindowPositionInList, MTapiMainWindowList);
			if(window != NULL && window->exists == 1) */
			/*if(window->dragging == 0) {
				if(MTtapControls.cross) {
					if(MTapiCursorIsOverWindow(window) == 1) {
						window->active = 1;
					} else {
						if(MTapiCursorIsOverWindowsBar() == 0) window->active = 0;
					}
				}
			}*/
			MTapiDrawWindow(firstWindow);
		}
	}
	return;
}

void MTapiMinimizeAllWindows() {
	MTapiWindow* window;
	MTpositionInList WindowPositionInList;
	
	WindowPositionInList = MTfirstPositionInList (MTapiMainWindowsBarList);
	while ((WindowPositionInList != MTlastPositionInList(MTapiMainWindowsBarList))) {
		window = MTgetElementFromList(WindowPositionInList, MTapiMainWindowsBarList);
		if(window != NULL && window->exists == 1) {
			window->minimized = 1;
			window->visible = 0;
		}
		WindowPositionInList = MTnextPositionInList(WindowPositionInList,MTapiMainWindowsBarList);
	}
	return;
}

void MTapiDrawWindowsBar() {
	
	
	//int barThick = 18;
	//MTdrawRectangleWithGradient(int x0, int y0, int x1, int y1, MTcolor c1, MTcolor c2, MTcolor c3, MTcolor c4)
	//MTdrawRectangleWithGradient(0, screenHeight-barThick, screenWidth, screenHeight, RGBA(0,0,0,100), shadowRadius, 4);

	/*int marg = 8;
	MTdrawFillRoundRectangleWithGradient(0-marg, screenHeight-barThick-marg, screenWidth+marg, screenHeight+marg, RGBA(150,200,250,255), RGBA(150,200,250,0), marg, 4);

	MTdrawFillRoundRectangleWithGradient(0, screenHeight-barThick, screenWidth, screenHeight, RGBA(100,150,200,255), RGBA(150,200,250,255), marg, 4);

	int margin = 3;
	MTapiSetFontAndStyle(4, barThick-margin*2-marg*2, RGBA(255,255,255,255), 0, INTRAFONT_ALIGN_LEFT);

	MTdrawFillRoundRectangleWithGradient(0, screenHeight-barThick, 480/4, screenHeight, RGBA(150,200,250,255), RGBA(150,200,250,0), marg, 4);

	MTdrawFillRoundRectangleWithGradient(0+marg, screenHeight-barThick+marg, 480/4 -marg, screenHeight-marg, RGBA(100,150,200,255), RGBA(150,200,250,255), marg, 4);

	MTdrawStringLimited(0+marg+margin, screenHeight-barThick+marg, "Scenery Beta 2010",480/4-2*margin-marg);*/
	
	MTsetGlobalScreenClip(0, 0, 480, 272);
	
	int marg = 8;
	MTdrawFillRoundRectangleWithGradient(0-marg, screenHeight-MTapiWindowsBarThick-marg, screenWidth+marg, screenHeight+marg, MTapiWindowBarOutColor, MTgetZeroAlphaColor(MTapiWindowBarOutColor), marg, 4);
	
	MTdrawSmoothFillRoundRectangle(0, screenHeight-MTapiWindowsBarThick, MTapiWindowsBarThick, screenHeight-MTapiWindowsBarThick + MTapiWindowsBarThick, RGBA(255,255,255,128), 3, 4);
	MTapiDrawIcon(183, 0, screenHeight-MTapiWindowsBarThick, MTapiWindowsBarThick);
	if(MTapiCursorIsOverWindowsBarNow && MTisOverArea(mouse.x, mouse.y, 0, screenHeight-MTapiWindowsBarThick, MTapiWindowsBarThick, screenHeight-MTapiWindowsBarThick + MTapiWindowsBarThick)) {
		if(MTtapControls.cross) {
			//MTapiMoveWindowLast(window);
			//window->minimized = 1;
			MTapiMinimizeAllWindows();
		}
		MTdrawSmoothFillRoundRectangle(0, screenHeight-MTapiWindowsBarThick, MTapiWindowsBarThick, screenHeight-MTapiWindowsBarThick + MTapiWindowsBarThick, RGBA(255,255,255,128), 3, 4);
	}
	
	int margin = 4;
	
	MTapiWindow* window;
	MTpositionInList WindowPositionInList;
	
	int windowCount = MTnumberOfElementsInList(MTapiMainWindowsBarList);
	if( windowCount > 0) {
		WindowPositionInList = MTfirstPositionInList (MTapiMainWindowsBarList);
		//WindowPositionInList = MTnextPositionInList(WindowPositionInList,MTapiMainWindowsBarList);
		window = MTgetElementFromList(WindowPositionInList, MTapiMainWindowsBarList);
		
		int leftMargin = MTapiWindowsBarThick;
		int rightMargin = 0;
		
		float buttonX = leftMargin;
		bool buttonWasPressed = 0;
		while ((WindowPositionInList != MTlastPositionInList(MTapiMainWindowsBarList))) {
			if(window != NULL && window->exists == 1) {
				//MTapiDrawWindow(window);
				float buttonWidth = (float)(screenWidth - leftMargin - rightMargin)/4.0f;
				if(windowCount > 4) buttonWidth = (float)(screenWidth - leftMargin - rightMargin)/(float)windowCount;
				MTsetGlobalScreenClip(0+buttonX, screenHeight-MTapiWindowsBarThick, buttonWidth+buttonX, screenHeight);
				
				MTcolor MTapiWindowBarInColorTWO = MTapiWindowBarInColor;
				MTcolor MTapiWindowBarOutColorTWO = MTapiWindowBarOutColor;
				
				if(window->active == 1) {
					MTapiWindowBarInColorTWO = MTchangeColorBright(MTapiWindowBarInColor,40);
					MTapiWindowBarOutColorTWO = MTchangeColorBright(MTapiWindowBarOutColor,40);
				}
				
				MTdrawFillRoundRectangleWithGradient(0+buttonX, screenHeight-MTapiWindowsBarThick, buttonWidth+buttonX, screenHeight, MTapiWindowBarInColorTWO, MTapiWindowBarOutColorTWO, margin, 4);
				
				int textLimitIcon = 0;
				if(window->icon != NULL) {
					//Adaptamos la altura al ancho de la barra
					window->icon->stretchY = MTapiWindowsBarThick;
					window->icon->stretchX = MTapiWindowsBarThick;
			
					window->icon->x = 0+buttonX;
					window->icon->y = screenHeight-MTapiWindowsBarThick;
			
					textLimitIcon = window->icon->stretchX;
					MTdrawImage(window->icon);
				}
				
				MTapiSetFontAndStyle(MTapiWindowFontType, MTapiWindowsBarThick-marg, MTapiWindowFontColor, 0, INTRAFONT_ALIGN_LEFT);
				
				MTdrawStringLimited(0+margin+buttonX+textLimitIcon, screenHeight-MTapiWindowsBarThick+margin/*ajuste Y*/, window->name,buttonWidth-margin-textLimitIcon);
				
				if(MTapiCursorIsOverWindowsBarNow && buttonWasPressed == 0) {
					if(MTtapControls.cross) {
						if(MTisOverArea(mouse.x, mouse.y, buttonX, screenHeight-MTapiWindowsBarThick, buttonWidth, MTapiWindowsBarThick)) {
							MTapiMoveWindowFirst(window);
							window->minimized = 0;
							window->visible = 1;
							buttonWasPressed = 1;
						}
					}
				}
								
				buttonX += buttonWidth;
				WindowPositionInList = MTnextPositionInList(WindowPositionInList,MTapiMainWindowsBarList);
				
			} else break; //Si algo falla, puede que la lista de ventanas haya cambiado :-S por lo que salimos antes de que haya algun error
			window = MTgetElementFromList(WindowPositionInList, MTapiMainWindowsBarList);
		}
		MTresetScreenClip();
	}
	
	/*if(MTapiCursorIsOverWindowsBar()) {
		MTapiSetCursorType(0);
	}*/
	
	return;
}


/*typedef struct {
	bool direction;//1 = vertical
	MTvector pos;
	MTvector size;
	
	int dragPos;
	bool dragging;
	
	int newVal;
	
	int maxVal;
	int currVal;
	int shownNum;
	
} MTapiScrollBar;*/

bool MTapiDrawScrollBar(MTapiScrollBar* bar, bool selecting, bool allowDrag) {
	if(bar == NULL) return 0;
	
	if(bar->shownNum < bar->totalNum) {
		
		//if(bar->allowDrag == 1) {
			/*if(cursor != NULL) {
				bar->mouseIsOver = MTisOverArea(cursor->x, cursor->y, bar->pos.x, bar->pos.y, bar->size.x, bar->size.y);
			} else bar->mouseIsOver = 0;*/
		//} else bar->mouseIsOver = 0;
		
		if(bar->totalNum < 1) bar->totalNum = 1;
		if(bar->maxVal < 1) bar->maxVal = 1;
		
		if(bar->currVal < 0) bar->currVal = 0;
		if(bar->currVal > bar->maxVal) bar->currVal = bar->maxVal;
		
		if(bar->shownNum < 0) bar->shownNum = 0;
		if(bar->shownNum > bar->totalNum) bar->shownNum = bar->totalNum;
		
		if(bar->direction == 1) {//Vertical
		//int barWidth = 10;
		MTdrawSmoothFillRoundRectangle(bar->pos.x, bar->pos.y, bar->pos.x + bar->size.x, bar->pos.y + bar->size.y, MTapiWindowBarInColor, 3, 4);
		
		//int maxNum = menu->maxFirstNum;
		int buttonHeight = MTpercent(bar->shownNum, bar->size.y, bar->totalNum);//(menu->size.y - yOffset)/10;//10;
		if(buttonHeight < 10) buttonHeight = 10;
		if(buttonHeight > bar->size.y) buttonHeight = bar->size.y;
		//int semiHeight = buttonHeight/2;
		int buttonPosition = MTpercent(bar->currVal, bar->size.y - buttonHeight, bar->maxVal);
		
		//if(menu->firstNum == 1) buttonPosition = 0;
		MTdrawSmoothFillRoundRectangle(bar->pos.x, bar->pos.y + buttonPosition, bar->pos.x + bar->size.x, bar->pos.y + buttonPosition + buttonHeight, MTapiWindowBarOutColor, 3, 4);
		
		
		if(bar->allowDrag && (selecting || mouse.dragging)) {
			if(allowDrag && bar->dragging == 0 && MTisOverArea(mouse.x - MTdrawingOrigin.x, mouse.y - MTdrawingOrigin.y, bar->pos.x, bar->pos.y + buttonPosition, bar->size.x, buttonHeight)) {
				bar->dragPos = bar->pos.y - (mouse.y - MTdrawingOrigin.y) + buttonPosition;
				bar->dragging = 1;
			}
			if(bar->dragging == 1) {
				int buttonNewPosition = bar->dragPos + (mouse.y - MTdrawingOrigin.y) - bar->pos.y;
				bar->newVal = MTpercent(buttonNewPosition, bar->maxVal, bar->size.y - buttonHeight);
				
				if(bar->newVal < 0) bar->newVal = 0;
				if(bar->newVal > bar->maxVal) bar->newVal = bar->maxVal;
			}
		} else bar->dragging = 0;
		
		}
		/*MTisOverArea(cursor->x, cursor->y, menu->pos.x, menu->pos.y + yOffset, menu->size.x, menu->size.y - yOffset)) {
		int dragPos;
		bool dragging;
		
		int newVal;
		}*/
		
		//char text2print[512];
		//sprintf(text2print,"menu->scrollBarV.maxVal = %d\nmenu->scrollBarV.currVal = %d\nmenu->scrollBarV.shownNum = %d\nmenu->scrollBarV.totalNum = %d\n",bar->maxVal,bar->currVal,bar->shownNum,bar->totalNum);
		
		//MTdrawString(200,50,text2print);
		return 1;
	}// else bar->mouseIsOver = 0;
	return 0;
}

void MTapiFreeTextBox(MTapiTextBox* textBox) {
	
	if(textBox == NULL) return;
	if(textBox->text != NULL) free(textBox->text);
	textBox->text = NULL;
	if(textBox->textReadyToPrint != NULL) free(textBox->textReadyToPrint);
	textBox->textReadyToPrint = NULL;
	
	//message("6");
	
	free(textBox);
	textBox = NULL;
	
	//message("7");
	return;
}

MTapiTextBox* MTapiCreateTextBox(int posX, int posY, int width, int height, char* text) {
	//message(name);
	//MTapiAddWindow(MTapiWindow* window)
	MTapiTextBox* textBox = (MTapiTextBox*)malloc(sizeof(MTapiTextBox));
	if(textBox == NULL) return NULL;
	
	if(text != NULL) {
	textBox->len = MTstrlen(text);
	if(textBox->len > 0) {
		textBox->text = (char*)malloc((textBox->len + 1) * sizeof(char));
		strcpy(textBox->text,text);
		//if(textBox->len > 3*1024) textBox->len = 3*1024;
		textBox->text[textBox->len] = '\0';
	}
	} else asprintf(&textBox->text,"Error. No text specified.");
	
	textBox->textReadyToPrint = NULL;
	
	textBox->pos.x = posX;
	textBox->pos.y = posY;
	
	textBox->size.x = width;
	textBox->size.y = height;
	
	textBox->realSize.x = 0;
	textBox->realSize.y = 0;
	
	textBox->prevSize.x = 0;
	textBox->prevSize.y = 0;
	
	textBox->font = 0;//default
	
	textBox->align = 0;
	
	textBox->textHeight = MTdefaultFontHeight;
	textBox->prevTextHeight = MTdefaultFontHeight;
	textBox->textColor = MTdefaultFontColor;
	
	textBox->textJumpSize = 1;
	textBox->prevTextJumpSize = 1;
	
	textBox->prevLen = textBox->len;
	
	textBox->lines = 1;
	
	textBox->autoLineBreak = 1;
	textBox->prevAutoLineBreak = 1;
	
	textBox->adjustSpaces = 50;
	textBox->prevAdjustSpaces = 50;
	
	textBox->firstLine = 1;
	textBox->firstLinePos = 0;
	textBox->prevFirstLine = 1;
	
	textBox->maxFirstLine = 1;
	
	textBox->cursorPos = 0;
	
	textBox->active = 0;
	
	//textBox->barIsActive = 0;
	//textBox->textReadyToPrint = name;
	//textBox->needRefresh = 0;
	

	
	return textBox;
}

void MTapiDrawTextBox(MTapiTextBox* textBox, bool selecting, MTmouse* cursor) {
	if(textBox == NULL || textBox->text == NULL) return;
	textBox->scrollBarV.size.x = 15;
	
	if(textBox->firstLine < 1) {
		textBox->firstLine = 1;
	} else if(textBox->firstLine > textBox->maxFirstLine) {
		textBox->firstLine = textBox->maxFirstLine;
	}
	
	bool dontDrawText = 1;
	
	if(textBox->firstLine != textBox->prevFirstLine || textBox->len != textBox->prevLen || textBox->size.x != textBox->prevSize.x || textBox->size.y != textBox->prevSize.y || textBox->textHeight != textBox->prevTextHeight || textBox->textJumpSize != textBox->prevTextJumpSize || textBox->autoLineBreak != textBox->prevAutoLineBreak || textBox->adjustSpaces != textBox->prevAdjustSpaces ) {
		
		int bufferSize = 1;
		if(textBox->textReadyToPrint != NULL) {
			//textBox->textReadyToPrint[0] = '\0';
			free(textBox->textReadyToPrint);
			textBox->textReadyToPrint = NULL;
		}
		
		textBox->textReadyToPrint = (char*) malloc ( bufferSize * sizeof(char) + 3);
		if(textBox->textReadyToPrint == NULL) return;
		textBox->textReadyToPrint[bufferSize-1] = '\0';
		
		if( textBox->len > 0) {
			
			//int finalHeight = 0;
			
			bool optimise = 0;
			if(textBox->textHeight == textBox->prevTextHeight && textBox->textJumpSize == textBox->prevTextJumpSize && textBox->autoLineBreak == textBox->prevAutoLineBreak && textBox->adjustSpaces == textBox->prevAdjustSpaces && textBox->len == textBox->prevLen && textBox->size.x == textBox->prevSize.x) {
				optimise = 1;
			}
			
			if(optimise == 0) textBox->realSize.y = 0;
			
			int pos = 0;//0;
			float lineWidth = 0;
			
			//textBox->lines = 1;
			
			int lines = 1;
			
			if(optimise && textBox->firstLine >= textBox->prevFirstLine) {
				pos = textBox->firstLinePos;
				lines = textBox->prevFirstLine;
			}
			
			//int currLine = textBox->prevFirstLine;
			
			
			MTapiSetFontAndStyle(textBox->font, textBox->textHeight, textBox->textColor, 0, INTRAFONT_ALIGN_LEFT);
			//int linebreaksize = MTapiGetFontLineBreak(textBox->font);
			//MTapiSetFontLineBreak(textBox->font, textBox->textJumpSize + textBox->textHeight);
			
			bool skipnext = 0;
			
			bool repeatnext = 0;
			
			bool stopCreating = 1;
			
			if(lines == textBox->firstLine) {
				textBox->firstLinePos = pos;
				stopCreating = 0;
			}
			
			int height = 0;
			
			int count = 0;
			
			bool invalid = 0;
			
			int charLen = 0;
			
			while (count < 20000 && pos < textBox->len) {
				skipnext = 0;
				repeatnext = 0;
				invalid = 1;
				charLen = 0;
					if (textBox->text[pos] <= 0x7F) { //ASCII
						char text[2];
						text[0] = textBox->text[pos];
						text[1] = '\0';
						lineWidth += MTgetTextWidth(text);
						invalid = 0;
						charLen = 1;
					}
					else if (textBox->text[pos] <= 0xC1) { //part of multi-byte or overlong encoding ->ignore
						invalid = 1;
					} else if (textBox->text[pos] <= 0xDF) { //2-byte
						char text[3];
						text[0] = textBox->text[pos];
						text[1] = textBox->text[pos+1];
						text[2] = '\0';
						lineWidth += MTgetTextWidth(text);
						invalid = 0;
						charLen = 2;
					} else if (textBox->text[pos] <= 0xEF) { //3-byte
						char text[4];
						text[0] = textBox->text[pos];
						text[1] = textBox->text[pos+1];
						text[2] = textBox->text[pos+2];
						text[3] = '\0';
						lineWidth += MTgetTextWidth(text);
						invalid = 0;
						charLen = 3;
					} else invalid = 1;//else 4-byte, restricted or invalid range ->ignore
				
				if(charLen == 1) {
				
				if(textBox->text[pos] == '\0') {
					break;
				} else if(textBox->text[pos] == '\n') {
					lineWidth = 0;
					lines++;
					
					//currLine++;
					
					if(lines == textBox->firstLine) {
						textBox->firstLinePos = pos + 1;
						stopCreating = 0;
					}
					
					if(optimise == 0) textBox->realSize.y += (textBox->textHeight + textBox->textJumpSize);
					if(stopCreating == 0) height += (textBox->textHeight + textBox->textJumpSize);
				} else if(textBox->autoLineBreak) {
					
					if(lineWidth + textBox->adjustSpaces >= textBox->size.x - textBox->scrollBarV.size.x) {
						if(lineWidth >= textBox->size.x - textBox->scrollBarV.size.x) {
								
								//skipnext = 1;
								
								if(textBox->text[pos] == ' ') {
									skipnext = 1;
									//pos++;
								} else {
									//repeatnext = 1;
								}
								
								if(stopCreating == 0) {
								if(textBox->adjustSpaces > 0 && textBox->text[pos] != ' ') {
									bufferSize += 1;
									textBox->textReadyToPrint = (char*) realloc (textBox->textReadyToPrint, bufferSize * sizeof(char) + 3);
									if(textBox->textReadyToPrint == NULL) return;
									textBox->textReadyToPrint[bufferSize -2] = '-';
									textBox->textReadyToPrint[bufferSize -1] = '\0';
									//bufferSize += 1;
								}
								bufferSize += 1;
								textBox->textReadyToPrint = (char*) realloc (textBox->textReadyToPrint, bufferSize * sizeof(char) + 3);
								if(textBox->textReadyToPrint == NULL) return;
								textBox->textReadyToPrint[bufferSize -2] = '\n';
								textBox->textReadyToPrint[bufferSize -1] = '\0';
								//bufferSize += 1;
								}
								
								lineWidth = 0;
								lines++;
							
								if(lines == textBox->firstLine) {
									textBox->firstLinePos = pos;
									if(skipnext) textBox->firstLinePos++;
									stopCreating = 0;
								}
							
								if(optimise == 0) textBox->realSize.y += (textBox->textHeight + textBox->textJumpSize);
								
								if(stopCreating == 0) height += (textBox->textHeight + textBox->textJumpSize);
								
						} else {
							if(textBox->adjustSpaces > 0) {
								if(textBox->text[pos] == ' ') {
									if(stopCreating == 0) {
									bufferSize += 1;
									textBox->textReadyToPrint = (char*) realloc (textBox->textReadyToPrint, bufferSize * sizeof(char) + 3);
									if(textBox->textReadyToPrint == NULL) return;
									textBox->textReadyToPrint[bufferSize -2] = '\n';
									textBox->textReadyToPrint[bufferSize -1] = '\0';
									//bufferSize += 1;
									}
								
									lineWidth = 0;
									lines++;
								
									if(lines == textBox->firstLine) {
										textBox->firstLinePos = pos +1;
										stopCreating = 0;
									}
								
									if(optimise == 0) textBox->realSize.y += (textBox->textHeight + textBox->textJumpSize);
									
									if(stopCreating == 0) height += (textBox->textHeight + textBox->textJumpSize);
									skipnext = 1;
								}
							}
						}
					}
				}
				
				
				} else message("invalid");
				
				if(invalid == 0 && skipnext == 0) {
					
					if(stopCreating == 0) {
					bufferSize += charLen;
					textBox->textReadyToPrint = (char*) realloc (textBox->textReadyToPrint, bufferSize * sizeof(char) + 3);
					if(textBox->textReadyToPrint == NULL) return;
					
					if(charLen == 1) {
						textBox->textReadyToPrint[bufferSize -2] = textBox->text[pos];
						textBox->textReadyToPrint[bufferSize -1] = '\0';
					} else if(charLen == 2) {
						textBox->textReadyToPrint[bufferSize -3] = textBox->text[pos];
						textBox->textReadyToPrint[bufferSize -2] = textBox->text[pos + 1];
						textBox->textReadyToPrint[bufferSize -1] = '\0';
					} else if(charLen == 3) {
						textBox->textReadyToPrint[bufferSize -4] = textBox->text[pos];
						textBox->textReadyToPrint[bufferSize -3] = textBox->text[pos + 1];
						textBox->textReadyToPrint[bufferSize -2] = textBox->text[pos + 2];
						textBox->textReadyToPrint[bufferSize -1] = '\0';
					}
					//bufferSize += 1;
					}
				}
				
				if(height > textBox->size.y + textBox->textHeight) {
					stopCreating = 1;
					if(optimise) break;
				}
				
				if(invalid) {
					pos++;
				} else pos += charLen;
				
				count++;
			}
			
			if(optimise == 0) {
				textBox->lines = lines;
			}
			
			//MTapiSetFontLineBreak(textBox->font, linebreaksize);
		}
		
		textBox->textReadyToPrint[bufferSize -1] = '\0';
		textBox->textReadyToPrint[bufferSize] = '\0';
		textBox->textReadyToPrint[bufferSize+1] = '\0';
		textBox->textReadyToPrint[bufferSize+2] = '\0';
		
		unsigned short *mybuff = (unsigned short *) malloc((bufferSize + 1)*sizeof(unsigned short));
		
		cccUTF8toUCS2(mybuff, bufferSize, (unsigned char*)textBox->textReadyToPrint);
		
		MTfree(textBox->textReadyToPrint);
		
		textBox->textReadyToPrint = NULL;
		
		int size = 0;
		textBox->textReadyToPrint = (char*)utf16le_to_utf8_alloc2(mybuff,&size);
		
		MTfree(mybuff);
		mybuff = NULL;
		
		//if(size != bufferSize) message("diff");
		
		if(textBox->textReadyToPrint == NULL || size != bufferSize) dontDrawText = 1;
		
		bufferSize = size;
		
		
		if((textBox->textHeight + textBox->textJumpSize) > 0)	{
			textBox->maxFirstLine = textBox->lines - (textBox->size.y/(textBox->textHeight + textBox->textJumpSize)) + 1;
			if(textBox->maxFirstLine < 1) textBox->maxFirstLine = 1;
		} else {
			textBox->maxFirstLine = 1;
		}
		
		textBox->prevAutoLineBreak = textBox->autoLineBreak;
		textBox->prevAdjustSpaces = textBox->adjustSpaces;
		textBox->prevFirstLine = textBox->firstLine;
		textBox->prevLen = textBox->len;
		textBox->prevSize.y = textBox->size.y;
		textBox->prevSize.x = textBox->size.x;
		textBox->prevTextHeight = textBox->textHeight;
		textBox->prevTextJumpSize = textBox->textJumpSize;
		
		if(textBox->firstLine < 1) {
			textBox->firstLine = 1;
		} else if(textBox->firstLine > textBox->maxFirstLine) {
			textBox->firstLine = textBox->maxFirstLine;
		}
	} else dontDrawText = 0;
	
	if(textBox->textReadyToPrint != NULL && textBox->size.y > 0) {
		
		MTclipArea clipBackup = MTgetCurrentClipArea();
		MTsetScreenClip(textBox->pos.x, textBox->pos.y, textBox->pos.x+textBox->size.x, textBox->pos.y+textBox->size.y);
		//MTdrawSmoothFillRoundRectangle(textBox->pos.x, textBox->pos.y, textBox->pos.x+textBox->size.x, textBox->pos.y+textBox->size.y, RGB(255,255,255), 5, 4);
		
		int posx = textBox->pos.x;
		int posy = textBox->pos.y;
		int align = INTRAFONT_ALIGN_LEFT;
		
		if(textBox->align == 1) {//center
			posx += (textBox->size.x - textBox->scrollBarV.size.x)/2;
			//posy += textBox->size.y/2;
			align = INTRAFONT_ALIGN_CENTER;
		} else if(textBox->align == 2) {//right
			posx += textBox->size.x - textBox->scrollBarV.size.x;
			//posy += textBox->size.y;
			align = INTRAFONT_ALIGN_RIGHT;
		}//default LEFT
		
		
		MTapiSetFontAndStyle(textBox->font, textBox->textHeight, textBox->textColor, 0, align);
		int linebreaksize = MTapiGetFontLineBreak(textBox->font);
		MTapiSetFontLineBreak(textBox->font, textBox->textJumpSize + textBox->textHeight);
		/*if(dontDrawText == 0)*/ MTdrawString(posx,posy,textBox->textReadyToPrint);
		MTapiSetFontLineBreak(textBox->font, linebreaksize);
		
		
		textBox->scrollBarV.allowDrag = 1;
		
		textBox->scrollBarV.direction = 1;
		textBox->scrollBarV.size.x = 15;
		textBox->scrollBarV.size.y = textBox->size.y;
		textBox->scrollBarV.pos.x = textBox->pos.x + textBox->size.x - textBox->scrollBarV.size.x;
		textBox->scrollBarV.pos.y = textBox->pos.y;
		//textBox->scrollBarV.dragPos = 0;
		//textBox->scrollBarV.dragging = 0;
		//textBox->scrollBarV.newVal = 0;
		textBox->scrollBarV.maxVal = textBox->maxFirstLine - 1;
		textBox->scrollBarV.currVal =  (textBox->firstLine - 1 );
		textBox->scrollBarV.shownNum = textBox->size.y;
		textBox->scrollBarV.totalNum = textBox->realSize.y;
		
		bool allowDragBar = 1;
		if(textBox->active == 0) allowDragBar = 0;
		
		MTapiDrawScrollBar(&(textBox->scrollBarV), selecting, allowDragBar);
		
		if(textBox->scrollBarV.dragging == 1) {
			textBox->firstLine = textBox->scrollBarV.newVal + 1;
			if(textBox->scrollBarV.newVal == textBox->scrollBarV.maxVal) textBox->firstLine = textBox->maxFirstLine;
		}
		
		MTsetClipArea(clipBackup);
		
		//MTresetScreenClip();
	}
	
	return;
}

MTapiMenu* MTapiCreateMenu(int posX, int posY, int width, int height, char* name, char* prefix) {
	//message(name);
	//MTapiAddWindow(MTapiWindow* window)
	MTapiMenu* list = (MTapiMenu*)malloc(sizeof(MTapiMenu));
	if(list == NULL) return NULL;
	list->list = MTcreateElementsList();
	if(list->list == NULL) return NULL;
	if(name != NULL) {
		/*list->menuReadyToPrint = (char*) malloc ((MTstrlen(name) + 1 + 1) * sizeof(char));
		strcpy(list->menuReadyToPrint,name);
		strcat(list->menuReadyToPrint,"\n");*/
		//sprintf(list->menuReadyToPrint,"%s\n",name);
		
		//list->name = name;
		list->name = (char*) malloc ((MTstrlen(name) + 1) * sizeof(char));
		strcpy(list->name,name);
		//asprintf(&list->name,"%s",name);
		//message("8");
	} else {
		//list->menuReadyToPrint = NULL;//(char*) malloc (1 * sizeof(char));
		list->name = NULL;
	}
	
	if(prefix != NULL) {
		list->prefix = (char*) malloc ((MTstrlen(prefix) + 1) * sizeof(char));
		strcpy(list->prefix,prefix);
	} else list->prefix = NULL;
	
	list->pos.x = posX;
	list->pos.y = posY;
	
	list->size.x = width;
	list->size.y = height;
	
	list->realSize.x = 0;
	list->realSize.y = 0;
	
	list->prevSize.x = width;
	list->prevSize.y = height;
	
	/*list->height = height;
	list->realHeight = 0;
	list->prevHeight = height;*/
	
	list->font = 0;//default
	//list->prevFont = 0;//default
	
	list->align = 0;
	
	list->textHeight = MTdefaultFontHeight;
	list->prevTextHeight = MTdefaultFontHeight;
	list->textColor = MTdefaultFontColor;
	
	list->textJumpSize = 0;
	list->prevTextJumpSize = 0;
	
	list->count = 0;
	list->prevCount = 0;
	list->firstNum = 1;
	list->maxFirstNum = 1;
	list->prevFirstNum = 1;
	list->firstShownElementPosition = MTfirstPositionInList (list->list);
	list->selectedElement = NULL;
	
	list->active = 0;
	
	//list->barIsActive = 0;
	//list->menuReadyToPrint = name;
	//list->needRefresh = 0;
	

	
	return list;
}

MTapiMenuElement* MTapiCreateMenuElement(char* name, char* data) {
	//message(name);
	//message(data);
	//MTapiAddWindow(MTapiWindow* window)
	MTapiMenuElement* element = (MTapiMenuElement*)malloc(sizeof(MTapiMenuElement));
	if(element == NULL) return NULL;
	
	element->name = NULL;
	element->data = NULL;
	if(name != NULL) {
		asprintf(&element->name,"%s",name);
		//element->name = (char*) malloc ((MTstrlen(name) + 1) * sizeof(char));
		//strcpy(element->name,name);
	}
	//message("2");
	if(data != NULL) {
		//asprintf(&element->data,"%s",name);
		element->data = (char*) malloc ((MTstrlen(data) + 1) * sizeof(char));
		strcpy(element->data,data);
	}
	element->data2 = NULL;
	
	element->img = NULL;
	//element->name = name;
	//element->data = data;
	//message("3");
	return element;
}

MTapiMenuElement* MTapiCreateMenuElementWithData2(char* name, char* data, void* data2) {
	//message(name);
	//message(data);
	//MTapiAddWindow(MTapiWindow* window)
	MTapiMenuElement* element = MTapiCreateMenuElement(name, data);
	element->data2 = data2;
	return element;
}

MTapiMenuElement* MTapiCreateMenuElementWithFunction(char* name, char* data, void* function) {
	//message(name);
	//message(data);
	//MTapiAddWindow(MTapiWindow* window)
	MTapiMenuElement* element = MTapiCreateMenuElement(name, data);
	element->function = function;
	return element;
}

void MTapiFreeMenuElement(MTapiMenuElement* element) {
	if(element == NULL) return;
	if(element->name != NULL) free(element->name);
	if(element->data != NULL) free(element->data);
	element->name = NULL;
	element->data = NULL;
	//if(element->data2 != NULL) free(element->data2);
	free(element);
	element = NULL;
	return;
}

void MTapiFreeMenu(MTapiMenu* menu) {
	//message("1");
	if(menu == NULL) return;
	if(menu->name != NULL) free(menu->name);
	menu->name = NULL;
	//message("2");
	MTapiMenuElement* element;
	MTpositionInList elementPositionInList = MTfirstPositionInList(menu->list);
	while ((elementPositionInList != MTlastPositionInList(menu->list))) {
		//message("9");
		
		element = MTgetElementFromList(elementPositionInList, menu->list);
		if(element != NULL) {
			MTapiFreeMenuElement(element);
			element = NULL;
		}
		elementPositionInList = MTnextPositionInList(elementPositionInList,menu->list);
	}
	
	//message("3");
	
	/*element = MTgetElementFromList(elementPositionInList, menu->list);
	if(element != NULL) {
		MTapiFreeMenuElement(element);
		element = NULL;
	}
	
	message("4");*/
	
	MTdestroyList(menu->list);
	
	//message("5");
	
	menu->selectedElement = NULL;
	//if(menu->menuReadyToPrint != NULL) free(menu->menuReadyToPrint);
	//menu->menuReadyToPrint = NULL;
	
	//message("6");
	
	free(menu);
	menu = NULL;
	
	//message("7");
	return;
}

void MTapiAddMenuElement(MTapiMenu* menu, MTapiMenuElement* element, bool pos) {
	if(menu == NULL || menu->list == NULL || element == NULL) return;
	if(pos == 1) {
		MTinsertElementInList (element, MTfirstPositionInList (menu->list), menu->list);
	} else MTinsertElementInList (element, MTlastPositionInList (menu->list), menu->list);
	menu->count++;
	
	menu->firstShownElementPosition = MTfirstPositionInList (menu->list);
	menu->firstNum = 1;
	menu->prevFirstNum = 0;
	//menu->realHeight += (menu->textHeight + menu->textJumpSize);
	return;
}

void MTapiDrawMenu(MTapiMenu* menu, bool highlight, bool selecting, MTmouse* cursor) {
	if(menu == NULL || menu->list == NULL) return;
	//menu->firstNum = varY;
	//menu->textJumpSize = varX;
	
	/*if(cursor->dragging && MTisOverArea(cursor->x, cursor->y, menu->pos.x, menu->pos.y + yOffset, menu->size.x, menu->size.y - yOffset)) {
		buttonPosition = (menu->pos.y - cursor->y) - yOffset;
	}*/
	bool dontDrawText = 0;
	if(menu->firstNum != menu->prevFirstNum || menu->count != menu->prevCount || menu->size.y != menu->prevSize.y || menu->textHeight != menu->prevTextHeight || menu->textJumpSize != menu->prevTextJumpSize ) {
		
		menu->realSize.y = menu->count*(menu->textHeight + menu->textJumpSize);
		//message("bucle");
		
		if((menu->textHeight + menu->textJumpSize) > 0)	{
			menu->maxFirstNum = menu->count - (menu->size.y/(menu->textHeight + menu->textJumpSize)) + 2;
			if(menu->maxFirstNum < 1) menu->maxFirstNum = 1;
		} else {
			menu->maxFirstNum = 1;
		}
		
		if(menu->firstNum < 1) {
			menu->firstNum = 1;
		} else if(menu->firstNum > menu->maxFirstNum) {
			menu->firstNum = menu->maxFirstNum;
		}
		
		if(menu->firstNum != menu->prevFirstNum) {
			//message("2");
			if(menu->firstNum <= 1) {
				menu->firstShownElementPosition = MTfirstPositionInList (menu->list);
				//message("3");
			} else if(menu->firstNum >= menu->count) {
				menu->firstShownElementPosition = MTlastPositionInList (menu->list);
			} else {
				if(menu->firstShownElementPosition == NULL) menu->firstShownElementPosition = MTfirstPositionInList (menu->list);
				//menu->firstShownElementPosition = MTelementNumPositionInListOptimal(menu->firstNum, menu->list, menu->prevFirstNum, menu->firstShownElementPosition);
				menu->firstShownElementPosition = MTelementNumPositionInList(menu->firstNum, menu->list);
				//message("4");
			}
		}
		
		memset (menu->menuReadyToPrint,'\0',1023);
		/*int bufferSize = 1;
		if(menu->menuReadyToPrint != NULL) {
			//menu->menuReadyToPrint[0] = '\0';
			free(menu->menuReadyToPrint);
			menu->menuReadyToPrint = NULL;
		}*/
		
		if(menu->name != NULL) {
			//bufferSize += MTstrlen(menu->name) + 1;//+ linebreak ;-)
			//menu->menuReadyToPrint = (char*) malloc (bufferSize * sizeof(char));
			strcpy(menu->menuReadyToPrint,menu->name);
			strcat(menu->menuReadyToPrint,"\n");
			//sprintf(menu->menuReadyToPrint,"%s\n",menu->name);
			//menu->menuReadyToPrint[bufferSize-1] = '\0';
			//message("8");
		} else {
			//menu->menuReadyToPrint = (char*) malloc ( bufferSize * sizeof(char));
			//menu->menuReadyToPrint[bufferSize-1] = '\0';
		}
		
		//int elementCount = MTnumberOfElementsInList(menu->list);
		if( menu->count > 0) {
			//message("5");
			MTapiMenuElement* element;
			MTpositionInList elementPositionInList;
			
			if(menu->firstShownElementPosition != NULL) {
				elementPositionInList = menu->firstShownElementPosition;
			} else {
				//message("6");
				menu->firstShownElementPosition = MTfirstPositionInList (menu->list);
				elementPositionInList = menu->firstShownElementPosition;
			}
			//message("7");
			
			int finalHeight = 0;
			if(menu->name != NULL) finalHeight += (menu->textHeight + menu->textJumpSize);
			while ((elementPositionInList != MTlastPositionInList(menu->list))) {
				//message("9");
				element = MTgetElementFromList(elementPositionInList, menu->list);
				if(element != NULL) {
					//message("10");
					//element->name
					//menu->menuReadyToPrint
					
					if(finalHeight > menu->size.y) break;
					
					finalHeight += (menu->textHeight + menu->textJumpSize);
					
					/*int lenght = 1;//linebreak character and EOF
					if(element->name != NULL) {
						if(menu->prefix != NULL) lenght += MTstrlen(menu->prefix);//Spaces to make margin
						lenght += MTstrlen(element->name);
					}*/
					//bufferSize += lenght;
					
					//message(element->name);
					
					//message("11");
					
					//menu->menuReadyToPrint = (char*) realloc (menu->menuReadyToPrint, bufferSize * sizeof(char));
					//message("12");
					
					char buffer[512];
					memset (buffer,'\0',511);
					//buffer[0] = '\0';
					if(element->name != NULL) {
						if(menu->prefix != NULL) {
							strcat(buffer,menu->prefix);
							//sprintf(buffer,"  %s\n",element->name);
						}//sprintf(buffer,"%s\n",element->name);
						strcat(buffer,element->name);
					}// else sprintf(buffer,"\n");
					
					strcat(buffer,"\n");
					
					//message("13");
					
					strcat (menu->menuReadyToPrint,buffer);
					
					//menu->menuReadyToPrint[bufferSize-1] = '\0';
					
					//message("14");
					elementPositionInList = MTnextPositionInList(elementPositionInList,menu->list);
				} else break; //Si algo falla, puede que la lista haya cambiado :-S por lo que salimos antes de que haya algun error
			}
		}
		
		
		/*unsigned short *mybuff = (unsigned short *) malloc((bufferSize + 1)*sizeof(unsigned short));
		
		cccUTF8toUCS2(mybuff, bufferSize, (unsigned char*)menu->menuReadyToPrint);
		
		MTfree(menu->menuReadyToPrint);
		
		menu->menuReadyToPrint = NULL;
		
		int size = 0;
		menu->menuReadyToPrint = (char*)utf16le_to_utf8_alloc2(mybuff,&size);
		
		MTfree(mybuff);
		mybuff = NULL;
		
		//if(size != bufferSize) message("diff");
		
		if(menu->menuReadyToPrint == NULL) dontDrawText = 1;
		
		bufferSize = size;*/
		
		menu->prevFirstNum = menu->firstNum;
		menu->prevCount = menu->count;
		menu->prevSize.y = menu->size.y;
		menu->prevTextHeight = menu->textHeight;
		menu->prevTextJumpSize = menu->textJumpSize;
	} else dontDrawText = 0;
	
	if(/*menu->menuReadyToPrint != NULL &&*/ menu->size.y > 0) {
		
		MTclipArea clipBackup = MTgetCurrentClipArea();
		MTsetScreenClip(menu->pos.x, menu->pos.y, menu->pos.x+menu->size.x, menu->pos.y+menu->size.y);
		//MTdrawSmoothFillRoundRectangle(menu->pos.x, menu->pos.y, menu->pos.x+menu->size.x, menu->pos.y+menu->size.y, RGB(255,255,255), 5, 4);
		
		menu->selectedElement = NULL;
		int yOffset = 0;
		if(menu->name != NULL) yOffset = (menu->textHeight + menu->textJumpSize);
		
		if(highlight == 1 && menu->count > 0 && (menu->textHeight + menu->textJumpSize) > 0 && MTisOverArea(cursor->x, cursor->y, menu->pos.x, menu->pos.y + yOffset, menu->size.x, menu->size.y - yOffset)) {
			
			//int dispElemNum = (menu->maxFirstNum - menu->count - 1);
			
			//int desplazamiento = 0;
			//if(menu->name != NULL) desplazamiento = menu->textHeight + menu->textJumpSize;
			
			int pos = 0;
			
			int selected = 0;
			while(pos <= menu->size.y) {
				pos += (menu->textHeight + menu->textJumpSize);
				selected ++;
				if((cursor->y - menu->pos.y) < pos) break;
			}
			
			//if(pos <= menu->size.y) {
			
			pos -= (menu->textHeight + menu->textJumpSize);
			
			if(menu->name != NULL) {
				//pos -= (menu->textHeight + menu->textJumpSize);
				selected --;
			}
			
			//selected--;
			
			
			if(selected < 1) {
				selected = 1;
				pos = (menu->textHeight + menu->textJumpSize);
			}
			
			selected += menu->firstNum - 1;
			
			if(selected <= menu->count) {
				
				if(selecting) {
					menu->selectedElement = MTgetElementFromList(MTelementNumPositionInListOptimal(selected, menu->list, menu->firstNum, menu->firstShownElementPosition), menu->list);
				}
				
				//varX = selected;
				
				/*float piece = (float)(menu->size.y - desplazamiento) / (float)dispElemNum;
				int overElemNum = (float)cursor->y/piece;*/
				
				MTdrawSmoothFillRoundRectangle(menu->pos.x, menu->pos.y + pos, menu->pos.x + menu->size.x, menu->pos.y + pos + menu->textHeight, MTchangeColorBright(MTapiWindowBarInColor,40), 3, 4);
				
				//MTcolor MTapiWindowBarInColorTWO = MTchangeColorBright(MTapiWindowBarInColor,40);
				//MTcolor MTapiWindowBarOutColorTWO = MTchangeColorBright(MTapiWindowBarOutColor,40);
				//MTdrawFillRoundRectangleWithGradient(0, desplazamiento + piece*(float)overElemNum, menu->size.x, desplazamiento + piece + piece*(float)overElemNum, MTapiWindowBarInColorTWO, MTapiWindowBarOutColorTWO, 3, 4);
			}
			//}
		}
		
		int posx = menu->pos.x;
		int posy = menu->pos.y;
		int align = INTRAFONT_ALIGN_LEFT;
		
		if(menu->align == 1) {//center
			posx += (menu->size.x - menu->scrollBarV.size.x)/2;
			//posy += textBox->size.y/2;
			align = INTRAFONT_ALIGN_CENTER;
		} else if(menu->align == 2) {//right
			posx += menu->size.x - menu->scrollBarV.size.x;
			//posy += textBox->size.y;
			align = INTRAFONT_ALIGN_RIGHT;
		}//default LEFT
		
		MTapiSetFontAndStyle(menu->font, menu->textHeight, menu->textColor, 0, align);
		int linebreaksize = MTapiGetFontLineBreak(menu->font);
		MTapiSetFontLineBreak(menu->font, menu->textJumpSize + menu->textHeight);
		if(dontDrawText == 0) MTdrawString(posx,posy,menu->menuReadyToPrint);
		MTapiSetFontLineBreak(menu->font, linebreaksize);
		
		/*typedef struct {
			bool direction;//1 = vertical
			MTvector pos;
			MTvector size;
	
			int dragPos;
			bool dragging;
	
			int newVal;
	
			int maxVal;
			int currVal;
			int shownNum;
	
		} MTapiScrollBar;*/
		
		//MTapiScrollBar bar;
		
		menu->scrollBarV.allowDrag = 1;
		
		menu->scrollBarV.direction = 1;
		menu->scrollBarV.size.x = 15;
		menu->scrollBarV.size.y = menu->size.y - yOffset;
		menu->scrollBarV.pos.x = menu->pos.x + menu->size.x - menu->scrollBarV.size.x;
		menu->scrollBarV.pos.y = menu->pos.y + yOffset;
		//menu->scrollBarV.dragPos = 0;
		//menu->scrollBarV.dragging = 0;
		//menu->scrollBarV.newVal = 0;
		menu->scrollBarV.maxVal = menu->realSize.y - menu->size.y;
		menu->scrollBarV.currVal =  (menu->firstNum - 1 ) * (menu->textHeight + menu->textJumpSize);
		menu->scrollBarV.shownNum = menu->size.y - yOffset;
		menu->scrollBarV.totalNum = menu->realSize.y;
		
		bool allowDragBar = 1;
		if(menu->active == 0) allowDragBar = 0;
		
		MTapiDrawScrollBar(&(menu->scrollBarV), selecting, allowDragBar);
		if(menu->active && MTisOverArea(cursor->x, cursor->y, menu->scrollBarV.pos.x, menu->scrollBarV.pos.y, menu->scrollBarV.size.x, menu->scrollBarV.size.y)) menu->selectedElement = NULL;
		
		if(menu->scrollBarV.dragging == 1) {
			menu->firstNum = (menu->scrollBarV.newVal / (menu->textHeight + menu->textJumpSize) ) + 1;
			if(menu->scrollBarV.newVal == menu->scrollBarV.maxVal) menu->firstNum = menu->maxFirstNum;
		}
		
		/*if(menu->maxFirstNum != 1) {
			int barWidth = 10;
			MTdrawSmoothFillRoundRectangle(menu->pos.x + menu->size.x - barWidth, menu->pos.y + yOffset, menu->pos.x + menu->size.x, menu->pos.y + menu->size.y, MTapiWindowBarInColor, 3, 4);
			
			//int maxNum = menu->maxFirstNum;
			int buttonHeight = MTpercent(menu->size.y - yOffset, menu->size.y - yOffset, menu->realSize.y - yOffset);//(menu->size.y - yOffset)/10;//10;
			if(buttonHeight < 10) buttonHeight = 10;
			if(buttonHeight > (menu->size.y - yOffset)) buttonHeight = (menu->size.y - yOffset);
			//int semiHeight = buttonHeight/2;
			int buttonPosition = MTpercent(menu->firstNum, (menu->size.y - yOffset - buttonHeight), menu->maxFirstNum);
			
			if(menu->firstNum == 1) buttonPosition = 0;
			MTdrawSmoothFillRoundRectangle(menu->pos.x + menu->size.x - barWidth, menu->pos.y + yOffset + buttonPosition, menu->pos.x + menu->size.x, menu->pos.y + yOffset + buttonPosition + buttonHeight, MTapiWindowBarOutColor, 3, 4);
		}*/
		
		MTsetClipArea(clipBackup);
		
		//MTresetScreenClip();
	}
	
	return;
}

/*void MTapiAppendDirectory(char* ) {
	char buffer[256];
	buffer[0] = '\0';
	strcat(buffer,window->data);
	if(realPath[strlen(realPath)-1] != '/') strcat(realPath,"/");
	strcat(buffer,testMenu->selectedElement->data);
}*/

MTapiMenu* MTapiCreateFileListMenu(int posX, int posY, int width, int height, char* folder, char* prefix, char* search, int* types, bool showFolders) {
	MTapiMenu* menu = MTapiCreateMenu(posX, posY, width, height, folder, prefix);
	if(folder == NULL) return menu;
	
	DIR *dp;
	struct dirent *ep;
	
	dp = opendir (folder);
	if (dp != NULL)
	{
		while ((ep = readdir (dp))) {
			//char buff[256];
			//MTgetFileName(ep->d_name, buff);
			
			if(ep->d_name[0] != '.') {//we reject "." and ".." and ".example"
				
				char buffer[256];
				buffer[0] = '\0';
				strcat(buffer,folder);
				if(buffer[strlen(buffer)-1] != '/' && ep->d_name[0] != '/') strcat(buffer,"/");
				strcat(buffer,ep->d_name);
				
				int fileType = MTgetFileType(buffer);
				
				char buffer2[256];
				strcpy(buffer2,ep->d_name);
				if(fileType == 0) {
					//sprintf(buffer2,"%s/",ep->d_name);
					buffer2[MTstrlen(buffer2) + 1] = '\0';
					buffer2[MTstrlen(buffer2)] = '/';
				}
				
				bool skip = 0;
				if(search != NULL) {
					skip = 1;
					if(MTstristr(ep->d_name,search)) skip = 0;
				}
				
				if(types != NULL) {
					skip = 1;
					int i = 0;
					while(types[i] != -1) {
						if(fileType == types[i]) skip = 0;
						i++;
					}
				}
				
				if(showFolders && fileType == 0) skip = 0;
				
				if(skip == 0) {
				MTapiMenuElement* element = MTapiCreateMenuElement(buffer2, buffer);
				element->data2 = (int*)fileType;
				
				if(fileType == 0) {//is folder (we put the folders at the begining and files at the end)
				MTapiAddMenuElement(menu,element,1);
				} else MTapiAddMenuElement(menu,element,0);
				}
			}
		}
		//puts (ep->d_name);
		closedir (dp);
	}
	
	return menu;
}

//#include "newTextFunction.h"


