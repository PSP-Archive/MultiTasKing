/*
typedef struct		{
	//protected
	u16 sizeX, sizeY;						//!< Displayable size
	u16 sysSizeX, sysSizeY;					//!< Size aligned to the next power of two
	u16 realSizeX, realSizeY;				//!< Actual buffer size (never write outside of these dimensions!)
	void *data;								//!< Raw image data in memory
	u8 flags;								//!< Special image flags
	int totalSize;							//!< Total image size in bytes
	short location;							//!< Image location (OSL_IN_RAM or OSL_IN_VRAM)
	short pixelFormat;						//!< Image pixel format

	//public
	OSL_PALETTE *palette;					//!< Palette for 4 and 8 bit modes
	u16 frameSizeX, frameSizeY;				//!< Size of a frame in the image

	//Special
	int x, y;								//!< Image positions
	int stretchX, stretchY;					//!< Final image size when drawn (stretched)
	//Morceau
	float offsetX0, offsetY0, offsetX1, offsetY1;		//!< Offset in the texture
	//Rotation
	int centerX, centerY;					//!< Rotation center
	int angle;								//!< Angle (rotation) in degrees
	//Paramètres
//	bool autoStrip;							//!< Automatic stripping (let it one)
} OSL_IMAGE;
*/

#define COLOR_VERTEX OSL_LINE_VERTEX_COLOR32

//#define MTgetDrawFX oslGetAlphaEx
//#define MTsetDrawFX oslSetAlphaEx

/*#define MTdrawFXnone OSL_FX_NONE//: Blending is purely disabled.
#define MTdrawFXdefault OSL_FX_DEFAULT//: Reverts to default mode (RGBA).
#define MTdrawFXrgba OSL_FX_RGBA//: The alpha value of images or colors is taken in account for blending. For example, drawing a rectangle with the following color: RGBA(0, 0, 0, 128) will draw it semi-transparent.
#define MTdrawFXalpha OSL_FX_ALPHA//: Regular alpha blending: dstColor = dstColor = srcColor * coeff1 + dstColor * (1 - coeff1).
#define MTdrawFXadd OSL_FX_ADD//: Color addition: dstColor = dstColor * coeff2 + srcColor * coeff1. With oslSetAlpha, coeff2 is not supplied and set to full (255) by default.
#define MTdrawFXsub OSL_FX_SUB//: Color substraction: dstColor = dstColor * coeff2 - srcColor * coeff1. Please note that substracting colors produce the invert effect that you'd expect to see: substracting red (255, 0, 0) from a white image (255, 255, 255) for example gives cyan output (0, 255, 255). Furthermore, you can OR the value with the following:
#define MTdrawFXtint OSL_FX_COLOR//: Coeff1 and coeff2 are treated as 32-bit colors instead of 8-bit alpha values. Multiplication between a color and another gives a tinted color.


typedef struct
{
	MTcolor tint;
	MTcolor alpha;
	MTcolor add;
	MTcolor sub;
} MTdrawFX;*/

/*MTdrawFX MTgetDrawFX() {//for FX
	return currentEffects;
}

//Default alpha
oslSetAlpha(OSL_FX_DEFAULT, 0);
//Tint an image with red
oslSetAlpha(OSL_FX_TINT, RGBA(255, 0, 0, 255));
//The image will be drawn semi-transparent.
oslSetAlpha(OSL_FX_ALPHA, 128);
//Tinted + semi-transparent image
oslSetAlpha(OSL_FX_ALPHA | OSL_FX_COLOR, RGBA(255, 0, 0, 128));
//Simple addition as on the Super Nintendo
oslSetAlpha(OSL_FX_ADD, 255);
//dst = 0.5 * src + 0.5 * dst
oslSetAlpha2(OSL_FX_ADD, 128, 128);

void MTsetDrawFX(MTdrawFX* effects) {//for FX
	if(effects == NULL) return;
	if(&effects->alpha != NULL) {
		if(&effects->tint != NULL) {
			oslSetAlpha(OSL_FX_TINT, RGBA(255, 0, 0, 255));
			oslSetAlpha(OSL_FX_ALPHA | OSL_FX_COLOR, effects->tint);
		} else oslSetAlpha(OSL_FX_ALPHA, effects->alpha);
	} else if(&effects->tint != NULL) {
		oslSetAlpha(OSL_FX_TINT, effects->tint);
	} else if(&effects->add != NULL) {
		oslSetAlpha(OSL_FX_ADD, effects->add);
	} else if(&effects->sub != NULL) {
		
	} else oslSetAlpha(OSL_FX_DEFAULT, 0);
	
	return;
}*/

/*#define MTdrawFX OSL_ALPHA_PARAMS
#define MTgetDrawFX oslGetAlphaEx
#define MTsetDrawFX oslSetAlphaEx

#define MTdrawFXnone OSL_FX_NONE//: Blending is purely disabled.
#define MTdrawFXdefault OSL_FX_DEFAULT//: Reverts to default mode (RGBA).
#define MTdrawFXrgba OSL_FX_RGBA//: The alpha value of images or colors is taken in account for blending. For example, drawing a rectangle with the following color: RGBA(0, 0, 0, 128) will draw it semi-transparent.
#define MTdrawFXalpha OSL_FX_ALPHA//: Regular alpha blending: dstColor = dstColor = srcColor * coeff1 + dstColor * (1 - coeff1).
#define MTdrawFXadd OSL_FX_ADD//: Color addition: dstColor = dstColor * coeff2 + srcColor * coeff1. With oslSetAlpha, coeff2 is not supplied and set to full (255) by default.
#define MTdrawFXsub OSL_FX_SUB//: Color substraction: dstColor = dstColor * coeff2 - srcColor * coeff1. Please note that substracting colors produce the invert effect that you'd expect to see: substracting red (255, 0, 0) from a white image (255, 255, 255) for example gives cyan output (0, 255, 255). Furthermore, you can OR the value with the following:
#define MTdrawFXtint OSL_FX_COLOR//: Coeff1 and coeff2 are treated as 32-bit colors instead of 8-bit alpha values. Multiplication between a color and another gives a tinted color.
*/

MTdrawFX MTgetDrawFX() {
	MTdrawFX effects;
	oslGetAlphaEx(&effects);
	return effects;
}

void MTsetDrawFX2(MTdrawFX effects) {
	oslSetAlphaEx(&effects);
	return;
}

void MTsetDrawFX(int eff, int coeff1, int coeff2) {
	MTdrawFX effects;
	effects.effect = eff;
	effects.coeff1 = coeff1;
	effects.coeff2 = coeff2;
	MTsetDrawFX2(effects);
	return;
}

void MTresetDrawFX() {
	MTdrawFX effects;
	effects.effect = MTdrawFXdefault;
	effects.coeff1 = 255;
	effects.coeff2 = 255;
	MTsetDrawFX2(effects);
	return;
}

MTcolor MTcolorBlend(MTcolor color) {//for FX
	return oslBlendColor(color);
}

void MTsetFontStyle(MTfont* font, float size, MTcolor color, MTcolor shadowColor, int prop) {
	oslIntraFontSetStyle(font, size, color, shadowColor, prop);
	return;
}

void MTsetFont(MTfont* font) {
	oslSetFont(font);
	return;
}

/*void MTresetDefaultFont() {
	MTsetFontStyle(MTdefaultFont, MTdefaultFontSize, MTdefaultFontColor, 0, INTRAFONT_ALIGN_LEFT);
	return;
}*/

MTfont* MTloadFontFile(char* filename) {
	return oslLoadFontFile (filename);
}

void MTfreeFont (MTfont* font) {
	return oslDeleteFont (font);
}

MTvector MTdrawingOrigin;

void MTsetGlobalDrawingOrigin(int x, int y) {
	MTdrawingOrigin.x = x;
	MTdrawingOrigin.y = y;
	return;
}

void MTsetDrawingOrigin(int x, int y) {
	MTdrawingOrigin.x += x;
	MTdrawingOrigin.y += y;
	return;
}

void MTresetDrawingOrigin() {
	MTdrawingOrigin.x = 0;
	MTdrawingOrigin.y = 0;
	return;
}

void MTdrawString(int x, int y, const char* str) {
	oslEnableTexturing();
	oslDrawString(x+MTdrawingOrigin.x, y+MTdrawingOrigin.y,str);
	oslDisableTexturing();
	return;
}

void MTdrawStringEx(int x, int y, const char* str, int len) {
	oslEnableTexturing();
	//oslDrawString(x+MTdrawingOrigin.x, y+MTdrawingOrigin.y,str);
	intraFontPrintEx(osl_curFont->intra, x+MTdrawingOrigin.x, y+MTdrawingOrigin.y,str, len);
	oslDisableTexturing();
	return;
}

void MTdrawChar(int x, int y, const char c) {
	oslEnableTexturing();
	oslDrawChar(x+MTdrawingOrigin.x, y+MTdrawingOrigin.y, c);
	oslDisableTexturing();
	return;
}

int MTgetTextHeight() {
	return (float)osl_curFont->intra->advancey*((float)osl_curFont->intra->size * 0.25f);
}

void MTsetFontLineBreakSize(MTfont* font, int height) {
	if(font == NULL) return;
	//height = font->intra->advancey * font->intra->size * 0.25;
	font->intra->advancey = height / (font->intra->size * 0.25);
	return;
}

int MTgetFontLineBreakSize(MTfont* font) {
	if(font == NULL) return 10;
	//height = font->intra->advancey * font->intra->size * 0.25;
	return font->intra->advancey * font->intra->size * 0.25;
}

float MTgetFontSize() {
	return (float)osl_curFont->intra->size;
}

void MTsetTextHeight(int height) {
	osl_curFont->intra->size = (float)height/((float)osl_curFont->intra->advancey* 0.25f);
	return;
}

void MTsetFontHeight(MTfont* font, int height) {
	if(font == NULL) return;
	font->intra->size = (float)height/((float)font->intra->advancey* 0.25f);
	return;
}

void MTsetCurrentFontHeight(int height) {
	MTsetFontHeight(osl_curFont, height);
	return;
}

float MTgetFontTextWidth(MTfont* font, const char *str)
{
    if (!font)
        return 0;

    float x = 0;
    if (font->fontType == OSL_FONT_OFT){
        unsigned char c;

        //Parcourt tous les caractères
        while(*str)			{
            c = *(unsigned char*)str++;
            x += font->charWidths[c];
        }
    }else if (font->fontType == OSL_FONT_INTRA){
        x = intraFontMeasureText(font->intra, str);
    }
	return x;
}

float MTgetTextWidth(char* text) {//hmm... this can't be used from a thread since you need to specify the properties with a function that would interfere with the graphic thread...
	return intraFontMeasureText(osl_curFont->intra, text);
}

void MTdrawStringLimited(int x, int y, const char* str, int len) {
	if(len <= MTgetTextHeight() ) return;
	oslEnableTexturing();
	oslDrawStringLimited(x+MTdrawingOrigin.x, y+MTdrawingOrigin.y, len, str);
	oslDisableTexturing();
	return;
}

//Recuerda modificar el source de OSlib para que intra->size sea unsigned int en vez de char
//osl_curFont->intra->advancey = Salto_De_Linea/(osl_curFont->intra->size * 0.25f);
//Salto_De_Linea = osl_curFont->intra->advancey*(osl_curFont->intra->size * 0.25f);
/*
int MTdrawTextBox(int x, int y, int width, int height, int jump, int limit, int scrollX, int scrollY, char* text, int len) {	
	if(text == NULL) return -3;
	
	int i;
	int lineWidth = 0;
	int lineLen = 0;
	int breakWhenSpace = 0;
	int maxLineWidth = 0;
	int posx = 0;
	int posy = 0;
	char caracter[1];
	for(i = 0; i <= len;i++) {
		if(text[i] == '\0') break;
		if(text[i] == '\n') {//Real line break
			posy += jump;
			posx = 0;
			i++;//jump the character
			if(lineWidth > maxLineWidth) maxLineWidth = lineWidth;
			lineWidth = 0;
			lineLen = 0;
			breakWhenSpace = 0;
			//printf("%d\n",2);
		}
		
		if(limit > 1 && (lineWidth + limit) >= width) {//Auto line-break
			breakWhenSpace = 1;
			//printf("%d\n",6);
		}
		if(breakWhenSpace == 1 && text[i] == ' ') {
			posy += jump;
			posx = 0;
			i++;//jump the character
			if(lineWidth > maxLineWidth) maxLineWidth = lineWidth;
			lineWidth = 0;
			lineLen = 0;
			breakWhenSpace = 0;
			//printf("%d\n",3);
		}
		
		if((breakWhenSpace == 1 || limit == 1) && lineWidth >= width) {//Forced line-break
			posy += jump;
			posx = 0;
			i++;//jump the character
			if(lineWidth > maxLineWidth) maxLineWidth = lineWidth;
			lineWidth = 0;
			lineLen = 0;
			breakWhenSpace = 0;
			//printf("%d\n",4);
		}
		
		caracter[0] = text[i];
		int charwidth = MTgetTextWidth(caracter);
		lineWidth += charwidth;
		lineLen++;
		
		int newPosX = x+posx-scrollX;
		int newPosY = y+posy-scrollY;
		//if(newPosX >= x && newPosY >= y && newPosX <= x+width && newPosY <= y+height) MTdrawChar(newPosX, newPosY, caracter[0]);
		
		if(newPosY > y+height) return 2;
		
		posx += charwidth;
		//printf("%d\n",5);
	}
	
	return 0;
}*/



