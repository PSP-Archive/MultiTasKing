int MTnetGetFile(const char *url, const char *filepath, char* cookie)
{
	int template, connection, request, ret, status, dataend, fd, byteswritten;
	SceULong64 contentsize;
	unsigned char readbuffer[8192];
/*										//<-- STAS: HTTP library was already initialized (see oslNetInit()) !
	ret = sceHttpInit(20000);

	if(ret < 0)
		return OSL_ERR_HTTP_INIT;
*/										//<-- STAS END -->
	char headtxt[200];
	sprintf(headtxt,"MultiTasKing/%d MTnetGetFile/1.0.0",MTKversion);
	template = sceHttpCreateTemplate(headtxt, 1, 1);
	if(template < 0)
		return OSL_ERR_HTTP_TEMPLATE;

	ret = sceHttpSetResolveTimeOut(template, 3000000);
	if(ret < 0)
		return OSL_ERR_HTTP_TIMEOUT;

	ret = sceHttpSetRecvTimeOut(template, 60000000);
	if(ret < 0)
		return OSL_ERR_HTTP_TIMEOUT;

	ret = sceHttpSetSendTimeOut(template, 60000000);
	if(ret < 0)
		return OSL_ERR_HTTP_TIMEOUT;

	connection = sceHttpCreateConnectionWithURL(template, url, 0);
	if(connection < 0)
		return OSL_ERR_HTTP_CONNECT;

	request = sceHttpCreateRequestWithURL(connection, PSP_HTTP_METHOD_GET, (char*)url, 0);
	if(request < 0)
		return OSL_ERR_HTTP_REQUEST;
	
	if(cookie != NULL && MTstrlen(cookie) > 0) sceHttpAddExtraHeader(request, "Cookie", cookie, 0);

	ret = sceHttpSendRequest(request, 0, 0);
	if(ret < 0)
		return OSL_ERR_HTTP_REQUEST;

	ret = sceHttpGetStatusCode(request, &status);
	if(ret < 0)
		return OSL_ERR_HTTP_GENERIC;

	if(status != 200)
		return 0;

	ret = sceHttpGetContentLength(request, &contentsize);
	if(ret < 0) {
		//download->knownsize = 0;
		//download->size = 0;
	} else {
		//download->size = contentsize;
		//download->knownsize = 1;
	}

	dataend = 0;
	byteswritten = 0;

	remove(filepath);
	fd = sceIoOpen(filepath, PSP_O_WRONLY | PSP_O_CREAT, 0777);

	while(dataend == 0)
	{
		ret = sceHttpReadData(request, readbuffer, 8192);
		if(ret < 0)
		{
			sceIoWrite(fd, filepath, 4);
			sceIoClose(fd);
			return OSL_ERR_HTTP_GENERIC;
		}

		if(ret == 0)
			dataend = 1;

		if(ret > 0)
		{
			byteswritten += ret;
			sceIoWrite(fd, readbuffer, ret);
		}
	}

	sceIoClose(fd);
	sceHttpDeleteRequest(request);
	sceHttpDeleteConnection(connection);
	sceHttpDeleteTemplate(template);
//	sceHttpEnd();						//<-- STAS: This should be done in oslNetTerm() only !

	return 0;
}

MTdownload* MTcreateDownload(char* url, char* file) {
	
	if(url == NULL) return NULL;
	//message(name);
	//message(data);
	//MTapiAddWindow(MTapiWindow* window)
	MTdownload* download = (MTdownload*)MTmalloc(sizeof(MTdownload));
	
	download->url = (char*) MTmalloc ((MTstrlen(url) + 1) * sizeof(char));
	strcpy(download->url,url);
	
	if(file != NULL) {
	download->file = (char*) MTmalloc ((MTstrlen(file) + 1) * sizeof(char));
	strcpy(download->file,file);
	} else download->file = NULL;
	
	download->post = NULL;	
	
	download->cookie = NULL;
	download->size = 0;
	download->knownsize = 0;
	download->receivedsize = 0;
	download->percent = 0;
	download->done = 0;
	download->error = 0;
	download->ret = 0;
	download->response = 0;
	return download;
}

void MTcancelDownload(MTdownload* download);

void MTfreeDownload(MTdownload* download) {
	if(download == NULL) return;
	
	MTcancelDownload(download);
	
	if(download->url != NULL) MTfree(download->url);
	if(download->file != NULL) MTfree(download->file);
	if(download->cookie != NULL) MTfree(download->cookie);
	MTfree(download);
	return;
}

/*typedef struct {
	char* url;
	char* path;
	char* cookie;
	int size;
	bool knownsize;
	int receivedsize;
	char percent;
	bool done;
	bool error;
	int response;
} MTdownload;*/

int MTdoDownload(MTdownload* download);

