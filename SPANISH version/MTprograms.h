/*typedef struct {
	MTvector ballPos;
	MTvector ballSpeed;
	int userPosY;
	int enemyPosY;
	int userScore;
	int enemyScore;
} MTappPongGame;

//maxspeed = 15;

void MTlaunchAppPongGame() {
	
	void drawMyWin(MTapiWindow* window) {
		if(window == NULL) return;
		char* text = window->data;
		if(text == NULL) return;
		if(window->visible == 0) return;
		MTdrawString(10,10,text);
	}
	
	MTappPongGame* pong = (MTappPongGame*)MTmalloc(sizeof(MTappPongGame));
	
	MTapiWindow* window = MTapiCreateWindow("Pong", 300, 200, &drawMyWin, RGB(255,255,255));
	
	
	pong->ballPos.x = window->size.x/2;
	pong->ballPos.y = window->size.y/2;
	pong->ballSpeed.x = 5;
	pong->ballSpeed.y = 1;
	
	
	window->cursorType = 0;
	
	window->data = (char*) datatext;
	
	window->icon = MTapiGetIcon(105);
	
	window->autoFree = 1;
	
	window->freeData = MTfree;
	
	MTapiAddWindow(window);
	return;
}*/

void MTshowErrorWindow(char* name, char* text) {
	if(name == NULL || text == NULL) return;
	void drawMyWin(MTapiWindow* window) {
		if(window == NULL) return;
		char* text = window->data;
		if(text == NULL) return;
		if(window->visible == 0) return;
		MTdrawString(10,10,text);
	}
	
	MTapiWindow* window = MTapiCreateWindow(name, -1, 100, &drawMyWin, RGB(255,255,255));
	
	window->cursorType = 14;
	
	char* datatext = NULL;
	
	asprintf(&datatext,text);
	
	window->data = (char*) datatext;
	
	window->icon = MTapiGetIcon(168);
	
	window->autoFree = 1;
	
	window->freeData = MTfree;
	
	MTapiAddWindow(window);
	return;
}