void MTsetGlobalScreenClip(int x0, int y0, int x1, int y1) {
	/*x0 += MTdrawingOrigin.x;
	y0 += MTdrawingOrigin.y;
	x1 += MTdrawingOrigin.x;
	y1 += MTdrawingOrigin.y;*/
	
	if(x0 < 0) x0 = 0;
	if(y0 < 0) y0 = 0;
	
	if(x1 < 0) x1 = 1;
	if(y1 < 0) y1 = 1;
	
	if(x0 > 480) x0 = 480;
	if(y0 > 272) y0 = 272;
	
	if(x1 > 480) x1 = 480;
	if(y1 > 272) y1 = 272;
	
	if(x0 >= x1 || y0 >= y1) {
		x0 = 0;
		y0 = 0;
		x1 = 1;
		y1 = 1;
	}
	
	//MTdrawRect(x0, y0, x1, y1, RGB(0,255,0));
	//MTdrawPixel(x0, y0, RGB(0,0,255));
	
	oslSetScreenClipping(x0,y0,x1,y1);
	
	MTcurrentScreenClip.pos.x = x0;
	MTcurrentScreenClip.pos.y = y0;
	MTcurrentScreenClip.size.x = x1-x0;
	MTcurrentScreenClip.size.y = y1-y0;
	MTcurrentScreenClip.use = 1;
	return;
}

void MTsetScreenClip(int x0, int y0, int x1, int y1) {
	
	MTvector pos1;
	MTvector pos2;
	
	pos1.x = x0 + MTdrawingOrigin.x;
	pos1.y = y0 + MTdrawingOrigin.y;
	
	pos2.x = x1 + MTdrawingOrigin.x;
	pos2.y = y1 + MTdrawingOrigin.y;
	
	if(pos1.x < MTcurrentScreenClip.pos.x) pos1.x = MTcurrentScreenClip.pos.x;
	if(pos1.y < MTcurrentScreenClip.pos.y) pos1.y = MTcurrentScreenClip.pos.y;
	
	if(pos1.x > (MTcurrentScreenClip.pos.x + MTcurrentScreenClip.size.x)) pos1.x = (MTcurrentScreenClip.pos.x + MTcurrentScreenClip.size.x);
	if(pos1.y > (MTcurrentScreenClip.pos.y + MTcurrentScreenClip.size.y)) pos1.y = (MTcurrentScreenClip.pos.y + MTcurrentScreenClip.size.y);
	
	if(pos2.x < MTcurrentScreenClip.pos.x) pos2.x = MTcurrentScreenClip.pos.x;
	if(pos2.y < MTcurrentScreenClip.pos.y) pos2.y = MTcurrentScreenClip.pos.y;
	
	if(pos2.x > (MTcurrentScreenClip.pos.x + MTcurrentScreenClip.size.x)) pos2.x = (MTcurrentScreenClip.pos.x + MTcurrentScreenClip.size.x);
	if(pos2.y > (MTcurrentScreenClip.pos.y + MTcurrentScreenClip.size.y)) pos2.y = (MTcurrentScreenClip.pos.y + MTcurrentScreenClip.size.y);
	
	MTsetGlobalScreenClip(pos1.x, pos1.y, pos2.x, pos2.y);
	return;
}

void MTsetClipArea(MTclipArea clip) {
	MTsetGlobalScreenClip(clip.pos.x,clip.pos.y,clip.pos.x+clip.size.x,clip.pos.y+clip.size.y);
	return;
}

MTclipArea MTgetCurrentClipArea() {
	return MTcurrentScreenClip;
}

void MTsetClipAreaInsideCurrentClip(MTclipArea clip) {
	/*MTclipArea newClip;
	newClip.pos.x = clip.pos.x + origin.x;
	newClip.pos.y = clip.pos.y + origin.y;
	newClip.size.x = clip.size.x;
	newClip.size.y = clip.size.y;
	newClip.use = 1;
	if(newClip.pos.x < MTcurrentScreenClip.pos.x) {
		newClip.size.x = MTcurrentScreenClip.pos.x - newClip.pos.x;
		newClip.pos.x = MTcurrentScreenClip.pos.x;
	}
	if(newClip.pos.y < MTcurrentScreenClip.pos.y) {
		newClip.size.y = MTcurrentScreenClip.pos.y - newClip.pos.y;
		newClip.pos.y = MTcurrentScreenClip.pos.y;
	}
	if((newClip.size.x + newClip.pos.x) > (MTcurrentScreenClip.size.x + MTcurrentScreenClip.pos.x)) newClip.size.x = MTcurrentScreenClip.size.x + MTcurrentScreenClip.pos.x - newClip.pos.x;
	if((newClip.size.y + newClip.pos.y) > (MTcurrentScreenClip.size.y + MTcurrentScreenClip.pos.y)) newClip.size.y = MTcurrentScreenClip.size.y + MTcurrentScreenClip.pos.y - newClip.pos.y;
	MTsetClipArea(newClip);*/
	
	MTvector pos1;
	MTvector pos2;
	
	pos1.x = clip.pos.x;
	pos1.y = clip.pos.y;
	
	pos2.x = pos1.x + clip.size.x;
	pos2.y = pos1.y + clip.size.y;
	
	if(pos1.x < MTcurrentScreenClip.pos.x) pos1.x = MTcurrentScreenClip.pos.x;
	if(pos1.y < MTcurrentScreenClip.pos.y) pos1.y = MTcurrentScreenClip.pos.y;
	
	if(pos1.x > (MTcurrentScreenClip.pos.x + MTcurrentScreenClip.size.x)) pos1.x = (MTcurrentScreenClip.pos.x + MTcurrentScreenClip.size.x);
	if(pos1.y > (MTcurrentScreenClip.pos.y + MTcurrentScreenClip.size.y)) pos1.y = (MTcurrentScreenClip.pos.y + MTcurrentScreenClip.size.y);
	
	if(pos2.x < MTcurrentScreenClip.pos.x) pos2.x = MTcurrentScreenClip.pos.x;
	if(pos2.y < MTcurrentScreenClip.pos.y) pos2.y = MTcurrentScreenClip.pos.y;
	
	if(pos2.x > (MTcurrentScreenClip.pos.x + MTcurrentScreenClip.size.x)) pos2.x = (MTcurrentScreenClip.pos.x + MTcurrentScreenClip.size.x);
	if(pos2.y > (MTcurrentScreenClip.pos.y + MTcurrentScreenClip.size.y)) pos2.y = (MTcurrentScreenClip.pos.y + MTcurrentScreenClip.size.y);
	
	MTsetGlobalScreenClip(pos1.x, pos1.y, pos2.x, pos2.y);
	
	//MTsetScreenClipInsideCurrentClip(clip.pos.x, int y0, int x1, int y1)
	return;
}

void MTresetScreenClip() {
	MTcurrentScreenClip.pos.x = 0;
	MTcurrentScreenClip.pos.y = 0;
	MTcurrentScreenClip.size.x = 480;
	MTcurrentScreenClip.size.y = 272;
	MTcurrentScreenClip.use = 1;
	oslResetScreenClipping();
	return;
}

void MTsmoothImages(int num) {
	oslSetBilinearFilter(num);
	return;
}

void MTloadFailCallback(const char *filename, u32 reserved);

int MTinitMainLib(){
	oslInit(0);
	oslInitGfx(OSL_PF_8888, 1);
	oslInitAudio();
	oslSetQuitOnLoadFailure(2);//2 means it will only execute the MTloadFailCallback function
	oslSetQuitOnLoadFailureCallback(MTloadFailCallback);
	oslSetKeyAutorepeatInit(40);
	oslSetKeyAutorepeatInterval(10);
	oslIntraFontInit(INTRAFONT_CACHE_ALL | INTRAFONT_STRING_UTF8); // All fonts loaded with oslLoadIntraFontFile will have UTF8 support
	
	oslSetBilinearFilter(1);//smooth images
	
	MTinitControls();
	
	//Enable frameskipping, 0 frame out of 1 is skipped by default (60 fps)
	oslSetFrameskip(1);
	//But skip a maximum of 3 frames out of 4, else the game seems very laggy
	oslSetMaxFrameskip(4);
	
	oslNetInit();
	
	oslDisableTexturing();
	
	MTresetDrawingOrigin();
	MTresetScreenClip();
	MTresetDrawFX();
	
	getcwd(MTrootDir, 255);//get Current working directory, so we can specify it in each thread
	
	//oslInitAudioME(OSL_FMT_MP3);
	
	//oslAudioSetDefaultSampleNumber(5*1024);
	oslInitAudioME(OSL_FMT_MP3);
	
	//oslInitUsbStorage();
	
	return 0;
}

void MTsetupThread() {//Does all the stuff required to use OSlib ;-)
	chdir(MTrootDir);//Specifies root directory
	return;
}

void MTstartDrawing() {
	isDrawing = 1;
	oslStartDrawing();
	return;
}

void MTendDrawing() {
	isDrawing = 0;
	oslEndDrawing();
	return;
}

int MTsyncDrawing() {
	oslEndFrame();
	return oslSyncFrame();
}

int MTendMainLib(){
	//oslDeinitUsbStorage();
	oslIntraFontShutdown();
	oslEndGfx();
	osl_quit = 1;
	return 0;
}

void MTclearScreen (MTcolor backColor) {
	oslClearScreen (backColor);
	return;
}

/*void MTdrawSimpleLine(int x0, int y0, int x1, int y1, MTcolor color) {
	oslDrawLine (x0,y0,x1,y1, color);
	return;
}*/

void MTdrawLineWithGradient(int x0, int y0, int x1, int y1, MTcolor color0, MTcolor color1) {
	
	color0 = MTcolorBlend(color0);
	color1 = MTcolorBlend(color1);
	
	COLOR_VERTEX* vertices;
	vertices = (COLOR_VERTEX*)sceGuGetMemory(2 * sizeof(COLOR_VERTEX));
	vertices[0].color = color0;
	vertices[0].x = x0 + MTdrawingOrigin.x;
	vertices[0].y = y0 + MTdrawingOrigin.y;
	vertices[0].z = 0;
	vertices[1].color = color1;
	vertices[1].x = x1 + MTdrawingOrigin.x;
	vertices[1].y = y1 + MTdrawingOrigin.y;
	vertices[1].z = 0;
	//sceGuDisable(GU_TEXTURE_2D);
	//oslDisableTexturing();
	sceGuDrawArray(GU_LINES, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 2, 0, vertices);
	//sceKernelDcacheWritebackRange(vertices, 2 * sizeof(COLOR_VERTEX));
	//sceGuEnable(GU_TEXTURE_2D);
	//MTdrawString(x0,y0,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	return;
}

void MTdrawLine(int x0, int y0, int x1, int y1, MTcolor color) {
	MTdrawLineWithGradient (x0,y0,x1,y1, color, color);
	return;
}

/*MTimage* MTcreateImage (int width, int height) {
	return oslCreateImage (width, height, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
}*/

void MTfreeImage (MTimage* img) {
	return oslDeleteImage (img);
}