int MTdoDownload_old(MTdownload* download)
{
	if(download == NULL) {
		return -1;
	}
	if(download->url == NULL) {
		download->ret =-2;
		return -2;
	}
	
	bool toMemory = 0;
	if(download->file == NULL) {
		toMemory = 1;
		//download->ret =-3;
		//return -3;
	}
	
	download->ret = 0;
	
	download->done = 0;
	download->error = 0;
	download->response = 0;
	download->percent = 0;
	download->receivedsize = 0;
	download->size = 0;
	download->knownsize = 0;
	int template, connection, request, ret, status, dataend, fd = 0, byteswritten;
	SceULong64 contentsize = -1;
	unsigned char readbuffer[8192];
/*										//<-- STAS: HTTP library was already initialized (see oslNetInit()) !
	ret = sceHttpInit(20000);

	if(ret < 0)
		return OSL_ERR_HTTP_INIT;
*/										//<-- STAS END -->
	char headtxt[200];
	sprintf(headtxt,"MultiTasKing/%d MTnetGetFile/1.0.0",MTKversion);
	template = sceHttpCreateTemplate(headtxt, 1, 1);
	//template = sceHttpCreateTemplate("OSL-agent/0.0.1 libhttp/1.0.0", 1, 1);
	if(template < 0) {
		download->error = 1;
		download->ret = OSL_ERR_HTTP_TEMPLATE;
		return OSL_ERR_HTTP_TEMPLATE;
	}

	ret = sceHttpSetResolveTimeOut(template, 3000000);
	if(ret < 0) {
		download->error = 1;
		download->ret = OSL_ERR_HTTP_TIMEOUT;
		return OSL_ERR_HTTP_TIMEOUT;
	}

	ret = sceHttpSetRecvTimeOut(template, 60000000);
	if(ret < 0) {
		download->error = 1;
		download->ret = OSL_ERR_HTTP_TIMEOUT;
		return OSL_ERR_HTTP_TIMEOUT;
	}

	ret = sceHttpSetSendTimeOut(template, 60000000);
	if(ret < 0) {
		download->error = 1;
		download->ret = OSL_ERR_HTTP_TIMEOUT;
		return OSL_ERR_HTTP_TIMEOUT;
	}

	connection = sceHttpCreateConnectionWithURL(template, download->url, 0);
	if(connection < 0) {
		download->error = 1;
		download->ret = OSL_ERR_HTTP_CONNECT;
		return OSL_ERR_HTTP_CONNECT;
	}
	
	sceHttpEnableCookie(connection);

	request = sceHttpCreateRequestWithURL(connection, PSP_HTTP_METHOD_GET, (char*)download->url, 0);
	if(request < 0) {
		download->error = 1;
		download->ret = OSL_ERR_HTTP_REQUEST;
		return OSL_ERR_HTTP_REQUEST;
	}
	
	if(download->cookie != NULL && MTstrlen(download->cookie) > 0) {
		if(sceHttpAddExtraHeader(request, "Cookie", download->cookie, 0) < 0) {
			download->error = 1;
		download->ret = OSL_ERR_HTTP_REQUEST;
			return OSL_ERR_HTTP_REQUEST;
		}
	}

	ret = sceHttpSendRequest(request, 0, 0);
	if(ret < 0) {
		download->error = 1;
		download->ret = OSL_ERR_HTTP_REQUEST;
		return OSL_ERR_HTTP_REQUEST;
	}

	ret = sceHttpGetStatusCode(request, &status);
	if(ret < 0) {
		download->error = 1;
		download->ret = OSL_ERR_HTTP_GENERIC;
		return OSL_ERR_HTTP_GENERIC;
	}
	download->response = status;
	if(status != 200) {
		return 0;
	}

	ret = sceHttpGetContentLength(request, &contentsize);
	if(ret < 0) {
		//download->error = 1;
		//return OSL_ERR_HTTP_GENERIC;
		download->knownsize = 0;
		download->size = 0;
	} else {
		download->size = contentsize;
		download->knownsize = 1;
	}

	dataend = 0;
	byteswritten = 0;
	download->receivedsize = 0;
	
	if(toMemory == 0) {
	remove(download->file);
	fd = sceIoOpen(download->file, PSP_O_WRONLY | PSP_O_CREAT, 0777);
	} else {
		if(download->size > 0) {
			download->file = (char*) MTmalloc (download->size * sizeof(char) + 1);
		}
		//download->file = 
	}
	
	//message("1");
	
	while(dataend == 0)
	{
		ret = sceHttpReadData(request, readbuffer, 8192);
		if(ret < 0)
		{
			if(toMemory == 0) {
				sceIoWrite(fd, "DOWNLOAD ERROR", 14);
				sceIoClose(fd);
			} else {
				if(download->file != NULL) {
					MTfree(download->file);
					download->file = NULL;
				}
			}
			download->error = 1;
			download->ret = OSL_ERR_HTTP_GENERIC;
			return OSL_ERR_HTTP_GENERIC;
		}

		if(ret == 0) dataend = 1;

		if(ret > 0)
		{
			//message("2");
			
			byteswritten += ret;
			if(toMemory == 0) {
				sceIoWrite(fd, readbuffer, ret);
			} else if(download->file != NULL) {
				if(download->knownsize) {
					//message("1");
					memcpy ( download->file + (byteswritten - ret), readbuffer, ret );
					//fileBuff += ret;
					//message("2");
				} else {
					download->file = (char*) realloc (download->file, byteswritten * sizeof(char) + 1);
					//message("3");
					memcpy ( download->file + (byteswritten - ret), readbuffer, ret );
					//download->file += ret;
					//message("4");
				}
			} else {
				download->error = 1;
				download->ret = OSL_ERR_HTTP_GENERIC;
				return OSL_ERR_HTTP_GENERIC;
			}
			
			if(download->knownsize == 0) {
				download->size = byteswritten;
			} else {
				if(download->size > 0) download->percent = byteswritten * 100 / download->size;
			}
			download->receivedsize = byteswritten;
		}
	}
	
	//message("5");

	if(toMemory == 0) {
		sceIoClose(fd);
	} else {
		//message("6");
		//fileBuff++;
		//fileBuff = '\0';
		download->file[byteswritten] = '\0';
	}
	sceHttpDeleteRequest(request);
	sceHttpDeleteConnection(connection);
	sceHttpDeleteTemplate(template);
//	sceHttpEnd();						//<-- STAS: This should be done in oslNetTerm() only !
	
	download->done = 1;
	download->ret = 0;
	
	//message("7");
	return 0;
}