void MTlaunchAppSoundPlayer(char* name, char* path) {
	if(name == NULL || path == NULL) return;
	
	/*int stream = OSL_FMT_NONE;
	if(MTgetFileSize(path) > 1024*1024) stream = OSL_FMT_STREAM;*/
	
	MTsound* snd = MTloadSoundFile(path);
	
	if(snd == NULL) return;
	
	if(MTgetFileType(path) == 7) {
		snd->tags = MTcreateID3tags(path);
		if(snd->tags != NULL)
		snd->coverImg = MTloadImageFromID3(snd->tags);
	}
	
	/*MTimage* img = NULL;
	size
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
	} else img = img1;*/
	
	
	
	void drawMyWin(MTapiWindow* window) {
		if(window == NULL) return;
		MTsound* snd = window->data;
		if(snd == NULL) return;
		
		if(window->visible == 0) return;
		
		/*if(window->controls.up) {
			if(window->controls.right) snd->snd->volumeRight++;
			if(window->controls.left) snd->snd->volumeLeft++;
		}
		if(window->controls.down) {
			if(window->controls.right) snd->snd->volumeRight--;
			if(window->controls.left) snd->snd->volumeLeft--;
		}
		
		if(snd->snd->volumeLeft < 0) snd->snd->volumeLeft = 0;
		if(snd->snd->volumeLeft > OSL_VOLUME_MAX) snd->snd->volumeLeft = OSL_VOLUME_MAX;
		
		if(snd->snd->volumeRight < 0) snd->snd->volumeRight = 0;
		if(snd->snd->volumeRight > OSL_VOLUME_MAX) snd->snd->volumeRight = OSL_VOLUME_MAX;*/
		
		int songState = 0;//stopped
		
		if(MTisSoundStopped(snd)) {
			songState = 0;
		} else {
			if(snd->paused) {
				songState = 2;//paused
			} else songState = 1;//playing
		}
		
		//bool playing = 0;
		//if(oslGetSoundChannel(snd->snd) != -1) playing = 1;
		if(window->tapControls.cross) {
			if(songState == 0) {
				MTplaySound(snd);
			} else {
				if(songState == 2) {
					MTresumeSound(snd);
				} else MTpauseSound(snd);
			}
		}
		
		if(window->tapControls.square) {
			if(songState != 0) MTstopSound(snd);
		}
		
		int icon = 127;
		
		if(songState == 1) icon = 126;
		if(songState == 0) icon = 128;
		
		if(snd->tags != NULL) {
			//char buff[64];
			//char res[512];
			//sprintf(res,"Title: %s\nArtist: %s\nAlbum: %s\nYear: %s Genre: %s\nTrack %d: %s\n%s\n",snd->tags->ID3Title,snd->tags->ID3Artist,snd->tags->ID3Album,snd->tags->ID3Year,snd->tags->ID3GenreText,snd->tags->ID3Track,snd->tags->ID3TrackText,snd->tags->ID3Comment);
			/*fprintf(pFile2,"1 %s\n",snd->tags.ID3Title);
			fprintf(pFile2,"2 %s\n",snd->tags->ID3Artist);
			fprintf(pFile2,"3 %s\n",snd->tags->ID3Album);
			fprintf(pFile2,"4 %s\n",snd->tags->ID3Year);
			fprintf(pFile2,"5 %s\n",snd->tags->ID3Comment);
			fprintf(pFile2,"6 %d\n",snd->tags->ID3GenreCode[0]);
			fprintf(pFile2,"7 %s\n",snd->tags->ID3GenreText);
			fprintf(pFile2,"8 %s\n",snd->tags->versionfound);
			fprintf(pFile2,"9 %d\n",snd->tags->ID3Track);
			fprintf(pFile2,"10 %s\n",snd->tags->ID3TrackText);*/
			int offsetX = 0;
			if(snd->coverImg != NULL) {
				snd->coverImg->x = 5;
				snd->coverImg->y = 0;
				
				snd->coverImg->stretchY = window->size.y - 10;
				
				float proportion = ((float)snd->coverImg->stretchY)/((float)snd->coverImg->sizeY);
				snd->coverImg->stretchX = proportion*snd->coverImg->sizeX;
				
				offsetX = snd->coverImg->stretchX + snd->coverImg->x;
				
				snd->coverImg->y = window->size.y/2 - snd->coverImg->stretchY/2;
				
				MTdrawImage(snd->coverImg);
			}
			
			int size = 32;
			MTapiDrawIcon(icon, window->size.x - size - 5, window->size.y/2-size/2, size);
			
			if(snd->data) {
			MTapiSetFontAndStyle(7, 10, RGB(255,255,255), 0, INTRAFONT_ALIGN_LEFT);
			MTdrawString(offsetX + 5,5,snd->data);
			}
		} else {
			
			int size = 32;//window->size.y;
			MTapiDrawIcon(icon, window->size.x/2 - size/2, window->size.y/2-size/2, size);
		}
	}
	
	MTapiWindow* window = NULL;
	if(snd->tags != NULL) {
		asprintf(&snd->data,"Titulo: %s\nArtista: %s\nAlbum: %s\nAño: %s Genero: %s\nPista %d: %s\n%s\n",snd->tags->ID3Title,snd->tags->ID3Artist,snd->tags->ID3Album,snd->tags->ID3Year,snd->tags->ID3GenreText,snd->tags->ID3Track,snd->tags->ID3TrackText,snd->tags->ID3Comment);
		if(snd->coverImg != NULL) {
			window = MTapiCreateWindow(name, 300, 80, &drawMyWin, MTapiWindowBarInColor);
		} else window = MTapiCreateWindow(name, 250, 80, &drawMyWin, MTapiWindowBarInColor);
	} else window = MTapiCreateWindow(name, 200, 40, &drawMyWin, MTapiWindowBarInColor);
	
	window->cursorType = 0;
	MTplaySound(snd);
	window->data = snd;
	window->icon = MTapiGetIcon(22);
	
	window->autoFree = 1;
	
	window->freeData = MTfreeSound;
	
	MTapiAddWindow(window);
	return;
}

