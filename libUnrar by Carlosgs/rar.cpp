#include "rar.hpp"

#include <pspkernel.h>
#include <pspthreadman.h>
#include <pspdebug.h>
#include <psppower.h>
#include <pspiofilemgr.h>
#include <pspgu.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <time.h> 
#include <malloc.h>

/*PSP_MODULE_INFO("unRar", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-5*1024);*/

#if !defined(GUI) && !defined(RARDLL)
int mainRAR(int argc, char *argv[])
{

	//pspDebugScreenPrintf("extractingRar");
	//pspDebugScreenPrintf(" 1 ");

#ifdef _UNIX
  setlocale(LC_ALL,"");
#endif

#if defined(_EMX) && !defined(_DJGPP)
  uni_init(0);
#endif

#if !defined(_SFX_RTL_) && !defined(_WIN_32)
  setbuf(stdout,NULL);
#endif

//pspDebugScreenPrintf(" 2 ");

#if !defined(SFX_MODULE) && defined(_EMX)
  EnumConfigPaths(argv[0],-1);
#endif

  ErrHandler.SetSignalHandlers(true);

  RARInitData();

#ifdef SFX_MODULE
  char ModuleName[NM];
#ifdef _WIN_32
  GetModuleFileName(NULL,ModuleName,sizeof(ModuleName));
#else
  strcpy(ModuleName,argv[0]);
#endif
#endif

#ifdef _WIN_32
  SetErrorMode(SEM_NOALIGNMENTFAULTEXCEPT|SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);


#endif

//pspDebugScreenPrintf(" 3 ");

#if defined(_WIN_32) && !defined(SFX_MODULE) && !defined(SHELL_EXT)
  bool ShutdownOnClose;
#endif

#ifdef ALLOW_EXCEPTIONS
  try 
#endif
  {
  
    CommandData Cmd;
#ifdef SFX_MODULE
    strcpy(Cmd.Command,"X");
    char *Switch=NULL;
    //pspDebugScreenPrintf(" 3.5 ");
#ifdef _SFX_RTL_
    char *CmdLine=GetCommandLine();
    
    //pspDebugScreenPrintf(" 4 ");
    
    if (CmdLine!=NULL && *CmdLine=='\"')
      CmdLine=strchr(CmdLine+1,'\"');
    if (CmdLine!=NULL && (CmdLine=strpbrk(CmdLine," /"))!=NULL)
    {
      while (IsSpace(*CmdLine))
        CmdLine++;
      Switch=CmdLine;
    }
#else
    Switch=argc>1 ? argv[1]:NULL;
    
    //pspDebugScreenPrintf(" 5 ");
#endif
    if (Switch!=NULL && Cmd.IsSwitch(Switch[0]))
    {
      int UpperCmd=etoupper(Switch[1]);
      switch(UpperCmd)
      {
        case 'T':
        case 'V':
          Cmd.Command[0]=UpperCmd;
          break;
        case '?':
          Cmd.OutHelp();
          break;
      }
    }
    Cmd.AddArcName(ModuleName,NULL);
    
    //pspDebugScreenPrintf(" 6 ");
#else
    if (Cmd.IsConfigEnabled(argc,argv))
    {
      Cmd.ReadConfig(argc,argv);
      //pspDebugScreenPrintf(" 7 ");
      Cmd.ParseEnvVar();
    }
    for (int I=1;I<argc;I++)
      Cmd.ParseArg(argv[I],NULL);
#endif
    Cmd.ParseDone();
    
    //pspDebugScreenPrintf(" 8 ");

#if defined(_WIN_32) && !defined(SFX_MODULE) && !defined(SHELL_EXT)
    ShutdownOnClose=Cmd.Shutdown;
#endif
//pspDebugScreenPrintf(" 8.1 ");
    InitConsoleOptions(Cmd.MsgStream,Cmd.Sound);
    //pspDebugScreenPrintf(" 8.2 ");
    InitLogOptions(Cmd.LogName);
    //pspDebugScreenPrintf(" 8.3 ");
    ErrHandler.SetSilent(Cmd.AllYes || Cmd.MsgStream==MSG_NULL);
    //pspDebugScreenPrintf(" 8.4 ");
    ErrHandler.SetShutdown(Cmd.Shutdown);
//pspDebugScreenPrintf(" 8.5 ");
    Cmd.OutTitle();
    //pspDebugScreenPrintf(" 8.6 ");
    Cmd.ProcessCommand();
    
    //pspDebugScreenPrintf(" 9 ");
    //pspDebugScreenPrintf(" END SUCCESS ");
  }
  
#ifdef ALLOW_EXCEPTIONS
  catch (int ErrCode)
  {
    //pspDebugScreenPrintf(" 11 ");
    ErrHandler.SetErrorCode(ErrCode);
  }
#ifdef ENABLE_BAD_ALLOC
  catch (bad_alloc)
  {
    //pspDebugScreenPrintf(" 12 ");
    ErrHandler.SetErrorCode(MEMORY_ERROR);
  }
#endif
  catch (...)
  {
    //pspDebugScreenPrintf(" 13 ");
    ErrHandler.SetErrorCode(FATAL_ERROR);
  }
#endif
  File::RemoveCreated();
#if defined(SFX_MODULE) && defined(_DJGPP)
  _chmod(ModuleName,1,0x20);
#endif
#if defined(_EMX) && !defined(_DJGPP)
  uni_done();
#endif
#if defined(_WIN_32) && !defined(SFX_MODULE) && !defined(SHELL_EXT)
  if (ShutdownOnClose)
    Shutdown();
    
    //pspDebugScreenPrintf(" 14 ");
#endif
  return(ErrHandler.GetErrorCode());
}
#endif


