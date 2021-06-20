/*
Nombre/Name: multiTasKing for PSP
Desarrollador/Developer: Carlosgs (carlosgarciasaura@gmail.com)

AL UTILIZAR ESTE CODIGO FUENTE ACEPTAS QUE:
- Tienes que notificar al autor (Carlosgs) de que vas a usar este codigo (contactar en: carlosgarciasaura@gmail.com).
- Carlosgs no se hace responsable de cualquier daño que este codigo pueda causar.
- No hay NINGUNA garantía de funcionalidad.
- No ganarás dinero ni beneficios con un programa que contenga parte de este código.

WHEN YOU USE THIS SOURCE CODE YOU ACCEPT THAT:
- You have to notify the author (Carlosgs) that you are going to use this source (contact trough: carlosgarciasaura@gmail.com).
- Carlosgs is NOT responsable of any damage that this source code can cause.
- There is no warranty of functionality.
- You wont earn money or benefits with a program that contais part of this source.
*/

int MTKversion = 300;

#include "libs/basicHeaders.h"

//int RAR_RESULT = -1;

int mouseControllerThread(SceSize args, void *argp)
{
	MTsetupThread();
	
	int analogX = 0;
	int analogY = 0;
	
	int newCursorX = 0;
	int newCursorY = 0;
	
	float time = 0.55;//0.5
	
	int minimumCursorPadSensitivity = 60;
	int aceleracionMaximaCursor = 8;
	int CursorXAcceleration = 0;
	int CursorYAcceleration = 0;
	int CursorXSpeed = 0;
	int CursorYSpeed = 0;
	
	bool pressingCross = 0;
	bool samePosition = 0;
	
	//MTextract extraction;
	
	while(!MT_quit) {
		if(screenLocked == 0) {
			if(controlsLocked == 0) {
				MTreadControls(&MTrealPressedControls);//leemos controles
				
						if(MTrealPressedControls.triangle) {
							MTrealPressedControls.triangle = 0;
							/*MTnetGetFile("http://carlosgs.no-ip.org:8080/PSPhomebrew/multiTasKing/multiTasKing-ver101.rar","file2.rar",NULL);
							extraction.file = "file2.rar";
							extraction.path = "file2";
							extraction.type = MTunpackTypeRar;
							extraction.error = 0;
							extraction.ret = 0;
							extraction.done = 0;
							extraction.pass = NULL;
							MTaddToExtractQueue(&extraction);
							//MTextractRAR("file2.rar","file2",NULL);
							MTreadControls(&MTpressedControls);
							while(MTpressedControls.triangle) {
								MTreadControls(&MTpressedControls);
								MTdelay(100000);//0.1seg
							}*/
							//MTshowNetworkDialog();
						}
				
				analogX = MTrealPressedControls.ax;
				analogY = MTrealPressedControls.ay;
				
				newCursorX = realMouse.x;//No usamos los reales puesto que pueden confundir a los threads
				newCursorY = realMouse.y;
				
				CursorXAcceleration = 0;
				CursorYAcceleration = 0;
				
				if(analogX > minimumCursorPadSensitivity) {
					CursorXAcceleration = MTpercent(analogX - minimumCursorPadSensitivity,aceleracionMaximaCursor,127.5 - minimumCursorPadSensitivity);
				} else if(analogX < -minimumCursorPadSensitivity) {
					CursorXAcceleration = MTpercent(analogX + minimumCursorPadSensitivity,aceleracionMaximaCursor,127.5 - minimumCursorPadSensitivity);
				} else CursorXSpeed = 0;

				if(analogY > minimumCursorPadSensitivity) {
					CursorYAcceleration = MTpercent(analogY - minimumCursorPadSensitivity,aceleracionMaximaCursor,127.5 - minimumCursorPadSensitivity);
				} else if(analogY < -minimumCursorPadSensitivity) {
					CursorYAcceleration = MTpercent(analogY + minimumCursorPadSensitivity,aceleracionMaximaCursor,127.5 - minimumCursorPadSensitivity);
				} else CursorYSpeed = 0;
				
				CursorXSpeed += (CursorXAcceleration * time);
				CursorYSpeed += (CursorYAcceleration * time);
				
				newCursorX += (CursorXSpeed * (time*time) * 0.5);
				newCursorY += (CursorYSpeed * (time*time) * 0.5);
				
				if( newCursorX > screenWidth ) newCursorX = screenWidth;
				if( newCursorX < 0 ) newCursorX = 0;
				if( newCursorY > screenHeight ) newCursorY = screenHeight;
				if( newCursorY < 0 ) newCursorY = 0;
				
				
				if(realMouse.x == newCursorX && realMouse.y == newCursorY) {
					samePosition = 1;
				} else {
					samePosition = 0;
				}
				
				realMouse.x = newCursorX;
				realMouse.y = newCursorY;
				
				if(MTrealPressedControls.cross) {
					if(pressingCross == 0) {
						pressingCross = 1;
						realMouse.tap++;
						realMouse.drag.x = realMouse.x;
						realMouse.drag.y = realMouse.y;
					}
					realMouse.dragging = 1;
					
				} else {
					pressingCross = 0;
					realMouse.dragging = 0;
				}
				
				if(samePosition == 0) realMouse.tap = 0;
				
			} else {
				MTemptyControls(&MTrealPressedControls);
				CursorXSpeed = 0;
				CursorYSpeed = 0;
			}
		} else {
			MTemptyControls(&MTrealPressedControls);
			CursorXSpeed = 0;
			CursorYSpeed = 0;
		}
		MTdelay(10000);
	}
	
	if(MTisUsbOn()) MTstopUsb();
	MTdelay(1000000);
	//MTendMainLib();
	sceKernelExitGame();
	sceKernelExitDeleteThread(0);
	return 0;
}

MTdownload descarga;

void MTlaunchAppImageViewer(char* name, char* path);