void MTlaunchAppImageViewer(char* name, char* path) {
	if(name == NULL || path == NULL) return;
	
	MTimage* img = MTloadImageFile(path);
	
	if(img == NULL) return;
	
	/*MTimage* img = NULL;
	
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
	} else img = img1;*/
	
	
	
	void drawMyWin(MTapiWindow* window) {
		if(window == NULL) return;
		MTimage* img = window->data;
		if(img == NULL) return;
		
				if(window->visible == 0) return;
				
				//window->cursorType = 10;
				
				/*if(window->wasMaximized) {
					window->wasMaximized = 0;
					img->stretchX = window->size.x;
					float proportion = ((float)img->stretchX)/((float)img->sizeX);
					img->stretchY = proportion*img->sizeY;
	
					if(img->stretchY > window->size.y) {
						img->stretchY = window->size.y;
						float proportion = ((float)img->stretchY)/((float)img->sizeY);
						img->stretchX = proportion*img->sizeX;
					}
				}*/
				
				img->x = window->size.x/2;
				img->y = window->size.y/2;
				
				/*if(window->controls.square == 0) {
				if(window->controls.up) img->y++;
				if(window->controls.down) img->y--;
				
				if(window->controls.left) img->x++;
				if(window->controls.right) img->x--;
				
				} else {
					if(window->controls.up) img->stretchX++;
					if(window->controls.down) img->stretchX--;
					if(img->stretchX < 5) img->stretchX = 5;
				}
				
				if(img->x < -img->stretchX) img->x = -img->stretchX;
				if(img->y < -img->stretchY) img->y = -img->stretchY;
				if(img->y > img->stretchY + window->size.y) img->y = img->stretchY + window->size.y;
				if(img->x > img->stretchX + window->size.x) img->x = img->stretchX + window->size.x;*/
				
				//if(window->controls.square == 0) {
//				if(window->controls.up) img->centerY++;
//				if(window->controls.down) img->centerY--;
//				
//				if(window->controls.left) img->centerX++;
//				if(window->controls.right) img->centerX--;
				
				//} else {
					if(window->controls.up) img->stretchX+=10;
					if(window->controls.down) img->stretchX-=10;
					if(img->stretchX < 10) img->stretchX = 10;
				//}
				
//				if(img->centerX < 0) img->centerX = 0;
//				if(img->centerY < 0) img->centerY = 0;
//				if(img->centerY > img->stretchY) img->centerY = img->stretchY;
//				if(img->centerX > img->stretchX) img->centerX = img->stretchX;
				
				if(window->tapControls.left) img->angle -= 45;
				if(window->tapControls.right) img->angle += 45;
				
				if(img->angle > 360) img->angle -=360;
				if(img->angle < 0) img->angle += 360;
				
				//int windowWidth = window->size.x;
				//int windowHeight = window->size.y;
				
				//img->stretchX = windowWidth;
				float proportion = ((float)img->stretchX)/((float)img->sizeX);
				img->stretchY = proportion*img->sizeY;
				
				/*if(img->stretchY > windowHeight) {
					img->stretchY = windowHeight;
					float proportion = ((float)img->stretchY)/((float)img->sizeY);
					img->stretchX = proportion*img->sizeX;
				}*/
				
				//img->x = (windowWidth/2)-(img->stretchX/2);
				//img->y = (windowHeight/2)-(img->stretchY/2);
				
				//img->x += img->stretchX/2;
				//img->y += img->stretchY/2;
				
				MTdrawImage(img);
				
				//img->x -= img->stretchX/2;
				//img->y -= img->stretchY/2;
	}
	
	int winSizeX = img->stretchX;
	int winSizeY = img->stretchY;
	
	if(img->stretchX > 440 || img->stretchY > 240) {
	
	winSizeX = 400;
	winSizeY = 200;
	
	img->stretchX = winSizeX;
	float proportion = ((float)img->stretchX)/((float)img->sizeX);
	img->stretchY = proportion*img->sizeY;
	
	if(img->stretchY > winSizeY) {
		img->stretchY = winSizeY;
		float proportion = ((float)img->stretchY)/((float)img->sizeY);
		img->stretchX = proportion*img->sizeX;
	}
	
	}
	
	MTapiWindow* window = MTapiCreateWindow(name, winSizeX, winSizeY, &drawMyWin, RGBA(255,255,255,255));
	
	img->x = window->size.x/2;
	img->y = window->size.y/2;
	MTsetImageCenterMiddle(img);
	
	window->cursorType = 10;
	window->data = img;
	window->icon = MTapiGetIcon(104);
	
	window->autoFree = 1;
	
	window->freeData = MTfreeImage;
	
	MTapiAddWindow(window);
	return;
}


void MTlaunchAppMultiTasKingCredits() {
	
	void drawMyWin(MTapiWindow* window) {
		if(window == NULL || window->data == NULL) return;
		
		MTapiTextBox* textBox = window->data;
		if(textBox == NULL) return;
		
		
		textBox->size.x = window->size.x;
		textBox->size.y = window->size.y;
		
		//textBox->adjustSpaces = 0;
		//textBox->autoLineBreak = 0;
		
		//if(window->tapControls.square) {
			textBox->align = 1;
			//if(textBox->align > 2) textBox->align = 0;
		//}
		
		/*if(window->tapControls.triangle) {
			textViewerKeyboard = MTcreateKeyboard("Huge keyboard ask", MTgetFileContent("ms0:/text", &size), size + 512);
			textViewerKeyboard->lines = 500;
			MTaddToKeyboardQueue(textViewerKeyboard);
		}
		
		if(textViewerKeyboard != NULL) {
			if(textViewerKeyboard->done) {
				if(textViewerKeyboard->cancel == 0) {
					
					MTputToFile("ms0:/keyboardinput", textViewerKeyboard->text);
				}
				MTfreeKeyboard(textViewerKeyboard);
				textViewerKeyboard = NULL;
			}
		}*/
			
		if(window->controls.up) textBox->firstLine--;
		if(window->controls.down) textBox->firstLine++;
		
		if(window->controls.left) textBox->firstLine-=5;
		if(window->controls.right) textBox->firstLine+=5;
		
		textBox->active = 0;
		if(window->mouseIsOver && window->active) textBox->active = 1;
		MTapiDrawTextBox(textBox, window->tapControls.cross, &window->mouse);
	}
	
	MTapiWindow* window = MTapiCreateWindow("Creditos de MultiTasKing", -1, 150, &drawMyWin, RGBA(255,255,255,255));
	
	window->cursorType = 0;
	window->data = MTapiCreateTextBox(0, 0, window->size.x, window->size.y, "MultiTasKing v300\nDesarrollado por Carlosgs\nhttp://www.carlosgs.es\n\nSi te ha gustado, por favor vota por\nMultiTasKing en el SceneryBeta 2010.\n\n\nAgradecimientos:\n\nDeViaNTe (http://www.gcrew.es)\nhttp://psp.scenebeta.com\nNiteman\nUnicorn\nArber\n\n\nBetatesters:\n\nRubenelportero\nDark-Santi\nQarl\nAlbert\nArber\nDeviante\n\n\nRecursos:\n\nOSlib mod 1.0.1\nTango icon pack\nVanilla DMZ cursors\n\nMira en las carpetas de MultiTasKing para ver mas informacion sobre copyright.\n\n");
	
	//((MTapiTextBox*)window->data)->len = size -1;
	//((MTapiTextBox*)window->data)->prevLen = size -1;
	
	window->icon = MTapiGetIcon(53);
	
	window->autoFree = 1;
	
	window->freeData = MTapiFreeTextBox;
	
	MTapiAddWindow(window);
	
	return;
}


