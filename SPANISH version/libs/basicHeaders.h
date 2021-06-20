#include <pspkernel.h>
#include <oslib/oslib.h>

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspsdk.h>
#include <string.h>

#include <sys/types.h>
#include <sys/unistd.h>

#include <stdlib.h>
#include <pspctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <psppower.h>
#include <string.h>
#include <math.h>

#include <dirent.h> // for getcwd

#include <psphttp.h>//NET functions

//Files, directoryies... etc management:
//#include <dirent.h>
//#include <unistd.h>

#include <ctype.h> //toupper...(MTstristr)

#include <libUnrar.h>

#include <zzip/zzip.h>

#include <iconv.h>

#include "minIni/minIni.h"
#include "id3/id3.h"
#include "doublyLinkedLists.h"
#include "../multiTasKingLibrary.h"
#include "basicVariables.h"

#include "libs/miniConv/miniconv.h"

#include "libs/supportlib/supportlib.h"

#define MT_quit osl_quit


PSP_MODULE_INFO("multiTasKing", 0, 1, 0);//0,1,0
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(-4*1024); //toda la ram menos x mb - Bastante memoria para alojar las ventanas y funciones ;-)

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common) {
	MT_quit = 1;
	return 0;
}

void MTshowErrorWindow(char* name, char* text);
void MTloadFailCallback(const char *filename, u32 reserved);

SceUID MTthreadNameExists(const char *name) {
	SceUID readbuf[256];
	int idcount, ret;
	SceKernelThreadInfo info;
	ret = sceKernelGetThreadmanIdList(SCE_KERNEL_TMID_Thread, (void *)(SceUID)&readbuf, sizeof(readbuf)/sizeof(SceUID), &idcount);
	for(info.size=sizeof(info); idcount>0; idcount--) {
		ret = sceKernelReferThreadStatus(readbuf[idcount-1], &info);
		if (strcmp(info.name, name) == 0) { return readbuf[idcount-1]; }
	}
	return 0;
}

int MTgetSystemLanguage() {
	int language;
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE,&language);
	switch (language) {
		case 0: return 0; break;//japanese
		case 1: return 1; break;//english
		case 2: return 2; break;//french
		case 3: return 3; break;//spanish
		case 4: return 4; break;//german
		case 5: return 5; break;//italian
		case 6: return 6; break;//dutch
		case 7: return 7; break;//portuguese
		case 8: return 8; break;//russian
		case 9: return 9; break;//korean
		case 10: return 10; break;//chinese traditional
		case 11: return 11; break;//chinese simplified
		default : return -1; break;//unknown
	}
	return -1;
}

SceUID MTdriverInit(char *path) {
	SceUID sysloadResult = sceKernelLoadModule(path, 0, NULL);
	if(sysloadResult & 0x80000000) { MTcanUseKernel = 0; return -1; }
    else {	
		sysloadResult = sceKernelStartModule(sysloadResult, 0, NULL, NULL, NULL);
		if(sysloadResult & 0x80000000) { MTcanUseKernel = 0; return -1; }
		else { MTcanUseKernel = 1; return 0; }
	}
}

void MTrunPBP(char *path) {
	if(path == NULL) return;
	if(MTcanUseKernel == 0) {
		MTshowErrorWindow("Error al cargar PBP", "Está ejecutando MultiTasKing sin soporte\npara modo kernel, no funciona el cargador de PBP.");
		return;
	}
	runebootfile(path);
	return;
}

void MTrunPBPrelativePath(char *path) { //Se puede ejecutar un PRX desde el directorio del EBOOT sin pasar por "ms0:/PSP/GAME/... etc"
	char directory[601];
	char buffer[601];
	getcwd(buffer, 600);
	sprintf(directory,"%s/%s",buffer,path);
	MTrunPBP(directory);
	return;
}

/*int MTrunIso(lua_State *L) {
	if ( cankernel ) {
		char* isopath = luaL_checkstring(L, 1);
		struct SceKernelLoadExecVSHParam param;
		SEConfig config;	
		int apitype = 0x120;
		char *program = "disc0:/PSP_GAME/SYSDIR/EBOOT.BIN";
		char *mode = "game";
		SetUmdFile(isopath);
		sctrlSEGetConfigEx(&config, sizeof(config));
		if (config.umdmode == MODE_MARCH33) { SetConfFile(1); }
		else if (config.umdmode == MODE_NP9660) { SetConfFile(2); }
		else { SetConfFile(O); }
		memset(&param, 0, sizeof(param));
		param.size = sizeof(param);
		param.args = strlen(program)+1;
		param.argp = program;
		param.key = mode;
		sctrlKernelLoadExecVSHWithApitype(apitype, program, &param);
		return 0;
	}
	else { lua_pushinteger(L,-1); return 1; }
	return 0;
}*/

/*static int lua_startpsx(lua_State *L) {
	if (cankernel) {
		struct SceKernelLoadExecVSHParam param;
		char* loadpath = luaL_checkstring(L, 1);
		int apitype = 0x143;
		char *mode = "pops"; 
		memset(&param, 0, sizeof(param));
		param.size = sizeof(param);
		param.args = strlen(loadpath)+1;
		param.argp = loadpath;
		param.key = mode;
		sctrlKernelLoadExecVSHWithApitype(apitype, loadpath, &param);
    }
	else { lua_pushinteger(L,-1); return 1; }
	return 0;
}*/

void MTdelay(int time) {
	sceKernelDelayThreadCB(time);//1000000 = 1 sec
	return;
}

/*int MTrandom(void) {
	return rand();
}*/

int MTrandom(int num1, int num2) {
	return rand () % (num2-num1+1) + num1;
}

float MTround(float valor) {
	return round(valor);
}

float MTgetTotalFreeRAM(void) {
	return sceKernelTotalFreeMemSize();
}