MTimage* MTloadImageFilePNG (char *filename) {
	return oslLoadImageFilePNG (filename, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
}

MTimage* MTloadImageFileJPG (char *filename) {
	MTimage* img = oslLoadImageFileJPG (filename, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	oslCorrectImageHalfBorder(img);
	oslCorrectImageHalfBorder(img);
	return img;
}

MTimage* MTloadImageFileGIF (char *filename) {
	return oslLoadImageFileGIF (filename, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
}

MTimage* MTcreateScaledImageCopy(MTimage* img, int newWidth, int newHeight) {
	return oslScaleImageCreate (img, OSL_IN_RAM | OSL_SWIZZLED, newWidth, newHeight, OSL_PF_8888);
}

MTimage* MTloadImageFileSimple (char *filename) {
	/*char *ext, extension[10];
	int i;
	ext = strrchr(filename, '.');
	if (!ext)
		return NULL;
	i = 0;
	while(ext[i] && i < sizeof(extension) - 2)
	{
		extension[i] = tolower(ext[i]);
		i++;
	}
	extension[i] = 0;
	if (!strcmp(extension, ".png"))
		return MTloadImageFilePNG(filename);
	else if (!strcmp(extension, ".jpg"))
		return MTloadImageFileJPG(filename);
	else if (!strcmp(extension, ".gif"))
		return MTloadImageFileGIF(filename);*/
		
	int type = MTgetFileType(filename);
	if(type == 8) return MTloadImageFileJPG(filename);
	if(type == 6) return MTloadImageFilePNG(filename);
	if(type == 9) return MTloadImageFileGIF(filename);
	
	MTloadFailCallback(filename, 0);

	return NULL;
	
	//return oslLoadImageFile (filename, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
}

MTimage* MTloadImageFile (char *filename) {
	MTimage* img1 = MTloadImageFileSimple(filename);
	
	if(img1 == NULL) return NULL;
	
	MTimage* img = NULL;
	
	if(img1->sizeX > 512 || img1->sizeY > 512) {
		int newSizeX = 512;
		float proportion = ((float)newSizeX)/((float)img1->sizeX);
		int newSizeY = proportion*img1->sizeY;
	
		if(newSizeY > 512) {
			newSizeY = 512;
			float proportion = ((float)newSizeY)/((float)img1->sizeY);
			newSizeX = proportion*img1->sizeX;
		}
		img = MTcreateScaledImageCopy(img1, newSizeX, newSizeY);
		
		if(img == NULL) {
			img = img1;
		} else MTfreeImage(img1);
	} else img = img1;
	return img;
}

#include <jpeglib.h>
#include <stdio.h>

MTid3Tags * MTcreateID3tags(char *mp3path) {
	if(mp3path == NULL) return NULL;
	MTid3Tags *target = (MTid3Tags *)MTmalloc(sizeof(MTid3Tags));
	if(target == NULL) return NULL;
	ParseID3(mp3path, target);
	return target;
}

void MTfreeID3tags(MTid3Tags * tags) {
	if(tags == NULL) return;
	MTfree(tags);
	tags = NULL;
	return;
}

MTimage *MTloadImageFromID3(MTid3Tags *target)
{
	if(target == NULL) return NULL;
	if(target->ID3EncapsulatedPictureLength <= 0) return NULL;
	if(target->ID3EncapsulatedPictureType != JPEG_IMAGE && target->ID3EncapsulatedPictureType != PNG_IMAGE ) return NULL;
	FILE* infile;
	//char * buffer;

	infile = fopen ( target->filePath , "rb" );

	if (infile==NULL) return NULL;

	fseek (infile , target->ID3EncapsulatedPictureOffset , SEEK_SET);
	
	//buffer = (char*) malloc (target->ID3EncapsulatedPictureLength + 1);

	//if (buffer == NULL) return NULL;

	//fread (buffer,1,target->ID3EncapsulatedPictureLength,pFile);
	
	//buffer[id3tags.ID3EncapsulatedPictureLength+1] = '\0';

	//fclose (pFile);
	
	
  //FILE * pFile;
  //char buff[255];
OSL_IMAGE * img1;
  if(target->ID3EncapsulatedPictureType == JPEG_IMAGE) {
  	//sprintf(buff,"%s.jpg",imgpath);
  	img1 = oslLoadImageFileJPGfromMemory(infile, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
  } else img1 = oslLoadImageFilePNGfromMemory(infile, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
  fclose (infile);
  
	if(img1 == NULL) return NULL;
	
	MTimage* img = NULL;
	
	if(img1->sizeX > 512 || img1->sizeY > 512) {
		int newSizeX = 512;
		float proportion = ((float)newSizeX)/((float)img1->sizeX);
		int newSizeY = proportion*img1->sizeY;
	
		if(newSizeY > 512) {
			newSizeY = 512;
			float proportion = ((float)newSizeY)/((float)img1->sizeY);
			newSizeX = proportion*img1->sizeX;
		}
		img = MTcreateScaledImageCopy(img1, newSizeX, newSizeY);
		
		if(img == NULL) {
			img = img1;
		} else MTfreeImage(img1);
	} else img = img1;
	return img;
}

void MTdrawImage(MTimage* img) {
	if(img == NULL) return;
	
	int imgX = img->x;
	int imgY = img->y;
	
	img->x += MTdrawingOrigin.x;
	img->y += MTdrawingOrigin.y;
	
	oslEnableTexturing();
	oslDrawImage (img);
	oslDisableTexturing();
	
	img->x = imgX;
	img->y = imgY;
	
	//MTdrawString(img->x,img->y,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	return;
}

void MTdrawImageDeformed(MTimage* img, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) {
	if(img == NULL) return;
	
		/*OSL_PRECISE_VERTEX *vertices;
//		int rsX, rsY;
		float cX, cY, tmpY;
		float uVal, xVal;
		float uCoeff;
		float xCoeff;

		//Avoid divisions by zero
		if (img->stretchX == 0 || img->stretchY == 0)
			return;

		oslSetTexture(img);

		xVal = 0.f;
		uVal = img->offsetX0;

		//Mirrored?
		if (img->offsetX1 >= img->offsetX0)
			uCoeff = 64;
		else
			uCoeff = -64;

		//X scaling coefficient (pixels per stripe)
		xCoeff = uCoeff / ( (float)oslAbs(img->offsetX1 - img->offsetX0) / (float)img->stretchX );

		cX = 0;
		cY = 0;

		tmpY = cY + img->stretchY;
		xVal = cX;

		while (uVal != img->offsetX1)		{
			vertices = (OSL_PRECISE_VERTEX*)sceGuGetMemory(4 * sizeof(OSL_PRECISE_VERTEX));

			vertices[0].u = uVal;
			vertices[0].v = img->offsetY0;
			vertices[0].x = xVal+x0;
			vertices[0].y = y0;
			vertices[0].z = 0;

			vertices[2].u = uVal;
			vertices[2].v = img->offsetY1;
			vertices[2].x = xVal+x3;
			vertices[2].y = y3;
			vertices[2].z = 0;

			uVal += uCoeff;
			xVal += xCoeff;
			if (uVal > img->offsetX1)		{
				xVal = cX + img->stretchX;
				uVal = img->offsetX1;
			}

			vertices[1].u = uVal;
			vertices[1].v = img->offsetY0;
			vertices[1].x = xVal+x1;
			vertices[1].y = y1;
			vertices[1].z = 0;

			vertices[3].u = uVal;
			vertices[3].v = img->offsetY1;
			vertices[3].x = xVal+x2;
			vertices[3].y = y2;
			vertices[3].z = 0;

			sceGuDrawArray(GU_TRIANGLE_STRIP,GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_2D,4,0,vertices);
		}*/
	
		OSL_UVFLOAT_VERTEX *vertices;
		oslSetTexture(img);

		vertices = (OSL_UVFLOAT_VERTEX*)sceGuGetMemory(4 * sizeof(OSL_UVFLOAT_VERTEX));

		vertices[0].u = img->offsetX0;
		vertices[0].v = img->offsetY0;
		vertices[0].x = x0;
		vertices[0].y = y0;
		vertices[0].z = 0;

		vertices[1].u = img->offsetX1;
		vertices[1].v = img->offsetY0;
		vertices[1].x = x1;
		vertices[1].y = y1;
		vertices[1].z = 0;
		
		vertices[2].u = img->offsetX0;
		vertices[2].v = img->offsetY1;
		vertices[2].x = x3;
		vertices[2].y = y3;
		vertices[2].z = 0;
		
		vertices[3].u = img->offsetX1;
		vertices[3].v = img->offsetY1;
		vertices[3].x = x2;
		vertices[3].y = y2;
		vertices[3].z = 0;
		sceGuDisable( GU_CULL_FACE );//Gracias a esto da = el orden
		sceGuDrawArray(GU_TRIANGLE_STRIP,GU_TEXTURE_32BITF|GU_VERTEX_16BIT|GU_TRANSFORM_2D,4,0,vertices);
		sceGuEnable( GU_CULL_FACE );
	return;
}

typedef struct {
	float u, v;			// Texture Coordinates
	unsigned int color;	// Color value
	float nx, ny, nz;	// Normal values (NEW)
	float x, y, z;		// Position values
} Vertex;

struct Vertex
{
	float u, v;
	unsigned int color;
	float x,y,z;
};

struct Vertex __attribute__((aligned(16))) obj[36] = {
	{0, 0, 0xff6666ff,-1,-1, 1},
	{1, 0, 0xff6666ff, 1,-1, 1},
	{1, 1, 0xff6666ff, 1, 1, 1},
	{0, 0, 0xff6666ff,-1,-1, 1},
	{1, 1, 0xff6666ff, 1, 1, 1},
	{0, 1, 0xff6666ff,-1, 1, 1},
	
	{1, 1, 0xff66ff66, 1, 1, 1},
	{0, 1, 0xff66ff66, 1,-1, 1},
	{0, 0, 0xff66ff66, 1,-1,-1},
	{1, 1, 0xff66ff66, 1, 1, 1},
	{0, 0, 0xff66ff66, 1,-1,-1},
	{1, 0, 0xff66ff66, 1, 1,-1},
	
	{0, 1, 0xffff6666,-1, 1, 1},
	{0, 0, 0xffff6666, 1, 1, 1},
	{1, 0, 0xffff6666, 1, 1,-1},
	{0, 1, 0xffff6666,-1, 1, 1},
	{1, 0, 0xffff6666, 1, 1,-1},
	{1, 1, 0xffff6666,-1, 1,-1},
	
	
	{1, 1, 0xff6666ff, 1,-1,-1},
	{0, 1, 0xff6666ff,-1,-1,-1},
	{0, 0, 0xff6666ff,-1, 1,-1},
	{1, 1, 0xff6666ff, 1,-1,-1},
	{0, 0, 0xff6666ff,-1, 1,-1},
	{1, 0, 0xff6666ff, 1, 1,-1},
	
	{1, 0, 0xff66ff66,-1,-1,-1},
	{1, 1, 0xff66ff66,-1,-1, 1},
	{0, 1, 0xff66ff66,-1, 1, 1},
	{1, 0, 0xff66ff66,-1,-1,-1},
	{0, 1, 0xff66ff66,-1, 1, 1},
	{0, 0, 0xff66ff66,-1, 1,-1},
	
	{0, 1, 0xffff6666,-1,-1,-1},
	{0, 0, 0xffff6666, 1,-1,-1},
	{1, 0, 0xffff6666, 1,-1, 1},
	{0, 1, 0xffff6666,-1,-1,-1},
	{1, 0, 0xffff6666, 1,-1, 1},
	{1, 1, 0xffff6666,-1,-1, 1},
};

struct Vertex __attribute__((aligned(16))) mirror[6] = {
	{0, 0, 0xaa000000, -2.0f,  0, -2.0f},
	{2, 2, 0xaa000000,  2.0f,  0,  2.0f},
	{2, 0, 0xaa000000,  2.0f,  0, -2.0f},
	
	{0, 0, 0xaa000000, -2.0f,  0, -2.0f},
	{0, 2, 0xaa000000, -2.0f,  0,  2.0f},
	{2, 2, 0xaa000000,  2.0f,  0,  2.0f}
};

struct Vertex __attribute__((aligned(16))) border[6] = {
	{0, 0, 0xff0055aa, -2.125f,  -0.01, -2.125f},
	{2, 2, 0xff0055aa,  2.125f,  -0.01,  2.125f},
	{2, 0, 0xff0055aa,  2.125f,  -0.01, -2.125f},
	
	{0, 0, 0xff0055aa, -2.125f,  -0.01, -2.125f},
	{0, 2, 0xff0055aa, -2.125f,  -0.01,  2.125f},
	{2, 2, 0xff0055aa,  2.125f,  -0.01,  2.125f}
};

float val = 0.0f;
void render3Dtest(MTimage* img) {
	float move = sinf(((float)val / 180.0f) * GU_PI);
	if(move < 0) {
		move *= -1;
	}
	move += 1;
	float rot = ((float)val / 180.0f) * GU_PI;
	/*sceGumMatrixMode(GU_VIEW);
	{
		ScePspFVector3 pos = {0,-0.5f,-5.5f};
		ScePspFVector3 rota = {rot*1.0f, rot*0.9f, rot*0.8f};
		sceGumLoadIdentity();
		sceGumTranslate(&pos);
		sceGumRotateXYZ(&rota);
	}*/
	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
	{
		ScePspFVector3 pos = {0,0,-5.5f};
		ScePspFVector3 rota = {rot*1.0f, rot*0.9f, rot*0.8f};
		sceGumTranslate(&pos);
		sceGumRotateXYZ(&rota);
	}
	// draw mirrored view
	//sceGumPushMatrix();
	{
		ScePspFVector3 scale = {((float)varY*0.001f),-((float)varY*0.001f),((float)varY*0.001f)};
		sceGumScale(&scale);
	}
	OSL_PRECISE_VERTEX *vertices;
	vertices = (OSL_PRECISE_VERTEX*)sceGuGetMemory(4 * sizeof(OSL_PRECISE_VERTEX));
	oslSetTexture(img);
	
	//sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGBA);
	//sceGuTexFilter(GU_LINEAR,GU_LINEAR);
	sceGuTexScale((float)img->sizeX/(float)img->sysSizeX,(float)img->sizeY/(float)img->sysSizeY);
	//sceGuTexOffset(0.0f,0.0f);
	//sceGuShadeModel(GU_SMOOTH);

	vertices[0].u = 0;
	vertices[0].v = 0;
	vertices[0].x = 0;
	vertices[0].y = 0;
	vertices[0].z = 0;

	vertices[1].u = 1;
	vertices[1].v = 0;
	vertices[1].x = (float)img->sizeX;
	vertices[1].y = 0;
	vertices[1].z = 0;

	vertices[2].u = 0;
	vertices[2].v = 1;
	vertices[2].x = 0;
	vertices[2].y = (float)img->sizeY;
	vertices[2].z = 0;
	
	vertices[3].u = 1;
	vertices[3].v = 1;
	vertices[3].x = (float)img->sizeX;
	vertices[3].y = (float)img->sizeY;
	vertices[3].z = 0;

	//sceGuEnable(GU_DEPTH_TEST);
	//sceGuDepthRange(0xc350,0x2710);
	//sceGuClearDepth(0);
	//sceGuClear(GU_DEPTH_BUFFER_BIT);
	
	sceGuClearDepth(0);
	sceGuClear(GU_DEPTH_BUFFER_BIT);
	
	sceGuColor(0xffffffff);
	sceGuDisable(GU_BLEND);
	
	sceGuDisable( GU_CULL_FACE );//Gracias a esto da = el orden
	//sceGumDrawArray(GU_TRIANGLES,GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D,3,0,vertices);
	sceGumDrawArray(GU_TRIANGLE_STRIP, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 4, 0, vertices);
	sceGuEnable( GU_CULL_FACE );
	
	sceGuClearDepth(0);
	sceGuClear(GU_DEPTH_BUFFER_BIT);

	//sceGuDisable(GU_DEPTH_TEST);
	val += 0.5f;
	//sceGumPopMatrix();
}
float cam_pos = 0.25f, cam_rot = 0.042f;
void DrawThing(MTimage* img)
{
	sceGumMatrixMode(GU_PROJECTION);
	sceGumLoadIdentity();
	
	sceGumPerspective(75.0f,16.0f/9.0f,0.5f,1000.0f);
	
	cam_pos = (float)1*0.25f;
	cam_rot = (float)1*0.042f;
	// render scene for the left eye
	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();
	{
	ScePspFVector3 pos = { cam_pos, 0, 0 };
	ScePspFVector3 rot = { 0, cam_rot, 0 };
	sceGumTranslate(&pos);
	sceGumRotateXYZ(&rot);
	}
	sceGuPixelMask(RGB(0,255,255)); // Only cyan
	render3Dtest(img);
	// render scene for the right eye
	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();
	{
		ScePspFVector3 pos = { -cam_pos, 0, 0 };
		ScePspFVector3 rot = { 0, -cam_rot, 0 };
		sceGumTranslate(&pos);
		sceGumRotateXYZ(&rot);
	}
	sceGuPixelMask(RGB(255,0,0)); // Only red
	render3Dtest(img);

	// UI
	sceGuPixelMask(0x00000000);
	
	sceGuDisable(GU_DEPTH_TEST);
	sceGuEnable(GU_BLEND);
	sceGuBlendFunc(GU_ADD,GU_SRC_ALPHA,GU_ONE_MINUS_SRC_ALPHA,0,0);
	
}


#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)
#define PIXEL_SIZE (4) /* change this if you change to another screenmode */
#define FRAME_SIZE (BUF_WIDTH * SCR_HEIGHT * PIXEL_SIZE)
#define ZBUF_SIZE (BUF_WIDTH SCR_HEIGHT * 2) /* zbuffer seems to be 16-bit? */

void test3d(MTimage* img) {
	if(img == NULL) return;
	
	//sceGuDepthBuffer((void*)0x110000,BUF_WIDTH);
	sceGuDepthRange(0xc350,0x2710);
	sceGuScissor(0,0,SCR_WIDTH,SCR_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuFrontFace(GU_CCW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_CULL_FACE);
	
			float move = sinf(((float)val / 180.0f) * GU_PI);
		if(move < 0) {
			move *= -1;
		}
		move += 1;
		
		float rot = ((float)val / 180.0f) * GU_PI;

		// clear screen

			//sceGuClearColor(0xff554433);
		sceGuClearDepth(0);
		sceGuClearStencil(0);
		sceGuClear(GU_STENCIL_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);
		
				sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();
		//sceGumOrtho(0.0f, 480.0f, 0.0f, 272.0f, 0.0f, 1.0f);
		sceGumPerspective(75.0f,16.0f/9.0f,0.5f,1000.0f);
		
				sceGumMatrixMode(GU_VIEW);
		{
			ScePspFVector3 pos = {0,-0.5f,-5.5f};
			ScePspFVector3 rot = {(30.0f * GU_PI) / 180.0f, (val * 0.2f * GU_PI) / 180.0f, 0.0f};

			sceGumLoadIdentity();
			sceGumTranslate(&pos);
			sceGumRotateXYZ(&rot);
		}
		
				sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		
		// draw mirrored view

		sceGumPushMatrix();
		{
			ScePspFVector3 scale = {1,-1,1};
			sceGumScale(&scale);
		}
		
		sceGuFrontFace(GU_CCW);
		sceGuEnable(GU_STENCIL_TEST); // Stencil test
		sceGuDepthMask(GU_TRUE);
		sceGuStencilFunc(GU_ALWAYS, 1, 1); // always set 1 bit in 1 bit mask
		sceGuStencilOp(GU_KEEP, GU_KEEP, GU_REPLACE); // keep value on failed test (fail and zfail) and replace on pass
		sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 6, 0, mirror);
		sceGuDepthMask(GU_FALSE);
		
		oslSetTexture(img);
				//sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGBA);
		//sceGuTexFilter(GU_LINEAR,GU_LINEAR);
		//sceGuTexScale(1.0f,1.0f);
		sceGuTexScale((float)img->sizeX/(float)img->sysSizeX,(float)img->sizeY/(float)img->sysSizeY);
		//sceGuTexOffset(0.0f,0.0f);
		
		sceGuEnable(GU_TEXTURE_2D);
		sceGuFrontFace(GU_CW);
		sceGuStencilFunc(GU_EQUAL, 1, 1); // allow drawing where stencil is 1
		sceGuStencilOp(GU_KEEP, GU_KEEP, GU_KEEP); // keep the stencil buffer unchanged

		{
			ScePspFVector3 pos = {0,move,0};
			ScePspFVector3 rvec = {0,rot * -0.83f, 0};

			sceGumTranslate(&pos);
			sceGumRotateXYZ(&rvec);
		}

		sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 36, 0, obj);
		sceGuDisable(GU_TEXTURE_2D);
		sceGuDisable(GU_STENCIL_TEST);
		
		sceGumPopMatrix();

		// draw normal view
		sceGuEnable(GU_TEXTURE_2D);
		oslSetTexture(img);
				//sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGBA);
		//sceGuTexFilter(GU_LINEAR,GU_LINEAR);
		//sceGuTexScale(1.0f,1.0f);
		//sceGuTexOffset(0.0f,0.0f);
		
		sceGuFrontFace(GU_CCW);
		sceGuEnable(GU_BLEND);
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
		sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 6, 0, mirror);
		sceGuDisable(GU_BLEND);
		sceGuDisable(GU_TEXTURE_2D);
		
		sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 6, 0, border);
		sceGuEnable(GU_TEXTURE_2D);
		oslSetTexture(img);
		//sceGuTexMode(GU_PSM_T8,0,0,0); // 8-bit image
		//sceGuTexImage(0,256,256,256,img->data);
		//sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGBA);
		//sceGuTexFilter(GU_LINEAR,GU_LINEAR);
		//sceGuTexScale(1.0f,1.0f);
		//sceGuTexOffset(0.0f,0.0f);
		//sceGuAmbientColor(0xffffffff);
		//sceGuTexMode(GU_PSM_4444,0,0,0);
		//sceGuTexImage(0,64,64,64,logo_start);
		//sceGuTexFunc(GU_TFX_ADD,GU_TCC_RGB);
		//sceGuTexFilter(GU_LINEAR,GU_LINEAR);
		//sceGuTexWrap(GU_REPEAT, GU_REPEAT);
		//sceGuTexMapMode(GU_TEXTURE_COORDS,0,1);
		//sceGuShadeModel(GU_SMOOTH);
		//sceGuTexScale(1.0f,1.0f);
		//sceGuTexOffset(0.0f,0.0f);

		{
			ScePspFVector3 pos = {0,move,0};
			ScePspFVector3 rvec = {0,rot * -0.83f, 0};

			sceGumTranslate(&pos);
			sceGumRotateXYZ(&rvec);
		}

		sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 36, 0, obj);
		DrawThing(img);
		//sceKernelDcacheWritebackInvalidateAll();
		//DrawThing();
		
		sceGuEnable(GU_TEXTURE_2D);
		sceGuDisable(GU_DEPTH_TEST);
		sceGuDisable(GU_STENCIL_TEST);
	/*sceGuDepthRange( 65535, 0);
	sceGuDepthFunc( GU_GEQUAL );
	sceGuEnable( GU_DEPTH_TEST );
	//sceGuFrontFace( GU_CW );					// Commented out, not needed (NEW)
	//sceGuEnable( GU_CULL_FACE );					// In this lesson (NEW)
	sceGuShadeModel( GU_SMOOTH );
	sceGuEnable( GU_CLIP_PLANES );
 
	sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);
	
	sceGuDepthRange( 10000, 10000);
	sceGuDepthFunc( GU_GEQUAL );
	sceGuEnable( GU_DEPTH_TEST );*/
	
		/*sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();		
		//sceGumOrtho(0.0f, 480.0f, 0.0f, 272.0f, 0.0f, 1.0f);
		sceGumMatrixMode(GU_VIEW);
		sceGumLoadIdentity();
		
	// clear screen
	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);	// Clears the Color and Depth Buffer
	sceGumMatrixMode(GU_MODEL);								// Selects the Model Matrix
	sceGumLoadIdentity();									// And Reset it/
	// Move 1.5 units left and 3 units back
		ScePspFVector3 move = { -1.5f, 0.0f, -3.0f };
		
		sceGumTranslate( &move ); 
		sceGumRotateY( rtri );
		
	DrawThing();*/
	val += 1.0f; //(1.0f * dt);
	
	//sceGuDisable( GU_DEPTH_TEST );
	
		return;
}

void MTsetImageCenterMiddle(MTimage* img) {
	oslSetImageRotCenter(img);
	return;
}

void MTmirrorImageH(MTimage* img) {
	oslMirrorImageH(img);
	return;
}

void MTmirrorImageV(MTimage* img) {
	oslMirrorImageV(img);
	return;
}

int MTimageIsMirroredH(MTimage* img) {
	return oslImageIsMirroredH(img);
}

int MTimageIsMirroredV(MTimage* img) {
	return oslImageIsMirroredV(img);
}

/*MTimage* MTcreateImageCopy(MTimage* img) {
	return oslCreateImageCopy(img, OSL_IN_RAM | OSL_SWIZZLED);
}*/

/*void MTclearImage(MTimage* img, MTcolor color) {
	oslClearImage(img, color);
	return;
}*/

void MTsaveImageToPNG(MTimage* img, const char * filename, int saveAlpha) {
	oslWriteImageFilePNG(img, filename, saveAlpha);
	return;
}

void MTlockScreen() {
	if(screenLocked != 1) {
		screenLocked = 1;
		MTdelay(100000);//Dejamos que la pantalla se estabilice
	}
}

void MTunLockScreen() {
	screenLocked = 0;
}

void MTsaveScreenToPNG(const char * filename) {
	MTsaveImageToPNG(OSL_SECONDARY_BUFFER, filename, 0);
	return;
}

MTimage* MTcreateImageCopy(MTimage* src) {
	return oslCreateImageCopy (src, OSL_IN_RAM);
}

MTimage* MTgetScreenImage() {
	MTimage* img = MTcreateImageCopy(OSL_SECONDARY_BUFFER);
	/*sizeX
	sizeY
	sysSizeX
	sysSizeY
	realSizeX
	realSizeY
	//void *data
	flags
	totalSize
	location
	pixelFormat
	//OSL_PALETTE *palette
	frameSizeX
	frameSizeY
	x
	y
	stretchX
	stretchY
	offsetX0
	offsetY0
	offsetX1
	offsetY1
	centerX
	centerY
	angle*/
	
	/*if(img->palette != NULL) {
		img->palette->pixelFormat = MTwallpaperImage->palette->pixelFormat;
		img->palette->nElements = MTwallpaperImage->palette->nElements;
		img->palette->location = MTwallpaperImage->palette->location;
	}
	
	img->sizeX = MTwallpaperImage->sizeX;
	img->sizeY = MTwallpaperImage->sizeY;
	img->sysSizeX = MTwallpaperImage->sysSizeX;
	img->sysSizeY = MTwallpaperImage->sysSizeY;
	img->realSizeX = MTwallpaperImage->realSizeX;
	img->realSizeY = MTwallpaperImage->realSizeY;
	img->flags = MTwallpaperImage->flags;
	img->totalSize = MTwallpaperImage->totalSize;
	img->location = MTwallpaperImage->location;
	img->pixelFormat = MTwallpaperImage->pixelFormat;
	img->frameSizeX = MTwallpaperImage->frameSizeX;
	img->frameSizeY = MTwallpaperImage->frameSizeY;
	img->x = MTwallpaperImage->x;
	img->y = MTwallpaperImage->y;
	img->stretchX = MTwallpaperImage->stretchX;
	img->stretchY = MTwallpaperImage->stretchY;
	img->offsetX0 = MTwallpaperImage->offsetX0;
	img->offsetY0 = MTwallpaperImage->offsetY0;
	img->offsetX1 = MTwallpaperImage->offsetX1;
	img->offsetY1 = MTwallpaperImage->offsetY1;
	img->centerX = MTwallpaperImage->centerX;
	img->centerY = MTwallpaperImage->centerY;
	img->angle = MTwallpaperImage->angle;*/
	return img;
}

void MTresetImageProperties(MTimage* img) {
	oslResetImageProperties	(img);
	return;
}



void MTdrawCross(int x,int y,int width,int height,MTcolor color) {
	MTdrawLine(x+MTround(width/2), y, x+MTround(width/2), y+height, color);
	MTdrawLine(x, y+MTround(height/2), x+width, y+MTround(height/2), color);
	return;
}

void MTdrawX(int x,int y,int width,int height,MTcolor color) {
	MTdrawLine(x, y, x+width, y+height, color);
	MTdrawLine(x, y+height, x+width, y, color);
	return;
}

void MTdrawPixel(int x, int y, MTcolor color) {
	
	color = MTcolorBlend(color);
	
	COLOR_VERTEX* vertices;
	vertices = (COLOR_VERTEX*)sceGuGetMemory(1 * sizeof(COLOR_VERTEX));
	vertices[0].color = color;
	vertices[0].x = x + MTdrawingOrigin.x;
	vertices[0].y = y + MTdrawingOrigin.y;
	vertices[0].z = 0;
	//sceGuDisable(GU_TEXTURE_2D);
	//oslDisableTexturing();
	sceGuDrawArray(GU_POINTS, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 1, 0, vertices);
	//oslEnableTexturing();
	//sceKernelDcacheWritebackRange(vertices, 1 * sizeof(COLOR_VERTEX));
	//sceGuEnable(GU_TEXTURE_2D);
	////MTdrawString(x,y,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	return;
}

//Funcion sacada de aqui: http://www.cs.unc.edu/~mcmillan/comp136/Lecture7/circle.html MUCHAS GRACIAS
void circunferencePoints(int cx, int cy, int x, int y,MTcolor colorCir)
{
	if (x == 0) {
		MTdrawPixel(cx, cy + y,colorCir);
		MTdrawPixel(cx, cy - y,colorCir);
		MTdrawPixel(cx + y, cy,colorCir);
		MTdrawPixel(cx - y, cy,colorCir);
	} else 
	if (x == y) {
		MTdrawPixel(cx + x, cy + y,colorCir);
		MTdrawPixel(cx - x, cy + y,colorCir);
		MTdrawPixel(cx + x, cy - y,colorCir);
		MTdrawPixel(cx - x, cy - y,colorCir);
	} else 
	if (x < y) {
		MTdrawPixel(cx + x, cy + y,colorCir);
		MTdrawPixel(cx - x, cy + y,colorCir);
		MTdrawPixel(cx + x, cy - y,colorCir);
		MTdrawPixel(cx - x, cy - y,colorCir);
		MTdrawPixel(cx + y, cy + x,colorCir);
		MTdrawPixel(cx - y, cy + x,colorCir);
		MTdrawPixel(cx + y, cy - x,colorCir);
		MTdrawPixel(cx - y, cy - x,colorCir);
	}
	return;
}
void MTdrawPixelCircunference(int xCenter,int yCenter,int radius,MTcolor colorCir) {		
	int x = 0;
	int y = radius;
	int p = (5 - radius*4)/4;
	
	xCenter += MTdrawingOrigin.x;
	yCenter += MTdrawingOrigin.y;

	circunferencePoints(xCenter, yCenter, x, y,colorCir);
	while (x < y) {
		x++;
		if (p < 0) {
			p += 2*x+1;
		} else {
			y--;
			p += 2*(x-y)+1;
		}
		circunferencePoints(xCenter, yCenter, x, y,colorCir);
	}
	return;
}

void MTdrawTriangleWithGradient(int x0, int y0, int x1, int y1, int x2, int y2, MTcolor c1, MTcolor c2, MTcolor c3) {
	
	c1 = MTcolorBlend(c1);
	c2 = MTcolorBlend(c2);
	c3 = MTcolorBlend(c3);
	
	COLOR_VERTEX* vertices;
	vertices = (COLOR_VERTEX*)sceGuGetMemory(3 * sizeof(COLOR_VERTEX));
	
	vertices[0].color = c1;
	vertices[0].x = x0 + MTdrawingOrigin.x;
	vertices[0].y = y0 + MTdrawingOrigin.y;
	vertices[0].z = 0;
	
	vertices[1].color = c2;
	vertices[1].x = x1 + MTdrawingOrigin.x;
	vertices[1].y = y1 + MTdrawingOrigin.y;
	vertices[1].z = 0;
	
	vertices[2].color = c3;
	vertices[2].x = x2 + MTdrawingOrigin.x;
	vertices[2].y = y2 + MTdrawingOrigin.y;
	vertices[2].z = 0;
	
	//sceGuDisable(GU_TEXTURE_2D);
	//oslDisableTexturing();
	sceGuDisable( GU_CULL_FACE );//Gracias a esto da = el orden
	sceGuDrawArray(GU_TRIANGLES, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 3, 0, vertices);
	//sceKernelDcacheWritebackRange(vertices, 3 * sizeof(COLOR_VERTEX));
	sceGuEnable( GU_CULL_FACE );
	//sceGuEnable(GU_TEXTURE_2D);
	//MTdrawString(x0,y0,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
}

void MTdrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, MTcolor c1) {
	MTdrawTriangleWithGradient(x0, y0, x1, y1, x2, y2, c1, c1, c1);
	return;
}