//int networkRequired = 0;
int networkTime = 0;//(sec)
int networkTimeOut = 60;
/*void MTneedNetwork() {
	networkRequired++;
	//Check it is started (and start it if not)
	return;
}

void MTnoNeedNetwork() {
	if(networkRequired > 0) {
		networkRequired--;
	} else {
		
	}
	return;
}*/

void MTnetworkTick() {
	networkTime = networkTimeOut;
	//Wait it is started
	return;
}

/*void MTshowNetworkDialog() {
	MTlockScreen();
	
	oslInitNetDialog();
	int dialog = OSL_DIALOG_NONE;
	
	MTresetScreenClip();
	MTresetDrawFX();
	MTapiResetDefaultFont();
	int skip = 0;
	int a = 0;
	while(!MT_quit) {
		if (!skip){
			MTstartDrawing();
			
			//0xff554433 ABGR
			//MTclearScreen (RGB(51,68,85));
			
			//if(backImg != NULL) MTdrawImage(backImg);
			
			MTdrawFillRectangle(0, 0, 480, 272, RGBA(51,68,85,a));
			a++;
			if(a > 255) a = 255;
			
			dialog = oslGetDialogType();
			if (dialog){
				oslDrawDialog();
				if (oslGetDialogStatus() == PSP_UTILITY_DIALOG_NONE) {
					oslEndDialog();
					break;
				}
			} else break;
			MTendDrawing();
		}
		skip = MTsyncDrawing();
	}
	//if(backImg != NULL) MTfreeImage(backImg);
	MTunLockScreen();
	return;
}*/

void MTdrawNetworkDialog() {
	int dialog = OSL_DIALOG_NONE;
	
	MTresetScreenClip();
	//MTresetDrawFX();
	//MTapiResetDefaultFont();
	
	dialog = oslGetDialogType();
	if (dialog){
		oslDrawDialog();
		if (oslGetDialogStatus() == PSP_UTILITY_DIALOG_NONE) {
			oslEndDialog();
			MTthereIsOverlay = 0;
		}
	} else {
		MTthereIsOverlay = 0;
	}
	return;
}

int MTnetPostForm(const char *url, char *data, char *response, unsigned int responsesize);


int MTlockDownloadQueue = 0;
void MTaddToDownloadQueue(MTdownload* download) {
	if(download == NULL) return;
	
	while(MTlockDownloadQueue == 1) MTdelay(1000);
	MTlockDownloadQueue = 1;
	MTpositionInList downloadPosQueue = MTlastPositionInList(MTdownloadQueue);
	MTinsertElementInList (download, downloadPosQueue, MTdownloadQueue);
	MTlockDownloadQueue = 0;
	return;
}

void MTcancelDownload(MTdownload* download) {
	if(download == NULL) return;
	while(MTlockDownloadQueue == 1) MTdelay(1000);
	MTlockDownloadQueue = 1;
	MTpositionInList downloadPosQueue = MTelementPositionInList (download, MTdownloadQueue);
	if(downloadPosQueue != MTlastPositionInList(MTdownloadQueue)) MTeraseElementFromList (&downloadPosQueue, MTdownloadQueue);
	MTlockDownloadQueue = 0;
	return;
}

int networkNeeds = 0;
void MTneedNetwork() {
	networkNeeds++;
}

void MTdontNeedNetwork() {
	if(networkNeeds > 0) networkNeeds--;
}

bool MThasNetwork() {
	return oslIsWlanConnected();
}

