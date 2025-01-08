	.set noreorder

#include "pspstub.s"

	STUB_START "fusaGamePad",0x40090000,0x00050005
	STUB_FUNC  0x88B3EF0D,fusaIsConnected
	STUB_FUNC  0xD3DADF7E,fusaGetConfig
	STUB_FUNC  0xDCFBA056,fusaSetConfig
	STUB_FUNC  0x4A04F7AD,fusaCtrlReadBufferPositive
	STUB_FUNC  0x8A97CBA1,fusaDisplay
	STUB_END
