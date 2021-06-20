#ifndef LUADEVDRIVER
#define LUADEVDRIVER


extern int getBrightness();
extern void setBrightness(int brightness);
extern int imposeGetBrightness();
extern int imposeSetBrightness(int value);
extern int imposeGetVolume();
extern int imposeSetVolume(int value);
extern int imposeGetMute();
extern int imposeSetMute(int value);
extern int imposeGetEqualizer();
extern int imposeSetEqualizer(int value);
extern int displayEnable(void);
extern int displayDisable(void);
extern void readButtons(SceCtrlData *pad_data, int count);
extern int imposeSetHomePopup(int value);
extern int getModelKernel();
extern void runebootfile(char *path);
extern void SetConfFile(int n);
extern void SetUmdFile(char *umdfile);

#endif