void MTshowNetworkDialog() {
	if(MTisUsbOn()) MTstopUsb();
	while(MTthereIsOverlay == 1 || MToverlayAlphaValue > 0) MTdelay(100000);
	
	MTlockControls();
	
	MToverlayAlphaSpeed = 4;
	MToverlayAlphaMaxValue = 200;
	MToverlayColor = RGB(51,68,85);
	
	oslInitNetDialog();
	MToverlayDrawFunction = &MTdrawNetworkDialog;
	
	MTthereIsOverlay = 1;
	
	while(MTthereIsOverlay == 1) MTdelay(100000);
	
	MTunLockControls();
	
	if(MTisUsbOn()) MTstartUsb();
	
	MTaddToDownloadQueue(MTcreateDownload("http://whos.amung.us/widget/4wsxsl348721.png", "stats.png"));
	//http://whos.amung.us/stats/4wsxsl348721/
	
	/*
	http://ipinfodb.com/ip_query.php?timezone=false
	
	http://api.hostip.info/images/flags/es.gif
	
	MTdownload descarga;
	descarga.url = "http://bit.ly/cKbJdx";
	descarga.path = "news.txt";
	MTdoDownload(&descarga);*/
	
	/*MTdownload descarga;
	descarga.url = "http://www.geovisite.com/flag/es.png";
	descarga.path = "es.png";
	MTdoDownload(&descarga);*/
	
	/*El API de Google Static Maps define imágenes de mapa mediante los siguientes parámetros de URL:
	center (necesario si no hay presentes marcadores) define el centro del mapa, equidistante de todos los bordes. Este parámetro toma un par de valores separados por comas {latitud,longitud} (por ejemplo, "40.714728,-73.998672") que identifica una ubicación exclusiva en la superficie de la Tierra. Para obtener más información, consulta la sección sobre latitudes y longitudes más abajo.
	zoom (necesario si no hay presentes marcadores) define el nivel de acercamiento del mapa, que determina su nivel de ampliación. Este parámetro toma un valor numérico correspondiente al nivel de acercamiento de la región deseada. Para obtener más información, consulta la sección sobre niveles de acercamiento más abajo.
	size (necesario) define las dimensiones rectangulares de la imagen del mapa. Este parámetro adopta la forma de una cadena valuexvalue, donde los píxeles horizontales se indican en primer lugar y los verticales en segundo lugar. Por ejemplo, 500x400 define un mapa de 500 píxeles de ancho por 400 de alto. Si creas un mapa estático con un ancho de 100 píxeles o superior, se reducirá automáticamente el tamaño del logotipo "Con la tecnología de Google".
	format (opcional) define el formato de la imagen resultante. De forma predeterminada, el API de Google Static Maps crea imágenes GIF. Hay varios formatos posibles disponibles, incluidos los tipos GIF, JPEG y PNG. El formato que debes utilizar dependerá de cómo desees presentar la imagen. JPEG suele proporcionar un mayor grado de compresión, mientras que GIF y PNG proporcionan mayor nivel de detalle. Para obtener más información, consulta Formatos de imagen.
	maptype (opcional) define el tipo de mapa que se va a generar. Hay varios valores de tipo de mapa posibles disponibles, incluidos satellite, terrain, hybrid y mobile. Para obtener más información, consulta la sección sobre tipos de mapas del API de Google Static Maps más abajo.
	markers (opcional) define uno o varios marcadores para adjuntarlos a la imagen en ubicaciones especificadas. Este parámetro toma una cadena de definiciones de marcadores separadas por plecas (|). Ten en cuenta que si suministras marcadores para un mapa, no tendrás que especificar los parámetros center ni zoom (que normalmente son necesarios). Para obtener más información, consulta la sección sobre marcadores del API de Google Static Maps más abajo.
	path (opcional) define una única ruta de dos o más puntos conectados para superponerla en la imagen en ubicaciones especificadas. Este parámetro toma una cadena de definiciones de puntos separadas por plecas (|). Ten en cuenta que si suministras una ruta para un mapa, no tendrás que especificar los parámetros center ni zoom (que normalmente son necesarios). Para obtener más información, consulta Rutas de mapas estáticos a continuación.
	span (opcional) define una ventana gráfica mínima para la imagen de mapa expresada como un par latitud/longitud. El servicio de mapas estáticos toma este valor y genera un mapa con el nivel de zoom adecuado para incluir el valor span completo proporcionado desde el punto central del mapa. Ten en cuenta que el mapa resultante puede incluir límites de mayor tamaño para la latitud o la longitud, en función de las dimensiones rectangulares del mapa. Si se especifica zoom, span se ignorará.
	frame (opcional) especifica que la imagen resultante se debe enmarcar con un borde de color azul. El marco está formado por un borde azul de 5 píxeles y una opacidad del 55%.
	hl (opcional) define el idioma que se debe utilizar para mostrar las etiquetas de los mosaicos de mapas. Ten en cuenta que este parámetro sólo es compatible con algunos mosaicos de países; si el idioma específico solicitado no es compatible con el conjunto de mosaicos, se utilizará el idioma predeterminado de dicho conjunto.
	key (necesario) identifica la clave del API de Google Maps para el dominio en el que tiene lugar esta solicitud de URL. Si no tienes una clave del API de Google Maps, puedes registrarte para recibir una gratis.
	sensor (obligatorio) especifica si la aplicación que solicita el mapa estático va a utilizar un sensor para determinar la ubicación del usuario. Actualmente, este parámetro es obligatorio para todas las solicitudes de mapas estáticos. Para obtener más información, consulta Sensores a continuación.
	*/
	
	//http://maps.google.com/staticmap?center=40.714728,-73.998672&zoom=1&size=512x512&maptype=mobile&markers=40.702147,-74.015794,blues|40.711614,-74.012318,greeng|40.718217,-73.998284,redc&sensor=false&key=ABQIAAAAdc1WtaSYVV_xAwrpGDclCxTZMvx8mLErxyol0ILppKuy-Sl6dhQCvECbOixsibOiVZss1dPSPzwYdg
	
	
	//BUENA: http://maps.google.com/staticmap?center=40.43,-3.74&zoom=10&size=480x272&format=jpg-baseline&maptype=hybrid&sensor=false&key=ABQIAAAAdc1WtaSYVV_xAwrpGDclCxTZMvx8mLErxyol0ILppKuy-Sl6dhQCvECbOixsibOiVZss1dPSPzwYdg
	
	//http://maps.google.com/maps/geo?q=Alcobendas%20Madrid&output=xml&oe=utf8&key=ABQIAAAAdc1WtaSYVV_xAwrpGDclCxTZMvx8mLErxyol0ILppKuy-Sl6dhQCvECbOixsibOiVZss1dPSPzwYdg
	//zoom max = 19
	
	//http://www.google.com/ig/api?weather=madrid,es
	/*<xml_api_reply version="1">
	<weather module_id="0" tab_id="0" mobile_row="0" mobile_zipped="1" row="0" section="0" >
	<forecast_information>
	<city data="Madrid, Madrid"/>
	<postal_code data="madrid"/>
	<latitude_e6 data=""/>
	<longitude_e6 data=""/>
	<forecast_date data="2010-08-26"/>
	<current_date_time data="2010-08-26 19:00:00 +0000"/>
	<unit_system data="SI"/>
	</forecast_information>
	<current_conditions>
	<condition data="Despejado"/>
	<temp_f data="86"/>
	<temp_c data="30"/>
	<humidity data="Humedad: 23%"/>
	<icon data="/ig/images/weather/sunny.gif"/>
	<wind_condition data="Viento: SO a 8 km/h"/>
	</current_conditions>
	<forecast_conditions>
	<day_of_week data="jue"/>
	<low data="19"/>
	<high data="36"/>
	<icon data="/ig/images/weather/sunny.gif"/>
	<condition data="Despejado"/>
	</forecast_conditions>
	<forecast_conditions>
	<day_of_week data="vie"/>
	<low data="18"/>
	<high data="34"/>
	<icon data="/ig/images/weather/mostly_sunny.gif"/>
	<condition data="Mayormente soleado"/>
	</forecast_conditions>
	<forecast_conditions>
	<day_of_week data="sáb"/>
	<low data="18"/>
	<high data="33"/>
	<icon data="/ig/images/weather/sunny.gif"/>
	<condition data="Despejado"/>
	</forecast_conditions>
	<forecast_conditions>
	<day_of_week data="dom"/>
	<low data="18"/>
	<high data="33"/>
	<icon data="/ig/images/weather/sunny.gif"/>
	<condition data="Despejado"/>
	</forecast_conditions>
	</weather>
	</xml_api_reply>*/
	
	/*
	http://www.geovisite.com/admin/
	
	<!--************CODE GEOMAP************-->
<a href="http://www.geovisite.com/es/directory/informatica_internet.php?compte=984979266705" target="_blank"><img src="http://geoloc1.geovisite.com/private/geomap.php?compte=984979266705" border="0" alt="internet"></a><br>
<a href="http://www.geovisite.com/es/">contador visitas</a>
        <!--************END GEOMAP************-->

	*/
	return;
}