void MTdrawCircleSectionWithGradient(int xCenter,int yCenter,int radius,MTcolor originColorCir,MTcolor colorCir,float degreesInit, float degrees, int sections) {		
	if(sections < 2) return;
	
	int i;
	//int sections = 30; //number of triangles to use to estimate a circle
	// (a higher number yields a more perfect circle)
	
	originColorCir = MTcolorBlend(originColorCir);
	colorCir = MTcolorBlend(colorCir);
	
	COLOR_VERTEX* vertices;
	vertices = (COLOR_VERTEX*)sceGuGetMemory((sections+2) * sizeof(COLOR_VERTEX));
	
	vertices[0].color = originColorCir;
	vertices[0].x = xCenter + MTdrawingOrigin.x;
	vertices[0].y = yCenter + MTdrawingOrigin.y;
	vertices[0].z = 0;
	
	float angValue = ((float)degrees / (float)sections);
	
	for(i = 0; i < (sections+1);i++) { // make $section number of circles
		
		float angValueDef = (((float)i*angValue)+degreesInit);
		
		vertices[i+1].color = colorCir;
		vertices[i+1].x = xCenter + oslCos(angValueDef,radius) + MTdrawingOrigin.x;
		vertices[i+1].y = yCenter + oslSin(angValueDef,radius) + MTdrawingOrigin.y;
		vertices[i+1].z = 0;
	}
	
	//sceGuDisable(GU_TEXTURE_2D);
	//oslDisableTexturing();
	sceGuDisable( GU_CULL_FACE );//Gracias a esto da = el orden
	sceGuDrawArray(GU_TRIANGLE_FAN, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, (sections+2), 0, vertices);
	//sceKernelDcacheWritebackRange(vertices, (sections+2) * sizeof(COLOR_VERTEX));
	sceGuEnable( GU_CULL_FACE );
	//sceGuEnable(GU_TEXTURE_2D);
	//MTdrawString(xCenter,yCenter,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	return;
}

