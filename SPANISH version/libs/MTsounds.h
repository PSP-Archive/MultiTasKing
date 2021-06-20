

MTsound* MTloadSoundFileMP3 (char *filename) {
	if(MTcanUseKernel == 0) {
		MTshowErrorWindow("Error al cargar MP3", "Está ejecutando MultiTasKing sin soporte\npara modo kernel, no funciona el sistema de MP3.");
		return NULL;
	}
	
	if(filename == NULL) return NULL;
	int stream = OSL_FMT_NONE;
	if(MTgetFileSize(filename) > 1024*1024) stream = OSL_FMT_STREAM;
	
	MTsound* snd = (MTsound*) MTmalloc(sizeof(MTsound));
	if(snd == NULL) return NULL;
	
	snd->paused = 0;
	
	snd->coverImg = NULL;
	snd->tags = NULL;
	
	snd->data = NULL;
	
	snd->snd = oslLoadSoundFileMP3(filename,stream);
	
	if(snd->snd == NULL) return NULL;

	return snd;
	
	//return oslLoadImageFile (filename, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
}

MTsound* MTloadSoundFileAT3 (char *filename) {
	if(filename == NULL) return NULL;
	
	if(MTcanUseKernel == 0) {
		MTshowErrorWindow("Error al cargar AT3", "Está ejecutando MultiTasKing sin soporte\npara modo kernel, no funciona el sistema de AT3.");
		return NULL;
	}
	
	int stream = OSL_FMT_NONE;
	if(MTgetFileSize(filename) > 1024*1024) stream = OSL_FMT_STREAM;
	
	MTsound* snd = (MTsound*) MTmalloc(sizeof(MTsound));
	if(snd == NULL) return NULL;
	
	snd->paused = 0;
	
	snd->coverImg = NULL;
	snd->tags = NULL;
	
	snd->data = NULL;
	
	snd->snd = oslLoadSoundFileAT3(filename,stream);
	
	if(snd->snd == NULL) return NULL;

	return snd;
	
	//return oslLoadImageFile (filename, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
}

MTsound* MTloadSoundFileWAV (char *filename) {
	if(filename == NULL) return NULL;
	int stream = OSL_FMT_NONE;
	if(MTgetFileSize(filename) > 1024*1024) stream = OSL_FMT_STREAM;
	
	MTsound* snd = (MTsound*) MTmalloc(sizeof(MTsound));
	if(snd == NULL) return NULL;
	
	snd->paused = 0;
	
	snd->coverImg = NULL;
	snd->tags = NULL;
	
	snd->data = NULL;
	
	snd->snd = oslLoadSoundFileWAV(filename,stream);
	
	if(snd->snd == NULL) return NULL;

	return snd;
	
	//return oslLoadImageFile (filename, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
}

MTsound* MTloadSoundFile (char *filename) {
	if(filename == NULL) return NULL;
	
	int type = MTgetFileType(filename);
	if(type == 7) return MTloadSoundFileMP3(filename);
	if(type == 25) return MTloadSoundFileWAV(filename);
	if(type == 36) return MTloadSoundFileAT3(filename);
	
	MTloadFailCallback(filename, 0);

	return NULL;
	
	//return oslLoadImageFile (filename, OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
}

/*MTid3Tags * MTcreateID3tags(char *mp3path) {
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
}*/

void MTfreeSound(MTsound* snd) {
	if(snd == NULL) return;
	if(snd->snd != NULL) oslDeleteSound (snd->snd);
	if(snd->tags != NULL) MTfreeID3tags (snd->tags);
	if(snd->coverImg != NULL) MTfreeImage (snd->coverImg);
	if(snd->data != NULL) MTfree (snd->data);
	snd->data = NULL;
	snd->coverImg = NULL;
	snd->snd = NULL;
	snd->tags = NULL;
	MTfree(snd);
	
	snd = NULL;
	
	return;
}

int MTisSoundStopped(MTsound* snd) {
	if(snd == NULL) return -1;
	if(oslGetSoundChannel(snd->snd) != -1) return 0;
	return 1;
}

void MTplaySound(MTsound* snd) {
	if(snd == NULL) return;
	if(snd->snd == NULL) return;
	oslPlaySound(snd->snd,-1);
	snd->paused = 0;
	return;
}

void MTplaySoundWithChannel(MTsound* snd,int channel) {
	if(snd == NULL) return;
	if(snd->snd == NULL) return;
	if(channel < 0 || channel > 7) channel = -1;
	oslPlaySound(snd->snd,channel);
	snd->paused = 0;
	return;
}

void MTstopSound(MTsound* snd) {
	if(snd == NULL) return;
	if(snd->snd == NULL) return;
	oslStopSound(snd->snd);
	snd->paused = 0;
	return;
}

void MTpauseSound(MTsound* snd) {
	if(snd == NULL) return;
	if(snd->snd == NULL) return;
	//if(snd->paused == 1) return;
	oslPauseSound(snd->snd,1);
	snd->paused = 1;
	return;
}

void MTresumeSound(MTsound* snd) {
	if(snd == NULL) return;
	if(snd->snd == NULL) return;
	//if(snd->paused == 0) return;
	oslPauseSound(snd->snd,0);
	snd->paused = 0;
	return;
}

/*void oslPlaySound (OSL_SOUND *s, int voice)
void oslStopSound (OSL_SOUND *s)
void oslPauseSound (OSL_SOUND *s, int pause)
void oslDeleteSound (OSL_SOUND *s)*/
