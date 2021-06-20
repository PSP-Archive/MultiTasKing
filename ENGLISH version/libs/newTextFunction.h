/*typedef struct {
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
	
	bool adjustSpaces;
	bool prevAdjustSpaces;
	
	int firstLine;
	int firstLinePos;
	int prevFirstLine;
	
	int cursorPos;//selectedElement (NOT WORKING NOW)
	char* textReadyToPrint;
	
	//bool barIsActive;
	
	bool active;
	
	MTapiScrollBar scrollBarH;
	MTapiScrollBar scrollBarV;
	//bool needRefresh;
} MTapiTextBox;*/

void MTapiFreeTextBox(MTapiTextBox* textBox) {
	
	if(textBox == NULL) return;
	if(textBox->text != NULL) MTfree(textBox->text);
	textBox->text = NULL;
	if(textBox->textReadyToPrint != NULL) MTfree(textBox->textReadyToPrint);
	textBox->textReadyToPrint = NULL;
	
	//message("6");
	
	MTfree(textBox);
	textBox = NULL;
	
	//message("7");
	return;
}

MTapiTextBox* MTapiCreateTextBox(int posX, int posY, int width, int height, char* text) {
	//message(name);
	//MTapiAddWindow(MTapiWindow* window)
	MTapiTextBox* textBox = (MTapiTextBox*)MTmalloc(sizeof(MTapiTextBox));
	if(textBox == NULL) return NULL;
	
	textBox->len = MTstrlen(text);
	if(textBox->len > 0) {
		textBox->text = (char*)MTmalloc(textBox->len * sizeof(char));
		strcpy(textBox->text,text);
	}
	
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
	
	if(textBox->firstLine != textBox->prevFirstLine || textBox->len != textBox->prevLen || textBox->size.x != textBox->prevSize.x || textBox->size.y != textBox->prevSize.y || textBox->textHeight != textBox->prevTextHeight || textBox->textJumpSize != textBox->prevTextJumpSize || textBox->autoLineBreak != textBox->prevAutoLineBreak || textBox->adjustSpaces != textBox->prevAdjustSpaces ) {
		
		int bufferSize = 1;
		if(textBox->textReadyToPrint != NULL) {
			textBox->textReadyToPrint[0] = '\0';
			MTfree(textBox->textReadyToPrint);
		}
		
		textBox->textReadyToPrint = (char*) malloc ( bufferSize * sizeof(char));
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
			
			while (count < 100001 && pos <= textBox->len) {
				skipnext = 0;
				repeatnext = 0;
				
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
					char text[2];
					text[0] = textBox->text[pos];
					text[1] = '\0';
					lineWidth += MTgetTextWidth(text);
					
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
									textBox->textReadyToPrint = (char*) realloc (textBox->textReadyToPrint, bufferSize * sizeof(char) + 1);
									textBox->textReadyToPrint[bufferSize -1] = '-';
									textBox->textReadyToPrint[bufferSize] = '\0';
									bufferSize += 1;
								}
								
								textBox->textReadyToPrint = (char*) realloc (textBox->textReadyToPrint, bufferSize * sizeof(char) + 1);
								textBox->textReadyToPrint[bufferSize -1] = '\n';
								textBox->textReadyToPrint[bufferSize] = '\0';
								bufferSize += 1;
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
									textBox->textReadyToPrint = (char*) realloc (textBox->textReadyToPrint, bufferSize * sizeof(char) + 1);
									textBox->textReadyToPrint[bufferSize -1] = '\n';
									textBox->textReadyToPrint[bufferSize] = '\0';
									bufferSize += 1;
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
				
				if(skipnext == 0) {
					
					if(stopCreating == 0) {
					textBox->textReadyToPrint = (char*) realloc (textBox->textReadyToPrint, bufferSize * sizeof(char) + 1);
					textBox->textReadyToPrint[bufferSize -1] = textBox->text[pos];
					textBox->textReadyToPrint[bufferSize] = '\0';
					bufferSize += 1;
					}
				}
				if(repeatnext == 0) pos++;
				
				if(height > textBox->size.y + textBox->textHeight) {
					stopCreating = 1;
					if(optimise) break;
				}
				
				count++;
			}
			
			if(optimise == 0) {
				textBox->lines = lines;
			}
			
			//MTapiSetFontLineBreak(textBox->font, linebreaksize);
		}
		
		
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
	}
	
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
		MTdrawString(posx,posy,textBox->textReadyToPrint);
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

/*int MTgetTextAligned(char* text, char* result, int width) {
	if(text == NULL || result == NULL) return;
	
	
	
	return height;
}*/