char* extractDir;
char* password;
char* currentString;
int mainRAR(int argc, char *argv[]);



int rarExtract(const char *rarfile,const char *extDir,const char *pass){
	
mkdir(extDir,NULL);
int argc;int i=0;
char **argv = (char **)calloc ( 8, sizeof(char *) );
//argv = (char**)memalign(16,512);
int count;
char ** currentString = argv;
if(pass == NULL){
argc = 5;
count = argc+1;

for ( i = 0; i < count; i++ )
  {
	if(i==0)asprintf ( currentString, "unrar");
	if(i==1)asprintf ( currentString, "x");
	if(i==2)asprintf ( currentString, "%s",rarfile);
	if(i==3)asprintf ( currentString, "%s",extDir);
	if(i==4)asprintf ( currentString, "-y");
	if(i> 4)asprintf ( currentString, " ");
    currentString++;
    //printf("currentString address: %p\n", currentString);  
  }
}else{

argc = 6;
count = argc+1;

for ( i = 0; i < count; i++ )
  {
	if(i==0)asprintf ( currentString, "unrar");
	if(i==1)asprintf ( currentString, "x");
	if(i==2)asprintf ( currentString, "%s",rarfile);
	if(i==3)asprintf ( currentString, "%s",extDir);
	if(i==4)asprintf ( currentString, "-y");
	if(i==5)asprintf ( currentString, "-p%s",pass);
	if(i> 5)asprintf ( currentString, " ");
    currentString++;
    //printf("currentString address: %p\n", currentString);  
  }

}
  // reset memory block to the original address.  
  // In other words, go the beginning of the "array"
  currentString = argv;
  //printf("currentString address after reset: %p\n", currentString);
  

  // display the string at this particular slot.
  // we have to use the star to de-reference
  //for ( i = 0; i < count; i++ )
  //{
  //  printf( "%s\n", *currentString );
  //  currentString++;
  //}

	//argv[0] = "unrar";
	//argv[1] = "x";
	//argv[2] = rarfile;
	//argv[3] = extDir;
	//argv[4] = "-y";
	int retVal = mainRAR(argc,argv);

  // reset
  currentString = argv;


for ( i = 0; i < count; i++ )
  {
    free ( *currentString );
    currentString++;
  }
free(argv);

return retVal;
}

	/*
	Results:
	0: OK
	1: File doesnt exists
	2: File corrupt/Not rar
	3: Wrong password
	*/

/*int main(int argc, char *argv[])
	{
	pspDebugScreenInit();
	//DoExtractRAR("ms0:/unrar/file.rar","ms0:/unrar/file","testPWD");//pass not required, result ok: 0
	DoExtractRAR("ms0:/unrar/file.rar","ms0:/unrar/file",NULL);//ok: 0
	//DoExtractRAR("ms0:/unrar/file1.rar","ms0:/unrar/file",NULL);//doesnt exists: 1
	//DoExtractRAR("ms0:/unrar/file1.rar","ms0:/unrar/file","wtf");//doesnt exists: 1
	DoExtractRAR("ms0:/unrar/file2.rar","ms0:/unrar/file2",NULL);//ok: 0
	DoExtractRAR("ms0:/unrar/file3.rar","ms0:/unrar/file3","heyheyhey");//ok: 0
	//DoExtractRAR("ms0:/unrar/file3.rar","ms0:/unrar/file3",NULL);//wrong pass (and required): 3
	
	//DoExtractRAR("ms0:/unrar/file3_.rar","ms0:/unrar/file3_",NULL);//
	//DoExtractRAR("ms0:/unrar/file3_.rar","ms0:/unrar/file3_","heyheyhey");//
	
	//pspDebugScreenPrintf(" END \n");
	while(1) sceKernelDelayThreadCB(100000);
	sceKernelExitGame();
	return(0);
}*/