char *MTstristr(char *haystack, char *needle) { //search string case insensitive
	if ( !*needle ) {
		return haystack;
	}
	for ( ; *haystack; ++haystack ) {
		if ( toupper(*haystack) == toupper(*needle) ) {
			/*
			#
			* Matched starting char -- loop through remaining chars.
			#
			*/
			char *h, *n;
			for ( h = haystack, n = needle; *h && *n; ++h, ++n ) {
				if ( toupper(*h) != toupper(*n) ) {
					break;
				}
			}
			if ( !*n ) { /* matched all of 'needle' to null termination */
				return haystack; /* return the start of the match */
			}
		}
	}
	return 0;
}

int MTstrlen(char string[]) {
	int count = 0;
	while(string[count] != '\0') {
	count++;
	}
	return count;
}

int MTmeasureTextWidth(char* text/*+,FONT*/) {
	return oslGetStringWidth(text);//funcion de OSL
}

void * MTmalloc ( size_t size ) {//Al ser llamadas desde PRX directamente, malloc y free dan problemas. se soluciona pasandolos por MTK
	return malloc(size);
}

void MTfree(void *ptr) {
	/*if(ptr != NULL) free(ptr);//Failsafe
	ptr = NULL;*/
	free(ptr);
	return;
}

/*void MTfreeIfNotNull(void *ptr) {//Same as MTfree() but this gives logic to the program
	MTfree(ptr);
	return;
}*/

void * MTmemset ( void * ptr, int value, size_t num ) {
	return memset(ptr,value,num);
}

void * MTmemcpy ( void * destination, const void * source, size_t num ) {
	return memcpy(destination,source,num);
}

int MTrunPRX(char *path, SceSize args, void *argp) {
	SceUID sysloadResult = sceKernelLoadModule(path, 0, NULL);
	if(sysloadResult & 0x80000000) { return -1; }
    else {	
		sysloadResult = sceKernelStartModule(sysloadResult, args, argp, NULL, NULL);
		if(sysloadResult & 0x80000000) { return -1; }
		else { return 0; }
	}
}

/*int MTrunPRX(char *path, SceSize args, void *argp) {
	SceUID mod = sceKernelLoadModule(path, 0, NULL);
	if (mod > 0) {
	return sceKernelStartModule(mod, args, argp, NULL, NULL);
	}
	return 0;
}*/

int MTrunPRXrelativePath(char *path, SceSize args, void *argp) { //Se puede ejecutar un PRX desde el directorio del EBOOT sin pasar por "ms0:/PSP/GAME/... etc"
	char directory[601];
	char buffer[601];
	getcwd(buffer, 600);
	sprintf(directory,"%s/%s",buffer,path);
	return MTrunPRX(directory, args, argp);
}


bool MTdoesFileExists(char* path) {
	if(path == NULL) return 0;
	FILE *fp = fopen(path,"r");
	if( fp ) {
		// exists
		fclose(fp);
		return 1;
	} else {
		return 0;
	}
	return 0;
}

bool MTdoesDirectoryExists(char *path) {
	if(path == NULL) return 0;
	DIR *d = opendir(path);
	if( d ) {
		// exists
		closedir(d);
		return 1;
	} else {
		return 0;
	}
	return 0;
}

int MTdoesPathExists(char* path) {
	if(path == NULL) return 0;
	if(MTdoesDirectoryExists(path)) return 1;
	if(MTdoesFileExists(path)) return 2;
	return 0;
}


bool MTisUsbOn() {
	return usbConnectionIsActive;
}

void MTstartUsb() {
	oslInitUsbStorage();
	oslStartUsbStorage();
	usbConnectionIsActive = 1;
	return;
}

void MTstopUsb() {
	oslStopUsbStorage();
	oslDeinitUsbStorage();
	usbConnectionIsActive = 0;
	return;
}


int MTgetFileType(char* file) {//
	int type = MTdoesPathExists(file);
	if(type == 1) {
		return 0;//folder
	} else if(type == 2) {
	
	char *ext, extension[10];
	int i;
	ext = strrchr(file, '.');
	if (!ext)
		return 1;//no extension, (unknown), we handle it like a text file
	i = 0;
	while(ext[i] && i < sizeof(extension) - 2)
	{
		extension[i] = tolower(ext[i]);
		i++;
	}
	extension[i] = 0;
	
	if (!strcmp(extension, ".prx")) return 2;
	if (!strcmp(extension, ".pbp")) return 3;
	if (!strcmp(extension, ".rar")) return 4;
	if (!strcmp(extension, ".zip")) return 5;
	if (!strcmp(extension, ".png")) return 6;
	if (!strcmp(extension, ".mp3")) return 7;
	if (!strcmp(extension, ".jpg")) return 8;
	if (!strcmp(extension, ".jpeg")) return 8;
	if (!strcmp(extension, ".gif")) return 9;
	//if (!strcmp(extension, ".txt")) return 10;
	if (!strcmp(extension, ".gif")) return 11;
	//if (!strcmp(extension, ".ini")) return 12;
	if (!strcmp(extension, ".tiff")) return 13;
	if (!strcmp(extension, ".iso")) return 14;
	if (!strcmp(extension, ".cso")) return 15;
	if (!strcmp(extension, ".bmp")) return 16;
	if (!strcmp(extension, ".avi")) return 17;
	if (!strcmp(extension, ".mp4")) return 18;
	//if (!strcmp(extension, ".c")) return 19;
	//if (!strcmp(extension, ".h")) return 20;
	//if (!strcmp(extension, ".lua")) return 21;
	if (!strcmp(extension, ".bin")) return 22;
	if (!strcmp(extension, ".elf")) return 23;
	if (!strcmp(extension, ".ind")) return 24;
	if (!strcmp(extension, ".wav")) return 25;
	if (!strcmp(extension, ".ttf")) return 26;
	if (!strcmp(extension, ".raw")) return 27;
	if (!strcmp(extension, ".psd")) return 28;
	if (!strcmp(extension, ".o")) return 29;
	//if (!strcmp(extension, ".S")) return 30;
	if (!strcmp(extension, ".pcx")) return 31;
	if (!strcmp(extension, ".dat")) return 32;
	if (!strcmp(extension, ".db")) return 33;
	if (!strcmp(extension, ".sav")) return 34;
	if (!strcmp(extension, ".sfo")) return 35;
	if (!strcmp(extension, ".at3")) return 36;
	if (!strcmp(extension, ".pmf")) return 37;
	if (!strcmp(extension, ".lck")) return 38;
	if (!strcmp(extension, ".dbb")) return 39;
	//if (!strcmp(extension, ".xml")) return 40;
	if (!strcmp(extension, ".ctf")) return 41;
	if (!strcmp(extension, ".dbb")) return 42;
	
	return 1;
	}
	
	/*FILE* pFile;
	pFile = fopen ( file , "rb" );
	if (pFile==NULL) return 0;//folder/nonexisting
	
	char fileExtension[6];
	fread (fileExtension,1,4,pFile);//leemos 4 bytes, sin leer el primero que es nulo
	fileExtension[5] = '\0';
	fclose (pFile);
	
	if(MTstristr(fileExtension,"ELF") && MTstristr(file,"prx")) return 2; //PRX
	if(MTstristr(fileExtension,"PBP")) return 3; //EBOOT
	if(MTstristr(fileExtension,"RAR")) return 4; //RAR
	//if(MTstristr(fileExtension,"ZIP")) return 4; //ZIP
	if(MTstristr(fileExtension,"PNG")) return 5; //PNG
	if(MTstristr(fileExtension,"ID3")) return 6; //MP3
	return 1;//Unknown/other*/
	return -1;//does not exist
}