//MTkeyboard* textViewerKeyboard = NULL;
void MTlaunchAppTextViewer(char* name, char* path) {
	if(name == NULL || path == NULL) return;
	
	int size = 0;
	char* file = MTgetFileContent(path, &size);
	
	if(file == NULL || size <= 0) asprintf(&file,"Este archivo esta vacio.");
	
	//if(size > 1024*2) size = 1024*2;
	
	void drawMyWin(MTapiWindow* window) {
		if(window == NULL || window->data == NULL) return;
		
		MTapiTextBox* textBox = window->data;
		if(textBox == NULL) return;
		
		
		textBox->size.x = window->size.x;
		textBox->size.y = window->size.y;
		
		//textBox->adjustSpaces = 0;
		//textBox->autoLineBreak = 0;
		
		if(window->tapControls.square) {
			textBox->align++;
			if(textBox->align > 2) textBox->align = 0;
		}
		
		/*if(window->tapControls.triangle) {
			textViewerKeyboard = MTcreateKeyboard("Huge keyboard ask", MTgetFileContent("ms0:/text", &size), size + 512);
			textViewerKeyboard->lines = 500;
			MTaddToKeyboardQueue(textViewerKeyboard);
		}
		
		if(textViewerKeyboard != NULL) {
			if(textViewerKeyboard->done) {
				if(textViewerKeyboard->cancel == 0) {
					
					MTputToFile("ms0:/keyboardinput", textViewerKeyboard->text);
				}
				MTfreeKeyboard(textViewerKeyboard);
				textViewerKeyboard = NULL;
			}
		}*/
			
		if(window->controls.up) textBox->firstLine--;
		if(window->controls.down) textBox->firstLine++;
		
		if(window->controls.left) textBox->firstLine-=5;
		if(window->controls.right) textBox->firstLine+=5;
		
		textBox->active = 0;
		if(window->mouseIsOver && window->active) textBox->active = 1;
		MTapiDrawTextBox(textBox, window->tapControls.cross, &window->mouse);
	}
	
	MTapiWindow* window = MTapiCreateWindow(name, 220, 130, &drawMyWin, RGBA(255,255,255,255));
	
	window->cursorType = 0;
	window->data = MTapiCreateTextBox(0, 0, window->size.x, window->size.y, file);
	
	//((MTapiTextBox*)window->data)->len = size -1;
	//((MTapiTextBox*)window->data)->prevLen = size -1;
	
	window->icon = MTapiGetIcon(2);
	
	window->autoFree = 1;
	
	window->freeData = MTapiFreeTextBox;
	
	MTapiAddWindow(window);
	
	return;
}