void MTdrawCircleSection(int xCenter,int yCenter,int radius,MTcolor originColorCir,float degreesInit, float degrees, int sections) {
	MTdrawCircleSectionWithGradient( xCenter, yCenter, radius, originColorCir, originColorCir, degreesInit, degrees, sections);
	return;
}

void MTdrawCircleWithGradient(int xCenter,int yCenter,int radius,MTcolor originColorCir,MTcolor colorCir,int sections) {		
	
	if(sections < 3) return;
	
	int i;
	//int sections = 30; //number of triangles to use to estimate a circle
	// (a higher number yields a more perfect circle)
	
	originColorCir = MTcolorBlend(originColorCir);
	colorCir = MTcolorBlend(colorCir);
	
	COLOR_VERTEX* vertices;
	vertices = (COLOR_VERTEX*)sceGuGetMemory((sections+2) * sizeof(COLOR_VERTEX));
	
	vertices[0].color = originColorCir;
	vertices[0].x = xCenter + MTdrawingOrigin.x;
	vertices[0].y = yCenter + MTdrawingOrigin.y;
	vertices[0].z = 0;
	
	float angValue = (360.0f/ (float)sections);
	
	for(i = 1; i <= (sections+1);i++) { // make $section number of circles
		
		float angValueDef = ((float)i*angValue);
		
		vertices[i].color = colorCir;
		vertices[i].x = xCenter + oslCos(angValueDef,radius) + MTdrawingOrigin.x;
		vertices[i].y = yCenter + oslSin(angValueDef,radius) + MTdrawingOrigin.y;
		vertices[i].z = 0;
	}
	
	//sceGuDisable(GU_TEXTURE_2D);
	//oslDisableTexturing();
	sceGuDisable( GU_CULL_FACE );//Gracias a esto da = el orden
	sceGuDrawArray(GU_TRIANGLE_FAN, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, (sections+2), 0, vertices);
	//sceKernelDcacheWritebackRange(vertices, (sections+2) * sizeof(COLOR_VERTEX));
	sceGuEnable( GU_CULL_FACE );
	//sceGuEnable(GU_TEXTURE_2D);
	
	//MTdrawString(xCenter,yCenter,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	return;
}

void MTdrawCircle(int xCenter,int yCenter,int radius,MTcolor colorCir, int sections) {		
	MTdrawCircleWithGradient(xCenter,yCenter,radius,colorCir,colorCir, sections);
	return;
}

void MTdrawThickLineWithGradient(int x0, int y0, int x1, int y1, MTcolor color0, MTcolor color1, int w0, int w1, int roundEnd) {
	
	if(w0 == w1 && w0 < 2) MTdrawLineWithGradient(x0, y0, x1, y1, color0, color1);
	
	x0 += MTdrawingOrigin.x;
	y0 += MTdrawingOrigin.y;
	x1 += MTdrawingOrigin.x;
	y1 += MTdrawingOrigin.y;
	
	MTcolor color0bak = color0;
	MTcolor color1bak = color1;
	
	color0 = MTcolorBlend(color0);
	color1 = MTcolorBlend(color1);
	
	COLOR_VERTEX* vertices;
	vertices = (COLOR_VERTEX*)sceGuGetMemory(4 * sizeof(COLOR_VERTEX));
	
	float med0 = w0/2;
	float med1 = w1/2;
	
	float angle = atan2(y1 - y0, x1 - x0);
	float sinangle = sin(angle);
	float cosangle = cos(angle);
	float t2sina1 = med0 * sinangle;
	float t2cosa1 = med0 * cosangle;
	float t2sina2 = med1 * sinangle;
	float t2cosa2 = med1 * cosangle;
	
	vertices[0].color = color0;
	vertices[0].x = x0 + t2sina1;
	vertices[0].y = y0 - t2cosa1;
	vertices[0].z = 0;
	
	vertices[1].color = color1;
	vertices[1].x = x1 + t2sina2;
	vertices[1].y = y1 - t2cosa2;
	vertices[1].z = 0;
	
	vertices[3].color = color1;
	vertices[3].x = x1 - t2sina2;
	vertices[3].y = y1 + t2cosa2;
	vertices[3].z = 0;
	
	vertices[2].color = color0;
	vertices[2].x = x0 - t2sina1;
	vertices[2].y = y0 + t2cosa1;
	vertices[2].z = 0;
	
	//sceGuDisable(GU_TEXTURE_2D);
	//oslDisableTexturing();
	sceGuDisable( GU_CULL_FACE );//Gracias a esto da = el orden
	sceGuDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 4, 0, vertices);
	//sceKernelDcacheWritebackRange(vertices, 4 * sizeof(COLOR_VERTEX));
	sceGuEnable( GU_CULL_FACE );
	//sceGuEnable(GU_TEXTURE_2D);
	
	if(roundEnd > 1) {
		float posX = oslCos(MTrad2deg(angle),0.2);
		float posY = oslSin(MTrad2deg(angle),0.2);
		MTdrawCircleSection(x0 + posX,y0 + posY,med0,color0bak,(MTrad2deg(angle)+90), 180, roundEnd);
		MTdrawCircleSection(x1 - posX,y1 - posY,med1,color1bak,(MTrad2deg(angle)+90+180), 180, roundEnd);
	}
	
	//MTdrawString(x0,y0,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	
	return;
}

void MTdrawThickLine(int x0, int y0, int x1, int y1, MTcolor color0, int w0, int w1, int roundEnd) {
	MTdrawThickLineWithGradient(x0, y0, x1, y1, color0, color0, w0, w1, roundEnd);
	return;
}