int MTmkdir(const char *dir) {
	if(dir == NULL) return -1;
	return sceIoMkdir(dir, 0777);
}

int MTrmdir (const char *path) {
	if(path == NULL) return -1;
	return sceIoRmdir (path);
}

void MTcreateDirectory(char *dir) {
				char tmp[256];
				char *p = NULL;
				size_t len;
 
				snprintf(tmp, sizeof(tmp),"%s",dir);
				len = strlen(tmp);
				if(tmp[len - 1] == '/')
								tmp[len - 1] = 0;
				for(p = tmp + 1; *p; p++)
								if(*p == '/') {
												*p = 0;
												MTmkdir(tmp);
												*p = '/';
								}
				MTmkdir(tmp);
}

void MTcreatePath(char *dir) {
				char tmp[256];
				char *p = NULL;
				size_t len;
 
				snprintf(tmp, sizeof(tmp),"%s",dir);
				len = strlen(tmp);
				if(tmp[len - 1] == '/')
								tmp[len - 1] = 0;
				for(p = tmp + 1; *p; p++)
								if(*p == '/') {
												*p = 0;
												MTmkdir(tmp);
												*p = '/';
								}
				MTmkdir(tmp);
}

void MTcreateFile(char *Filename) {
	MTcreateDirectory(Filename);//create the tree
	MTrmdir (Filename);//Remove the directory that has the filename
	
	FILE* fw;
	fw = fopen(Filename,"a+");
	fclose(fw);
	return;
}

char* MTgetFileContent(char *filePath, int* size) {//Remember to free with freeBuffer() when called from an external module
	if(MTstrlen(filePath) <= 0) {
		*size = -1;
		return NULL;
	}
	
	FILE * pFile;
	long lSize;

	pFile = fopen ( filePath , "rb" );
	
	if (pFile != NULL) {
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);
	
	if(lSize <= 0) {
		*size = -1;
		return NULL;
	}
	
	int maxlen = 100000;//Limit (just in case it is too big -probably binary-)
		if (lSize > maxlen) lSize = maxlen;
		
	char *fileContent = (char*) malloc (lSize + 1);
	if (fileContent != NULL) {
		fread (fileContent,1,lSize,pFile);
		fclose (pFile);
		
		fileContent[lSize] = '\0';//Cerramos el string de datos
		*size = lSize;
		return fileContent;
	}
	fclose (pFile);
	*size = -1;
	return NULL;
	}
	fclose (pFile);
	*size = -1;
	return NULL;
}

int MTgetFileSize(char *filePath) {//Remember to free with freeBuffer() when called from an external module
	if(filePath == NULL) return -1;
	
	FILE * pFile;
	long lSize;

	pFile = fopen ( filePath , "rb" );
	
	if (pFile == NULL) return -1;
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	
	if(lSize <= 0) {
		//*size = -1;
		fclose (pFile);
		return -1;
	}
	fclose (pFile);
	return lSize;
}

void MTputToFile(char *filePath, char *text) {
	if(filePath == NULL || text == NULL) return;
	FILE *pFile;
	pFile = fopen (filePath , "w+");
	fwrite (text,1,strlen(text),pFile);
	fclose (pFile);
	return;
}

void MTgetFileName(char *fileName, char *onlyName){
	int slash = -1;
	int retCount;

	strcpy(onlyName, fileName);
	//Cerco l'ultimo slash:
	int i = 0;
	for (i = strlen(fileName) - 1; i >= 0; i--){
		if ((fileName[i] == '/') & (i != strlen(fileName))){
			slash = i;
			break;
		}
	}
	if (slash){
		retCount = 0;
		for (i = slash + 1; i < strlen(fileName); i++){
			onlyName[retCount] = fileName[i];
			retCount++;
		}
		onlyName[retCount] = '\0';
	}else{
		strcpy(onlyName, fileName);
	}
}