void MTlaunchAppFileBrowser(char* iniPath) {
	
	if(iniPath == NULL) asprintf(&iniPath,"ms0:/");
	
	void drawMyWin(MTapiWindow* window) {
		
		if(window->visible == 0) return;
		
		MTapiMenu* testMenu = window->data;
		if(testMenu == NULL) return;
		
		testMenu->size.x = window->size.x;
		testMenu->size.y = window->size.y;
			
		if(window->controls.up) testMenu->firstNum--;
		if(window->controls.down) testMenu->firstNum++;
		
		if(window->controls.left) testMenu->firstNum-=5;
		if(window->controls.right) testMenu->firstNum+=5;
		
		//testMenu->barIsActive = window->mouseIsOver;
		testMenu->active = 0;
		if(window->mouseIsOver && window->active) testMenu->active = 1;
		MTapiDrawMenu(testMenu, window->mouseIsOver, window->tapControls.cross, &window->mouse);
		
		if(window->tapControls.circle) {
			char buff[256];
			strcpy(buff,testMenu->name);
			//sprintf(buff,"%s",testMenu->name);
			
			if(MTdirectoryUp(buff) == 0) {
				
				if(window->data != NULL) MTapiFreeMenu(window->data);
				window->data = NULL;//LIBERAR LISTA
				window->data = MTapiCreateFileListMenu(0, 0, window->size.x, window->size.y, buff,"   · ",NULL,NULL,1);
			}
		} else if(testMenu->selectedElement != NULL && testMenu->selectedElement->data != NULL) {
			if((int)testMenu->selectedElement->data2 == 0) {//if folder
				char buff[256];
				//sprintf(buff,"%s",testMenu->selectedElement->data);
				strcpy(buff,testMenu->selectedElement->data);
				if(window->data != NULL) MTapiFreeMenu(window->data);
				window->data = NULL;//LIBERAR LISTA
				window->data = MTapiCreateFileListMenu(0, 0, window->size.x, window->size.y, buff,"   · ",NULL,NULL,1);
				//asprintf(&((MTapiMenu*)window->data)->prefix,"   · ");
			} else if((int)testMenu->selectedElement->data2 == 6 || (int)testMenu->selectedElement->data2 == 8 || (int)testMenu->selectedElement->data2 == 9) {
				MTlaunchAppImageViewer(testMenu->selectedElement->name, testMenu->selectedElement->data);
			} else if((int)testMenu->selectedElement->data2 == 1) {//unknown
				MTlaunchAppTextViewer(testMenu->selectedElement->name, testMenu->selectedElement->data);
			} else if((int)testMenu->selectedElement->data2 == 25 || (int)testMenu->selectedElement->data2 == 7) {//wav MP3
				MTlaunchAppSoundPlayer(testMenu->selectedElement->name, testMenu->selectedElement->data);
			} else if((int)testMenu->selectedElement->data2 == 3) {
				MTrunPBP(testMenu->selectedElement->data);
			}
		}
	}
	
	MTapiWindow* window = MTapiCreateWindow("Explorador de archivos", 220, 130, &drawMyWin, RGBA(255,255,255,255));
	
	window->cursorType = 0;
	window->data = MTapiCreateFileListMenu(0, 0, window->size.x, window->size.y, iniPath,"   · ",NULL,NULL,1);
	//asprintf(&((MTapiMenu*)window5->data)->prefix,"   · ");
	window->icon = MTapiGetIcon(170);
	
	window->autoFree = 1;
	
	window->freeData = MTapiFreeMenu;
	
	MTapiAddWindow(window);
	return;
}


void MTlaunchAppBackgroundSelector() {
	
	void drawMyWin(MTapiWindow* window) {
		
		if(window->visible == 0) return;
		
		MTapiMenu* testMenu = window->data;
		if(testMenu == NULL) return;
		
		testMenu->size.x = window->size.x;
		testMenu->size.y = window->size.y;
			
		if(window->controls.up) testMenu->firstNum--;
		if(window->controls.down) testMenu->firstNum++;
		
		if(window->controls.left) testMenu->firstNum-=5;
		if(window->controls.right) testMenu->firstNum+=5;
		
		//testMenu->barIsActive = window->mouseIsOver;
		testMenu->active = 0;
		if(window->mouseIsOver && window->active) testMenu->active = 1;
		MTapiDrawMenu(testMenu, window->mouseIsOver, window->tapControls.cross, &window->mouse);
		
		if(window->tapControls.circle) {
			char buff[256];
			strcpy(buff,testMenu->name);
			//sprintf(buff,"%s",testMenu->name);
			
			if(MTdirectoryUp(buff) == 0) {
				
				if(window->data != NULL) MTapiFreeMenu(window->data);
				window->data = NULL;//LIBERAR LISTA
				
				int types[4];
				types[0] = 6;
				types[1] = 8;
				types[2] = 9;
				types[3] = -1;
				
				window->data = MTapiCreateFileListMenu(0, 0, window->size.x, window->size.y, buff,"   · ",NULL,types,1);
			}
		} else if(testMenu->selectedElement != NULL && testMenu->selectedElement->data != NULL) {
			if((int)testMenu->selectedElement->data2 == 0) {//if folder
				char buff[256];
				//sprintf(buff,"%s",testMenu->selectedElement->data);
				strcpy(buff,testMenu->selectedElement->data);
				if(window->data != NULL) MTapiFreeMenu(window->data);
				window->data = NULL;//LIBERAR LISTA
				
				int types[4];
				types[0] = 6;
				types[1] = 8;
				types[2] = 9;
				types[3] = -1;
				
				window->data = MTapiCreateFileListMenu(0, 0, window->size.x, window->size.y, buff,"   · ",NULL,types,1);
				//asprintf(&((MTapiMenu*)window->data)->prefix,"   · ");
			} else if((int)testMenu->selectedElement->data2 == 6 || (int)testMenu->selectedElement->data2 == 8 || (int)testMenu->selectedElement->data2 == 9) {
				//MTlaunchAppImageViewer(testMenu->selectedElement->name, testMenu->selectedElement->data);
				MTapiSetNewWallpaperImage(testMenu->selectedElement->data);
			}
		}
	}
	
	MTapiWindow* window = MTapiCreateWindow("Elige el fondo de pantalla", 220, 130, &drawMyWin, RGBA(255,255,255,255));
	
	window->cursorType = 3;
	char* menuName = NULL;
	asprintf(&menuName,"%s%s",MTrootDir,"/Data/Backgrounds");
	
	int types[4];
	types[0] = 6;
	types[1] = 8;
	types[2] = 9;
	types[3] = -1;
	
	window->data = MTapiCreateFileListMenu(0, 0, window->size.x, window->size.y, menuName,"   · ",NULL,types,1);
	//asprintf(&((MTapiMenu*)window5->data)->prefix,"   · ");
	window->icon = MTapiGetIcon(158);
	
	window->autoFree = 1;
	
	window->freeData = MTapiFreeMenu;
	
	MTapiAddWindow(window);
	return;
}