int screenControllerThread(SceSize args, void *argp)
{
	MTsetupThread();
	
	//MTdelay(10000000);
	//oslNetGetFile("http://carlosgs.no-ip.org:8080/PSPhomebrew/multiTasKing/multiTasKing-ver102.rar","ms0:/multiTasKing-ver102_2.rar");
	
	int count = 0;
	//char path[200];
	
	/*MTextract extraction;
	extraction.type = MTunpackTypeRar;
	extraction.error = 0;
	extraction.ret = 0;
	extraction.done = 0;
	extraction.file = "file.rar";
	extraction.path = "file";
	extraction.pass = NULL;*/
	
	while(!MT_quit) {
		scePowerTick(0);
		//scePowerSetClockFrequency(333, 333, 166); //Maxima velocidad de la CPU
		
		if(MTrealPressedControls.select) {
			//MTnetGetFile("http://carlosgs.no-ip.org:8080/PSPhomebrew/multiTasKing/multiTasKing-ver102.rar","file.rar",NULL);
			//MTnetGetFile("http://psp.scenebeta.com/system/files/private/downPSPCv222CFPlusMulti2.rar","file.rar","PRIVATE");
			
			//descarga.url = "http://psp.scenebeta.com/system/files/private/downPSPCv222CFPlusMulti2.rar";
			//descarga.path = "file.rar";
			//descarga.cookie = "PRIVATE";
			/*descarga.url = "http://carlosgs.no-ip.org:8080/PSPhomebrew/multiTasKing/multiTasKing-ver102.rar";
			descarga.path = "file.rar";*/
			//MTdoDownload(&descarga);
			//MTaddToExtractQueue(&extraction);
			//RAR_RESULT = MTextractRAR("file.rar","file",NULL);
			//MTshowNetworkDialog();
			while(MTrealPressedControls.select) MTdelay(100000);//0.1seg
		}
		
		//RAR_RESULT = extraction.ret;
		
		if(MTrealPressedControls.start) {
			sprintf(MTscreenshotPath,"ms0:/PICTURE/multiTasKing%i.png",count);
			while(MTdoesFileExists(MTscreenshotPath) == 1) {
				count++;
				sprintf(MTscreenshotPath,"ms0:/PICTURE/multiTasKing%i.png",count);
			}
			MTlockControls();
			MTlockScreen();
			//MTsaveScreenToPNG(path);
			MTscreenshotImage = MTgetScreenImage();
			MTunLockScreen();
			
			MToverlayAlphaSpeed = 10;
			MToverlayAlphaMaxValue = 200;
			MToverlayColor = RGB(255,255,255);
			
			void MTdrawCameraLogo() {
				MTresetScreenClip();
				MTapiResetDefaultFont();
				/*MTdrawFX effects;
				effects.effect = MTdrawFXalpha;
				effects.coeff1 = MToverlayAlphaValue;
				effects.coeff2 = 0;*/
				MTsetDrawFX(MTdrawFXalpha,MToverlayAlphaValue,0);
				
				int size = MToverlayAlphaValue;//200;
				MTapiDrawIcon(26, 480/2-size/2, 272/2-size/2, size);
				
				/*MTscreenshotImage->stretchX = MToverlayAlphaValue;
				float proportion = (float)MTscreenshotImage->stretchX/(float)MTscreenshotImage->sizeX;
				MTscreenshotImage->stretchY = (float)MTscreenshotImage->sizeY*proportion;
				MTscreenshotImage->x = 480/2;
				MTscreenshotImage->y = 272/2-MTscreenshotImage->stretchY/2;
				MTscreenshotImage->offsetX0 = 0;
				MTscreenshotImage->offsetY0 = 0;
				MTscreenshotImage->offsetX1 = 480;
				MTscreenshotImage->offsetY1 = 272;
				MTscreenshotImage->centerX = 0;
				MTscreenshotImage->centerY = 0;
				MTscreenshotImage->angle = 0;
				
				//MTscreenshotImage->x = 0;
				//MTscreenshotImage->y = 0;
				//MTscreenshotImage->stretchX = 480;
				//MTscreenshotImage->stretchY = 272;
				MTdrawImage(MTscreenshotImage);*/
				//oslDrawImageSimple(OSL_SECONDARY_BUFFER);
				//oslDrawImageXY(OSL_SECONDARY_BUFFER,40,40);
				
				//DrawThing(OSL_SECONDARY_BUFFER);
				
				/*osl_secondaryBufferImage.x = 30;
				osl_secondaryBufferImage.y = 30;
				MTdrawImage(&osl_secondaryBufferImage);*/
				
				MTsetTextHeight(MToverlayAlphaValue/10);
				int txtWidth = MTgetTextWidth(MTscreenshotPath);
				MTdrawString(480/2-txtWidth/2,272/2+size/2,MTscreenshotPath);
				return;
			}
			
			MToverlayDrawFunction = MTdrawCameraLogo;
			MTthereIsOverlay = 1;
			
			while(MToverlayAlphaValue < MToverlayAlphaMaxValue && MTthereIsOverlay == 1) MTdelay(10000);
			sceKernelChangeThreadPriority (sceKernelGetThreadId(), priorityVeryHigh);
			MTsaveImageToPNG(MTscreenshotImage, MTscreenshotPath, 0);
			sceKernelChangeThreadPriority (sceKernelGetThreadId(), priorityNormal);
			
			//while(MToverlayAlphaValue < MToverlayAlphaMaxValue && MTthereIsOverlay == 1) MTdelay(100000);
			//MTdelay(1000000);
			
			MTthereIsOverlay = 0;
			MTunLockControls();
			
			MTdelay(1000000);
			
			//sceKernelChangeThreadPriority (sceKernelGetThreadId(), priorityVeryHigh);
			//MTsaveImageToPNG(MTscreenshotImage, MTscreenshotPath, 0);
			//sceKernelChangeThreadPriority (sceKernelGetThreadId(), priorityNormal);
			
			MTfreeImage(MTscreenshotImage);
			
			char name[128];
			sprintf(name,"Screenshot: multiTasKing%i.png",count);
			MTlaunchAppImageViewer(name, MTscreenshotPath);
			
			//runPRXfromRelativePath("./Programs/PNGer/app.prx",measureTextWidth(path)+1,path);//Mostramos la captura que hemos hecho con PNGer
			count++;
			while(MTrealPressedControls.start) MTdelay(100000);//0.1seg
		}
		MTdelay(500000);//Cada medio segundo (no se necesita mas velocidad)
	}
	sceKernelExitDeleteThread(0);
	return 0;
}

int unpackThread(SceSize args, void *argp)
{
	MTsetupThread();
	
	//MTextractQueue = MTcreateElementsList();
	
	/*MTextract extraction;
	extraction.type = MTunpackTypeRar;
	extraction.error = 0;
	extraction.ret = 0;
	extraction.done = 0;
	extraction.file = "file.rar";
	extraction.path = "file";
	extraction.pass = NULL;
	MTaddToExtractQueue(&extraction);
	
	MTdelay(10000000);*/
	
	MTpositionInList rarPosQueue;
	
	MTextract* RarData;
	
	//MTdoExtractRAR(&extraction);
	
	while(!MT_quit) {
		rarPosQueue = MTfirstPositionInList(MTextractQueue);
		if ((rarPosQueue != MTlastPositionInList(MTextractQueue))) {
			RarData = (MTextract*)MTgetElementFromList(rarPosQueue, MTextractQueue);
			if(RarData != NULL) {
			MTdoExtract(RarData);
			
			/*RarData->error = 0;
			RarData->done = 0;
	
			if(RarData->type == MTunpackTypeRar) {
				RarData->ret = MTextractRAR(RarData->file,RarData->path,RarData->pass);
			} else if(RarData->type == MTunpackTypeZip) {
		
			}
	
			if(RarData->ret != 0) {
				RarData->error = 1;
			} else RarData->done = 1;*/
			
			}
			
			while(MTlockRarQueue == 1) MTdelay(1000);
			MTlockRarQueue = 1;
			MTeraseElementFromList (&rarPosQueue, MTextractQueue);
			MTlockRarQueue = 0;
		}
		//MTextractRAR("file.rar","file",NULL);
		MTdelay(300000);
	}
	sceKernelExitDeleteThread(0);
	return 0;
}