int MTdirectoryUp(char *dirName)
{
	if (strcmp(dirName, "ms0:/") != 0){
		//Cerco l'ultimo slash:
		int i = 0;
		for (i = strlen(dirName) - 1; i >= 0; i--){
			if ((dirName[i] == '/') & (i != strlen(dirName))){
				if (i > 4){
					dirName[i] = '\0';
				}else{
					dirName[i+1] = '\0';
				}
				break;
			}
		}
		return(0);
	}else{
		return(-1);
	}
}

int MTrename(const char *oldname, const char *newname) {
	return sceIoRename (oldname, newname);
}

float MTpercent(float num, float max, float maxAbs) {//Just for knowing what are we doing and to make everything cleaner ;-)
	if(maxAbs == 0) return 100000;
	return (num*max)/maxAbs;
}

int MTareaShowBorder = 5;

bool MTisOverAreaSimple(int mouseX, int mouseY, int x, int y, int width, int height) {
	if(mouseX >= x && mouseX <= x+width && mouseY >= y && mouseY <= y+height) {
		//MTdrawFillRectangle(x, y, x+width, y+height, RGBA(0,255,0,128));
		//if(MTapiDrawMouseOverAreas) MTdrawSmoothFillRoundRectangle(x-MTareaShowBorder, y-MTareaShowBorder, x+width+MTareaShowBorder, y+height+MTareaShowBorder, RGBA(0,255,0,128), MTareaShowBorder, 4);
		return 1;
	}
	//else
	//MTdrawFillRectangle(x, y, x+width, y+height, RGBA(255,0,0,128));
	//if(MTapiDrawMouseOverAreas) MTdrawSmoothFillRoundRectangle(x-MTareaShowBorder, y-MTareaShowBorder, x+width+MTareaShowBorder, y+height+MTareaShowBorder, RGBA(255,0,0,128), MTareaShowBorder, 4);
	return 0;
}

void MTresetScreenClip();
void MTsetClipArea(MTclipArea clip);
MTclipArea MTgetCurrentClipArea();

bool MTisOverArea(int mouseX, int mouseY, int x, int y, int width, int height) {
	
	bool res = MTisOverAreaSimple(mouseX, mouseY, x, y, width, height);
	
	if(isDrawing && MTapiDrawMouseOverArea) {
		MTclipArea clipBackup = MTgetCurrentClipArea();
		MTresetScreenClip();
		//MTdrawFillRectangle(x, y, x+width, y+height, RGBA(0,255,0,128));
		if(res == 1) {
			MTdrawSmoothFillRoundRectangle(x-MTareaShowBorder, y-MTareaShowBorder, x+width+MTareaShowBorder, y+height+MTareaShowBorder, RGBA(0,255,0,128), MTareaShowBorder, 4);
		} else MTdrawSmoothFillRoundRectangle(x-MTareaShowBorder, y-MTareaShowBorder, x+width+MTareaShowBorder, y+height+MTareaShowBorder, RGBA(255,0,0,128), MTareaShowBorder, 4);
		MTsetClipArea(clipBackup);
	}
	
	return res;
}

bool MTisOverImage(int mouseX, int mouseY, MTimage* img) {
	if(img == NULL) return 0;
	return MTisOverArea(mouseX, mouseY, img->x, img->y, img->stretchX, img->stretchY);
}

float MTdeg2rad(float valor) {
	return ((valor*2*PI)/360);
}

float MTrad2deg(float valor) {
	return ((valor*360)/(2*PI));
}

time_t MTscreenFpsLastTime;
int MTscreenFps = 0;
int MTscreenFps2 = 0;

int MTreadFPS() {
	time_t currentTime;
	time(&currentTime);
	// Increase the frame counter
	++MTscreenFps2;
	if( difftime(currentTime,MTscreenFpsLastTime) > 0.5f ) {
		MTscreenFpsLastTime = currentTime;
		MTscreenFps = MTscreenFps2*2;
		MTscreenFps2 = 0;
	}
	return MTscreenFps;
}

#include "minIni/MTiniFunctions.h"

void MTsaveConfig() {
	
	char * ini = "./Data/Configuration/General.ini";
	
	MTcreateFile(ini);
	
	MTiniSetInteger(ini, "DefaultFont","MTdefaultFontHeight", MTdefaultFontHeight);
	
	//MTiniSetString(ini, "DefaultFont","MTdefaultFontPath", MTdefaultFontPath);
	
	MTiniSetColor(ini, "DefaultFont","MTdefaultFontColor", MTdefaultFontColor);
	
	//MTiniSetString(ini, "Cursor","MTmainCursorImagePath", MTmainCursorImagePath);
	
	//MTiniSetVector(ini, "Cursor","MTmainCursorCenter", MTmainCursorCenter);
	
	MTiniSetString(ini, "Wallpaper","MTmainWalpaperImagePath", MTmainWalpaperImagePath);
	
	//MTiniSetString(ini, "Test","test2", test);
	
	
	ini = "./Data/Configuration/Window.ini";
	MTcreateFile(ini);
	
	MTiniSetString(ini, "Buttons","MTwindowCloseButtonPath",MTwindowCloseButtonPath);
	MTiniSetString(ini, "Buttons","MTwindowMaximizeButtonPath",MTwindowMaximizeButtonPath);
	MTiniSetString(ini, "Buttons","MTwindowMinimizeButtonPath",MTwindowMinimizeButtonPath);
	MTiniSetString(ini, "Buttons","MTwindowRestoreButtonPath",MTwindowRestoreButtonPath);
	
	return;
}

MTvector* MTvct(int x, int y) {
	MTvector* vect;
	vect = (MTvector*)malloc(sizeof(MTvector));
	vect->x = x;
	vect->y = y;
	return vect;
}

MTimage* MTloadImageFile (char *filename);
void MTfreeImage (MTimage* img);