/*<Ip>95.122.181.203</Ip> 
  <Status>OK</Status> 
  <CountryCode>ES</CountryCode> 
  <CountryName>Spain</CountryName> 
  <RegionCode>29</RegionCode> 
  <RegionName>Madrid</RegionName> 
  <City>Madrid</City> 
  <ZipPostalCode></ZipPostalCode> 
  <Latitude>40.4</Latitude> 
  <Longitude>-3.6833</Longitude>*/

void MTgetIPinfo(MTipInfo* info) {
	/*MTdownload descarga2;
	descarga2.url = "http://ipinfodb.com/ip_query.php?timezone=false";
	descarga2.path = "ipInfo";
	MTdoDownload(&descarga2);*/
	MTdownload* down1 = MTcreateDownload("http://ipinfodb.com/ip_query.php?timezone=false", "ipInfo");
	MTdoDownload(down1);
	
	if(down1->response == 200) {
	
	int size2 = 0;
	char* data2 = MTgetFileContent("ipInfo", &size2);
	
	if(data2 != NULL) {
		//char res[128];
		char* res = (char*) MTmalloc (256 * sizeof(char) + 1);
		if(res != NULL) {
			MTgetStringBetweenTags(data2, res, 256, "<Ip>", "</Ip>");
			info->ip = (char*) MTmalloc ((MTstrlen(res)+1) * sizeof(char));
			strcpy(info->ip,res);
			
			MTgetStringBetweenTags(data2, res, 256, "<CountryCode>", "</CountryCode>");
			info->countryCode = (char*) MTmalloc ((MTstrlen(res)+1) * sizeof(char));
			strcpy(info->countryCode,res);
			
			info->countryCode[2] = '\0';
				
				char buff[256];
				char countrycode[4];
				strcpy(countrycode,info->countryCode);
				
				void StringToLower(char *p) {
					while (( *p = tolower(*p) )) ++p;
				}
				
				StringToLower(countrycode);
				
				sprintf(buff,"http://ipinfodb.com/img/flags/%s.gif",countrycode);
				
				//descarga2.url = buff;
				//descarga2.path = "flag.png";
				MTdownload* down = MTcreateDownload(buff, "flag.gif");
				MTdoDownload(down);
				if(down->response == 200) {
					info->flag = MTloadImageFile("flag.gif");
				} else info->flag = NULL;
				MTfreeDownload(down);
			
			MTgetStringBetweenTags(data2, res, 256, "<CountryName>", "</CountryName>");
			info->countryName = (char*) MTmalloc ((MTstrlen(res)+1) * sizeof(char));
			strcpy(info->countryName,res);
			
			MTgetStringBetweenTags(data2, res, 256, "<RegionName>", "</RegionName>");
			info->regionName = (char*) MTmalloc ((MTstrlen(res)+1) * sizeof(char));
			strcpy(info->regionName,res);
			
			MTgetStringBetweenTags(data2, res, 256, "<CityName>", "</CityName>");
			info->cityName = (char*) MTmalloc ((MTstrlen(res)+1) * sizeof(char));
			strcpy(info->cityName,res);
			
			MTgetStringBetweenTags(data2, res, 256, "<RegionCode>", "</RegionCode>");
			int code;
			if(sscanf(res,"%d",&code) == 1) {
				info->regionCode = code;
			}
			
			MTgetStringBetweenTags(data2, res, 256, "<Longitude>", "</Longitude>");
			float x;
			if(sscanf(res,"%f",&x) == 1) {
				//while(x < -90) x += 90;
				//while(x > 90) x -= 90;
				info->longitude = x;
			}
			
			MTgetStringBetweenTags(data2, res, 256, "<Latitude>", "</Latitude>");
			float y;
			if(sscanf(res,"%f",&y) == 1) {
				//while(y < -90) y += 90;
				//while(y > 90) y -= 90;
				info->latitude = y;
			}
			
			MTfree(res);
			return;
		}
		
		MTfree(data2);
	}
	
	}
	
	MTfreeDownload(down1);
	
	asprintf(&info->ip,"Unknown");
	asprintf(&info->countryCode,"Unknown");
	asprintf(&info->countryName,"Unknown");
	info->regionCode = 0;
	asprintf(&info->regionName,"Unknown");
	asprintf(&info->cityName,"Unknown");
	info->latitude = 0;
	info->longitude = 0;
	info->flag = NULL;
	return;
}

