#include <pspkernel.h>
#include <pspctrl.h>
#include <string.h>
#include "fusainterface.h"
#include "usbhid.h"

void sceDisplayEnable();
void sceDisplayDisable();
void sceDisplaySetBrightness( int, int );


struct UsbbdDeviceRequest UsbBulkinReq, UsbControlReq;
SceCtrlData pad;

int connected = 0, active = 1;
static unsigned char data[DATA_SIZE] __attribute__ ((aligned(64)));
GP_Config GPsettings = {
	PSP_CTRL_NOTE,
	
	PSP_CTRL_CROSS,
	PSP_CTRL_CIRCLE,
	PSP_CTRL_SQUARE,
	PSP_CTRL_TRIANGLE,
	PSP_CTRL_LTRIGGER,
	PSP_CTRL_RTRIGGER,
	PSP_CTRL_SELECT,
	PSP_CTRL_START,
	
	PSP_CTRL_HOME,
	PSP_CTRL_VOLDOWN,
	PSP_CTRL_VOLUP,
	PSP_CTRL_SCREEN,
	
	PSP_CTRL_RIGHT,
	PSP_CTRL_LEFT,
	PSP_CTRL_UP,
	PSP_CTRL_DOWN
};

PSP_MODULE_INFO ("FuSaPAD", 0x1006, 1, 1);

// TODO: hook sceIdStorageLookup to send our StringDescriptors (not very important)

void fusaDisplay(int mode){
	if (mode) { 
	sceDisplaySetBrightness( 68, 0 );
	sceDisplayEnable(); 
	} else {
	sceDisplaySetBrightness( 0, 0 );
	sceDisplayDisable();
	}
}

int fusaIsConnected(void)
{
	return(connected);
}

void fusaSetConfig(GP_Config * buf)
{
	memcpy(&GPsettings,buf,sizeof(GP_Config));
	return;
}

void fusaGetConfig(GP_Config * buf)
{
	memcpy(buf,&GPsettings,sizeof(GP_Config));
	return;
}

void fusaCtrlReadBufferPositive(SceCtrlData * Pad)
{
	
	sceCtrlReadBufferPositive(&pad, 1);
	memcpy(Pad,&pad,sizeof(pad));
	return;
}
/* Device request */
static void UsbBulkinReqDone( struct UsbbdDeviceRequest *req)
{
	req->unused = NULL;
	return;
}

void usbSendData(void *data, int size)
{
  if (!UsbBulkinReq.unused) {
		memset( &UsbBulkinReq, 0, sizeof(UsbBulkinReq) );
	  	UsbBulkinReq.endpoint = &endpoints[1];
		UsbBulkinReq.data = data;
		UsbBulkinReq.size = size;
		UsbBulkinReq.onComplete = &UsbBulkinReqDone;
		UsbBulkinReq.unused = &UsbBulkinReq;
		sceUsbbdReqSend( &UsbBulkinReq );
	}
	return;
}

void usbSendSetupPacket(void *data, int size, int length)
{
	if (!UsbControlReq.unused) {
		memset( &UsbControlReq, 0, sizeof(UsbControlReq) );
		UsbControlReq.data = data;
		UsbControlReq.size = size;
		UsbControlReq.endpoint = &endpoints[0];
		if (UsbControlReq.size > length)
		UsbControlReq.size = length;
		UsbControlReq.onComplete = &UsbBulkinReqDone;
		UsbControlReq.unused = &UsbControlReq;
		sceUsbbdReqSend (&UsbControlReq);
	}
	return;
}

static
int usb_recvctl (int arg1, int arg2, struct DeviceRequest *req)
{
  if ((req->bRequest == USB_REQ_GET_DESCRIPTOR) && ((req->wValue) == (USB_DT_REPORT << 8)) && (arg2 != -1)) {
  	  usbSendSetupPacket(ReportDescriptorGamepad,sizeof(ReportDescriptorGamepad),req->wLength);
  };
  
  return 0;
}

/* Alternate settings */
static
int usb_change (int interfaceNumber, int alternateSetting)
{
  return 0;
}

/* Attach callback */
static
int usb_attach (int usb_version)
{
  connected = 1;
  return 0;
}

/* Detach callback */
static
void usb_detach (void)
{
  connected = 0;
  return;
}

static
void usb_configure (int usb_version, int desc_count, struct InterfaceSettings *settings)
{
  return;
}

/* USB start function */
static
int start_func (int size, void *p)
{
  return 0;
}

/* USB stop function */
static
int stop_func (int size, void *p)
{
  return 0;
}

