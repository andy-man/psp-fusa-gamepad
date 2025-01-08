typedef struct fusaGPConfig
{
  int AxisSwitcher;
  
  int  Button1;
  int  Button2;
  int  Button3;
  int  Button4;
  int  Button5;
  int  Button6;
  int  Button7;
  int  Button8;
  
  int  Button9;
  int  Button10;
  int  Button11;
  int  Button12;
  
  int  POV_RX;
  int  POV_LX;
  int  POV_UY;
  int  POV_DY;
} GP_Config;

int fusaIsConnected(void);
void fusaSetConfig(GP_Config*);
void fusaGetConfig(GP_Config*);
void fusaDisplay(int);
void fusaCtrlReadBufferPositive(SceCtrlData * pad);