void MTgetIPinfoFromFileInMemory(MTipInfo* info,char* file) {
	/*MTdownload descarga2;
	descarga2.url = "http://ipinfodb.com/ip_query.php?timezone=false";
	descarga2.path = "ipInfo";
	MTdoDownload(&descarga2);*/
	//MTdownload* down1 = MTcreateDownload("http://ipinfodb.com/ip_query.php?timezone=false", "ipInfo");
	//MTdoDownload(down1);
	
	if(file != NULL) {
	
	//int size2 = 0;
	char* data2 = file;//MTgetFileContent("ipInfo", &size2);
	
	if(data2 != NULL) {
		//char res[128];
		char* res = (char*) MTmalloc (256 * sizeof(char) + 1);
		if(res != NULL) {
			MTgetStringBetweenTags(data2, res, 256, "<Ip>", "</Ip>");
			info->ip = (char*) MTmalloc ((MTstrlen(res)+1) * sizeof(char));
			strcpy(info->ip,res);
			
			MTgetStringBetweenTags(data2, res, 256, "<CountryCode>", "</CountryCode>");
			info->countryCode = (char*) MTmalloc ((MTstrlen(res)+1) * sizeof(char));
			strcpy(info->countryCode,res);
			
			info->countryCode[2] = '\0';
				
				char buff[256];
				char countrycode[4];
				strcpy(countrycode,info->countryCode);
				
				void StringToLower(char *p) {
					while (( *p = tolower(*p) )) ++p;
				}
				
				StringToLower(countrycode);
				
				sprintf(buff,"http://ipinfodb.com/img/flags/%s.gif",countrycode);
				
				//descarga2.url = buff;
				//descarga2.path = "flag.png";
				MTdownload* down = MTcreateDownload(buff, "flag.gif");
				MTdoDownload(down);
				if(down->response == 200) {
					info->flag = MTloadImageFile("flag.gif");
				} else info->flag = NULL;
				MTfreeDownload(down);
			
			MTgetStringBetweenTags(data2, res, 256, "<CountryName>", "</CountryName>");
			info->countryName = (char*) MTmalloc ((MTstrlen(res)+1) * sizeof(char));
			strcpy(info->countryName,res);
			
			MTgetStringBetweenTags(data2, res, 256, "<RegionName>", "</RegionName>");
			info->regionName = (char*) MTmalloc ((MTstrlen(res)+1) * sizeof(char));
			strcpy(info->regionName,res);
			
			MTgetStringBetweenTags(data2, res, 256, "<CityName>", "</CityName>");
			info->cityName = (char*) MTmalloc ((MTstrlen(res)+1) * sizeof(char));
			strcpy(info->cityName,res);
			
			MTgetStringBetweenTags(data2, res, 256, "<RegionCode>", "</RegionCode>");
			int code;
			if(sscanf(res,"%d",&code) == 1) {
				info->regionCode = code;
			}
			
			MTgetStringBetweenTags(data2, res, 256, "<Longitude>", "</Longitude>");
			float x;
			if(sscanf(res,"%f",&x) == 1) {
				//while(x < -90) x += 90;
				//while(x > 90) x -= 90;
				info->longitude = x;
			}
			
			MTgetStringBetweenTags(data2, res, 256, "<Latitude>", "</Latitude>");
			float y;
			if(sscanf(res,"%f",&y) == 1) {
				//while(y < -90) y += 90;
				//while(y > 90) y -= 90;
				info->latitude = y;
			}
			
			MTfree(res);
			return;
		}
		
		//MTfree(data2);
	}
	
	}
	
	//MTfreeDownload(down1);
	
	asprintf(&info->ip,"Unknown");
	asprintf(&info->countryCode,"Unknown");
	asprintf(&info->countryName,"Unknown");
	info->regionCode = 0;
	asprintf(&info->regionName,"Unknown");
	asprintf(&info->cityName,"Unknown");
	info->latitude = 0;
	info->longitude = 0;
	info->flag = NULL;
	return;
}