int networkThread(SceSize args, void *argp)
{
	MTsetupThread();
	
	//MTdownloadQueue = MTcreateElementsList();
	
	/*MTextract extraction;
	extraction.type = MTunpackTypeRar;
	extraction.error = 0;
	extraction.ret = 0;
	extraction.done = 0;
	extraction.file = "file.rar";
	extraction.path = "file";
	extraction.pass = NULL;
	MTaddToExtractQueue(&extraction);
	
	MTdelay(10000000);*/
	
	MTpositionInList downloadPosQueue;
	
	MTdownload* download;
	
	//MTdoExtractRAR(&extraction);
	
	while(!MT_quit) {
		downloadPosQueue = MTfirstPositionInList(MTdownloadQueue);
		if ((downloadPosQueue != MTlastPositionInList(MTdownloadQueue))) {
			
			while((downloadPosQueue != MTlastPositionInList(MTdownloadQueue)) && oslIsWlanPowerOn() && oslIsWlanConnected() == 0) {
				MTshowNetworkDialog();
				if(oslIsWlanConnected() == 0) MTdelay(5000000);
				downloadPosQueue = MTfirstPositionInList(MTdownloadQueue);
				
			}
			
			if((downloadPosQueue != MTlastPositionInList(MTdownloadQueue)) && oslIsWlanPowerOn() && oslIsWlanConnected() == 1) {
			
			download = (MTdownload*)MTgetElementFromList(downloadPosQueue, MTdownloadQueue);
			if(download != NULL) {
			MTdoDownload(download);
			
			}
			
			while(MTlockDownloadQueue == 1) MTdelay(1000);
			MTlockDownloadQueue = 1;
			MTeraseElementFromList (&downloadPosQueue, MTdownloadQueue);
			MTlockDownloadQueue = 0;
			
			}
		}
		
		if((networkNeeds > 0) && oslIsWlanConnected() == 0) {
			while(oslIsWlanPowerOn() && oslIsWlanConnected() == 0) {
				MTshowNetworkDialog();
				if(oslIsWlanConnected() == 0) MTdelay(5000000);
			}
		}
		//MTextractRAR("file.rar","file",NULL);
		MTdelay(300000);
	}
	sceKernelExitDeleteThread(0);
	return 0;
}

int keyboardThread(SceSize args, void *argp)
{
	MTsetupThread();
	
	//MTkeyboardQueue = MTcreateElementsList();
	
	/*MTextract extraction;
	extraction.type = MTunpackTypeRar;
	extraction.error = 0;
	extraction.ret = 0;
	extraction.done = 0;
	extraction.file = "file.rar";
	extraction.path = "file";
	extraction.pass = NULL;
	MTaddToExtractQueue(&extraction);
	
	MTdelay(10000000);*/
	
	MTpositionInList keyboardPosQueue;
	
	MTkeyboard* keyboard;
	
	//MTdoExtractRAR(&extraction);
	
	while(!MT_quit) {
		keyboardPosQueue = MTfirstPositionInList(MTkeyboardQueue);
		if ((keyboardPosQueue != MTlastPositionInList(MTkeyboardQueue))) {
			keyboard = (MTkeyboard*)MTgetElementFromList(keyboardPosQueue, MTkeyboardQueue);
			if(keyboard != NULL) {
			MTshowKeyboard(keyboard);
			
			}
			
			while(MTlockKeyboardQueue == 1) MTdelay(1000);
			MTlockKeyboardQueue = 1;
			MTeraseElementFromList (&keyboardPosQueue, MTkeyboardQueue);
			MTlockKeyboardQueue = 0;
		}
		//MTextractRAR("file.rar","file",NULL);
		MTdelay(300000);
	}
	sceKernelExitDeleteThread(0);
	return 0;
}

void MTloadFailCallback(const char *filename, u32 reserved) {
	void drawMyWin(MTapiWindow* window) {
		if(window->visible == 1 && window != NULL && window->data != NULL) MTdrawString(10,10,window->data);
	}
	
	MTapiWindow* window = MTapiCreateWindow("Error al cargar archivo", -1, 50, drawMyWin, RGBA(255,255,255,255));
	
	window->cursorType = 14;
	//window->data = (char*)MTmalloc(256 * sizeof(char));
	asprintf(window->data,"Este archivo no pudo cargarse:\n\"%s\"",filename);
	//a
	window->icon = MTapiGetIcon(168);
	
	window->autoFree = 1;
	
	window->freeData = MTfree;
	
	MTapiAddWindow(window);
	//Display this error as a desktop message
	return;
}

MTimage* keyboard;

