void MTinitControls() {
	sceCtrlSetSamplingCycle(0);//Init the controls
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	return;
}

SceCtrlData MTreadingControls;
void MTreadControls(MTcontrols* ctrls) {
	sceCtrlReadBufferPositive(&MTreadingControls, 1);//leemos controles
	
	if(MTreadingControls.Buttons & PSP_CTRL_SELECT) ctrls->select = 1; else ctrls->select = 0;
	if(MTreadingControls.Buttons & PSP_CTRL_START) ctrls->start = 1; else ctrls->start = 0;
	if(MTreadingControls.Buttons & PSP_CTRL_UP) ctrls->up = 1; else ctrls->up = 0;
	if(MTreadingControls.Buttons & PSP_CTRL_RIGHT) ctrls->right = 1; else ctrls->right = 0;
	if(MTreadingControls.Buttons & PSP_CTRL_DOWN) ctrls->down = 1; else ctrls->down = 0;
	if(MTreadingControls.Buttons & PSP_CTRL_LEFT) ctrls->left = 1; else ctrls->left = 0;
	if(MTreadingControls.Buttons & PSP_CTRL_RTRIGGER) ctrls->r = 1; else ctrls->r = 0;
	if(MTreadingControls.Buttons & PSP_CTRL_LTRIGGER) ctrls->l = 1; else ctrls->l = 0;
	if(MTreadingControls.Buttons & PSP_CTRL_TRIANGLE) ctrls->triangle = 1; else ctrls->triangle = 0;
	if(MTreadingControls.Buttons & PSP_CTRL_CIRCLE) ctrls->circle = 1; else ctrls->circle = 0;
	if(MTreadingControls.Buttons & PSP_CTRL_CROSS) ctrls->cross = 1; else ctrls->cross = 0;
	if(MTreadingControls.Buttons & PSP_CTRL_SQUARE) ctrls->square = 1; else ctrls->square = 0;
	if(MTreadingControls.Buttons & PSP_CTRL_HOME) ctrls->home = 1; else ctrls->home = 0;
	if(MTreadingControls.Buttons & PSP_CTRL_HOLD) ctrls->hold = 1; else ctrls->hold = 0;
	if(MTreadingControls.Buttons & PSP_CTRL_NOTE) ctrls->note = 1; else ctrls->note = 0;
	
	ctrls->ax = MTreadingControls.Lx - 127;// de -128 (left) a +127
	ctrls->ay = MTreadingControls.Ly - 127;// de -128 (up) a +127
	
	if(ctrls->as != 0) {
		if(!(ctrls->ax >= ctrls->as || -ctrls->ax >= ctrls->as)) ctrls->ax = 0;
		if(!(ctrls->ay >= ctrls->as || -ctrls->ay >= ctrls->as)) ctrls->ay = 0;
	}
	
	return;
}

void MTemptyControls(MTcontrols* ctrls) {
	ctrls->select = 0;
	ctrls->start = 0;
	ctrls->up = 0;
	ctrls->right = 0;
	ctrls->down = 0;
	ctrls->left = 0;
	ctrls->l = 0;
	ctrls->r = 0;
	ctrls->triangle = 0;
	ctrls->circle = 0;
	ctrls->cross = 0;
	ctrls->square = 0;
	ctrls->home = 0;
	ctrls->hold = 0;
	ctrls->note = 0;

	ctrls->ax = 0;//analog
	ctrls->ay = 0;

	//ctrls->as = 0;//analog sensitivity (minimum displacement to give us ax or ay)
	return;
}

void MTcopyControls(MTcontrols* ctrls1,MTcontrols* ctrls0) {
	if(ctrls1 == NULL || ctrls0 == NULL) return;
	ctrls1->select = ctrls0->select;
	ctrls1->start = ctrls0->start;
	ctrls1->up = ctrls0->up;
	ctrls1->right = ctrls0->right;
	ctrls1->down = ctrls0->down;
	ctrls1->left = ctrls0->left;
	ctrls1->l = ctrls0->l;
	ctrls1->r = ctrls0->r;
	ctrls1->triangle = ctrls0->triangle;
	ctrls1->circle = ctrls0->circle;
	ctrls1->cross = ctrls0->cross;
	ctrls1->square = ctrls0->square;
	ctrls1->home = ctrls0->home;
	ctrls1->hold = ctrls0->hold;
	ctrls1->note = ctrls0->note;
	
	ctrls1->ax = ctrls0->ax;
	ctrls1->ay = ctrls0->ay;
	
	//ctrls1->as = ctrls0->as;
	return;
}