void MTloadConfig() {
	
	char * ini = "./Data/Configuration/General.ini";
	MTcreateFile(ini);
	
	MTdefaultFontHeight = MTiniGetInteger(ini, "DefaultFont","MTdefaultFontHeight", 10);
	
	
	//MTiniGetString(ini, "DefaultFont","MTdefaultFontPath", "flash0:/font/ltn8.pgf",MTdefaultFontPath);
	
	MTdefaultFontColor = MTiniGetColor(ini, "DefaultFont","MTdefaultFontColor", RGB(128,128,128));
	
	//MTiniGetString(ini, "Cursor","MTmainCursorImagePath", "./Data/Cursors/Normal.png",MTmainCursorImagePath);
	
	//MTvector* vect = MTvct(11,8);
	//MTmainCursorCenter = MTiniGetVector(ini, "Cursor","MTmainCursorCenter", vect);
	//if(vect != MTmainCursorCenter) MTfreeIfNotNull(vect);
	
	MTiniGetString(ini, "Wallpaper","MTmainWalpaperImagePath", "./Data/Backgrounds/Blue.jpg",MTmainWalpaperImagePath);
	
	//MTiniGetString(ini, "Test","test2", "aáéíóúñçÁÉÍÓÚª!\"·$%&/()=?¿º|@#~½¬{[]}\\'¡*^àèìòù prueba",test);
	
	MTapiSetWallpaperImage(MTmainWalpaperImagePath);
	
	//MTapiSetCursorImage(MTmainCursorImagePath,MTmainCursorCenter->x, MTmainCursorCenter->y);
	
	
	ini = "./Data/Configuration/Window.ini";
	MTcreateFile(ini);
	
	MTiniGetString(ini, "Buttons","MTwindowCloseButtonPath", "./Data/Buttons/Close.png",MTwindowCloseButtonPath);
	MTiniGetString(ini, "Buttons","MTwindowMaximizeButtonPath", "./Data/Buttons/Maximize.png",MTwindowMaximizeButtonPath);
	MTiniGetString(ini, "Buttons","MTwindowMinimizeButtonPath", "./Data/Buttons/Minimize.png",MTwindowMinimizeButtonPath);
	MTiniGetString(ini, "Buttons","MTwindowRestoreButtonPath", "./Data/Buttons/Restore.png",MTwindowRestoreButtonPath);
	
	if(MTwindowCloseButton != NULL) MTfreeImage(MTwindowCloseButton);
	MTwindowCloseButton = MTloadImageFile(MTwindowCloseButtonPath);
	
	if(MTwindowRestoreButton != NULL) MTfreeImage(MTwindowRestoreButton);
	MTwindowRestoreButton = MTloadImageFile(MTwindowRestoreButtonPath);
	
	if(MTwindowMinimizeButton != NULL) MTfreeImage(MTwindowMinimizeButton);
	MTwindowMinimizeButton = MTloadImageFile(MTwindowMinimizeButtonPath);
	
	if(MTwindowMaximizeButton != NULL) MTfreeImage(MTwindowMaximizeButton);
	MTwindowMaximizeButton = MTloadImageFile(MTwindowMaximizeButtonPath);
	
	MTsaveConfig();
	return;
}

int MTremoveTagsFromString(char* buf, char* result, int len, char* head, char* end, char* taginit, char* tagend) {
	if(buf == NULL || result == NULL) return -1;
	char* data;
	if(head != NULL) data = strstr(buf, head); else data = buf;
	if(data == NULL) return -2;
	if(head != NULL) data += strlen(head);
	
	//printf(data);
	
	int endLen;
	if(end != NULL) endLen = strlen(end); else endLen = 0;
	//int i = 0;
	int bufferi = 0;
	while(bufferi < len) {
		if(endLen > 0 && strncmp (data,end,endLen) == 0) break;
		if(*data == '\0') break;
		while(*data == *taginit) {
			data = strstr(data, tagend);
			if(data == NULL) break;
			if(*data == '\0') break;
			data++;
			//if(i >= len) break;
			//i++;//jump the '>'
			//*data++;
		}
		if(*data == '\0') break;
		result[bufferi] = *data;
		//i++;
		data++;
		bufferi++;
	}
	result[bufferi] = '\0';
	//bufferi++;
	//result[bufferi] = '\0';
	return 1;
}

int MTgetStringBetweenTags(char* buf, char* result, int len, char* head, char* end) {
	if(buf == NULL || result == NULL) return -1;
	char* data;
	if(head != NULL) data = strstr(buf, head); else return -1;
	if(data == NULL) return -2;
	if(head != NULL) data += strlen(head);
	
	//printf(data);
	
	int endLen;
	if(end != NULL) endLen = strlen(end); else return -1;
	//int i = 0;
	int bufferi = 0;
	while(bufferi < len) {
		if(endLen > 0 && strncmp (data,end,endLen) == 0) break;
		if(*data == '\0') break;
		result[bufferi] = *data;
		//i++;
		data++;
		bufferi++;
	}
	result[bufferi] = '\0';
	//bufferi++;
	//result[bufferi] = '\0';
	return 1;
}

int MTcopyFile(char *orig,char *dest) {
	int inF, ouF;
	char line[512];
	int bytes;
	
	int ret = -1;
	
	if((inF = open(orig, O_RDONLY)) != -1) {
		MTcreateFile(dest);
	if((ouF = open(dest, O_WRONLY | O_CREAT)) != -1) {
		
		while((bytes = read(inF, line, sizeof(line))) > 0) write(ouF, line, bytes);
		close(ouF);
		ret = 0;
	}
	
	close(inF);
	}
	
	return ret;
}

