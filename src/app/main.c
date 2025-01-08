#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspsdk.h>
#include <string.h>
#include <stdio.h>
#include "fusainterface.h"

PSP_MODULE_INFO("GAMEPAD", 0, 0, 2);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

#define printf	pspDebugScreenPrintf

int done = 0;
SceCtrlData pad;
GP_Config settings;
char buf[0xFF];

void waitbuttonup(int button)
{
	while ((pad.Buttons & button)) {
	fusaCtrlReadBufferPositive(&pad);
	sceKernelDelayThread(10*1000);
	}
}

#define pspDebugScreenSetX(a) pspDebugScreenSetXY(a,pspDebugScreenGetY());

void display_config();

void display_info(){
	
	pspDebugScreenSetXY(0,0);
	printf(" .: FuSa GAMEPAD :: v0.2 :: coded by Andy_maN :: www.foosa.do.am :.\n");
	pspDebugScreenSetXY(0,32);
	printf(" ': Axises - (X/Y/Z/Rz) (Analog+S) :: Buttons - 12 :: rd.ver 0.5 :'\n");
}

void user_wanna_exit(){
	
	pspDebugScreenSetXY(22,30);
	printf("(X) EXIT | (O) CANCEL");
	
	display_config();
	
	int loop = 1;
	
	while (loop) {
	fusaCtrlReadBufferPositive(&pad);
	
	if (pad.Buttons & PSP_CTRL_CROSS) {
		waitbuttonup(PSP_CTRL_CROSS);
		done = 1;
		loop = 0;
		pspDebugScreenSetXY(22,30);
		printf("     Exiting...      ");
		};
		
	if (pad.Buttons & (PSP_CTRL_CIRCLE | PSP_CTRL_HOME)) {
	waitbuttonup((PSP_CTRL_CIRCLE | PSP_CTRL_HOME));
	loop = 0;
	pspDebugScreenSetXY(22,2);
	pspDebugScreenClear();
	display_info();
	}
	
	sceKernelDelayThread(10*1000);
	}
	

}

#define str_22 " (SELECT)"
#define str_23 " (START)"
#define str_24 " UP"
#define str_25 " LEFT"
#define str_26 " DOWN"
#define str_27 " RIGHT"
#define str_28 " /L|"
#define str_29 " |R\\"
#define str_30 " (/\\)"
#define str_31 " (O)"
#define str_32 " (X)"
#define str_33 " ([])"
#define str_34 " (HOME)"
#define str_35 " |HOLD|"
#define str_36 " (\x0d)"
#define str_37 " ( )"
#define str_38 " (+)"
#define str_39 " (-)"
#define str_40 " |WLAN|"

char * button2str(int key)
{
//key &= ~(PSP_CTRL_WLAN_UP|PSP_CTRL_REMOTE|PSP_CTRL_DISC|PSP_CTRL_MS);
if (key){
int offset = 0;
#define str "%s"
#define add_to_str(a) offset += sprintf(&buf[offset],str,a);

	if (key & PSP_CTRL_SELECT)    add_to_str(str_22);
	if (key & PSP_CTRL_START)     add_to_str(str_23);
	if (key & PSP_CTRL_UP)        add_to_str(str_24);
	if (key & PSP_CTRL_RIGHT)     add_to_str(str_25);
	if (key & PSP_CTRL_DOWN)      add_to_str(str_26);
	if (key & PSP_CTRL_LEFT)      add_to_str(str_27);
	if (key & PSP_CTRL_LTRIGGER)  add_to_str(str_28);
	if (key & PSP_CTRL_RTRIGGER)  add_to_str(str_29);
	if (key & PSP_CTRL_TRIANGLE)  add_to_str(str_30);
	if (key & PSP_CTRL_CIRCLE)    add_to_str(str_31);
	if (key & PSP_CTRL_CROSS)     add_to_str(str_32);
	if (key & PSP_CTRL_SQUARE)    add_to_str(str_33);
	if (key & PSP_CTRL_HOME)      add_to_str(str_34);
	if (key & PSP_CTRL_HOLD)      add_to_str(str_35);
	if (key & PSP_CTRL_NOTE)      add_to_str(str_36);
	if (key & PSP_CTRL_SCREEN)    add_to_str(str_37);
	if (key & PSP_CTRL_VOLUP)     add_to_str(str_38);
	if (key & PSP_CTRL_VOLDOWN)   add_to_str(str_39);
	if (key & PSP_CTRL_WLAN_UP)   add_to_str(str_40);
	return buf;
}
return " none";
}

