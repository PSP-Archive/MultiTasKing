//Todo sacado de http://decsai.ugr.es/~jfv/ed1/tedi/cdrom/docs/ldoble.html
//Se sustituyó tElemento por void*

typedef struct MTcell {
	void* element;
	struct MTcell *next,*previous;
} MTcellType;

typedef MTcellType *MTpositionInList;
typedef MTcellType *MTelementsList;

static void MTlistError(char *cad)
{
	//fprintf(stderr, "MTlistError: %s\n", cad);
	//exit(1);
}

MTelementsList MTcreateElementsList()
{
	MTelementsList l;
	
	l = (MTelementsList)malloc(sizeof(MTcellType));
	if (l == NULL)
	MTlistError("When trying to create list; Not enough memory");
	l->next = l->previous = l;
	return l;
}

void MTdestroyList (MTelementsList l)
{
	MTpositionInList p;
	
	for (p=l, l->previous->next=NULL; l!=NULL; p=l) {
	l = l->next;
	free(p);
	}
}	

MTpositionInList MTfirstPositionInList (MTelementsList l)
{
	return l->next;
}

MTpositionInList MTlastPositionInList (MTelementsList l)
{
	return l;
}

void MTinsertElementInList (void* x, MTpositionInList p, MTelementsList l)
{
	MTpositionInList newPos;
	
	newPos = (MTpositionInList)malloc(sizeof(MTcellType));
	if (newPos == NULL) 
	MTlistError("When trying to insert in list; Not enough memory");
	newPos->element = x;
	newPos->next = p;
	newPos->previous = p->previous;
	p->previous->next = newPos;
	p->previous = newPos;
}

void MTeraseElementFromList (MTpositionInList *p, MTelementsList l)
{
	MTpositionInList q;
	
	if (*p == l){ 
	MTlistError("When trying to erase from list; The position is the last one");
	}
	q = (*p)->next;
	(*p)->previous->next = q;
	q->previous = (*p)->previous;
	free(*p);
	(*p) = q; 
}

void* MTgetElementFromList(MTpositionInList p, MTelementsList l)
{
	if (p == l){ 
	MTlistError("When trying to get an element from list; The positon is the last one");
	}
	return p->element;
}

MTpositionInList MTnextPositionInList (MTpositionInList p, MTelementsList l)
{
	if (p == l){ 
	MTlistError("When trying to get the next element from list; The positon is the last one");
	}
	return p->next;
}

MTpositionInList MTpreviousPositionInList ( MTpositionInList p, MTelementsList l)
{
	if (p == l->next){
	MTlistError("When trying to get the previous element from list; The positon is the first one");
	}
	return p->previous;
}

MTpositionInList MTelementPositionInList (void* x, MTelementsList l)
{
	MTpositionInList p;
	int encontrado;
	
	p = MTfirstPositionInList(l);
	encontrado = 0;
	while ((p != MTlastPositionInList(l)) && (!encontrado))
	if (p->element == x)
		encontrado = 1;
	else
		p = p->next;
	return p;
}

int MTnumberOfElementsInList(MTelementsList l) {
	int count = 0;
	MTpositionInList position = MTfirstPositionInList (l);
	while ((position  != MTlastPositionInList(l))) {
		count++;
		position  = MTnextPositionInList(position ,l);
	}
	return count;
}

/*MTpositionInList MTelementPositionInList (void* x, MTelementsList l)
{
	MTpositionInList p;
	int encontrado;
	
	p = MTfirstPositionInList(l);
	encontrado = 0;
	while ((p != MTlastPositionInList(l)) && (!encontrado))
	if (p->element == x)
		encontrado = 1;
	else
		p = p->next;
	return p;
}*/

MTpositionInList MTelementNumPositionInList(int num, MTelementsList l) {
	if(num > 1) {
		int count = 0;
		MTpositionInList position = MTfirstPositionInList (l);
		while ((position  != MTlastPositionInList(l))) {
			count++;
			if(count == num) return position;
			position  = MTnextPositionInList(position ,l);
		}
		return MTlastPositionInList (l);
	} else {
		return MTfirstPositionInList (l);
	}
}

MTpositionInList MTelementNumPositionInListOptimal(int num, MTelementsList l, int prevNum, MTpositionInList prevPos) {
	
	if(num == prevNum) return prevPos;
	
	if(num > 1) {
		int count = prevNum;
		bool direction = 1;
		
		if(num > prevNum) {
			direction = 1;
		} else direction = 0;
		
		MTpositionInList position = prevPos;
		while ((direction == 1 && position  != MTlastPositionInList(l)) || (direction == 0 && position  != MTfirstPositionInList(l)) ) {
			
			if(count == num) return position;
			if(direction == 1) {
				position = MTnextPositionInList(position ,l);
				count++;
			} else {
				position = MTpreviousPositionInList(position ,l);
				count--;
			}
		}
		if(direction == 1) {
			return MTlastPositionInList (l);
		} else return MTfirstPositionInList (l);
	} else {
		return MTfirstPositionInList (l);
	}
}