int MTzipFileCount(char *zipfile) {
	if(zipfile == NULL) return 0;
	int i = 0;
	//MTcreatePath(extDir);
	//chdir(extDir);
	ZZIP_DIR* dir = zzip_dir_open(zipfile,0);
	if (dir) {
		ZZIP_DIRENT dirent;
		while (zzip_dir_read(dir,&dirent)) {
			i++;
		}
		zzip_dir_close(dir);
	}
	return i;
}

int MTextractZIP(char *zipfile,char *extDir, char *percent) {
	if(zipfile == NULL || extDir == NULL) return 1;
	int i = 0;
	
	int fileCount = MTzipFileCount(zipfile);
	
	if(fileCount <= 0) return 1;
	
	//int error = 0;
	
	//MTcreatePath(extDir);
	//chdir(extDir);
	ZZIP_DIR* dir = zzip_dir_open(zipfile,0);
	if (dir != NULL) {
		ZZIP_DIRENT dirent;
		while (zzip_dir_read(dir,&dirent)) {
			
			
			ZZIP_FILE * newfile = zzip_file_open(dir, dirent.d_name, O_RDONLY);
			
			if(newfile != NULL) {
				int ouF;
				
				char realPath[512];
				realPath[0] = '\0';
				strcat(realPath,extDir);
				//int varLen = MTstrlen(realPath);
				//if(realPath[varLen - 1] != '\\')
				//strcat(realPath,"\\");
				
				if(realPath[strlen(realPath)-1] != '/') strcat(realPath,"/");
				
				//int lenght = strlen(realPath);
				//char back = realPath[lenght-1];
				
				//realPath[lenght-1] = '\0';
				
				//if(MTdoesDirectoryExists(realPath) == 0) realPath[lenght-1] = back;
				
				//strcat(realPath,"\\");
				
				strcat(realPath,dirent.d_name);
				
				//char* realPath = extDir;
				
				MTcreateFile(realPath);
				
				ouF = open(realPath, O_WRONLY | O_CREAT);
				
				if(ouF != -1) {
					
					char line[512];
					int bytes;
					
					while((bytes = zzip_file_read(newfile, line, sizeof(line))) > 0) write(ouF, line, bytes);
					
					close(ouF);
				}
				
				zzip_file_close(newfile);
				
				//lua_pushstring(L, "name"); lua_pushstring(L, dirent.d_name); lua_settable(L, -3); 
				//lua_pushstring(L, "size"); lua_pushnumber(L, dirent.st_size); lua_settable(L, -3);		
				//lua_pushstring(L, "csize"); lua_pushnumber(L, dirent.d_csize); lua_settable(L, -3);
				//lua_pushstring(L, "method"); lua_pushnumber(L, dirent.d_compr); lua_settable(L, -3);
				//lua_pushstring(L, "ratio"); lua_pushnumber(L, (100.0f -(100.0f*dirent.d_csize / dirent.st_size))); lua_settable(L, -3);
			
			}
			i++;
			
			*percent = MTpercent(i,100,fileCount);
			
		}
		zzip_dir_close(dir);
	} else return 1;
	//chdir(MTrootDir);
	return 0;
}

/*
Results:
0: OK
1: File doesnt exists
2: (File corrupt/Not rar)?
3: Wrong password
8: Not enough memory?
*/
int MTlockRar = 0;
int MTextractRAR(char *rarfile,char *extDir,char *pass) {
	while(MTlockRar == 1) MTdelay(1000);
	MTlockRar = 1;
	int ret = rarExtract(rarfile,extDir,pass);
	MTlockRar = 0;
	return ret;
}

MTextract* MTcreateExtraction(char* file, char* dest, int type) {
	
	if(file == NULL || dest == NULL) return NULL;
	//message(name);
	//message(data);
	//MTapiAddWindow(MTapiWindow* window)
	MTextract* extraction = (MTextract*)MTmalloc(sizeof(MTextract));
	
	asprintf(&extraction->file,file);
	asprintf(&extraction->path,dest);
	
	extraction->type = type;
	extraction->error = 0;
	extraction->ret = 0;
	extraction->percent = 0;
	extraction->done = 0;
	extraction->pass = NULL;
	return extraction;
}

void MTcancelExtraction(MTextract* extraction);

void MTfreeExtraction(MTextract* extraction) {
	if(extraction == NULL) return;
	
	MTcancelExtraction(extraction);
	
	if(extraction->path != NULL) MTfree(extraction->path);
	if(extraction->file != NULL) MTfree(extraction->file);
	extraction->path = NULL;
	extraction->file = NULL;
	MTfree(extraction);
	extraction = NULL;
	return;
}

int MTdoExtract(MTextract* extraction) {
	if(extraction == NULL) return -1;
	if(extraction->file == NULL) return -2;
	if(extraction->path == NULL) return -3;
	
	extraction->error = 0;
	extraction->ret = 0;
	extraction->done = 0;
	
	extraction->percent = 0;
	
	if(extraction->type == MTunpackTypeRar) {
		extraction->ret = MTextractRAR(extraction->file,extraction->path,extraction->pass);
	} else if(extraction->type == MTunpackTypeZip) {
		extraction->ret = MTextractZIP(extraction->file,extraction->path,&extraction->percent);
	}
	
	extraction->percent = 100;
	
	if(extraction->ret != 0) {
		extraction->error = 1;
	} else extraction->done = 1;
	
	return extraction->ret;
}

int MTlockRarQueue = 0;
void MTaddToExtractQueue(MTextract* extraction) {
	if(extraction == NULL) return;
	while(MTlockRarQueue == 1) MTdelay(1000);
	MTlockRarQueue = 1;
	MTpositionInList rarPosQueue = MTlastPositionInList(MTextractQueue);
	MTinsertElementInList (extraction, rarPosQueue, MTextractQueue);
	MTlockRarQueue = 0;
	return;
}