void display_config(){
	memset(buf, 0, sizeof(buf));
	
	fusaGetConfig(&settings);
	
	pspDebugScreenSetXY(22,4);
	
	printf("Axis Switcher:%s\n\n",button2str(settings.AxisSwitcher));
	pspDebugScreenSetX(22);
	printf("POV RIGHT:%s\n",button2str(settings.POV_RX));
	pspDebugScreenSetX(22);
	printf("POV LEFT: %s\n",button2str(settings.POV_LX));
	pspDebugScreenSetX(22);
	printf("POV UP:   %s\n",button2str(settings.POV_UY));
	pspDebugScreenSetX(22);
	printf("POV DOWN: %s\n\n",button2str(settings.POV_DY));
	pspDebugScreenSetX(22);
	
	printf("Button1: %s\n",button2str(settings.Button1));
	pspDebugScreenSetX(22);
	printf("Button2: %s\n",button2str(settings.Button2));
	pspDebugScreenSetX(22);
	printf("Button3: %s\n",button2str(settings.Button3));
	pspDebugScreenSetX(22);
	printf("Button4: %s\n",button2str(settings.Button4));
	pspDebugScreenSetX(22);
	printf("Button5: %s\n",button2str(settings.Button5));
	pspDebugScreenSetX(22);
	printf("Button6: %s\n",button2str(settings.Button6));
	pspDebugScreenSetX(22);
	printf("Button7: %s\n",button2str(settings.Button7));
	pspDebugScreenSetX(22);
	printf("Button8: %s\n",button2str(settings.Button8));
	pspDebugScreenSetX(22);
	printf("Button9: %s\n",button2str(settings.Button9));
	pspDebugScreenSetX(22);
	printf("Button10:%s\n",button2str(settings.Button10));
	pspDebugScreenSetX(22);
	printf("Button11:%s\n",button2str(settings.Button11));
	pspDebugScreenSetX(22);
	printf("Button12:%s\n",button2str(settings.Button12));
	pspDebugScreenSetX(22);
}

int main(void)
{
	pspDebugScreenInit();
	
	display_info();
	
	SceUID mod;
	mod = pspSdkLoadStartModule("usbgamepad.prx", PSP_MEMORY_PARTITION_KERNEL);
	
	if (mod < 0) {
	pspDebugScreenSetXY(2,3);
	printf(" Error - driver was not loaded\nExitting..."); 
	sceKernelDelayThread(2000*1000);
	done = 1;
	}
	
	int count = 0;
	
	while(!done){
		
		if (fusaIsConnected()) {
			
			if (count > 700) fusaDisplay(0); else count++;
			
			pspDebugScreenSetXY(25,15);
			printf(":: CONNECTED ::\n");
		} else {
			
			fusaDisplay(1);
			
			count = 0;
			
			fusaCtrlReadBufferPositive(&pad);
			
			pspDebugScreenSetXY(25,15);
	    	printf(">-CONNECTING-->\n");
			
			if (pad.Buttons & PSP_CTRL_HOME){
			waitbuttonup(PSP_CTRL_HOME);
			user_wanna_exit();
			}
    	}
    	
		sceKernelDelayThread(10*1000);
	}
	
	sceKernelExitGame();
	return 0;
}