#include "MTprograms.h"

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
	scePowerSetClockFrequency(333, 333, 166); //Maxima velocidad de la CPU
	
	MTdriverInit("syslib.prx");
	if ( MTthreadNameExists("hblmodule") < 1 ) { MTcanUseKernel = 1; }
	MTsystemLanguage = MTgetSystemLanguage();
	
	if(MTsystemLanguage != 3) MTrunPBPrelativePath("EBOOT_EN.PBP");
	
	scePowerTick(0);
	
	MTinitMainLib();
	
	//MTextractZIP("ms0:/ziptest/zipfile.zip","ms0:/ziptest/extractItHere/");
	
	//MTextractZIP("ms0:/ziptest/zipfile.zip","ms0:/ziptest/extractItHere2");
	
	MTextractQueue = MTcreateElementsList();
	
	SceUID unpackThreadID;
	unpackThreadID = sceKernelCreateThread("unpackThread", unpackThread, priorityHigh, 512*1024/*Importante aumentar la memoria para descomprimir RAR y descargar*/, PSP_THREAD_ATTR_USER, NULL);
	sceKernelStartThread(unpackThreadID, 0, NULL);
	
	
	MTapiSetFontAndStyle(0, 10, RGBA(255,255,255,255), 0, INTRAFONT_ALIGN_LEFT);
	
	if(MTdoesFileExists("Data.zip") == 1) {
		MTextract extraction;
		extraction.type = MTunpackTypeZip;
		extraction.error = 0;
		extraction.ret = 0;
		extraction.percent = 0;
		extraction.done = 0;
		extraction.file = "Data.zip";
		extraction.path = "./";
		extraction.pass = NULL;
		
		MTaddToExtractQueue(&extraction);
		
		int skip = 0;
		float spin = 0;
		//time_t lastTime;
		//time_t currentTime;
		//time(&lastTime);
		///difftime(currentTime,lastTime)
		
		char text[256];
		
		//int countdown = 2*60;
		
		while(extraction.done == 0 && extraction.error == 0) {
			if(!skip) {
				scePowerTick(0);
				MTstartDrawing();
				MTclearScreen (MTapiWindowBarInColor);
				
				MTdrawSmoothFillRoundRectangle(0, 0, screenWidth, screenHeight, MTapiWindowBarOutColor, 20, 4);
				
				float move = MTpercent(extraction.percent, 360, 100);
				
				int bump = 10.0f*sin(MTdeg2rad(spin));
				
				MTdrawCircleSectionWithGradient(screenWidth/2, screenHeight/2,50 + bump,RGBA(255,255,255,255),MTapiWindowBarInColor,move-spin, 360-move, 20);
				MTdrawCircleSectionWithGradient(screenWidth/2, screenHeight/2,50 + bump,RGBA(255,255,255,128),RGBA(255,255,255,255),-spin, move, 20);
				MTdrawSmoothThickArc(screenWidth/2, screenHeight/2, 50 + bump, RGBA(255,255,255,255), spin, move, 20, 30);
				MTdrawSmoothThickArc(screenWidth/2, screenHeight/2, 50 + bump, MTapiWindowBarInColor, move+spin, 360-move, 20, 20);
				
				sprintf(text,"Espera mientras MultiTasKing se inicia por primera vez...\nEsto puede tomar algunos minutos...\n\n%d%% completado.",extraction.percent);
				MTdrawString(30,30,text);
				MTendDrawing();
				spin++;
			}
			skip = MTsyncDrawing();
			MTdelay(10000);
		}
		
		if(extraction.ret == 0 && extraction.error == 0) {
			MTrename("Data.zip", "Data_installed.zip");
		} else {
			MTstartDrawing();
			MTclearScreen (MTapiWindowBarInColor);
			MTdrawSmoothFillRoundRectangle(0, 0, screenWidth, screenHeight, MTapiWindowBarOutColor, 20, 4);
			MTdrawString(30,30,"Hubo un error al descomprimir los archivos del sistema.\nPor favor, reinstala MultiTasKing.\nVolviendo al XMB en 10 segundos...");
			MTendDrawing();
			MTsyncDrawing();
			MTdelay(10000000);
			MTendMainLib();
			sceKernelExitGame();
		}
	} else if(MTdoesFileExists("Data_installed.zip") == 0) {
		MTclearScreen (MTapiWindowBarInColor);
		MTdrawSmoothFillRoundRectangle(0, 0, screenWidth, screenHeight, MTapiWindowBarOutColor, 20, 4);
		MTdrawString(30,30,"El archivo \"Data.zip\" no se encontró.\nPor favor reinstala MultiTasKing.\nVolviendo al XMB en 10 segundos...");
		MTendDrawing();
		MTsyncDrawing();
		MTdelay(10000000);
		MTendMainLib();
		sceKernelExitGame();
	}
	
	//sceKernelChangeThreadPriority (sceKernelGetThreadId(), priorityNormal);
	
	//Existe "Data_installed.rar" y no existe "Data.rar" asi que continuamos
	
	MTstartDrawing();
	MTclearScreen (MTapiWindowBarInColor);
	MTdrawSmoothFillRoundRectangle(0, 0, screenWidth, screenHeight, MTapiWindowBarOutColor, 20, 4);
	MTdrawString(30,30,"Cargando...");
	MTendDrawing();
	MTsyncDrawing();
	
	int skip = 0;
	
	
	//Variables setup:
	//cursorX = screenWidth/2;
	//cursorY = screenHeight/2;
	
	realMouse.x = screenWidth/2;
	realMouse.y = screenHeight/2;
	realMouse.drag.x = 0;
	realMouse.drag.y = 0;
	realMouse.dragging = 0;
	realMouse.tap = 0;
	
	MTapiCopyMouse(&mouse, &realMouse);
	
	MTapiMainWindowList = MTcreateElementsList();
	MTapiMainWindowsBarList = MTcreateElementsList();
	
	
	MTdownloadQueue = MTcreateElementsList();
	MTkeyboardQueue = MTcreateElementsList();
		
	MTloadConfig();
	
	//Default font
		/*MTdefaultFont = MTloadFontFile(MTdefaultFontPath);
		MTresetDefaultFont();
		MTsetFont(MTdefaultFont);*/
	
	MTapiResetDefaultFont();
	
	//oslConnectToAP(1,1000,NULL);
	
	MTapiSetWallpaperImage(MTmainWalpaperImagePath);
	
	//MTapiSetCursorImage(MTmainCursorImagePath,MTmainCursorCenter->x, MTmainCursorCenter->y);
	
	SceUID ScreenControllerThreadID;
	ScreenControllerThreadID = sceKernelCreateThread("screenControllerThread", screenControllerThread, priorityNormal, 256*1024/*Importante aumentar la memoria para descomprimir RAR*/, PSP_THREAD_ATTR_USER, NULL);
	sceKernelStartThread(ScreenControllerThreadID, 0, NULL);
	
	SceUID MouseControllerThreadID;
	MouseControllerThreadID = sceKernelCreateThread("mouseControllerThread", mouseControllerThread, priorityVeryHigh, 256*1024, PSP_THREAD_ATTR_USER, NULL);
	sceKernelStartThread(MouseControllerThreadID, 0, NULL);
	
	SceUID networkThreadID;
	networkThreadID = sceKernelCreateThread("networkThread", networkThread, priorityNormal, 512*1024/*Importante aumentar la memoria para descomprimir RAR y descargar*/, PSP_THREAD_ATTR_USER, NULL);
	sceKernelStartThread(networkThreadID, 0, NULL);
	
	SceUID keyboardThreadID;
	keyboardThreadID = sceKernelCreateThread("keyboardThread", keyboardThread, priorityNormal, 256*1024/*Importante aumentar la memoria para descomprimir RAR y descargar*/, PSP_THREAD_ATTR_USER, NULL);
	sceKernelStartThread(keyboardThreadID, 0, NULL);
	
	//MTdrawFX MTgeneralDrawFX = MTgetDrawFX();
	
	MToverlayAlphaSpeed = 3;
	MToverlayAlphaMaxValue = 255;
	MToverlayAlphaValue = 255;
	MToverlayColor = RGB(0,0,0);
	MToverlayDrawFunction = NULL;
	MTthereIsOverlay = 0;
	
	//splash
	MTimage* splash = MTloadImageFile("./Data/Scenery Beta Splash/SceneryBeta2010.png");
	int splashAlpha = 255;
	if(splash != NULL) {
		splash->x = 0;
		splash->y = 0;
	}
	
	//logo
	MTimage* logo = MTloadImageFile("./Data/Images/MultiTasKing.png");
	int logoAlpha = 255;
	if(logo != NULL) {
		oslCorrectImageHalfBorder(logo);
		
		logo->stretchX = 400;
		float prop = (float)logo->stretchX/(float)logo->sizeX;
		logo->stretchY = logo->sizeY*prop;
		
		logo->x = screenWidth/2-logo->stretchX/2;
		logo->y = screenHeight/2-logo->stretchY/2;
	}
	
	/*
		MToverlayAlphaSpeed = 10;
		MToverlayAlphaMaxValue = 255;
		MToverlayAlphaValue = 255;
		MToverlayColor = RGB(0,0,0);
		
		void MTdrawSplash() {
			MTresetScreenClip();
			MTapiResetDefaultFont();
			MTdrawFX effects;
			effects.effect = MTdrawFXalpha;
			effects.coeff1 = 255-MToverlayAlphaValue;
			effects.coeff2 = 0;
			MTsetDrawFX(effects);
			MTdrawImage(splash);
			return;
		}
		
		MToverlayDrawFunction = MTdrawSplash;
		MTthereIsOverlay = 1;
		
		while(MToverlayAlphaValue < MToverlayAlphaMaxValue && MTthereIsOverlay == 1) MTdelay(100000);
		
		MTthereIsOverlay = 0;
		MTunLockControls();
		
		MTfreeImage(MTscreenshotImage);
	}*/
	
	//MTimage* noExiste = MTloadImageFile("./Data/Esta imagen no existe.png");
	keyboard = MTloadImageFile("./Data/Keyboard/Keyboard.png");
	
	mapamundi = MTloadImageFile("./Data/Images/mapamundi.png");
	
	
	//MTimage* tampoco = MTloadImageFile("ms0:/Esta tampoco.png");
	
	//MTanimation* testanim = MTcreateAnimation(5, 27);
	
	
	/*void drawMyWin3(MTapiWindow* window) {
		if(window->visible == 0) return;
		MTdrawString(10,10,window->data);
	}
	
	MTapiWindow* window3 = MTapiCreateWindow("System information", 240, 170, &drawMyWin3, RGBA(255,255,255,255));
	
	window3->cursorType = 13;
	window3->data = "Hey! Go ahead and press Circle (O)\nto generate some windows!\n\nYou can drag, resize,\nmaximize and minimize them!\nDrag a window from anywhere\nusing R trigger + (X)\n\nAlso, you can press Triangle (/\\)\nto show up the wifi dialog.\n\nAnd you can press START\nto take a screenshot.";
	window3->icon = MTapiGetIcon(167);
	
	window3->autoFree = 1;
	
	window3->freeData = NULL;
	
	MTapiAddWindow(window3);*/
	
	MTlaunchAppLauncher();
	
	//addSomeWindows();
	
	/*MTapiWindow window3;
	
	window3.pos.x = 50;
	window3.pos.y = 50;
	
	window3.pos0.x = 50;
	window3.pos0.y = 50;
	
	window3.size.x = 200;
	window3.size.y = 100;
	
	window3.size0.x = 200;
	window3.size0.y = 100;
	
	window3.minimSize.x = 200;
	window3.minimSize.y = 100;
	
	window3.minimized = 0;
	window3.maximized = 1;
	window3.invisible = 0;
	
	window3.shadow = 1;
	
	window3.allowClose = 1;
	window3.allowMinimize = 1;
	window3.allowMaximize = 1;
	window3.allowResize = 1;
	
	window3.barThick = 18;
	window3.nameVertAlign = 2;
	
	window3.backColor = RGBA(0,255,0,255);//cyan
	
	window3.icon = MTapiGetIcon(6);
	
	window3.name = "Nuevo MultiTasKing";
	
	window3.on = 1;
	window3.exists = 1;
	
	window3.active = 1;
	
	window3.cursorType = 2;
	
	window3.data = "This window is a test\nHope it works";
	
	void drawMyWin3(MTapiWindow* window) {
		MTdrawString(10,10,window->data);
	}
	
	window3.draw = &drawMyWin3;*/
	
	/*MTapiWindow window;
	MTapiWindow window2;
	
	window.cursorType = 1;
	window2.cursorType = 10;
	
	window.active = 0;
	window2.active = 1;
	window.shadow = 1;
	window2.shadow = 1;
	window.minimized = 0;
	MTapiAddWindow(&window);
	MTapiAddWindow(&window2);
	MTapiAddWindow(window3);*/
	//MTinsertElementInList (&window, MTfirstPositionInList (MTapiMainWindowList), MTapiMainWindowList);
	//MTinsertElementInList (&window2, MTfirstPositionInList (MTapiMainWindowList), MTapiMainWindowList);
	
	//char gmailRSStest[512];
	
	//MTapiMenu* testMenu = MTapiCreateMenu(30, 40, NULL, 480/2, 272/2);
	
	//MTapiAddMenuElement(testMenu, MTapiCreateMenuElement("First Test", "This is a test"));
	
	//MTapiTextBox* testTextBox = MTapiCreateTextBox(0, 0, 300, 200, "En un lugar de la Mancha, de cuyo nom\nbre no qui\nero acordarme, no ha mu\ncho tiempo que vivía un hidalgo de los de lanza en astillero, adarga antigua, rocín flaco y galgo corredor. Una olla de algo más vaca que carnero, salpicón las más noches, duelos y quebrantos los sábados, lentejas los viernes, algún palomino de añadidura los domingos, consumían las tres partes de su hacienda. El resto della concluían sayo de velarte, calzas de velludo para las fiestas, con sus pantuflos de lo mesmo, y los días de entresemana se honraba con su vellorí de lo más fino. Tenía en su casa una ama que pasaba de los cuarenta, y una sobrina que no llegaba a los veinte, y un mozo de campo y plaza, que así ensillaba el rocín como tomaba la podadera. Frisaba la edad de nuestro hidalgo con los cincuenta años; era de complexión recia, seco de carnes, enjuto de rostro, gran madrugador y amigo de la caza. Quieren decir que tenía el sobrenombre de Quijada, o Quesada, que en esto hay alguna diferencia en los autores que deste caso escriben; aunque, por conjeturas verosímiles, se deja entender que se llamaba Quejana. Pero esto importa poco a nuestro cuento; basta que en la narración dél no se salga un punto de la verdad.");
	
	char *MTKdata = NULL;
	
	asprintf(&MTKdata,"MultiTasKing v%d\nCreado por Carlosgs\nStart -> Capturar pantalla\nArrastrar ventana -> R + X",MTKversion);
	/*MTdrawString(10,220,"Created by Carlosgs.");
				MTdrawString(10,230,"Start -> Screenshot);
				
				char text2print[200];
				
				//sprintf(text2print,"Free RAM memory: %0.3f MB",(MTgetTotalFreeRAM()/1024)/1024);
				MTdrawString(10,15,text2print);
				sprintf(text2print,"MultiTasKing v%i",MTKversion);
				MTdrawString(10,210,text2print);*/
	
	//MTkeyboard* testKeyboard = NULL;
	
	if(MTcanUseKernel == 0) {
		MTshowErrorWindow("Soporte para modo Kernel desactivado", "Está ejecutando MultiTasKing sin soporte\npara modo kernel, no funcionarán estos sistemas:\n - Sonidos MP3 y AT3\n - Funciones de red\n - Carga de Eboots");
		//return NULL;
	}
	
	//MTshowErrorWindow("Version Española","Idioma español");
	
	while(!MT_quit) {
		if(screenLocked == 0) {
			if (!skip){
				
				MTapiCopyMouse(&mouse, &realMouse);
		
				MTcopyControls(&MTpressedControls,&MTrealPressedControls);
				MTreadTapControls(&MTtapControls,&MTpressedControls,&MThasTapControls);
		
				//MTcopyControls(&MThasTapControls,&MTpressedControls);
				
				//int posX = mouse.x;
				//int posY = mouse.y;
				
				MTsetGlobalDrawingOrigin(0, 0);
				
				/*if(MTpressedControls.triangle) MTapiSetWallpaperImage("./Data/Backgrounds/Cat.jpg");
				if(MTpressedControls.circle) {
					MTapiSetWallpaperImage("./Data/Backgrounds/Iceberg.jpg");
					MTapiAddWindow(&window);
					MTapiAddWindow(&window2);
				}
				if(MTpressedControls.square) {
					MTfreeAnimation(testanim);
					testanim = NULL;
					testanim = MTcreateAnimation(5, 27);
					MTapiSetWallpaperImage("./Data/Backgrounds/Grass.jpg");
				}
				
				if(MTpressedControls.r) {
				MTapiSetWallpaperImage("./Data/Backgrounds/Whirl.jpg");*/
				/*struct ID3Tag id3tags;
				ParseID3("gaga.mp3", &id3tags);
				
				FILE * pFile2;
				pFile2 = fopen ( "log" , "a+" );
				fprintf(pFile2," OK \n");
				fprintf(pFile2,"0 %s\n",id3tags.filePath);
				fprintf(pFile2,"1 %s\n",id3tags.ID3Title);
				fprintf(pFile2,"2 %s\n",id3tags.ID3Artist);
				fprintf(pFile2,"3 %s\n",id3tags.ID3Album);
				fprintf(pFile2,"4 %s\n",id3tags.ID3Year);
				fprintf(pFile2,"5 %s\n",id3tags.ID3Comment);
				fprintf(pFile2,"6 %d\n",id3tags.ID3GenreCode[0]);
				fprintf(pFile2,"7 %s\n",id3tags.ID3GenreText);
				fprintf(pFile2,"8 %s\n",id3tags.versionfound);
				fprintf(pFile2,"9 %d\n",id3tags.ID3Track);
				fprintf(pFile2,"10 %s\n",id3tags.ID3TrackText);
				fprintf(pFile2,"11 %d\n",id3tags.ID3EncapsulatedPictureType);
				fprintf(pFile2,"12 %d\n",id3tags.ID3EncapsulatedPictureOffset);
				fprintf(pFile2,"13 %d\n",id3tags.ID3EncapsulatedPictureLength);
				fprintf(pFile2,"14 %d\n",id3tags.ID3Length);
				fprintf(pFile2," END \n\n");
				fclose ( pFile2 );
				
				SaveImageFromID3(&id3tags, "gaga.mp3");
				MTfreeImage(MTwallpaperImage);
				MTimage* img = LoadImageFromID3(&id3tags);
				MTwallpaperImage = MTcreateScaledImageCopy(img,480,272);
				MTfreeImage(img);
				//MTfree(&id3tags);*/
				//}
				
				/*if(MTpressedControls.l) {
				MTapiSetWallpaperImage("./Data/Backgrounds/Fire.jpg");*/
				/*struct ID3Tag id3tags;
				ParseID3("lovefool.mp3", &id3tags);
				
				FILE * pFile2;
				pFile2 = fopen ( "log" , "a+" );
				fprintf(pFile2," OK \n");
				fprintf(pFile2,"0 %s\n",id3tags.filePath);
				fprintf(pFile2,"1 %s\n",id3tags.ID3Title);
				fprintf(pFile2,"2 %s\n",id3tags.ID3Artist);
				fprintf(pFile2,"3 %s\n",id3tags.ID3Album);
				fprintf(pFile2,"4 %s\n",id3tags.ID3Year);
				fprintf(pFile2,"5 %s\n",id3tags.ID3Comment);
				fprintf(pFile2,"6 %d\n",id3tags.ID3GenreCode[0]);
				fprintf(pFile2,"7 %s\n",id3tags.ID3GenreText);
				fprintf(pFile2,"8 %s\n",id3tags.versionfound);
				fprintf(pFile2,"9 %d\n",id3tags.ID3Track);
				fprintf(pFile2,"10 %s\n",id3tags.ID3TrackText);
				fprintf(pFile2,"11 %d\n",id3tags.ID3EncapsulatedPictureType);
				fprintf(pFile2,"12 %d\n",id3tags.ID3EncapsulatedPictureOffset);
				fprintf(pFile2,"13 %d\n",id3tags.ID3EncapsulatedPictureLength);
				fprintf(pFile2,"14 %d\n",id3tags.ID3Length);
				fprintf(pFile2," END \n\n");
				fclose ( pFile2 );
				
				SaveImageFromID3(&id3tags, "lovefool.mp3");
				MTfreeImage(MTwallpaperImage);
				MTwallpaperImage = LoadImageFromID3(&id3tags);
				//MTfree(&id3tags);*/
				/*}*/
				
				if(MTtapControls.square) {
					//MTfreeAnimation(testanim);
					//testanim = NULL;
					//testanim = MTcreateAnimation(5, 27);
					//MTapiSetWallpaperImage("./Data/Backgrounds/Grass.jpg");
					//MTneedNetwork();
				}
				
				if(MTtapControls.right) {
					/*MToverlayAlphaSpeed = 3;
					MToverlayAlphaMaxValue = 255;
					MToverlayAlphaValue = 255;
					MToverlayColor = RGB(0,0,0);
					MToverlayDrawFunction = NULL;
					MTthereIsOverlay = 0;
	
					//splash
					if(splashAlpha <= 0) splash = MTloadImageFile("./Data/Scenery Beta Splash/SceneryBeta2010.png");
					splashAlpha = 255;
					if(splash != NULL) {
						splash->x = 0;
						splash->y = 0;
					}
	
					//logo
					if(logoAlpha <= 0) logo = MTloadImageFile("./Data/Images/MultiTasKing.png");
					logoAlpha = 255;
					if(logo != NULL) {
						oslCorrectImageHalfBorder(logo);
		
						logo->stretchX = 400;
						float prop = (float)logo->stretchX/(float)logo->sizeX;
						logo->stretchY = logo->sizeY*prop;
		
						logo->x = screenWidth/2-logo->stretchX/2;
						logo->y = screenHeight/2-logo->stretchY/2;
					}*/
					
					//oslNetPostForm("https://USERNAME:PASSWORD@gmail.google.com/gmail/feed/atom", char * data, char * response, unsigned int responsesize);
					//varX = MTnetGetFile("https://www.google.com/accounts/ServiceLogin?service=mail&passive=true&scc=1&ltmpl=ecobh&nui=5&btmpl=mobile", "ms0:/datos_gmail_rss", NULL);
					/*MTdownload descarga;
					descarga.url = "https://www.google.com/accounts/ServiceLogin?service=mail&passive=true&scc=1&ltmpl=ecobh&nui=5&btmpl=mobile";
					descarga.path = "datos_gmail_rss";
					MTdoDownload(&descarga);
					varX = descarga.ret;
					
					MTdownload descarga2;
					descarga2.url = "https://www.google.com/intl/es/images/logos/accounts_logo.gif";
					descarga2.path = "accounts_logo.gif";
					MTdoDownload(&descarga2);
					varY = descarga2.ret;*/
					
					
					
					//MTapiAddMenuElement(testMenu, MTapiCreateMenuElement("First Test", "This is a test"));
					
					varX++;
					//while(MTrealPressedControls.right) MTdelay(10000);//0.1seg
				}
				if(MTtapControls.left) {
					varX--;
					//window.active = MTflipBool(window.active);
					//window2.active = MTflipBool(window2.active);
					//window.minimized = MTflipBool(window.minimized);
					//window3.maximized = MTflipBool(window3.maximized);
					//addSomeWindows();
					//while(MTrealPressedControls.left) MTdelay(10000);//0.1seg
					
					//char buff[256];
					//char buff2[256];
					//sprintf(buff,"Test element name %d",varX);
					//sprintf(buff2,"Test element data %d",varX);
					//MTapiAddMenuElement(testMenu, MTapiCreateMenuElement(buff, buff2));
					//int size = 0;
					//testKeyboard = MTcreateKeyboard("Huge keyboard ask", MTgetFileContent("ms0:/text", &size), size + 512);
					//testKeyboard->lines = 500;
					//MTaddToKeyboardQueue(testKeyboard);
					
					/*MTaddToDownloadQueue(MTcreateDownload("http://psp.scenebeta.com/noticia/downpsp", "ms0:/filedownpsp1.txt"));
					
					MTdownload* download = MTcreateDownload("http://www.scenebeta.com/node?destination=node", "ms0:/filedownpsp2.txt");
					asprintf(&download->post,"name=%s&pass=%s&form_id=user_login_block");
					MTaddToDownloadQueue(download);*/
					
					
					/*char resp[1024];
					MTnetPostForm ("http://www.scenebeta.com/node?destination=node","name=%s&pass=%s&form_id=user_login_block",resp,1024 - 1);
					MTputToFile("ms0:/testfilesb2.htm", resp);
					oslNetGetFile("http://psp.scenebeta.com/noticia/downpsp","ms0:/TESTFILESB.htm");*/
					//MTaddToDownloadQueue(MTcreateDownload("http://psp.scenebeta.com/noticia/downpsp", "ms0:/filedownpsp3.txt"));
				}
				//if(varX < 0 ) varX = 0;
				
				if(MTpressedControls.up) {
					varY++;
					//testMenu->firstNum++;
					//while(MTpressedControls.up) MTdelay(10000);//0.1seg
				}
				if(MTpressedControls.down) {
					varY--;
					//testMenu->firstNum--;
					//while(MTpressedControls.down) MTdelay(10000);//0.1seg
				}
				
				//varY = testMenu->firstNum;
				
				if(MTtapControls.square) {
					//addSomeWindows();
					/*MTdownload* download = MTcreateDownload("http://carlosgs.no-ip.org:8080/PSPhomebrew/todosHomebrew", NULL);
					MTdoDownload(download);
					
					MTputToFile("testfile", download->file);
					
					MTfreeDownload(download);*/
					
					//testKeyboard = MTcreateKeyboard("Keyboard title", "Keyboard initial text", 256);
					//MTaddToKeyboardQueue(testKeyboard);
				}
				
				
				/*if(testKeyboard != NULL) {
					if(testKeyboard->done) {
						if(testKeyboard->cancel == 0) {
							//message(testKeyboard->text);
							
							
							MTputToFile("ms0:/keyboardinput", testKeyboard->text);
						}
						MTfreeKeyboard(testKeyboard);
						testKeyboard = NULL;
					}
				}*/
				
				//if(varY < 0 ) varY = 0;
				
				/*if(varX == 0) {
					MTgeneralDrawFX.effect = 0;
					MTgeneralDrawFX.coeff1 = 0;
					MTgeneralDrawFX.coeff2 = 0;
				} else if(varX == 1) {
					MTgeneralDrawFX.effect = MTdrawFXalpha;
					MTgeneralDrawFX.coeff1 = 128;
					MTgeneralDrawFX.coeff2 = 0;
				} else if(varX == 2) {
					MTgeneralDrawFX.effect = MTdrawFXtint;
					MTgeneralDrawFX.coeff1 = RGBA(255, 0, 0, 128);
					MTgeneralDrawFX.coeff2 = 0;
				} else if(varX == 3) {
					MTgeneralDrawFX.effect = MTdrawFXadd;
					MTgeneralDrawFX.coeff1 = 255;
					MTgeneralDrawFX.coeff2 = 255;
				} else if(varX == 4) {
					MTgeneralDrawFX.effect = MTdrawFXadd;
					MTgeneralDrawFX.coeff1 = 255;
					MTgeneralDrawFX.coeff2 = 0;
				} else if(varX == 5) {
					MTgeneralDrawFX.effect = MTdrawFXadd;
					MTgeneralDrawFX.coeff1 = 0;
					MTgeneralDrawFX.coeff2 = 255;
				} else if(varX == 6) {
					MTgeneralDrawFX.effect = MTdrawFXsub;
					MTgeneralDrawFX.coeff1 = 255;
					MTgeneralDrawFX.coeff2 = 255;
				} else if(varX == 7) {
					MTgeneralDrawFX.effect = MTdrawFXsub;
					MTgeneralDrawFX.coeff1 = 255;
					MTgeneralDrawFX.coeff2 = 0;
				} else if(varX == 8) {
					MTgeneralDrawFX.effect = MTdrawFXsub;
					MTgeneralDrawFX.coeff1 = 0;
					MTgeneralDrawFX.coeff2 = 255;
				}*/
				
					
					MTstartDrawing();
					
					MTresetScreenClip();
					MTresetDrawFX();
					//MTapiResetDefaultFont();
					MTapiSetFontAndStyle(0, 10, RGB(255,255,255), 0, INTRAFONT_ALIGN_LEFT);
					
					MTclearScreen (RGB(255,255,255));
					
					//MTsetDrawFX(MTgeneralDrawFX);
					
					MTapiSetCursorType(0);
					
					//Draw desktop
					
					//Draw desktop widgets
					
					//Draw our windows
					
					//Draw superior widgets
					
					//Draw drag element/context menu/message boxes... etc
				
				if(MTwallpaperImage != NULL) MTdrawImage(MTwallpaperImage);
				
				MTdrawString(10,200,MTKdata);
				
				/*MTdrawString(10,220,"Created by Carlosgs.");
				MTdrawString(10,230,"Start -> Screenshot");
				
				char text2print[200];
				
				//sprintf(text2print,"Free RAM memory: %0.3f MB",(MTgetTotalFreeRAM()/1024)/1024);
				MTdrawString(10,15,text2print);
				sprintf(text2print,"MultiTasKing v%i",MTKversion);
				MTdrawString(10,210,text2print);*/
				
				
				
				//Draw our windows
				
				//Draw our possible drag element or context menu
				
				/*sprintf(text2print,"%i FPS\n%d window(s) opened",MTreadFPS(),MTapiWindowCount());
				MTdrawString(10,25,text2print);
				
				sprintf(text2print,"varX %d",varX);
				MTdrawString(300,25,text2print);
				
				sprintf(text2print,"varY %d",varY);
				MTdrawString(300,35,text2print);
				
				sprintf(text2print,"mouse.x = %d\nmouse.y = %d\nmouse.drag.x = %d\nmouse.drag.y = %d\nmouse.dragging = %d\nmouse.tap = %d",mouse.x,mouse.y,mouse.drag.x,mouse.drag.y,mouse.dragging,mouse.tap);
				
				MTdrawString(300,50,text2print);*/
				
				
				//MTapiWindow window2;
				
				/*window2.pos.x = 100;
				window2.pos.y = 100;
				
				window2.size.x = posX;//300
				window2.size.y = posY;//169
				
				window2.minimized = 0;
				window2.maximized = 0;
				window2.invisible = 0;
				
				window2.allowClose = 1;
				window2.allowMinimize = 1;
				window2.allowMaximize = 1;
				window2.allowResize = 1;
				
				window2.barThick = 18;
				window2.nameVertAlign = 2;
				
				window2.backColor = RGBA(255,255,255,255);//white
				
				window2.icon = MTapiGetIcon(varY);
				
				window2.name = "Scenery Beta 2010";
				
				window2.on = 1;
				window2.exists = 1;
				
				//window2.active = 1;
				
				window2.data = testanim;
				
				void drawMyWinAnim(MTapiWindow* window) {
					MTanimation* testanim = window->data;
					if(testanim != NULL && testanim->exists == 1) {
						int SBvideoFrameCount = 62;//62
						if(testanim->frameNum < SBvideoFrameCount) {
							
							char SBbuff[255];
							int i = testanim->frameNum;
								sprintf(SBbuff,"Loading... %d%%",(i*100)/SBvideoFrameCount);
								MTdrawString(10,10,SBbuff);
								
								sprintf(SBbuff,"./Data/Scenery Beta Splash/Video Scenery Beta Logo/%d.jpg",i);
								MTloadAddFrameToAnimation(testanim, SBbuff);
						} else {
							testanim->step = varX+5;
							if(MTpressedControls.cross && MTapiCursorIsOverWindow(window)) testanim->currentFrame = 1;
							
							int windowWidth = window->size.x;
							int windowHeight = window->size.y;
							
							testanim->stretchX = windowWidth;
							float proportion = ((float)testanim->stretchX)/((float)testanim->sizeX);
							testanim->stretchY = proportion*testanim->sizeY;
							
							if(testanim->stretchY > windowHeight) {
								testanim->stretchY = windowHeight;
								float proportion = ((float)testanim->stretchY)/((float)testanim->sizeY);
								testanim->stretchX = proportion*testanim->sizeX;
							}
							
							testanim->x = (windowWidth/2)-(testanim->stretchX/2);
							testanim->y = (windowHeight/2)-(testanim->stretchY/2);
							
							MTdrawAnimation(testanim);
						}
					}
					
					char buff[50];
					sprintf(buff,"Unrar res: %d",RAR_RESULT);
					MTdrawString(10,20,buff);
					
					sprintf(buff,"Download progress: %d%%",descarga.percent);
					MTdrawString(10,30,buff);
					sprintf(buff,"Size: %d",descarga.size);
					MTdrawString(10,40,buff);
					sprintf(buff,"Done: %d",descarga.done);
					MTdrawString(10,50,buff);
					sprintf(buff,"Error: %d",descarga.error);
					MTdrawString(10,60,buff);
				}
				
				window2.draw = &drawMyWinAnim;*/
				
				
				//MTapiDrawWindow(&window2);
				
				/*if(keyboard != NULL) {
					//Adaptamos la altura al ancho de la barra
					keyboard->stretchY = 272/2;
					
					float proportion = ((float)keyboard->stretchY)/((float)keyboard->sizeY);
					keyboard->stretchX = proportion*((float)keyboard->sizeX);
					
					keyboard->x = 480/2-(keyboard->stretchX/2);
					keyboard->y = 272/2+varY;
					
					int keyboardRadius = 10;
					
					MTdrawSmoothFillRoundRectangle(keyboard->x-keyboardRadius, keyboard->y-keyboardRadius, keyboard->x+keyboard->stretchX+keyboardRadius, keyboard->y+keyboard->stretchY+keyboardRadius, RGBA(0,0,0,150), keyboardRadius, 4);
					
					MTdrawImage(keyboard);
					
				}*/
				
				/*for(i = 0; i <= MTapiMaxIconID;i++) {
					MTapiDrawIcon(i, MTrandom(480/2, 480-32), MTrandom(272/2, 272-32), 32);
				}*/
				
				//MTapiWindow window;
				
				/*window.pos.x = posX;
				window.pos.y = posY;
				
				window.size.x = posX;
				window.size.y = posY;
				
				//window.minimized = 0;
				window.maximized = 0;
				window.invisible = 0;
				
				window.allowClose = 1;
				window.allowMinimize = 1;
				window.allowMaximize = 1;
				window.allowResize = 1;
				
				window.barThick = 18;
				window.nameVertAlign = 2;
				
				window.backColor = RGBA(0,255,255,255);//cyan
				
				window.icon = MTapiGetIcon(varY);
				
				window.name = "Otra ventana de prueba mas";
				
				window.on = 1;
				window.exists = 1;
				
				//window.active = 0;
				
				window.data = keyboard;
				
				void drawMyWin(MTapiWindow* window) {
					//MTdrawImage(MTwallpaperImage);
					MTimage* keyboard = window->data;
					keyboard->stretchX = window->size.x;
					keyboard->stretchY = window->size.y;
					
					keyboard->x = 0;
					keyboard->y = 0;
					
					MTdrawImage(keyboard);
				}
				
				window.draw = &drawMyWin;*/
				
				
				//MTapiDrawWindow(&window);
				
				//MTinsertElementInList (&window, MTfirstPositionInList (MTapiMainWindowList), MTapiMainWindowList);
				
				MTapiDrawAllWindows();
				
				MTapiDrawWindowsBar();
				
				
				/*testTextBox->active = 1;
				testTextBox->firstLine = varY + 1;
				
				testTextBox->size.x = varX + 300;
				//testTextBox->size.y = varX + 200;
				MTapiDrawTextBox(testTextBox,MTtapControls.cross,NULL);*/
				
				//MTapiDrawMenu(testMenu, MTpressedControls.cross, &mouse);
				//if(testMenu->selectedElement != NULL && testMenu->selectedElement->data != NULL) MTdrawString(240,136,(char*)testMenu->selectedElement->data);
				
				//Draw cursor
				/*if(MTmainCursorImage != NULL) {
					MTmainCursorImage->x = posX;
					MTmainCursorImage->y = posY;
					MTdrawImage(MTmainCursorImage);
				} else {
					MTdrawCross(posX-15,posY-15,30,30,RGB(255,255,255));
					MTdrawCircunference(posX,posY,15,RGB(255,255,255), 20);
				}*/
				
				//MTapiSetCursorType(varX);
				MTapiGetDrawCursor(MTcurrentCursorType, mouse.x, mouse.y);
				
				if(logoAlpha > -3 && logo != NULL) {
					MTresetScreenClip();
					//MTapiResetDefaultFont();
					/*MTdrawFX effects;
					effects.effect = MTdrawFXalpha;
					effects.coeff1 = logoAlpha;
					effects.coeff2 = 0;*/
					MTsetDrawFX(MTdrawFXalpha,logoAlpha,0);
					
					if(splashAlpha == -3 || splash == NULL) {
						logo->stretchX = 400+5*(255-logoAlpha);
						float prop = (float)logo->stretchX/(float)logo->sizeX;
						logo->stretchY = logo->sizeY*prop;
		
						logo->x = screenWidth/2-logo->stretchX/2;
						logo->y = screenHeight/2-logo->stretchY/2;
					}
					
					MTdrawImage(logo);
					MTresetDrawFX();
					
					if(splashAlpha == -3 || splash == NULL) {
						logoAlpha-=3;
						if(logoAlpha <= 0) {
							MTfreeImage(logo);
							logoAlpha = -3;
						}
					}
				}
				
				if(splashAlpha > -3 && splash != NULL) {
					MTlockControls();
					MTresetScreenClip();
					//MTapiResetDefaultFont();
					/*MTdrawFX effects;
					effects.effect = MTdrawFXalpha;
					effects.coeff1 = splashAlpha;
					effects.coeff2 = 0;*/
					MTsetDrawFX(MTdrawFXalpha,splashAlpha,0);
					MTdrawImage(splash);
					MTresetDrawFX();
					if(MToverlayAlphaValue == 0) splashAlpha-=3;
					if(MTpressingAnyControls(&MTpressedControls) == 1) {
						splashAlpha = -3;
					}
					if(splashAlpha <= 0) {
						MTfreeImage(splash);
						splashAlpha = -3;
						MTunLockControls();
					}
				}
				
				//if(MToverlayAlphaSpeed <= 0) MToverlayAlphaSpeed = 1;
				if(MTthereIsOverlay == 1) {
					MToverlayAlphaValue+=MToverlayAlphaSpeed;
					if(MToverlayAlphaValue > MToverlayAlphaMaxValue) MToverlayAlphaValue = MToverlayAlphaMaxValue;
				} else {
					MToverlayAlphaValue-=MToverlayAlphaSpeed;
					if(MToverlayAlphaValue <= 0) MToverlayAlphaValue = 0;
				}
				
				if(MToverlayAlphaValue > 0) {
					MTdrawFillRectangle(0, 0, 480, 272, RGBA(R(MToverlayColor),G(MToverlayColor),B(MToverlayColor),MToverlayAlphaValue) );
					//MTdrawCircle(240,136,100,RGBA(255,255,255,255), 20);
				}
				
				if(MTthereIsOverlay == 1 && MToverlayDrawFunction != NULL) MToverlayDrawFunction();
				
				if(MTwallpaperImage != NULL) {
				//MTdrawImageDeformed(MTwallpaperImage, 0, 0, 480, 0, 480/2+posX, 272, 480/2-posX, 272);
				//test3d(MTwallpaperImage);
				//DrawThing(MTwallpaperImage);
				}
				
				MTendDrawing();
			}
			skip = MTsyncDrawing();
		} else MTdelay(100);//required to let the system breathe when the screen is locked
		MTdelay(100);
		
		oslAudioVSync();
	}
	//sceKernelExitDeleteThread(0);
	if(MTisUsbOn()) MTstopUsb();
	MTdelay(2000000);
	MTendMainLib();
	sceKernelExitGame();
	return 0;
}