void MTcancelExtraction(MTextract* extraction) {
	if(extraction == NULL) return;
	while(MTlockRarQueue == 1) MTdelay(1000);
	MTlockRarQueue = 1;
	MTpositionInList rarPosQueue = MTelementPositionInList (extraction, MTextractQueue);
	if(rarPosQueue != MTlastPositionInList(MTextractQueue)) MTeraseElementFromList (&rarPosQueue, MTextractQueue);
	MTlockRarQueue = 0;
	return;
}

bool MTflipBool(bool num) {
	if(num == 0) return 1;
	return 0;
}



#include "MTcontrols.h"
#include "MTgraphics.h"
#include "MTnetwork.h"
#include "MTsounds.h"

/*typedef struct {
	char* title;
	char* text;
	char* initialText;
	int len;
	int maxLen;
	int lines;
	int language;
	bool cancel;
	bool done;
} MTkeyboard;*/

int cccStrlenUTF8(cccCode const * str);
int cccStrlenUCS2(cccUCS2 const * str); 

/**
 * Character code conversion 
 *
 * @param dst - output string 
 *
 * @param count - size of output buffer
 *
 * @param str - input string
 *
 * @param cp - codepage 
 *
 * @returns number of converted character codes
 */
//int cccUTF8toUCS2(cccUCS2 * dst, size_t count, cccCode const * str);


MTkeyboard* MTcreateKeyboard(char* title, char* initialText, int maxLen) {
	MTkeyboard* keyboard = (MTkeyboard*)MTmalloc(sizeof(MTkeyboard));
	
	keyboard->title = NULL;
	keyboard->text = NULL;
	keyboard->initialText = NULL;
	
	if(title != NULL) {
		keyboard->title = (char*)MTmalloc((MTstrlen(title) +1) * sizeof(char));
		strcpy(keyboard->title,title);
	}
	
	if(initialText != NULL) {
		keyboard->initialText = (char*)MTmalloc((MTstrlen(initialText) +1) * sizeof(char));
		strcpy(keyboard->initialText,initialText);
	}
	
	keyboard->len = 0;
	
	if(maxLen <= 0) maxLen = 1;
	keyboard->maxLen = maxLen;
	
	keyboard->len = 0;
	
	keyboard->lines = 1;
	
	keyboard->language = -1;
	
	keyboard->cancel = 0;
	
	keyboard->done = 0;
	
	return keyboard;
}


int MTkeyboardResult = 0;

char* MTkeyboardResultText;

int MTkeyboardMaxLen = 0;

int MTkeyboardResultTextLen = 0;

unsigned char* utf16le_to_utf8_alloc2(unsigned short* str, int* size) {
	
	unsigned char* dst = NULL;
	
	if (!str) if(size != NULL) {
		*size = 0;
		return NULL;
	}
	
	int len = 0;
	
	while(str[len] != 0) len++;	
	if(len <= 0) if(size != NULL) {
		*size = 0;
		return NULL;
	}
	
	len *= 2;
		
	//memset(convert_buffer, 0, CONVERT_BUFFER_SIZE+1);
	
	dst = (unsigned char*) malloc((len + 1) * sizeof(unsigned char*));
	
	unsigned char* p = (unsigned char*)str;
	
	int currDstSize = len;
	
	ucs4_t ucs4_char;
	int i,j, ret_val;
	i = 0;
	j = 0; 
    while (i < len) {
		ret_val = utf16le_mbtowc(&ucs4_char, p+i, len-i);
		if ( ret_val < 0 ) { 
			i++;
			if(j >= currDstSize) {
				currDstSize = j;
				dst = (unsigned char*) realloc(dst,(currDstSize + 1) * sizeof(unsigned char*));
			}
			dst[j] = '?';
			j++;
		}
		else {
			i += ret_val;
			if(j >= currDstSize) {
				currDstSize = j + 6;
				dst = (unsigned char*) realloc(dst,(currDstSize + 1) * sizeof(unsigned char*));
			}
			ret_val = utf8_wctomb((unsigned char*)dst+j, ucs4_char, 6);
			if ( ret_val > 0 ) j += ret_val;
		}
	}
	if(j >= len) dst = (unsigned char*) realloc(dst,(j + 1) * sizeof(unsigned char*));
	dst[j] = '\0';
	if(size != NULL) *size = j;
    return dst;
}

void MTdrawKeyboard() {
	MTresetScreenClip();
	//MTresetDrawFX();
	//MTapiResetDefaultFont();
	
	if (oslOskIsActive()){
		oslDrawOsk();
		if (oslGetOskStatus() == PSP_UTILITY_DIALOG_NONE){
			if (oslOskGetResult() == OSL_OSK_OK) {
				unsigned short* result1 = oslOskGetOutData();
				//message("0");
				if(result1 != NULL) {
					//message("1");
					unsigned char* result = NULL;// = utf16le_to_utf8_old(result1);
					
					result = utf16le_to_utf8_alloc2(result1,&MTkeyboardResultTextLen);
					//message("2");
					if(result != NULL) {
						//message("3");
						//message(result);
						//MTkeyboardResultTextLen = MTstrlen(result);
						if(MTkeyboardResultTextLen >= 0) {
							
							MTkeyboardResultText = (char*) result;//(char*) MTmalloc((MTkeyboardResultTextLen + 1) * sizeof(char));
							//message(MTkeyboardResultText);
							//strcpy(MTkeyboardResultText,result);
							result = NULL;
							MTkeyboardResult = 1;
						} else MTkeyboardResult = 2;
					} else MTkeyboardResult = 2;
				} else MTkeyboardResult = 2;
				
				//char userText[100] = "";
				//oslOskGetText(userText);
				//char* result = utf16le_to_utf8(oslOskGetOutData());
				//MTkeyboardResultText = (char*)MTmalloc((MTkeyboardMaxLen +1) * sizeof(char));
				//if(MTkeyboardResultText != NULL) {
					//strcpy(MTkeyboardResultText,result);
					//oslOskGetFullText(MTkeyboardResultText);
					MTkeyboardResult = 1;
				//} else MTkeyboardResult = 2;
			} else {
				//sprintf(message, "Cancel");
				MTkeyboardResult = 2;
			}
			oslEndOsk();
			MTthereIsOverlay = 0;
		}
	} else {
		MTthereIsOverlay = 0;
		MTkeyboardResult = 3;
	}
	return;
}