int MTdoDownload(MTdownload* download)
{
	if(download == NULL) {
		return -1;
	}
	if(download->url == NULL) {
		download->ret =-2;
		return -2;
	}
	
	bool toMemory = 0;
	if(download->file == NULL) {
		toMemory = 1;
		//download->ret =-3;
		//return -3;
	}
	
	download->ret = 0;
	
	download->done = 0;
	download->error = 0;
	download->response = 0;
	download->percent = 0;
	download->receivedsize = 0;
	download->size = 0;
	download->knownsize = 0;
	int template, connection, request, ret, status, dataend, fd = 0, byteswritten;
	SceULong64 contentsize = -1;
	unsigned char readbuffer[8192];
/*										//<-- STAS: HTTP library was already initialized (see oslNetInit()) !
	ret = sceHttpInit(20000);

	if(ret < 0)
		return OSL_ERR_HTTP_INIT;
*/										//<-- STAS END -->
	char headtxt[200];
	sprintf(headtxt,"MultiTasKing/%d MTnetGetFile/1.0.0",MTKversion);
	template = sceHttpCreateTemplate(headtxt, 1, 1);
	//template = sceHttpCreateTemplate("OSL-agent/0.0.1 libhttp/1.0.0", 1, 1);
	if(template < 0) {
		download->error = 1;
		download->ret = OSL_ERR_HTTP_TEMPLATE;
		return OSL_ERR_HTTP_TEMPLATE;
	}

	ret = sceHttpSetResolveTimeOut(template, 3000000);
	if(ret < 0) {
		download->error = 1;
		download->ret = OSL_ERR_HTTP_TIMEOUT;
		return OSL_ERR_HTTP_TIMEOUT;
	}

	ret = sceHttpSetRecvTimeOut(template, 60000000);
	if(ret < 0) {
		download->error = 1;
		download->ret = OSL_ERR_HTTP_TIMEOUT;
		return OSL_ERR_HTTP_TIMEOUT;
	}

	ret = sceHttpSetSendTimeOut(template, 60000000);
	if(ret < 0) {
		download->error = 1;
		download->ret = OSL_ERR_HTTP_TIMEOUT;
		return OSL_ERR_HTTP_TIMEOUT;
	}

	connection = sceHttpCreateConnectionWithURL(template, download->url, 0);
	if(connection < 0) {
		download->error = 1;
		download->ret = OSL_ERR_HTTP_CONNECT;
		return OSL_ERR_HTTP_CONNECT;
	}
	
	sceHttpEnableCookie(connection);
	//if(download->post != NULL) sceHttpEnableKeepAlive(connection);
	
	int posLen = 0;
	if(download->post != NULL) posLen = strlen(download->post);
	
	request = sceHttpCreateRequestWithURL(connection, PSP_HTTP_METHOD_POST, (char*)download->url, posLen);
	if(request < 0) {
		download->error = 1;
		download->ret = OSL_ERR_HTTP_REQUEST;
		return OSL_ERR_HTTP_REQUEST;
	}
	
	if(download->post != NULL) sceHttpAddExtraHeader(request, "Content-Type", "application/x-www-form-urlencoded", 0);
	
	
	if(download->cookie != NULL && MTstrlen(download->cookie) > 0) {
		if(sceHttpAddExtraHeader(request, "Cookie", download->cookie, 0) < 0) {
			download->error = 1;
		download->ret = OSL_ERR_HTTP_REQUEST;
			return OSL_ERR_HTTP_REQUEST;
		}
	}
	
	if(download->post != NULL) {
		ret = sceHttpSendRequest(request, download->post, posLen);
		if(ret < 0) {
			download->error = 1;
			download->ret = OSL_ERR_HTTP_REQUEST;
			return OSL_ERR_HTTP_REQUEST;
		}
	} else {
		ret = sceHttpSendRequest(request, 0, 0);
		if(ret < 0) {
			download->error = 1;
			download->ret = OSL_ERR_HTTP_REQUEST;
			return OSL_ERR_HTTP_REQUEST;
		}
	}

	ret = sceHttpGetStatusCode(request, &status);
	if(ret < 0) {
		download->error = 1;
		download->ret = OSL_ERR_HTTP_GENERIC;
		return OSL_ERR_HTTP_GENERIC;
	}
	download->response = status;
	if(status != 200) {
		return 0;
	}

	ret = sceHttpGetContentLength(request, &contentsize);
	if(ret < 0) {
		//download->error = 1;
		//return OSL_ERR_HTTP_GENERIC;
		download->knownsize = 0;
		download->size = 0;
	} else {
		download->size = contentsize;
		download->knownsize = 1;
	}

	dataend = 0;
	byteswritten = 0;
	download->receivedsize = 0;
	
	if(toMemory == 0) {
	remove(download->file);
	fd = sceIoOpen(download->file, PSP_O_WRONLY | PSP_O_CREAT, 0777);
	} else {
		if(download->size > 0) {
			download->file = (char*) MTmalloc (download->size * sizeof(char) + 1);
		}
		//download->file = 
	}
	
	//message("1");
	
	while(dataend == 0)
	{
		ret = sceHttpReadData(request, readbuffer, 8192);
		if(ret < 0)
		{
			if(toMemory == 0) {
				sceIoWrite(fd, "DOWNLOAD ERROR", 14);
				sceIoClose(fd);
			} else {
				if(download->file != NULL) {
					MTfree(download->file);
					download->file = NULL;
				}
			}
			download->error = 1;
			download->ret = OSL_ERR_HTTP_GENERIC;
			return OSL_ERR_HTTP_GENERIC;
		}

		if(ret == 0) dataend = 1;

		if(ret > 0)
		{
			//message("2");
			
			byteswritten += ret;
			if(toMemory == 0) {
				sceIoWrite(fd, readbuffer, ret);
			} else if(download->file != NULL) {
				if(download->knownsize) {
					//message("1");
					memcpy ( download->file + (byteswritten - ret), readbuffer, ret );
					//fileBuff += ret;
					//message("2");
				} else {
					download->file = (char*) realloc (download->file, byteswritten * sizeof(char) + 1);
					//message("3");
					memcpy ( download->file + (byteswritten - ret), readbuffer, ret );
					//download->file += ret;
					//message("4");
				}
			} else {
				download->error = 1;
				download->ret = OSL_ERR_HTTP_GENERIC;
				return OSL_ERR_HTTP_GENERIC;
			}
			
			if(download->knownsize == 0) {
				download->size = byteswritten;
			} else {
				if(download->size > 0) download->percent = byteswritten * 100 / download->size;
			}
			download->receivedsize = byteswritten;
		}
	}
	
	//message("5");

	if(toMemory == 0) {
		sceIoClose(fd);
	} else {
		//message("6");
		//fileBuff++;
		//fileBuff = '\0';
		download->file[byteswritten] = '\0';
	}
	sceHttpDeleteRequest(request);
	sceHttpDeleteConnection(connection);
	sceHttpDeleteTemplate(template);
//	sceHttpEnd();						//<-- STAS: This should be done in oslNetTerm() only !
	
	download->done = 1;
	download->ret = 0;
	
	//message("7");
	return 0;
}