#define check_but(b) ((pad.Buttons & b) == b) ? 1 : 0

int gamepad_int(SceSize args, void *argp)
{
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	
	usb_start();
	
	while(active) {
	
	sceCtrlReadBufferPositive(&pad, 1);
	
	memset( data, 0x00, sizeof(data) );
	memset( data, 0x7F, AXIS_Rz+1 );
		
		if (check_but(GPsettings.AxisSwitcher))
		{
		data[AXIS_Z] = pad.Ly;
		data[AXIS_Rz] = pad.Lx;
		} else {
		data[AXIS_X] = pad.Lx;
		data[AXIS_Y] = pad.Ly;
		};
		
		if ((pad.Buttons & GPsettings.POV_UY)) data[POV] = 1;
		if ((pad.Buttons & GPsettings.POV_RX)) data[POV] = 3;
		if ((pad.Buttons & GPsettings.POV_DY)) data[POV] = 5;
		if ((pad.Buttons & GPsettings.POV_LX)) data[POV] = 7;
		
		if ((pad.Buttons & GPsettings.POV_UY) && (pad.Buttons & GPsettings.POV_RX)) data[POV] = 2;
		if ((pad.Buttons & GPsettings.POV_RX) && (pad.Buttons & GPsettings.POV_DY)) data[POV] = 4;
		if ((pad.Buttons & GPsettings.POV_DY) && (pad.Buttons & GPsettings.POV_LX)) data[POV] = 6;
		if ((pad.Buttons & GPsettings.POV_LX) && (pad.Buttons & GPsettings.POV_UY)) data[POV] = 8;
		
		if (check_but(GPsettings.Button1)) data[BUTTONS_1_4] |= 0x01 << 4;
		if (check_but(GPsettings.Button2)) data[BUTTONS_1_4] |= 0x02 << 4;
		if (check_but(GPsettings.Button3)) data[BUTTONS_1_4] |= 0x04 << 4;
		if (check_but(GPsettings.Button4)) data[BUTTONS_1_4] |= 0x08 << 4;
		
		if (check_but(GPsettings.Button5)) data[BUTTONS_5_12] |= 0x01;
		if (check_but(GPsettings.Button6)) data[BUTTONS_5_12] |= 0x02;
		if (check_but(GPsettings.Button7)) data[BUTTONS_5_12] |= 0x04;
		if (check_but(GPsettings.Button8)) data[BUTTONS_5_12] |= 0x08;
		if (check_but(GPsettings.Button9)) data[BUTTONS_5_12] |= 0x10;
		if (check_but(GPsettings.Button10)) data[BUTTONS_5_12] |= 0x20;
		if (check_but(GPsettings.Button11)) data[BUTTONS_5_12] |= 0x40;
		if (check_but(GPsettings.Button12)) data[BUTTONS_5_12] |= 0x80;
		
	sceKernelDcacheWritebackRange (data, sizeof (data));
	
	if (sceUsbGetState () & PSP_USB_STATUS_CONNECTION_ESTABLISHED) usbSendData(data,DATA_SIZE);
	
	sceKernelDelayThread(4000);
	
	}
	
	usb_stop();
	
	return 0;
}

/* Usb start routine*/
int usb_start(void)
{
  int ret = 0;
  ret = sceUsbbdRegister (&driver);
  if (ret < 0) return ret;
  ret = sceUsbStart (PSP_USB_BUS_DRIVERNAME, 0, 0);
  if (ret < 0) return ret;
  ret = sceUsbStart ( PSP_USB_HID, 0, 0);
  if (ret < 0) return ret;
  ret = sceUsbActivate (PSP_USB_GAMEPAD_PID);
  return ret;
}

/* Usb stop */
int usb_stop (void)
{
  int ret;
  ret = sceUsbDeactivate ();
  if (ret < 0) return ret;
  
  ret = sceUsbStop (PSP_USB_HID, 0, 0);
  if (ret < 0) return ret;
  ret = sceUsbStop (PSP_USB_BUS_DRIVERNAME, 0, 0);
  if (ret < 0) return ret;
  ret = sceUsbbdUnregister (&driver);
  
  return ret;
}

int module_start (SceSize args, void *argp)
{
  SceUID main_thid = sceKernelCreateThread("gamepad_thread", gamepad_int, 32, 0x800, 0, NULL);
  if (main_thid >= 0) sceKernelStartThread(main_thid, args, argp);
  
  return 0;
}

int module_stop (SceSize args, void *argp)
{
  active = 0;
  
  return 0;
}