void MTshowKeyboard(MTkeyboard* keyboard) {
	
	
	if(keyboard == NULL) return;
	
	if(MTisUsbOn()) MTstopUsb();
	
	if(keyboard->lines < 1) keyboard->lines = 1;
	
	if(keyboard->maxLen <= 0) keyboard->maxLen = 1;
	
	keyboard->done = 0;
	keyboard->cancel = 0;
	
	while(MTthereIsOverlay == 1 || MToverlayAlphaValue > 0) MTdelay(100000);
	
	MTlockControls();
	
	MToverlayAlphaSpeed = 4;
	MToverlayAlphaMaxValue = 200;
	MToverlayColor = RGB(51,68,85);
	
	oslInitOsk(keyboard->title, keyboard->initialText, keyboard->maxLen, keyboard->lines, keyboard->language);
	
	MToverlayDrawFunction = &MTdrawKeyboard;
	
	MTthereIsOverlay = 1;
	
	MTkeyboardResult = 0;
	MTkeyboardMaxLen = keyboard->maxLen;
	
	MTkeyboardResultText = NULL;
	
	while(MTthereIsOverlay == 1) MTdelay(100000);
	
	if(MTkeyboardResult == 1) {
		//keyboard->text = (char*)MTmalloc((keyboard->maxLen +1) * sizeof(char));
		
		keyboard->len = 0;
		
		/*unsigned short* result1 = oslOskGetOutData();
		message("0");
		if(result1 != NULL) {
			message("1");
			char* result = utf16le_to_utf8(result1);
			message("2");
			if(result != NULL) {
				message("3");
				message(result);
				keyboard->len = MTkeyboardResultTextLen;
				if(keyboard->len >= 0) {
					keyboard->text = (char*) MTmalloc((keyboard->len + 1) * sizeof(char));
					strcpy(keyboard->text,result);
					keyboard->cancel = 0;
				} else keyboard->cancel = 1;
			} else keyboard->cancel = 1;
		} else keyboard->cancel = 1;*/
		
		//char userText[100] = "";
		//oslOskGetText(userText);
		//char* result = utf16le_to_utf8(oslOskGetOutData());
		//MTkeyboardResultText = (char*)MTmalloc((MTkeyboardMaxLen +1) * sizeof(char));
		//if(MTkeyboardResultText != NULL) {
			//strcpy(MTkeyboardResultText,result);
			//oslOskGetFullText(MTkeyboardResultText);
			//MTkeyboardResult = 1;
		//} else MTkeyboardResult = 2;
		
		if(MTkeyboardResultText != NULL) {
			keyboard->text = MTkeyboardResultText;
			//MTputToFile("ms0:/keyboardinput1", MTkeyboardResultText);
			//MTputToFile("ms0:/keyboardinput2", keyboard->text);
			MTkeyboardResultText = NULL;
			//MTputToFile("ms0:/keyboardinput3", keyboard->text);
			//memset (keyboard->text,'\0',(keyboard->maxLen));
			//oslOskGetFullText(keyboard->text);
			
			//message(keyboard->text);
			//MTputToFile("ms0:/keyboardinput", keyboard->text);
			
			keyboard->len = MTkeyboardResultTextLen;
			
			keyboard->cancel = 0;
		} else keyboard->cancel = 1;
		
		keyboard->done = 1;
		
	} else {
		keyboard->done = 1;
		keyboard->cancel = 1;
	}
	
	MTunLockControls();
	
	if(MTisUsbOn()) MTstartUsb();
	
	return;
}

int MTlockKeyboardQueue = 0;
void MTaddToKeyboardQueue(MTkeyboard* keyboard) {
	if(keyboard == NULL) return;
	while(MTlockKeyboardQueue == 1) MTdelay(1000);
	MTlockKeyboardQueue = 1;
	MTpositionInList keyboardPosQueue = MTlastPositionInList(MTkeyboardQueue);
	MTinsertElementInList (keyboard, keyboardPosQueue, MTkeyboardQueue);
	MTlockKeyboardQueue = 0;
	return;
}

void MTcancelKeyboard(MTkeyboard* keyboard) {
	if(keyboard == NULL) return;
	while(MTlockKeyboardQueue == 1) MTdelay(1000);
	MTlockKeyboardQueue = 1;
	MTpositionInList keyboardPosQueue = MTelementPositionInList (keyboard, MTkeyboardQueue);
	if(keyboardPosQueue != MTlastPositionInList(MTkeyboardQueue)) MTeraseElementFromList (&keyboardPosQueue, MTkeyboardQueue);
	MTlockKeyboardQueue = 0;
	return;
}

void MTfreeKeyboard(MTkeyboard* keyboard) {
	if(keyboard == NULL) return;
	
	MTcancelKeyboard(keyboard);
	
	if(keyboard->title != NULL) MTfree(keyboard->title);
	
	if(keyboard->text != NULL) MTfree(keyboard->text);
	
	if(keyboard->initialText != NULL) MTfree(keyboard->initialText);
	
	keyboard->title = NULL;
	keyboard->text = NULL;
	keyboard->initialText = NULL;
	
	MTfree(keyboard);
	
	keyboard = NULL;
	
	return;
}

#include "MTapi.h"
