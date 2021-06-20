char MTappDataDownPSPuser[128];
char MTappDataDownPSPpassword[128];

bool MTappDataDownPSPloginSuccess = 0;

typedef struct {
	MTdownload* download;
	MTextract* extraction;
	int state;
	char* data;
	char* url;
	char* path;
} MTappDownPSPfileInstaller;

void MTfreeAppDownPSPfileInstaller(MTappDownPSPfileInstaller* data) {
	if(data == NULL) return;
	if(data->data != NULL) MTfree(data->data);
	if(data->url != NULL) MTfree(data->url);
	if(data->path != NULL) MTfree(data->path);
	if(data->download != NULL) MTfreeDownload(data->download);
	if(data->extraction != NULL) MTfreeExtraction(data->extraction);
	data->data = NULL;
	data->url = NULL;
	data->download = NULL;
	data->extraction = NULL;
	data->path = NULL;
	MTfree(data);
	data = NULL;
	return;
}

void MTlaunchAppDownPSPfileInstaller(char* url) {
	if(url == NULL) return;
	if(MTappDataDownPSPloginSuccess == 0) MTshowErrorWindow("Error: No se ha iniciado sesion", "No se ha iniciado sesion en SB\ny no se puede continuar.");
	void drawMyWin(MTapiWindow* window) {
		if(window == NULL || window->data == NULL) return;
		
		MTappDownPSPfileInstaller* data = window->data;
		if(data == NULL) return;
		
		if(data->state == 0) {
			if(window->visible == 0) return;
			MTdrawString(10,10,data->data);
			window->cursorType = 0;
			if(window->tapControls.cross) {
				data->state = 1;
				data->download = MTcreateDownload(data->url, data->path);
				MTaddToDownloadQueue(data->download);
			}
		} else if(data->state == 1) {
			if(data->download->done && data->download->error == 0) {
				window->cursorType = 2;
				
				MTfreeDownload(data->download);
					
				data->download = NULL;
				data->extraction = MTcreateExtraction(data->path, "ms0:/", MTunpackTypeRar);
				MTaddToExtractQueue(data->extraction);
				data->state = 2;
			} else {
				if(data->download->error) {
					MTdrawString(10,10,"Error al descargar");
					window->cursorType = 14;
					data->state = 0;
					MTshowErrorWindow("Error al descargar", "Se produjo un error y\nno se pudo continuar.");
				} else {
					if(data->download->knownsize) {
					char buff[256];
					sprintf(buff,"Descargando homebrew...\n%d%% completado.",data->download->percent);
					MTdrawString(10,10,buff);
					} else MTdrawString(10,10,"Descargando homebrew...");
					window->cursorType = 2;
				}
			}
		} else if(data->state == 2) {
			if(data->extraction->done && data->extraction->error == 0) {
				window->cursorType = 2;
				
				sceIoRemove(data->path);
				
				MTfreeExtraction(data->extraction);
					
				data->extraction = NULL;
				data->state = 3;
			} else {
				if(data->extraction->error) {
					MTdrawString(10,10,"Error al descomprimir");
					window->cursorType = 14;
					data->state = 0;
					
					sceIoRemove(data->path);
					
					MTfreeExtraction(data->extraction);
					
					data->extraction = NULL;
					MTshowErrorWindow("Error al descomprimir", "Se produjo un error y\nno se pudo continuar.");
				} else {
					MTdrawString(10,10,"Descomprimiendo e instalando...");
					window->cursorType = 2;
				}
			}
		} else if(data->state == 3) {
			MTdrawString(10,10,"El homebrew se ha instalado correctamente...");
			window->cursorType = 0;
		}
	}
	
	MTappDownPSPfileInstaller* data = (MTappDownPSPfileInstaller*)MTmalloc(sizeof(MTappDownPSPfileInstaller));
	
	data->data = NULL;
	data->state = 0;
	data->path = NULL;
	data->download = NULL;//MTcreateDownload(url, NULL);
	asprintf(&data->url,url);
	
	data->extraction = NULL;
	
	data->path = (char*) MTmalloc(strlen(url)*sizeof(char*) + 1);
	MTgetFileName(url, data->path);
	asprintf(&data->data,"Pulsa X para instalar:\n%s",data->path);
	//MTfree(name);
	
	//MTdownload* download
	//MTaddToDownloadQueue(data->download);
	
	MTapiWindow* window = MTapiCreateWindow("downPSP - instalador", 260, 50, &drawMyWin, RGBA(255,255,255,255));
	
	
	window->cursorType = 0;
	window->data = data;
	window->icon = MTapiGetIcon(147);
	
	window->autoFree = 1;
	
	window->freeData = MTfreeAppDownPSPfileInstaller;
	
	MTapiAddWindow(window);
	
	return;
	
}