void MTdrawThickLineWithBorderGradient(int x0, int y0, int x1, int y1, MTcolor color0, MTcolor color1, MTcolor borderColor0, MTcolor borderColor1, int w0, int w1, int roundEnd) {
	
	if(w0 == w1 && w0 <= 3) MTdrawLineWithGradient(x0, y0, x1, y1, color0, color1);
	if(borderColor0 == color0 && borderColor1 == color1) MTdrawThickLineWithGradient(x0, y0, x1, y1, color0, color1, w0, w1, roundEnd);
	
	x0 += MTdrawingOrigin.x;
	y0 += MTdrawingOrigin.y;
	x1 += MTdrawingOrigin.x;
	y1 += MTdrawingOrigin.y;
	
	MTcolor color0bak = color0;
	MTcolor color1bak = color1;
	MTcolor borderColor0bak = borderColor0;
	MTcolor borderColor1bak = borderColor1;
	
	color0 = MTcolorBlend(color0);
	color1 = MTcolorBlend(color1);
	borderColor0 = MTcolorBlend(borderColor0);
	borderColor1 = MTcolorBlend(borderColor1);
	
	COLOR_VERTEX* vertices;
	vertices = (COLOR_VERTEX*)sceGuGetMemory(6 * sizeof(COLOR_VERTEX));
	
	float med0 = w0/2;
	float med1 = w1/2;
	
	float angle = atan2(y1 - y0, x1 - x0);
	float sinangle = sin(angle);
	float cosangle = cos(angle);
	float t2sina1 = med0 * sinangle;
	float t2cosa1 = med0 * cosangle;
	float t2sina2 = med1 * sinangle;
	float t2cosa2 = med1 * cosangle;
	
	vertices[0].color = color0;
	vertices[0].x = x0;
	vertices[0].y = y0;
	vertices[0].z = 0;
	
	vertices[1].color = borderColor0;
	vertices[1].x = x0 + t2sina1;
	vertices[1].y = y0 - t2cosa1;
	vertices[1].z = 0;
	
	vertices[2].color = borderColor1;
	vertices[2].x = x1 + t2sina2;
	vertices[2].y = y1 - t2cosa2;
	vertices[2].z = 0;
	
	vertices[3].color = color1;
	vertices[3].x = x1;
	vertices[3].y = y1;
	vertices[3].z = 0;
	
	vertices[4].color = borderColor1;
	vertices[4].x = x1 - t2sina2;
	vertices[4].y = y1 + t2cosa2;
	vertices[4].z = 0;
	
	vertices[5].color = borderColor0;
	vertices[5].x = x0 - t2sina1;
	vertices[5].y = y0 + t2cosa1;
	vertices[5].z = 0;
	
	//sceGuDisable(GU_TEXTURE_2D);
	//oslDisableTexturing();
	sceGuDisable( GU_CULL_FACE );//Gracias a esto da = el orden
	sceGuDrawArray(GU_TRIANGLE_FAN, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 6, 0, vertices);
	//sceKernelDcacheWritebackRange(vertices, 6 * sizeof(COLOR_VERTEX));
	sceGuEnable( GU_CULL_FACE );
	//sceGuEnable(GU_TEXTURE_2D);
	
	if(roundEnd > 0) {
		float posX = 0;//oslCos(MTrad2deg(angle),correctCornerAlign);
		float posY = 0;//oslSin(MTrad2deg(angle),correctCornerAlign);
		MTdrawCircleSectionWithGradient(x0 + posX,y0 + posY,med0,color0bak,borderColor0bak,(MTrad2deg(angle)+90), 180, roundEnd);
		MTdrawCircleSectionWithGradient(x1 - posX ,y1 - posY,med1,color1bak,borderColor1bak,(MTrad2deg(angle)+90+180), 180, roundEnd);
	}
	
	//MTdrawString(x0,y0,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	
	return;
}

void MTdrawSmoothThickLine(int x0, int y0, int x1, int y1, MTcolor color, int w0, int w1, int roundEnd) {
	MTcolor borderColor = RGBA(R(color),G(color),B(color),0);
	MTdrawThickLineWithBorderGradient(x0, y0, x1, y1, color, color, borderColor, borderColor, w0, w1, roundEnd);
	return;
}

void MTdrawCircunference(int xCenter, int yCenter, int radius, MTcolor colorCir, int sections) {		
	
	if(sections < 3) return;
	
	int i;
	//int sections = 30; //number of triangles to use to estimate a circle
	// (a higher number yields a more perfect circle)
	
	colorCir = MTcolorBlend(colorCir);
	
	COLOR_VERTEX* vertices;
	vertices = (COLOR_VERTEX*)sceGuGetMemory((sections + 1) * sizeof(COLOR_VERTEX));
	
	float angValue = (360.0f/ (float)sections);
	
	for(i = 0; i <= sections;i++) { // make section number of circles
		float angValueDef = ((float)i*angValue);
		
		vertices[i].color = colorCir;
		vertices[i].x = xCenter + oslCos(angValueDef,radius) + MTdrawingOrigin.x;
		vertices[i].y = yCenter + oslSin(angValueDef,radius) + MTdrawingOrigin.y;
		vertices[i].z = 0;
	}
	
	//sceGuDisable(GU_TEXTURE_2D);
	//oslDisableTexturing();
	sceGuDrawArray(GU_LINE_STRIP, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, (sections + 1), 0, vertices);
	//sceKernelDcacheWritebackRange(vertices, (sections + 1) * sizeof(COLOR_VERTEX));
	//sceGuEnable(GU_TEXTURE_2D);
	
	//MTdrawString(xCenter,yCenter,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	
	return;
}

void MTdrawThickCircunferenceWithGradient(int xCenter, int yCenter, int radius, MTcolor colorOut, MTcolor colorIn, int w0, int sections) {
	
	if(sections < 3) return;
	
	xCenter += MTdrawingOrigin.x;
	yCenter += MTdrawingOrigin.y;
	
	int i;
	//int sections = 30; //number of triangles to use to estimate a circle
	// (a higher number yields a more perfect circle)
	
	colorOut = MTcolorBlend(colorOut);
	colorIn = MTcolorBlend(colorIn);
	
	COLOR_VERTEX* vertices;
	vertices = (COLOR_VERTEX*)sceGuGetMemory((2*sections + 2) * sizeof(COLOR_VERTEX));
	
	float med = w0/2;
	
	float angValue = (360.0f/ (float)sections)/2;
	
	for(i = 0; i <= (sections*2);i+=2) { // make $section number of circles
		/*MTdrawPixel(point1x, point1y, colorCir);
		MTdrawPixel(point3x, point3y, colorCir);
		
		char text[100];
		sprintf(text,"%i",i);
		MTdrawString(point1x,point1y,text);
		sprintf(text,"%i",i+1);
		MTdrawString(point3x,point3y,text);*/
		
		float angValueDef = ((float)i*angValue);
		
		vertices[i].color = colorIn;
		vertices[i].x = xCenter + oslCos(angValueDef,radius-med);
		vertices[i].y = yCenter + oslSin(angValueDef,radius-med);
		vertices[i].z = 0;
		
		vertices[(i+1)].color = colorOut;
		vertices[(i+1)].x = xCenter + oslCos(angValueDef,radius+med);
		vertices[(i+1)].y = yCenter + oslSin(angValueDef,radius+med);
		vertices[(i+1)].z = 0;
	}
	//MTdrawString(xCenter,yCenter,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	
	//sceGuDisable(GU_TEXTURE_2D);
	//oslDisableTexturing();
	sceGuDisable( GU_CULL_FACE );//Gracias a esto da = el orden
	sceGuDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, (2*sections + 2), 0, vertices);
	//sceKernelDcacheWritebackRange(vertices, (2*sections + 2) * sizeof(COLOR_VERTEX));
	sceGuEnable( GU_CULL_FACE );//Gracias a esto da = el orden
	//sceGuEnable(GU_TEXTURE_2D);
	
	//MTdrawString(xCenter,yCenter,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	
	return;
}

void MTdrawThickCircunference(int xCenter, int yCenter, int radius, MTcolor colorCir, int w0, int sections) {
	MTdrawThickCircunferenceWithGradient(xCenter, yCenter, radius, colorCir, colorCir, w0, sections);
	return;
}

/*void MTdrawThickCircunference(int xCenter, int yCenter, int radius, MTcolor colorCir, int w0, int sections) {
	int i;
	//int sections = 30; //number of triangles to use to estimate a circle
	// (a higher number yields a more perfect circle)
	
	int point0x = 0;
	int point0y = 0;
	int point1x = 0;
	int point1y = 0;
	
	for(i = 0; i <= sections;i++) { // make $section number of circles
		point1x = xCenter + oslCos((float)i*(360.0f/ (float)sections),radius);
		point1y = yCenter + oslSin((float)i*(360.0f/ (float)sections),radius);
		if(i>0) MTdrawThickLine(point0x, point0y, point1x, point1y, colorCir, w0, w0, 0);
		point0x = point1x;
		point0y = point1y;
	}
	//MTdrawString(xCenter,yCenter,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	return;
}*/

void MTdrawThickCircunferenceWithBorderGradient(int xCenter, int yCenter, int radius, MTcolor color, MTcolor colorOut, MTcolor colorIn, int w0, int sections) {
	
	if(sections < 3) return;
	
	xCenter += MTdrawingOrigin.x;
	yCenter += MTdrawingOrigin.y;
	
	int i;
	//int sections = 30; //number of triangles to use to estimate a circle
	// (a higher number yields a more perfect circle)
	
	color = MTcolorBlend(color);
	colorOut = MTcolorBlend(colorOut);
	colorIn = MTcolorBlend(colorIn);
	
	COLOR_VERTEX* vertices;
	vertices = (COLOR_VERTEX*)sceGuGetMemory((2*sections + 2) * sizeof(COLOR_VERTEX));
	
	COLOR_VERTEX* vertices2;
	vertices2 = (COLOR_VERTEX*)sceGuGetMemory((2*sections + 2) * sizeof(COLOR_VERTEX));
	
	float med = w0/2;
	
	float angValue = (360.0f/ (float)sections)/2;
	
	for(i = 0; i <= (sections*2);i+=2) { // make $section number of circles
		float angValueDef = ((float)i*angValue);
		
		vertices[i].color = colorIn;
		vertices[i].x = xCenter + oslCos(angValueDef,radius-med);
		vertices[i].y = yCenter + oslSin(angValueDef,radius-med);
		vertices[i].z = 0;
		
		vertices[(i+1)].color = color;
		vertices[(i+1)].x = xCenter + oslCos(angValueDef,radius);
		vertices[(i+1)].y = yCenter + oslSin(angValueDef,radius);
		vertices[(i+1)].z = 0;
		
		
		vertices2[i].color = color;
		vertices2[i].x = xCenter + oslCos(angValueDef,radius);
		vertices2[i].y = yCenter + oslSin(angValueDef,radius);
		vertices2[i].z = 0;
		
		vertices2[(i+1)].color = colorOut;
		vertices2[(i+1)].x = xCenter + oslCos(angValueDef,radius+med);
		vertices2[(i+1)].y = yCenter + oslSin(angValueDef,radius+med);
		vertices2[(i+1)].z = 0;
	}
	//MTdrawString(xCenter,yCenter,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	
	//sceGuDisable(GU_TEXTURE_2D);
	//oslDisableTexturing();
	sceGuDisable( GU_CULL_FACE );//Gracias a esto da = el orden
	sceGuDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, (2*sections + 2), 0, vertices);
	sceGuDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, (2*sections + 2), 0, vertices2);
	//sceKernelDcacheWritebackRange(vertices, (2*sections + 2) * sizeof(COLOR_VERTEX));
	//sceKernelDcacheWritebackRange(vertices2, (2*sections + 2) * sizeof(COLOR_VERTEX));
	sceGuEnable( GU_CULL_FACE );//Gracias a esto da = el orden
	//sceGuEnable(GU_TEXTURE_2D);
	
	//MTdrawString(xCenter,yCenter,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	
	return;
}

void MTdrawSmoothThickCircunference(int xCenter, int yCenter, int radius, MTcolor color, int w0, int sections) {
	MTcolor borderColor = RGBA(R(color),G(color),B(color),0);
	MTdrawThickCircunferenceWithBorderGradient(xCenter, yCenter, radius, color, borderColor, borderColor, w0, sections);
	return;
}

/*void MTdrawSmoothThickCircunference(int xCenter, int yCenter, int radius, MTcolor colorCir, int w0, int sections) {
	int i;
	//int sections = 30; //number of triangles to use to estimate a circle
	// (a higher number yields a more perfect circle)
	
	int point0x = 0;
	int point0y = 0;
	int point1x = 0;
	int point1y = 0;
	
	for(i = 0; i <= sections;i++) { // make $section number of circles
		point1x = xCenter + oslCos((float)i*(360.0f/ (float)sections),radius);
		point1y = yCenter + oslSin((float)i*(360.0f/ (float)sections),radius);
		if(i>0) MTdrawSmoothThickLine(point0x, point0y, point1x, point1y, colorCir, w0, w0, 0);
		point0x = point1x;
		point0y = point1y;
	}
	//MTdrawString(xCenter,yCenter,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	return;
}*/