void MTlaunchAppSceneryBetaAnimation() {
	
	void drawMyWin(MTapiWindow* window) {
		MTanimation* testanim = window->data;
		if(testanim != NULL && testanim->exists == 1) {
			int SBvideoFrameCount = 62;//62
			if(testanim->frameNum < SBvideoFrameCount) {
				
				char SBbuff[255];
				int i = testanim->frameNum;
					sprintf(SBbuff,"./Data/Scenery Beta Splash/Video Scenery Beta Logo/%d.jpg",i);
					MTloadAddFrameToAnimation(testanim, SBbuff);
					
					if(window->visible == 0) return;
					sprintf(SBbuff,"Loading... %d%%",(i*100)/SBvideoFrameCount);
					MTdrawString(10,10,SBbuff);
					window->cursorType = 2;//loading
			} else {
				
				if(window->visible == 0) return;
				
				window->cursorType = 0;//normal
				testanim->step = /*MTsceneryAnimationNum+*/5;
				if(window->tapControls.cross == 1) testanim->currentFrame = 1;
				
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
		
		if(window->tapControls.square == 1) {
			if(window->data != NULL) MTfreeAnimation(window->data);
			window->data = NULL;
			window->data = MTcreateAnimation(5, 27);
		}
		
		//MTdrawCircle(window->mouse.x,window->mouse.y,40,RGBA(0,255,0,128), 17);
		
		/*char buff[50];
		sprintf(buff,"Unrar res: %d",RAR_RESULT);
		MTdrawString(10,20,buff);
		
		sprintf(buff,"Download progress: %d%%",descarga.percent);
		MTdrawString(10,30,buff);
		sprintf(buff,"Size: %d",descarga.size);
		MTdrawString(10,40,buff);
		sprintf(buff,"Done: %d",descarga.done);
		MTdrawString(10,50,buff);
		sprintf(buff,"Error: %d",descarga.error);
		MTdrawString(10,60,buff);*/
	}
	
	MTapiWindow* window = MTapiCreateWindow("Scenery Beta 2010", 250, 142, &drawMyWin, RGBA(255,255,255,255));
	
	window->cursorType = 2;
	window->data = MTcreateAnimation(5, 27);//testanim;
	window->icon = MTapiGetIcon(64);
	
	window->autoFree = 1;
	
	window->freeData = MTfreeAnimation;
	
	MTapiAddWindow(window);
	return;
}

#include "MTappMiniDownPSP.h"


void MTlaunchAppUSBconnect() {
	void drawMyWin(MTapiWindow* window) {
		
		if(window->visible == 0) return;
		
		bool state = MTisUsbOn();
		
		if(window->tapControls.cross) {
			if(state == 0) MTstartUsb(); else MTstopUsb();
		}
		
		int icon = 40;
		if(state) icon = 39;
		int size = 32;
		MTapiDrawIcon(icon, window->size.x/2 - size/2, window->size.y/2-size/2, size);
	}
	
	
	MTapiWindow* window = MTapiCreateWindow("Conexion USB", -1, 50, &drawMyWin, RGBA(255,255,255,255));
	
	window->cursorType = 0;
	
	window->icon = MTapiGetIcon(142);
	
	window->autoFree = 1;
	
	window->freeData = NULL;
	
	MTapiAddWindow(window);
}


void MTlaunchAppGeolocator() {
	void drawMyWin(MTapiWindow* window) {
		
		if(window->visible == 0) return;
		
		MTipInfo* info = window->data;
		if(info == NULL) return;
		if(mapamundi == NULL) return;
		window->cursorType = 0;//normal
		
		int windowWidth = window->size.x;
		int windowHeight = window->size.y;
		
		mapamundi->stretchX = windowWidth;
		float proportion = ((float)mapamundi->stretchX)/((float)mapamundi->sizeX);
		mapamundi->stretchY = proportion*mapamundi->sizeY;
		
		if(mapamundi->stretchY > windowHeight) {
			mapamundi->stretchY = windowHeight;
			float proportion = ((float)mapamundi->stretchY)/((float)mapamundi->sizeY);
			mapamundi->stretchX = proportion*mapamundi->sizeX;
		}
		
		mapamundi->x = (windowWidth/2)-(mapamundi->stretchX/2);
		mapamundi->y = (windowHeight/2)-(mapamundi->stretchY/2);
		
		MTdrawImage(mapamundi);
		
		//coordinates->x = varX;//longitude
		//coordinates->y = varY;//latitude
		int circlePosX = mapamundi->x + MTpercent(info->longitude + 180.0f,mapamundi->stretchX,360);
		int circlePosY = mapamundi->y + MTpercent(-info->latitude + 90.0f,mapamundi->stretchY,180);
		
		MTdrawCircleWithGradient(circlePosX,circlePosY,5,MTgetZeroAlphaColor(MTapiWindowBarOutColor),MTapiWindowBarOutColor,10);
		
		MTapiSetFontAndStyle(7, 10, RGB(255,255,255), 0, INTRAFONT_ALIGN_LEFT);
		int boxSizeX = MTgetTextWidth(info->ip) + MTgetTextWidth("IP: ");
		
		int radius = 3;
		MTdrawSmoothFillRoundRectangle(circlePosX + 10 - radius, circlePosY - radius, circlePosX + 10 + radius + boxSizeX, circlePosY + radius + 3*MTgetTextHeight(), MTapiWindowBarOutColor, radius, 4);
		
		char text2print[512];
		sprintf(text2print,"%s\n%s (%s)\nIP: %s",info->cityName,info->countryName,info->countryCode,info->ip);
		
		MTdrawString(circlePosX + 10,circlePosY,text2print);
		
		if(info->flag == NULL) return;
		//info->flag->stretchX = 50;
		//info->flag->stretchY = 33;
		
		info->flag->x = circlePosX + 10 + boxSizeX - info->flag->stretchX;
		info->flag->y = circlePosY + 5;
		
		MTdrawImage(info->flag);
	}
	
	void drawMyWinDownloading(MTapiWindow* window) {
		if(window == NULL || window->data == NULL) return;
		
		MTdownload* download = window->data;
		if(download == NULL) return;
		
		if(download->done && download->error == 0 && download->file != NULL) {
			
			//if(download->response == 0) {
				//DONE CHANGE WINDOW
			//} else {
			
				MTipInfo* ipinfo = (MTipInfo*) MTmalloc(sizeof(MTipInfo));
				MTgetIPinfoFromFileInMemory(ipinfo,download->file);
			
				window->cursorType = 0;
				window->data = (MTipInfo*)ipinfo;
				//asprintf(&((MTapiMenu*)window6->data)->prefix,"   - ");
				//window->icon = MTapiGetIcon(92);
	
				window->autoFree = 1;
	
				window->freeData = MTfree;
				
				window->function = drawMyWin;
				
				MTfreeDownload(download);
				//download->response = 0;
			//}
		} else {
			if(download->error) {
				MTdrawString(10,10,"Error al descargar informacion de ubicacion");
			} else {
				if(download->knownsize) {
				char buff[256];
				sprintf(buff,"Obteniendo informacion de ubicacion...\n%d%% completado.",download->percent);
				MTdrawString(10,10,buff);
				} else MTdrawString(10,10,"Obteniendo informacion de ubicacion...");
			}
		}
	}
	
	/*MTdownload descarga2;
	descarga2.url = "http://ipinfodb.com/ip_query.php?timezone=false";
	descarga2.path = "ipInfo";
	MTdoDownload(&descarga2);*/
	//MTdownload* down1 = MTcreateDownload("http://ipinfodb.com/ip_query.php?timezone=false", "ipInfo");
	//MTdoDownload(down1);
	
	MTapiWindow* window = MTapiCreateWindow("Geolocalizacion", 240, 120, &drawMyWinDownloading, RGBA(255,255,255,255));
	
	window->cursorType = 0;
	
	MTipInfo* ipinfo = (MTipInfo*) MTmalloc(sizeof(MTipInfo));
	MTgetIPinfo(ipinfo);
	window->data = MTcreateDownload("http://ipinfodb.com/ip_query.php?timezone=false", NULL);
	MTaddToDownloadQueue(window->data);
	/*window7->data = (MTvector*) MTmalloc(sizeof(MTvector));
	
	//&((MTapiMenu*)window6->data)->prefix
	
	((MTvector*)window7->data)->x = 0;
	((MTvector*)window7->data)->y = 0;
	
	MTdownload descarga2;
	descarga2.url = "http://ipinfodb.com/ip_query.php?timezone=false";
	descarga2.path = "ipInfo";
	MTdoDownload(&descarga2);
	
	int size2 = 0;
	char* data2 = MTgetFileContent("ipInfo", &size2);
	
	if(data2 != NULL) {
		//char res[128];
		char* res = (char*) MTmalloc (128 * sizeof(char) + 1);
		if(res != NULL) {
		MTgetStringBetweenTags(data2, res, 128, "<Longitude>", "</Longitude>");
		float x;
		if(sscanf(res,"%f",&x) == 1) {
			((MTvector*)window7->data)->x = x;
		}
		
		MTgetStringBetweenTags(data2, res, 128, "<Latitude>", "</Latitude>");
		float y;
		if(sscanf(res,"%f",&y) == 1) {
			((MTvector*)window7->data)->y = y;
		}
		
		MTfree(res);
		}
		
		MTfree(data2);
	}*/
	
	window->icon = MTapiGetIcon(111);
	
	window->autoFree = 1;
	
	window->freeData = MTfree;
	
	MTapiAddWindow(window);
}

void MTappLauncherFileBrowser() {
	MTlaunchAppFileBrowser("ms0:/");
}

void MTappLauncherBackgroundSelector() {
	MTlaunchAppBackgroundSelector();
}

//bool sceneryAnimationIsLaunched = 0;

void MTappLauncherSceneryBetaAnimation() {
	//if(sceneryAnimationIsLaunched) return;
	//sceneryAnimationIsLaunched = 1;
	MTlaunchAppSceneryBetaAnimation();
}

void MTappLauncherDownPSP() {
	MTlaunchAppDownPSP();
}

void MTappLauncherUSBconnect() {
	MTlaunchAppUSBconnect();
}

void MTappLauncherGeolocator() {
	MTlaunchAppGeolocator();
}

void MTlaunchAppLauncher() {
	void drawMyWin(MTapiWindow* window) {
		
		if(window->visible == 0) return;
		
		MTapiMenu* testMenu = window->data;
		if(testMenu == NULL) return;
		
		testMenu->size.x = window->size.x;
		testMenu->size.y = window->size.y;
			
		if(window->controls.up) testMenu->firstNum--;
		if(window->controls.down) testMenu->firstNum++;
		
		if(window->controls.left) testMenu->firstNum-=5;
		if(window->controls.right) testMenu->firstNum+=5;
		
		testMenu->active = 0;
		if(window->mouseIsOver && window->active) testMenu->active = 1;
		MTapiDrawMenu(testMenu, window->mouseIsOver, window->tapControls.cross, &window->mouse);
		
		if(testMenu->selectedElement != NULL && testMenu->selectedElement->function != NULL) {
			testMenu->selectedElement->function();
		}
	}
	
	MTapiWindow* window = MTapiCreateWindow("Menu principal", 250, 100, &drawMyWin, RGBA(255,255,255,255));
	
	window->cursorType = 0;
	window->data = MTapiCreateMenu(0, 0, window->size.x, window->size.y, "All available apps","  -> ");
	//asprintf(&((MTapiMenu*)window6->data)->prefix,"   - ");
	window->icon = MTapiGetIcon(13);
	
	window->autoFree = 1;
	
	window->allowClose = 0;
	
	window->freeData = MTapiFreeMenu;
	
	MTapiAddMenuElement(window->data,MTapiCreateMenuElementWithFunction("Explorador de archivos", NULL, MTappLauncherFileBrowser),0);
	MTapiAddMenuElement(window->data,MTapiCreateMenuElementWithFunction("Scenery Beta 2010 - Presentacion", NULL, MTappLauncherSceneryBetaAnimation),0);
	MTapiAddMenuElement(window->data,MTapiCreateMenuElementWithFunction("mini downPSP", NULL, MTappLauncherDownPSP),0);
	MTapiAddMenuElement(window->data,MTapiCreateMenuElementWithFunction("Geolocalizador", NULL, MTappLauncherGeolocator),0);
	MTapiAddMenuElement(window->data,MTapiCreateMenuElementWithFunction("Selector de fondo de pantalla", NULL, MTappLauncherBackgroundSelector),0);
	MTapiAddMenuElement(window->data,MTapiCreateMenuElementWithFunction("Conexion USB", NULL, MTappLauncherUSBconnect),0);
	MTapiAddMenuElement(window->data,MTapiCreateMenuElementWithFunction("Creditos de MultiTasKing", NULL, MTlaunchAppMultiTasKingCredits),0);
	
	MTapiAddWindow(window);
	return;
}