typedef struct {
	MTdownload* download;
	int state;
	char* data;
	char* url;
	MTapiTextBox* textBox;
	MTkeyboard* keyboard;
} MTappDownPSPnewViewerData;

/*MTaddToDownloadQueue(MTcreateDownload("http://psp.scenebeta.com/noticia/downpsp", "ms0:/filedownpsp1.txt"));
MTdownload* download = MTcreateDownload("http://www.scenebeta.com/node?destination=node", "ms0:/filedownpsp2.txt");
asprintf(&download->post,"name=%s&pass=%s&form_id=user_login_block");
MTaddToDownloadQueue(download);*/

void MTfreeAppDownPSPnewViewerData(MTappDownPSPnewViewerData* data) {
	if(data == NULL) return;
	if(data->data != NULL) MTfree(data->data);
	if(data->url != NULL) MTfree(data->url);
	if(data->textBox != NULL) MTapiFreeTextBox(data->textBox);
	if(data->download != NULL) MTfreeDownload(data->download);
	if(data->keyboard != NULL) MTfreeKeyboard(data->keyboard);
	data->keyboard = NULL;
	data->textBox = NULL;
	data->data = NULL;
	data->url = NULL;
	data->download = NULL;
	MTfree(data);
	data = NULL;
	return;
}