void MTdrawArc(int xCenter, int yCenter, int radius, MTcolor colorCir, float degreesInit, float degrees, int sections) {		
	
	if(sections < 2) return;
	
	xCenter += MTdrawingOrigin.x;
	yCenter += MTdrawingOrigin.y;
	
	int i;
	//int sections = 30; //number of triangles to use to estimate a circle
	// (a higher number yields a more perfect circle)
	
	colorCir = MTcolorBlend(colorCir);
	
	COLOR_VERTEX* vertices;
	vertices = (COLOR_VERTEX*)sceGuGetMemory((sections + 1) * sizeof(COLOR_VERTEX));
	
	float angValue = ((float)degrees / (float)sections);
	
	for(i = 0; i <= sections;i++) { // make section number of circles
		float angValueDef = (((float)i*angValue)+degreesInit);
		
		vertices[i].color = colorCir;
		vertices[i].x = xCenter + oslCos(angValueDef,radius);
		vertices[i].y = yCenter + oslSin(angValueDef,radius);
		vertices[i].z = 0;
		
	}
	
	//sceGuDisable(GU_TEXTURE_2D);
	//oslDisableTexturing();
	sceGuDrawArray(GU_LINE_STRIP, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, (sections + 1), 0, vertices);
	//sceKernelDcacheWritebackRange(vertices, (sections + 1) * sizeof(COLOR_VERTEX));
	//sceGuEnable(GU_TEXTURE_2D);
	
	//MTdrawString(xCenter,yCenter,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	
	return;
}

void MTdrawThickArcWithGradient(int xCenter, int yCenter, int radius, MTcolor colorOut, MTcolor colorIn, float degreesInit, float degrees, int w0, int sections) {
	
	if(sections < 2) return;
	
	xCenter += MTdrawingOrigin.x;
	yCenter += MTdrawingOrigin.y;
	
	int i;
	//int sections = 30; //number of triangles to use to estimate a circle
	// (a higher number yields a more perfect circle)
	
	colorOut = MTcolorBlend(colorOut);
	colorIn = MTcolorBlend(colorIn);
	
	COLOR_VERTEX* vertices;
	vertices = (COLOR_VERTEX*)sceGuGetMemory((2*sections + 2) * sizeof(COLOR_VERTEX));
	
	float med = w0/2;
	
	float angValue = ((float)degrees / (float)sections)/2;
	
	for(i = 0; i <= (sections*2);i+=2) { // make $section number of circles
		/*MTdrawPixel(point1x, point1y, colorCir);
		MTdrawPixel(point3x, point3y, colorCir);
		
		char text[100];
		sprintf(text,"%i",i);
		MTdrawString(point1x,point1y,text);
		sprintf(text,"%i",i+1);
		MTdrawString(point3x,point3y,text);*/
		
		float angValueDef = (((float)i*angValue) + degreesInit);
		
		vertices[i].color = colorIn;
		vertices[i].x = xCenter + oslCos(angValueDef,radius-med);
		vertices[i].y = yCenter + oslSin(angValueDef,radius-med);
		vertices[i].z = 0;
		
		vertices[(i+1)].color = colorOut;
		vertices[(i+1)].x = xCenter + oslCos(angValueDef,radius+med);
		vertices[(i+1)].y = yCenter + oslSin(angValueDef,radius+med);
		vertices[(i+1)].z = 0;
	}
	//MTdrawString(xCenter,yCenter,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	
	//sceGuDisable(GU_TEXTURE_2D);
	//oslDisableTexturing();
	sceGuDisable( GU_CULL_FACE );//Gracias a esto da = el orden
	sceGuDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, (2*sections + 2), 0, vertices);
	//sceKernelDcacheWritebackRange(vertices, (2*sections + 2) * sizeof(COLOR_VERTEX));
	sceGuEnable( GU_CULL_FACE );//Gracias a esto da = el orden
	//sceGuEnable(GU_TEXTURE_2D);
	
	//MTdrawString(xCenter,yCenter,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	
	return;
}

void MTdrawThickArc(int xCenter, int yCenter, int radius, MTcolor colorCir, float degreesInit, float degrees, int w0, int sections) {
	MTdrawThickArcWithGradient(xCenter, yCenter, radius, colorCir, colorCir, degreesInit, degrees, w0, sections);
	return;
}

void MTdrawThickArcWithBorderGradient(int xCenter, int yCenter, int radius, MTcolor color, MTcolor colorOut, MTcolor colorIn, float degreesInit, float degrees, int w0, int sections) {
	
	if(sections < 2) return;
	
	xCenter += MTdrawingOrigin.x;
	yCenter += MTdrawingOrigin.y;
	
	int i;
	//int sections = 30; //number of triangles to use to estimate a circle
	// (a higher number yields a more perfect circle)
	
	color = MTcolorBlend(color);
	colorOut = MTcolorBlend(colorOut);
	colorIn = MTcolorBlend(colorIn);
	
	COLOR_VERTEX* vertices;
	vertices = (COLOR_VERTEX*)sceGuGetMemory((2*sections + 2) * sizeof(COLOR_VERTEX));
	
	COLOR_VERTEX* vertices2;
	vertices2 = (COLOR_VERTEX*)sceGuGetMemory((2*sections + 2) * sizeof(COLOR_VERTEX));
	
	float med = w0/2;
	
	
	float angValue = ((float)degrees / (float)sections)/2;
	
	for(i = 0; i <= (sections*2);i+=2) { // make $section number of circles
		/*MTdrawPixel(point1x, point1y, colorCir);
		MTdrawPixel(point3x, point3y, colorCir);
		
		char text[100];
		sprintf(text,"%i",i);
		MTdrawString(point1x,point1y,text);
		sprintf(text,"%i",i+1);
		MTdrawString(point3x,point3y,text);*/
		
		float angValueDef = (((float)i*angValue) + degreesInit);
		
		vertices[i].color = colorIn;
		vertices[i].x = xCenter + oslCos(angValueDef,radius-med);
		vertices[i].y = yCenter + oslSin(angValueDef,radius-med);
		vertices[i].z = 0;
		
		vertices[(i+1)].color = color;
		vertices[(i+1)].x = xCenter + oslCos(angValueDef,radius);
		vertices[(i+1)].y = yCenter + oslSin(angValueDef,radius);
		vertices[(i+1)].z = 0;
		
		
		vertices2[i].color = color;
		vertices2[i].x = xCenter + oslCos(angValueDef,radius);
		vertices2[i].y = yCenter + oslSin(angValueDef,radius);
		vertices2[i].z = 0;
		
		vertices2[(i+1)].color = colorOut;
		vertices2[(i+1)].x = xCenter + oslCos(angValueDef,radius+med);
		vertices2[(i+1)].y = yCenter + oslSin(angValueDef,radius+med);
		vertices2[(i+1)].z = 0;
	}
	//MTdrawString(xCenter,yCenter,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	
	//sceGuDisable(GU_TEXTURE_2D);
	//oslDisableTexturing();
	sceGuDisable( GU_CULL_FACE );//Gracias a esto da = el orden
	sceGuDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, (2*sections + 2), 0, vertices);
	sceGuDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, (2*sections + 2), 0, vertices2);
	//sceKernelDcacheWritebackRange(vertices, (2*sections + 2) * sizeof(COLOR_VERTEX));
	//sceKernelDcacheWritebackRange(vertices2, (2*sections + 2) * sizeof(COLOR_VERTEX));
	sceGuEnable( GU_CULL_FACE );//Gracias a esto da = el orden
	//sceGuEnable(GU_TEXTURE_2D);
	
	//MTdrawString(xCenter,yCenter,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	
	return;
}

void MTdrawSmoothThickArc(int xCenter, int yCenter, int radius, MTcolor color, float degreesInit, float degrees, int w0, int sections) {
	MTcolor borderColor = RGBA(R(color),G(color),B(color),0);
	MTdrawThickArcWithBorderGradient(xCenter, yCenter, radius, color, borderColor, borderColor, degreesInit, degrees, w0, sections);
	return;
}

void MTdrawFillPolygonWithGradient(int xCenter,int yCenter,int radius,MTcolor originColorCir,MTcolor colorCir,float degreesInit, int sections) {		
	MTdrawCircleSectionWithGradient(xCenter,yCenter,radius,originColorCir,colorCir, degreesInit, 360.0f, sections);
	return;
}

void MTdrawFillPolygon(int xCenter,int yCenter,int radius,MTcolor originColorCir,float degreesInit, int sections) {
	MTdrawCircleSection( xCenter, yCenter, radius, originColorCir, degreesInit, 360.0f, sections);
	return;
}

void MTdrawPolygon(int xCenter, int yCenter, int radius, MTcolor colorCir, float degreesInit, int sections) {		
	MTdrawArc( xCenter,  yCenter,  radius, colorCir,  degreesInit, 360.0f,  sections);
	return;
}

void MTdrawThickPolygonWithGradient(int xCenter, int yCenter, int radius, MTcolor colorOut, MTcolor colorIn, float degreesInit, int w0, int sections) {
	MTdrawThickArcWithGradient( xCenter,  yCenter,  radius, colorOut, colorIn,  degreesInit, 360.0f,  w0,  sections);
	return;
}

void MTdrawThickPolygon(int xCenter, int yCenter, int radius, MTcolor colorCir, float degreesInit, int w0, int sections) {
	MTdrawThickArc(xCenter, yCenter, radius, colorCir, degreesInit, 360.0f, w0, sections);
	return;
}

void MTdrawThickPolygonWithBorderGradient(int xCenter, int yCenter, int radius, MTcolor color, MTcolor colorOut, MTcolor colorIn, float degreesInit, int w0, int sections) {
	MTdrawThickArcWithBorderGradient( xCenter,  yCenter,  radius, color, colorOut, colorIn,  degreesInit, 360.0f,  w0,  sections);
	return;
}

void MTdrawSmoothThickPolygon(int xCenter, int yCenter, int radius, MTcolor color, float degreesInit, int w0, int sections) {
	MTdrawSmoothThickArc(xCenter, yCenter, radius, color, degreesInit, 360.0f, w0, sections);
	return;
}


void MTdrawRectangleWithGradient(int x0, int y0, int x1, int y1, MTcolor c1, MTcolor c2, MTcolor c3, MTcolor c4) {
	
	COLOR_VERTEX* vertices;
	
	vertices = (COLOR_VERTEX*)sceGuGetMemory(5 * sizeof(COLOR_VERTEX));
	c1 = MTcolorBlend(c1);
	c2 = MTcolorBlend(c2);
	c3 = MTcolorBlend(c3);
	c4 = MTcolorBlend(c4);
	
//	sceGuShadeModel(GU_SMOOTH);
	
	vertices[0].color = c1;
	vertices[0].x = x0 + MTdrawingOrigin.x;
	vertices[0].y = y0 + MTdrawingOrigin.y;
	vertices[0].z = 0;
	
	vertices[1].color = c2;
	vertices[1].x = x1 + MTdrawingOrigin.x;
	vertices[1].y = y0 + MTdrawingOrigin.y;
	vertices[1].z = 0;
	
	vertices[2].color = c3;
	vertices[2].x = x1 + MTdrawingOrigin.x;
	vertices[2].y = y1 + MTdrawingOrigin.y;
	vertices[2].z = 0;
	
	vertices[3].color = c4;
	vertices[3].x = x0 + MTdrawingOrigin.x;
	vertices[3].y = y1 + MTdrawingOrigin.y;
	vertices[3].z = 0;
	
	vertices[4].color = c1;
	vertices[4].x = x0 + MTdrawingOrigin.x;
	vertices[4].y = y0 + MTdrawingOrigin.y;
	vertices[4].z = 0;
	
	//sceGuDisable( GU_CULL_FACE );
	//oslDisableTexturing();
	sceGuDrawArray(GU_LINE_STRIP, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 5, 0, vertices);
	//sceGuEnable( GU_CULL_FACE );
	//MTdrawString(x0,y0,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	return;
}

void MTdrawRectangle(int x0, int y0, int x1, int y1, MTcolor color) {
	oslDrawRect (x0 + MTdrawingOrigin.x, y0 + MTdrawingOrigin.y, x1 + MTdrawingOrigin.x, y1 + MTdrawingOrigin.y, color);
	return;
}