void MTlockControls() {
	if(controlsLocked != 1) {
		controlsLocked = 1;
		MTdelay(100000);//Dejamos que los controles se estabilicen
		MTemptyControls(&MTpressedControls);
	}
}

void MTunLockControls() {
	controlsLocked = 0;
}

bool MTpressingAnyControls(MTcontrols* ctrls) {
	if(ctrls->select == 1) return 1;
	if(ctrls->start == 1) return 1;
	if(ctrls->up == 1) return 1;
	if(ctrls->right == 1) return 1;
	if(ctrls->down == 1) return 1;
	if(ctrls->left == 1) return 1;
	if(ctrls->l == 1) return 1;
	if(ctrls->r == 1) return 1;
	if(ctrls->triangle == 1) return 1;
	if(ctrls->circle == 1) return 1;
	if(ctrls->cross == 1) return 1;
	if(ctrls->square == 1) return 1;
	//if(ctrls->home == 1) return 1;
	//if(ctrls->hold == 1) return 1;
	//if(ctrls->note == 1) return 1;
	return 0;
}

void MTctrlsTapCheck(bool* ctrlsTap,bool* ctrls, bool* ctrlsHasTap) {
	if(ctrlsTap == NULL || ctrls == NULL || ctrlsHasTap == NULL) return;
	if(*ctrls == 1) {
		if(*ctrlsHasTap == 0) {
			*ctrlsTap = 1;
			*ctrlsHasTap = 1;
		} else {
			*ctrlsTap = 0;
		}
	} else {
		*ctrlsTap = 0;
		*ctrlsHasTap = 0;
	}
	return;
}

void MTreadTapControls(MTcontrols* ctrlsTap,MTcontrols* ctrls,MTcontrols* ctrlsHasTap) {
	if(ctrlsTap == NULL || ctrls == NULL) return;
	
	MTctrlsTapCheck(&ctrlsTap->select,&ctrls->select, &ctrlsHasTap->select);
	MTctrlsTapCheck(&ctrlsTap->start,&ctrls->start, &ctrlsHasTap->start);
	MTctrlsTapCheck(&ctrlsTap->up,&ctrls->up, &ctrlsHasTap->up);
	MTctrlsTapCheck(&ctrlsTap->right,&ctrls->right, &ctrlsHasTap->right);
	MTctrlsTapCheck(&ctrlsTap->down,&ctrls->down, &ctrlsHasTap->down);
	MTctrlsTapCheck(&ctrlsTap->left,&ctrls->left, &ctrlsHasTap->left);
	MTctrlsTapCheck(&ctrlsTap->l,&ctrls->l, &ctrlsHasTap->l);
	MTctrlsTapCheck(&ctrlsTap->r,&ctrls->r, &ctrlsHasTap->r);
	MTctrlsTapCheck(&ctrlsTap->triangle,&ctrls->triangle, &ctrlsHasTap->triangle);
	MTctrlsTapCheck(&ctrlsTap->circle,&ctrls->circle, &ctrlsHasTap->circle);
	MTctrlsTapCheck(&ctrlsTap->cross,&ctrls->cross, &ctrlsHasTap->cross);
	MTctrlsTapCheck(&ctrlsTap->square,&ctrls->square, &ctrlsHasTap->square);
	MTctrlsTapCheck(&ctrlsTap->home,&ctrls->home, &ctrlsHasTap->home);
	MTctrlsTapCheck(&ctrlsTap->hold,&ctrls->hold, &ctrlsHasTap->hold);
	MTctrlsTapCheck(&ctrlsTap->note,&ctrls->note, &ctrlsHasTap->note);
	
	/*ctrls1->select = ctrls0->select;
	ctrls1->start = ctrls0->start;
	ctrls1->up = ctrls0->up;
	ctrls1->right = ctrls0->right;
	ctrls1->down = ctrls0->down;
	ctrls1->left = ctrls0->left;
	ctrls1->l = ctrls0->l;
	ctrls1->r = ctrls0->r;
	ctrls1->triangle = ctrls0->triangle;
	ctrls1->circle = ctrls0->circle;
	ctrls1->cross = ctrls0->cross;
	ctrls1->square = ctrls0->square;
	ctrls1->home = ctrls0->home;
	ctrls1->hold = ctrls0->hold;
	ctrls1->note = ctrls0->note;*/
	return;
}