void MTlaunchAppDownPSPnewViewer(char* name, char* url) {
	if(name == NULL || url == NULL) return;
	void drawMyWin(MTapiWindow* window) {
		if(window == NULL || window->data == NULL) return;
		
		MTappDownPSPnewViewerData* data = window->data;
		if(data == NULL) return;
		
		if(data->state == 0) {
			if(data->download->done && data->download->error == 0 && data->download->file != NULL) {
				window->cursorType = 2;
				char* data2 = data->download->file;
			
				char* parsed = (char*) MTmalloc (data->download->size * sizeof(char) + 1);
				if(parsed != NULL) {
					parsed[0] = '\0';
					if(data2 != NULL) {
						MTremoveTagsFromString(data2, parsed, data->download->size, "<!-- I C -->", "<!-- F C -->", "<", ">");
					
						MTremoveTagsFromString(parsed, data2, data->download->size, NULL, NULL, "&", ";");
						MTfree(parsed);
					}
				}
			
				data->textBox = MTapiCreateTextBox(0, 0, window->size.x, window->size.y, data->download->file);
			
				//asprintf(&data->data,data->download->file);
			
				MTfreeDownload(data->download);
			
				data->download = NULL;
			
				data->state = 1;
				//MTfreedata->download(data->download);
			} else {
				if(data->download->error) {
					MTdrawString(10,10,"Error al descargar");
					window->cursorType = 14;
				} else {
					if(data->download->knownsize) {
					char buff[256];
					sprintf(buff,"Recibiendo...\n%d%% completado.",data->download->percent);
					MTdrawString(10,10,buff);
					} else MTdrawString(10,10,"Recibiendo...");
					window->cursorType = 2;
				}
			}
		} else if(data->state == 1) {
			MTapiTextBox* textBox = data->textBox;
			if(textBox == NULL) return;
		
		
			textBox->size.x = window->size.x;
			textBox->size.y = window->size.y;
			
			if(window->tapControls.square) {
				textBox->align++;
				if(textBox->align > 2) textBox->align = 0;
			}
			
			//textBox->adjustSpaces = 0;
			//textBox->autoLineBreak = 0;
		
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
			
			window->cursorType = 0;
			
			if(MTisOverAreaSimple(window->mouse.x, window->mouse.y, 0, 0, window->size.x - 15, window->size.y) && window->tapControls.cross) {
				if(MTappDataDownPSPloginSuccess == 0) {
				data->state = 2;
				data->keyboard = MTcreateKeyboard("Usuario", "Usuario de scenebeta.com", 256);
				MTaddToKeyboardQueue(data->keyboard);
				} else {
					data->state = 5;
					data->download = MTcreateDownload(data->url, NULL);
					MTaddToDownloadQueue(data->download);
				}
			}
		} else if(data->state == 2) {
			window->cursorType = 2;
			MTdrawString(10,10,"Iniciando sesion en www.scenebeta.com...\nIntroduce tu nombre de usuario.");
			if(data->keyboard->done) {
				if(data->keyboard->cancel == 0) {
					sprintf(MTappDataDownPSPuser,data->keyboard->text);
					MTfreeKeyboard(data->keyboard);
					data->keyboard = NULL;
					data->keyboard = MTcreateKeyboard("Contraseña", "Contraseña de scenebeta.com", 256);
					MTaddToKeyboardQueue(data->keyboard);
					data->state = 3;
				} else {
					data->state = 1;
					MTfreeKeyboard(data->keyboard);
					data->keyboard = NULL;
				}
			}
		} else if(data->state == 3) {
			window->cursorType = 2;
			MTdrawString(10,10,"Iniciando sesion en www.scenebeta.com...\nIntroduce tu nombre de usuario.\nIntroduce tu contraseña.");
			if(data->keyboard->done) {
				if(data->keyboard->cancel == 0) {
					sprintf(MTappDataDownPSPpassword,data->keyboard->text);
					MTfreeKeyboard(data->keyboard);
					data->keyboard = NULL;
					data->state = 4;
					
					data->download = MTcreateDownload("http://www.scenebeta.com/node?destination=node", NULL);
					asprintf(&data->download->post,"name=%s&pass=%s&form_id=user_login_block",MTappDataDownPSPuser,MTappDataDownPSPpassword);
					MTaddToDownloadQueue(data->download);
				} else {
					data->state = 1;
					MTfreeKeyboard(data->keyboard);
					data->keyboard = NULL;
				}
			}
		} else if(data->state == 4) {
			if(data->download->done && data->download->error == 0 && data->download->file != NULL) {
				window->cursorType = 2;
				
				if(strstr(data->download->file, "Inicio de sesi") != NULL) {
					data->state = 1;
					MTshowErrorWindow("Error al inciar sesion", "No fue posible iniciar sesion\ncon los datos especificados.");
					MTfreeDownload(data->download);
			
					data->download = NULL;
				} else {
					data->state = 5;
					MTfreeDownload(data->download);
			
					data->download = NULL;
					
					MTappDataDownPSPloginSuccess = 1;
					data->download = MTcreateDownload(data->url, NULL);
					MTaddToDownloadQueue(data->download);
				}
				
				//MTfreedata->download(data->download);
			} else {
				if(data->download->error) {
					MTdrawString(10,10,"Error al descargar");
					window->cursorType = 14;
					data->state = 1;
					MTshowErrorWindow("Error al descargar", "Se produjo un error y\nno se pudo continuar");
				} else {
					if(data->download->knownsize) {
					char buff[256];
					sprintf(buff,"Iniciando sesion en www.scenebeta.com...\nIntroduce tu nombre de usuario.\nIntroduce tu contraseña.\nLogging in...\nRecibiendo...\n%d%% completado.",data->download->percent);
					MTdrawString(10,10,buff);
					} else MTdrawString(10,10,"Iniciando sesion en www.scenebeta.com...\nIntroduce tu nombre de usuario.\nIntroduce tu contraseña.\nLogging in...\nRecibiendo...");
					window->cursorType = 2;
				}
			}
			//window->cursorType = 2;
			//MTdrawString(10,10,"Iniciando sesion en www.scenebeta.com...\nIntroduce tu nombre de usuario.\nIntroduce tu contraseña.\nLogging in...");
		} else if(data->state == 5) {
			if(data->download->done && data->download->error == 0 && data->download->file != NULL) {
				window->cursorType = 2;
				char HomebrewFileURL[200];
	
				char *attachments = strstr(data->download->file, "<!-- I C -->");
				int element = 0;
				attachments = strstr(attachments, "id=\"attachments\">");
				if(attachments) {
					bool foundAny = 0;
					while(1) {
						attachments = strstr(attachments, "<a href=\"");
						if(attachments) {
							attachments += strlen("<a href=\"");

							int ATparser = 0;
							while(attachments[ATparser] != '"') {
									//Lista.Nombres[element][ATparser] = attachments[ATparser];
									HomebrewFileURL[ATparser] = attachments[ATparser];
									ATparser++;
							}
							//ATparser++;
							//Lista.Nombres[element][ATparser] = '\0';
							HomebrewFileURL[ATparser] = '\0';
							if(MTstristr(HomebrewFileURL, ".rar")) {//Si es RAR
								//if(!strstr(HomebrewFileURL, "PC")) {//Si no es para PC (hay aplicaciones como PSPCleaner, y falla)
									if(MTstristr(HomebrewFileURL, "plus")) {//Si es para CFplus (hay aplicaciones en las que pone CF_plus, por lo que solo compruebo "plus")
										if(!strstr(HomebrewFileURL, "SRC")) {//Si no es SRC
										   if(MTstristr(HomebrewFileURL, "http://psp.scenebeta.com/system/files/private/")) {
										      //entonces es lo que queríamos
										      //encontradoAlgunHB = 1;
										      //installHB(HomebrewFileURL);
										      //if(instaladoArchivo == 1) return;
										      foundAny = 1;
										      MTlaunchAppDownPSPfileInstaller(HomebrewFileURL);
										      /*if(message(HomebrewFileURL) == 1) {
										      	break;
										      }*/
										   }
										}
									}
								//}
							}
							element++;
						} else {
							if(foundAny == 0) MTshowErrorWindow("No se encontro ninguna descarga", "Este homebrew no tiene ninguna\ndescarga válida disponible.");
						   break;
						}
					}

				} else {
					MTshowErrorWindow("No se encontro ninguna descarga", "Este homebrew no tiene ninguna\ndescarga válida disponible.");
				}
				MTfreeDownload(data->download);
					
				data->download = NULL;
					
				data->state = 1;
			} else {
				if(data->download->error) {
					MTdrawString(10,10,"Error al descargar");
					window->cursorType = 14;
					data->state = 1;
					MTshowErrorWindow("Error al descargar", "Se produjo un error y\nno se pudo continuar");
				} else {
					if(data->download->knownsize) {
					char buff[256];
					sprintf(buff,"Recibiendo lista de descargas...\n%d%% completado.",data->download->percent);
					MTdrawString(10,10,buff);
					} else MTdrawString(10,10,"Recibiendo lista de descargas...");
					window->cursorType = 2;
				}
			}
		} else if(data->state == 6) {
			
		}
	}
	
	MTappDownPSPnewViewerData* data = (MTappDownPSPnewViewerData*)MTmalloc(sizeof(MTappDownPSPnewViewerData));
	
	data->data = NULL;
	data->state = 0;
	data->textBox = NULL;
	data->keyboard = NULL;
	data->download = MTcreateDownload(url, NULL);
	asprintf(&data->url,url);
	
	//MTdownload* download
	MTaddToDownloadQueue(data->download);
	
	MTapiWindow* window = MTapiCreateWindow(name, 260, 200, &drawMyWin, RGBA(255,255,255,255));
	
	
	window->cursorType = 2;
	window->data = data;
	window->icon = MTapiGetIcon(91);
	
	window->autoFree = 1;
	
	window->freeData = MTfreeAppDownPSPnewViewerData;
	
	MTapiAddWindow(window);
	
	return;
}