void MTdrawThickRectangleWithGradient(int x0, int y0, int x1, int y1, MTcolor colorOut, MTcolor colorIn, int w0) {
	
	x0 += MTdrawingOrigin.x;
	y0 += MTdrawingOrigin.y;
	x1 += MTdrawingOrigin.x;
	y1 += MTdrawingOrigin.y;
	
	colorOut = MTcolorBlend(colorOut);
	colorIn = MTcolorBlend(colorIn);
	
	COLOR_VERTEX* vertices;
	vertices = (COLOR_VERTEX*)sceGuGetMemory(9 * sizeof(COLOR_VERTEX));
	
	float med = w0/2;
	
	vertices[0].color = colorOut;
	vertices[0].x = x0-med;
	vertices[0].y = y0-med;
	vertices[0].z = 0;
	
	vertices[1].color = colorIn;
	vertices[1].x = x0+med;
	vertices[1].y = y0+med;
	vertices[1].z = 0;
	
	vertices[2].color = colorOut;
	vertices[2].x = x1+med;
	vertices[2].y = y0-med;
	vertices[2].z = 0;
	
	vertices[3].color = colorIn;
	vertices[3].x = x1-med;
	vertices[3].y = y0+med;
	vertices[3].z = 0;
	
	vertices[4].color = colorOut;
	vertices[4].x = x1+med;
	vertices[4].y = y1+med;
	vertices[4].z = 0;
	
	vertices[5].color = colorIn;
	vertices[5].x = x1-med;
	vertices[5].y = y1-med;
	vertices[5].z = 0;
	
	vertices[6].color = colorOut;
	vertices[6].x = x0-med;
	vertices[6].y = y1+med;
	vertices[6].z = 0;
	
	vertices[7].color = colorIn;
	vertices[7].x = x0+med;
	vertices[7].y = y1-med;
	vertices[7].z = 0;
	
	vertices[8].color = colorOut;
	vertices[8].x = x0-med;
	vertices[8].y = y0-med;
	vertices[8].z = 0;
	
	
	//sceGuDisable(GU_TEXTURE_2D);
	//oslDisableTexturing();
	sceGuDisable( GU_CULL_FACE );//Gracias a esto da = el orden
	sceGuDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 9, 0, vertices);
	//sceKernelDcacheWritebackRange(vertices, (2*sections + 2) * sizeof(COLOR_VERTEX));
	sceGuEnable( GU_CULL_FACE );//Gracias a esto da = el orden
	//sceGuEnable(GU_TEXTURE_2D);
	
	return;
}

void MTdrawFillRectangle(int x0, int y0, int x1, int y1, MTcolor color) {
	color = MTcolorBlend(color);
	
	COLOR_VERTEX* vertices;
	vertices = (COLOR_VERTEX*)sceGuGetMemory(2 * sizeof(COLOR_VERTEX));
	vertices[0].color = color;
	vertices[0].x = x0 + MTdrawingOrigin.x;
	vertices[0].y = y0 + MTdrawingOrigin.y;
	vertices[0].z = 0;
	vertices[1].color = color;
	vertices[1].x = x1 + MTdrawingOrigin.x;
	vertices[1].y = y1 + MTdrawingOrigin.y;
	vertices[1].z = 0;
	
	sceGuDisable( GU_CULL_FACE );
	//oslDisableTexturing();
	sceGuDrawArray(GU_SPRITES, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 2, 0, vertices);
	sceGuEnable( GU_CULL_FACE );
	return;
}

void MTdrawFillRectangleWithGradient(int x0, int y0, int x1, int y1, MTcolor c1, MTcolor c2, MTcolor c3, MTcolor c4) {
	
	COLOR_VERTEX* vertices;
	
	vertices = (COLOR_VERTEX*)sceGuGetMemory(4 * sizeof(COLOR_VERTEX));
	c1 = MTcolorBlend(c1);
	c2 = MTcolorBlend(c2);
	c3 = MTcolorBlend(c3);
	c4 = MTcolorBlend(c4);
	
//	sceGuShadeModel(GU_SMOOTH);
	
	vertices[0].color = c1;
	vertices[0].x = x0 + MTdrawingOrigin.x;
	vertices[0].y = y0 + MTdrawingOrigin.y;
	vertices[0].z = 0;
	
	vertices[1].color = c2;
	vertices[1].x = x1 + MTdrawingOrigin.x;
	vertices[1].y = y0 + MTdrawingOrigin.y;
	vertices[1].z = 0;
	
	vertices[2].color = c3;
	vertices[2].x = x0 + MTdrawingOrigin.x;
	vertices[2].y = y1 + MTdrawingOrigin.y;
	vertices[2].z = 0;
	
	vertices[3].color = c4;
	vertices[3].x = x1 + MTdrawingOrigin.x;
	vertices[3].y = y1 + MTdrawingOrigin.y;
	vertices[3].z = 0;
	
	sceGuDisable( GU_CULL_FACE );
	//oslDisableTexturing();
	sceGuDrawArray(GU_TRIANGLE_STRIP, GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D, 4, 0, vertices);
	sceGuEnable( GU_CULL_FACE );
	//MTdrawString(x0,y0,"prueba de texto. Vemos cuanto ralentiza. probando, probando");
	return;
}

void MTdrawFillRoundRectangleWithGradient(int x0, int y0, int x1, int y1, MTcolor color0, MTcolor color1, int radius, int sections) {
	MTdrawCircleSectionWithGradient(x0+radius,y0+radius,radius,color0,color1,180.0f, 90.0f, sections);
	MTdrawCircleSectionWithGradient(x1-radius,y0+radius,radius,color0,color1,270.0f, 90.0f, sections);
	MTdrawCircleSectionWithGradient(x0+radius,y1-radius,radius,color0,color1,90.0f, 90.0f, sections);
	MTdrawCircleSectionWithGradient(x1-radius,y1-radius,radius,color0,color1,0.0f, 90.0f, sections);
	MTdrawFillRectangle(x0+radius, y0+radius, x1-radius, y1-radius, color0);
	MTdrawFillRectangleWithGradient(x0+radius, y0, x1-radius, y0+radius, color1, color1, color0, color0);
	MTdrawFillRectangleWithGradient(x0+radius, y1-radius, x1-radius, y1, color0, color0, color1, color1);
	MTdrawFillRectangleWithGradient(x0, y0+radius, x0+radius, y1-radius, color1, color0, color1, color0);
	MTdrawFillRectangleWithGradient(x1-radius, y0+radius, x1, y1-radius, color0, color1, color0, color1);
	return;
}

void MTdrawRoundRectangleWithGradient(int x0, int y0, int x1, int y1, MTcolor color0, MTcolor color1, int radius, int sections) {
	MTdrawCircleSectionWithGradient(x0+radius,y0+radius,radius,color0,color1,180.0f, 90.0f, sections);
	MTdrawCircleSectionWithGradient(x1-radius,y0+radius,radius,color0,color1,270.0f, 90.0f, sections);
	MTdrawCircleSectionWithGradient(x0+radius,y1-radius,radius,color0,color1,90.0f, 90.0f, sections);
	MTdrawCircleSectionWithGradient(x1-radius,y1-radius,radius,color0,color1,0.0f, 90.0f, sections);
	//MTdrawFillRectangle(x0+radius, y0+radius, x1-radius, y1-radius, color0);
	MTdrawFillRectangleWithGradient(x0+radius, y0, x1-radius, y0+radius, color1, color1, color0, color0);
	MTdrawFillRectangleWithGradient(x0+radius, y1-radius, x1-radius, y1, color0, color0, color1, color1);
	MTdrawFillRectangleWithGradient(x0, y0+radius, x0+radius, y1-radius, color1, color0, color1, color0);
	MTdrawFillRectangleWithGradient(x1-radius, y0+radius, x1, y1-radius, color0, color1, color0, color1);
	return;
}

void MTdrawSmoothFillRoundRectangle(int x0, int y0, int x1, int y1, MTcolor color0, int radius, int sections) {
	MTcolor color1 = RGBA(R(color0),G(color0),B(color0),0);
	MTdrawFillRoundRectangleWithGradient(x0, y0, x1, y1, color0, color1, radius, sections);
	return;
}

void MTdrawSmoothRoundRectangle(int x0, int y0, int x1, int y1, MTcolor color0, int radius, int sections) {
	MTcolor color1 = RGBA(R(color0),G(color0),B(color0),0);
	MTdrawRoundRectangleWithGradient(x0, y0, x1, y1, color0, color1, radius, sections);
	return;
}

void MTdrawFillSemiRoundRectangleWithGradient(int x0, int y0, int x1, int y1, MTcolor color0, MTcolor color1, int radius, int sections) {
	MTdrawCircleSectionWithGradient(x0+radius,y0+radius,radius,color0,color1,180.0f, 90.0f, sections);
	MTdrawCircleSectionWithGradient(x1-radius,y0+radius,radius,color0,color1,270.0f, 90.0f, sections);
	MTdrawFillRectangle(x0+radius, y0+radius, x1-radius, y1, color0);
	MTdrawFillRectangleWithGradient(x0+radius, y0, x1-radius, y0+radius, color1, color1, color0, color0);
	MTdrawFillRectangleWithGradient(x0, y0+radius, x0+radius, y1, color1, color0, color1, color0);
	MTdrawFillRectangleWithGradient(x1-radius, y0+radius, x1, y1, color0, color1, color0, color1);
	return;
}


MTanimation* MTcreateAnimation(int step, int loop) {
	MTanimation* anim = (MTanimation*)malloc(sizeof(MTanimation));
	anim->frames = (MTimage*)malloc(sizeof(MTimage));
	anim->frameCount = 0;
	anim->frameNum = 0;
	anim->step = step;
	anim->currentFrame = 1;
	anim->loop = loop;
	
	anim->sizeX = 0;
	anim->sizeY = 0;
	
	anim->x = 0;
	anim->y = 0;
	anim->stretchX = 0;
	anim->stretchY = 0;
	anim->centerX = 0;
	anim->centerY = 0;
	anim->angle = 0;
	
	anim->exists = 1;
	
	return anim;
}

void MTaddFrameToAnimation(MTanimation* anim, MTimage* newFrame) {
	if(anim == NULL || newFrame == NULL || anim->exists == 0) return;
	//MTimage* newFrame = MTloadImageFile(path);
	//if(newFrame == NULL) return;
	anim->frames = (MTimage*) realloc (anim->frames, (anim->frameNum+2) * sizeof(MTimage));
	anim->frames[(anim->frameNum)] = *newFrame;
	oslCorrectImageHalfBorder(newFrame);
	if(anim->frameNum == 0) {
		anim->sizeX = newFrame->sizeX;
		anim->sizeY = newFrame->sizeY;
		anim->stretchX = newFrame->sizeX;
		anim->stretchY = newFrame->sizeY;
	}
	anim->frameNum++;
	return;
}

void MTloadAddFrameToAnimation(MTanimation* anim, char* path) {
	if(anim == NULL || path == NULL || anim->exists == 0) return;
	MTimage* newFrame = MTloadImageFile(path);
	MTaddFrameToAnimation(anim, newFrame);
	return;
}

void MTdrawAnimation(MTanimation* anim) {
	if(anim == NULL || anim->exists == 0) return;
	if(anim->frameNum == 0) return;
	
	//if(anim->loop < 1 && anim->currentFrame == anim->frameNum) {
		//Draw the frame
		//return;
	//} else {
	
	//}
	
	MTimage* img = &anim->frames[(anim->currentFrame-1)];
	
	if(img != NULL) {
		img->x = anim->x;
		img->y = anim->y;
		img->stretchX = anim->stretchX;
		img->stretchY = anim->stretchY;
		img->centerX = anim->centerX;
		img->centerY = anim->centerY;
		img->angle = anim->angle;
		
		MTdrawImage(img);
	}
	
	if(anim->frameCount >= anim->step) {
		anim->currentFrame++;
		if(anim->currentFrame > anim->frameNum) {
			if(anim->loop < 1) {
				anim->currentFrame--;
			} else anim->currentFrame = anim->loop;
		}
		anim->frameCount = 0;
		//MTreadFPS();
	}
	
	anim->frameCount++;
	return;
}

void MTfreeAnimation(MTanimation* anim) {
	if(anim == NULL || anim->exists == 0) return;
	anim->exists = 0;
	if(anim->frames != NULL){
		int i;
		for (i=0; i < anim->frameNum; i++) {
			//MTfree(&anim->frames[i]);
			MTimage* tmpImg = &anim->frames[i];
			//MTfreeImage (tmpImg);
			
			if(tmpImg != NULL) {
				if (!oslImageIsCopy(tmpImg)) {
				oslFreeImageData(tmpImg);
				if (tmpImg->palette)
					oslDeletePalette(tmpImg->palette);
				}
				//free(tmpImg);
				tmpImg = NULL;
			}
		}
		
		MTfree(anim->frames);
		anim->frames = NULL;
	}
	
	anim->frameCount = 0;
	anim->frameNum = 0;
	anim->step = 0;
	anim->currentFrame = 1;
	anim->loop = 0;
	
	anim->sizeX = 0;
	anim->sizeY = 0;
	
	anim->x = 0;
	anim->y = 0;
	anim->stretchX = 0;
	anim->stretchY = 0;
	anim->centerX = 0;
	anim->centerY = 0;
	anim->angle = 0;
	
	MTfree(anim);
	anim = NULL;
	return;
}

MTcolor MTchangeColorBright(MTcolor color, int val) {
	int r = R(color)+val;
	if(r > 255) r = 255;
	if(r < 0) r = 0;
	
	int g = G(color)+val;
	if(g > 255) g = 255;
	if(g < 0) g = 0;
	
	int b = B(color)+val;
	if(b > 255) b = 255;
	if(b < 0) b = 0;
	
	int a = A(color);/*+val;
	if(a > 255) a = 255;
	if(a < 0) a = 0;*/
	return RGBA(r,g,b,a);
}

MTcolor MTgetZeroAlphaColor(MTcolor color) {
	return RGBA(R(color),G(color),B(color),0);
}