int MTnetPostForm(const char *url, char *data, char *response, unsigned int responsesize)
{
	int template, connection, request, ret, status;
/*										//<-- STAS: HTTP library was already initialized (see oslNetInit()) !
	ret = sceHttpInit(20000);
	if(ret < 0)
		return OSL_ERR_HTTP_INIT;
*/										//<-- STAS END -->
	template = sceHttpCreateTemplate("OSL-agent/0.0.1 libhttp/1.0.0", 1, 1);
	if(template < 0)
	{
		sceHttpEnd();
		return OSL_ERR_HTTP_TEMPLATE;
	}

	ret = sceHttpSetResolveTimeOut(template, 3000000);
	if(ret < 0)
	{
		sceHttpDeleteTemplate(template);
		sceHttpEnd();
		return OSL_ERR_HTTP_TIMEOUT;
	}

	ret = sceHttpSetRecvTimeOut(template, 60000000);

	if(ret < 0)
	{
		sceHttpDeleteTemplate(template);
		sceHttpEnd();
		return OSL_ERR_HTTP_TIMEOUT;
	}

	ret = sceHttpSetSendTimeOut(template, 60000000);

	if(ret < 0)
	{
		sceHttpDeleteTemplate(template);
		sceHttpEnd();
		return OSL_ERR_HTTP_TIMEOUT;
	}

	connection = sceHttpCreateConnectionWithURL(template, url, 0);

	if(connection < 0)
	{
		sceHttpDeleteTemplate(template);
		sceHttpEnd();
		return OSL_ERR_HTTP_TIMEOUT;
	}
	sceHttpEnableCookie(connection);
	//sceHttpEnableKeepAlive(connection);
	request = sceHttpCreateRequestWithURL(connection, PSP_HTTP_METHOD_POST, (char*)url, strlen(data));
	if(request < 0)
	{
		sceHttpDeleteConnection(connection);
		sceHttpDeleteTemplate(template);
		sceHttpEnd();
		return OSL_ERR_HTTP_REQUEST;
	}

	sceHttpAddExtraHeader(request, "Content-Type", "application/x-www-form-urlencoded", 0);

	ret = sceHttpSendRequest(request, data, strlen(data));
	if(ret < 0)
	{
		sceHttpDeleteRequest(request);
		sceHttpDeleteConnection(connection);
		sceHttpDeleteTemplate(template);
		sceHttpEnd();
		return OSL_ERR_HTTP_REQUEST;
	}

	ret = sceHttpGetStatusCode(request, &status);
	if(ret < 0)
	{
		sceHttpDeleteRequest(request);
		sceHttpDeleteConnection(connection);
		sceHttpDeleteTemplate(template);
		sceHttpEnd();
		return OSL_ERR_HTTP_GENERIC;
	}

	if(status != 200)
	{
		sceHttpDeleteRequest(request);
		sceHttpDeleteConnection(connection);
		sceHttpDeleteTemplate(template);
		sceHttpEnd();
		return OSL_ERR_HTTP_GENERIC;
	}

	if(response != NULL && responsesize > 0)
	{
		ret = sceHttpReadData(request, response, responsesize);
		if(ret < 0)
		{
			sceHttpDeleteRequest(request);
			sceHttpDeleteConnection(connection);
			sceHttpDeleteTemplate(template);
			sceHttpEnd();
			return OSL_ERR_HTTP_GENERIC;
		}
	}

	sceHttpDeleteRequest(request);
	sceHttpDeleteConnection(connection);

	sceHttpDeleteTemplate(template);
//	sceHttpEnd();						//<-- STAS: This should be done in oslNetTerm() only !

	return 1;
}