/*void MTlaunchAppDownPSPnewViewer(char* name, char* url) {
	if(name == NULL || url == NULL) return;
	void drawMyWin(MTapiWindow* window) {
		if(window == NULL || window->data == NULL) return;
		
		MTdownload* download = window->data;
		if(download == NULL) return;
		
		if(download->done && download->error == 0 && download->file != NULL) {
			
			if(download->response == 0) {
				window->cursorType = 0;
				if(window->visible == 1) {
					MTdrawString(10,10,download->file);
					if(window->tapControls.cross == 1) {
						//MTlaunchAppDownPSPnewInstaller(char* name, char* url)
					}
				}
			} else {
				
				char* data = download->file;
				
				char* parsed = (char*) MTmalloc (download->size * sizeof(char) + 1);
				if(parsed != NULL) {
					parsed[0] = '\0';
					if(data != NULL) {
						MTremoveTagsFromString(data, parsed, download->size, "<!-- I C -->", "<!-- F C -->", "<", ">");
						
						MTremoveTagsFromString(parsed, data, download->size, NULL, NULL, "&", ";");
						MTfree(parsed);
					}
				}
				
				if(window->visible == 1) MTdrawString(10,10,download->file);
				
				download->response = 0;
			}
			//MTfreeDownload(download);
		} else {
			if(download->error) {
				MTdrawString(10,10,"Error al descargar");
			} else {
				if(download->knownsize) {
				char buff[256];
				sprintf(buff,"Downloading...\n%d%% completado.",download->percent);
				MTdrawString(10,10,buff);
				} else MTdrawString(10,10,"Downloading...");
			}
		}
	}
	
	MTdownload* download = MTcreateDownload(url, NULL);
	MTaddToDownloadQueue(download);
	
	MTapiWindow* window = MTapiCreateWindow(name, 150, 50, &drawMyWin, RGBA(255,255,255,255));
	
	
	window->cursorType = 2;
	window->data = download;
	window->icon = MTapiGetIcon(91);
	
	window->autoFree = 1;
	
	window->freeData = MTfreeDownload;
	
	MTapiAddWindow(window);
	
	return;
}*/

