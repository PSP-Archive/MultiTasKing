#define MTiniMaxStringSize 512

//0 if OK -1 if wrong
int MTiniGetString(char * ini, char * section, char * name, char * defVal, char * val) {
	if(ini == NULL) {
		val = defVal;
		return -1;
	}
	
	return ini_gets(section, name, defVal, val, MTiniMaxStringSize, ini);
}

//0 if OK -1 if wrong
int MTiniSetString(char * ini, char * section, char * name, char * val)
{
	if(ini == NULL) return -1;
	if(ini_puts(section, name,val, ini) == 1) {
		return 0;
	} else return -1;
}

/* Float */
float MTiniGetFloat(char * d, char * section, char * name, float notfound)
{
	/*char	*	str ;

	str = iniparser_getstring(d, key, INI_INVALID_KEY);
	if (str==INI_INVALID_KEY) return notfound ;*/
	
	if(d == NULL) return notfound;
	char str[512];
	int len = MTiniGetString(d, section, name, "", str);
	if(len == 0) return notfound;
	
	float result;
	if(sscanf(str,"%f",&result) == 1) return result;
	return notfound;
}

int MTiniSetFloat(char * ini, char * section, char * name, float val)
{
	char* str;
	asprintf(&str,"%f",val);
	int ret = MTiniSetString(ini, section, name, str);
	MTfree(str);
	return ret;
}

/* Double */
double MTiniGetDouble(char * d, char * section, char * name, double notfound)
{
	/*char	*	str ;

	str = iniparser_getstring(d, key, INI_INVALID_KEY);
	if (str==INI_INVALID_KEY) return notfound ;*/
	
	if(d == NULL) return notfound;
	char str[512];
	int len = MTiniGetString(d, section, name, "", str);
	if(len == 0) return notfound;
	
	float result;
	if(sscanf(str,"%e",&result) == 1) return result;
	return notfound;
}

int MTiniSetDouble(char * ini, char * section, char * name, double val)
{
	char* str;
	asprintf(&str,"%e",val);
	int ret = MTiniSetString(ini, section, name, str);
	MTfree(str);
	return ret;
}

/* Boolean */
int MTiniGetBoolean(char * d, char * section, char * name, int notfound)
{
	/*char	*	str ;

	str = iniparser_getstring(d, key, INI_INVALID_KEY);
	if (str==INI_INVALID_KEY) return notfound ;*/
	
	if(d == NULL) return notfound;
	char str[512];
	int len = MTiniGetString(d, section, name, "", str);
	if(len == 0) return notfound;
	
	int ret;
	
		if (str[0]=='y' || str[0]=='Y' || str[0]=='1' || str[0]=='t' || str[0]=='T') {
		ret = 1 ;
		} else if (str[0]=='n' || str[0]=='N' || str[0]=='0' || str[0]=='f' || str[0]=='F') {
		ret = 0 ;
		} else {
		ret = notfound ;
		}
		return ret;
}

int MTiniSetBoolean(char * ini, char * section, char * name, int val)
{
	char* str;
	if(val == 1) asprintf(&str,"TRUE"); else asprintf(&str,"FALSE");
	int ret = MTiniSetString(ini, section, name, str);
	MTfree(str);
	return ret;
}

/* Integer */
int MTiniGetInteger(char * d, char * section, char * name, int notfound)
{
	/*char	*	str ;

	str = iniparser_getstring(d, key, INI_INVALID_KEY);
	if (str==INI_INVALID_KEY) return notfound ;*/
	
	if(d == NULL) return notfound;
	char str[512];
	int len = MTiniGetString(d, section, name, "", str);
	if(len == 0) return notfound;
	
	int result;
	if(sscanf(str,"%d",&result) == 1) return result;
	return notfound;
}

int MTiniSetInteger(char * ini, char * section, char * name, int val)
{
	char* str;
	asprintf(&str,"%i",val);
	int ret = MTiniSetString(ini, section, name, str);
	MTfree(str);
	return ret;
}

/* Color */
MTcolor MTiniGetColor(char * d, char * section, char * name, MTcolor notfound)
{
	/*char	*	str ;

	str = iniparser_getstring(d, key, INI_INVALID_KEY);
	if (str==INI_INVALID_KEY) return notfound ;*/
	
	if(d == NULL) return notfound;
	char str[512];
	int len = MTiniGetString(d, section, name, "", str);
	if(len == 0) return notfound;
	
	int r,g,b,a;
	if(sscanf(str,"RGBA(%d,%d,%d,%d)",&r,&g,&b,&a) != 4) {
		if(sscanf(str,"RGB(%d,%d,%d)",&r,&g,&b) == 3) {
			//a = 255;
			return RGB(r,g,b);
		}
	} else return RGBA(r,g,b,a);
	return notfound;
}

int MTiniSetColor(char * ini, char * section, char * name, MTcolor val)
{
	char* str;
	
	if(A(val) != 255) {
		asprintf(&str,"RGBA(%d,%d,%d,%d)",R(val),G(val),B(val),A(val));
	} else asprintf(&str,"RGB(%d,%d,%d)",R(val),G(val),B(val));
	int ret = MTiniSetString(ini, section, name, str);
	MTfree(str);
	return ret;
}

/* Vector */
MTvector* MTiniGetVector(char * d, char * section, char * name, MTvector* notfound)
{
	/*char	*	str ;

	str = iniparser_getstring(d, key, INI_INVALID_KEY);
	if (str==INI_INVALID_KEY) return notfound ;*/
	
	if(d == NULL) return notfound;
	char str[512];
	int len = MTiniGetString(d, section, name, "", str);
	if(len == 0) return notfound;
	
	MTvector* result = (MTvector*)malloc(sizeof(MTvector));
	int x, y;
	if(sscanf(str,"vct(%d,%d)",&x,&y) == 2) {
		result->x = x;
		result->y = y;
		return result;
	}
	return notfound;
}

int MTiniSetVector(char * ini, char * section, char * name, MTvector* val)
{
	char* str;
	
	asprintf(&str,"vct(%d,%d)",val->x,val->y);
	int ret = MTiniSetString(ini, section, name, str);
	MTfree(str);
	return ret;
}