void MTlaunchAppDownPSP() {
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
		
		if(testMenu->selectedElement != NULL && testMenu->selectedElement->data != NULL) {
			
			char scenebetafile[256];
			sprintf(scenebetafile,"http://psp.scenebeta.com/noticia/%s",testMenu->selectedElement->data);
			
			MTlaunchAppDownPSPnewViewer(testMenu->selectedElement->name, scenebetafile);
		}
	}
	
	void drawMyWinDownloading(MTapiWindow* window) {
		if(window == NULL || window->data == NULL) return;
		
		MTdownload* download = window->data;
		if(download == NULL) return;
		
		if(download->done && download->error == 0 && download->file != NULL) {
			
			//if(download->response == 0) {
				//DONE CHANGE WINDOW
			//} else {
				window->cursorType = 0;
				window->data = MTapiCreateMenu(0, 0, window->size.x, window->size.y, "Todos los programas","   - ");
				//asprintf(&((MTapiMenu*)window6->data)->prefix,"   - ");
				//window->icon = MTapiGetIcon(92);
	
				window->autoFree = 1;
	
				window->freeData = MTapiFreeMenu;
				
				window->function = drawMyWin;
	
				/*MTdownload* down1 = MTcreateDownload("http://carlosgs.no-ip.org:8080/PSPhomebrew/todosHomebrew","todosHomebrew");
				MTdoDownload(down1);
				MTfreeDownload(down1);*/
	
				/*MTdownload descarga;
				descarga.url = "http://carlosgs.no-ip.org:8080/PSPhomebrew/todosHomebrew";
				descarga.path = "todosHomebrew";
				MTdoDownload(&descarga);*/
	
				int size = download->size;
				char* data = download->file;//MTgetFileContent("todosHomebrew", &size);
	
				if(data != NULL) {
					int parseri = 0;
					int firstParseri = 0;
					int parserLinei = 0;
					int parserBufferi = 0;
	
						while( (size - 1) > parseri ) {
							char buffer1[256];
							char buffer2[256];
				
							while (data[parseri] != '#') {
								parseri++;
							}
							parserBufferi = 0;
							while (parseri > firstParseri) {
								buffer2[parserBufferi] = data[firstParseri];
								parserBufferi++;
								firstParseri++;
							}
							buffer2[parserBufferi] = '\0';
							firstParseri = parseri + 1;
							while (data[parseri] != '\n') {
								parseri++;
							}
							parserBufferi = 0;
							while (parseri > firstParseri) {
								buffer1[parserBufferi] = data[firstParseri];
								parserBufferi++;
								firstParseri++;
							}
							buffer1[parserBufferi] = '\0';
							firstParseri = parseri + 1;
				
							MTapiAddMenuElement(window->data,MTapiCreateMenuElement(buffer1, buffer2),0);
				
							parserLinei++;
						}
						//MTfree(data);
				}
				
				MTfreeDownload(download);
				//download->response = 0;
			//}
		} else {
			if(download->error) {
				MTdrawString(10,10,"Error al descargar list");
			} else {
				if(download->knownsize) {
				char buff[256];
				sprintf(buff,"Descargando lista de homebrew...\n%d%% completado.",download->percent);
				MTdrawString(10,10,buff);
				} else MTdrawString(10,10,"Descargando lista de homebrew...");
			}
		}
	}
	
	MTapiWindow* window = MTapiCreateWindow("Mini downPSP", 220, 150, &drawMyWinDownloading, RGBA(255,255,255,255));
	
	window->cursorType = 2;
	
	MTdownload* download = MTcreateDownload("http://carlosgs.no-ip.org:8080/PSPhomebrew/todosHomebrew", NULL);
	MTaddToDownloadQueue(download);
	
	window->data = download;//MTapiCreateMenu(0, 0, window->size.x, window->size.y, "All programs","   - ");
	//asprintf(&((MTapiMenu*)window6->data)->prefix,"   - ");
	window->icon = MTapiGetIcon(92);
	
	window->autoFree = 1;
	
	window->freeData = MTfreeDownload;
	
	/*window->freeData = MTapiFreeMenu;
	
	MTdownload* down1 = MTcreateDownload("http://carlosgs.no-ip.org:8080/PSPhomebrew/todosHomebrew","todosHomebrew");
	MTdoDownload(down1);
	MTfreeDownload(down1);
	
	int size = 0;
	char* data = MTgetFileContent("todosHomebrew", &size);
	
	if(data != NULL) {
		int parseri = 0;
		int firstParseri = 0;
		int parserLinei = 0;
		int parserBufferi = 0;
	
			while( (size - 1) > parseri ) {
				char buffer1[256];
				char buffer2[256];
				
				while (data[parseri] != '#') {
					parseri++;
				}
				parserBufferi = 0;
				while (parseri > firstParseri) {
					buffer2[parserBufferi] = data[firstParseri];
					parserBufferi++;
					firstParseri++;
				}
				buffer2[parserBufferi] = '\0';
				firstParseri = parseri + 1;
				while (data[parseri] != '\n') {
					parseri++;
				}
				parserBufferi = 0;
				while (parseri > firstParseri) {
					buffer1[parserBufferi] = data[firstParseri];
					parserBufferi++;
					firstParseri++;
				}
				buffer1[parserBufferi] = '\0';
				firstParseri = parseri + 1;
				
				MTapiAddMenuElement(window->data,MTapiCreateMenuElement(buffer1, buffer2),0);
				
				parserLinei++;
			}
			MTfree(data);
	}*/
	
	MTapiAddWindow(window);
	return;
}
