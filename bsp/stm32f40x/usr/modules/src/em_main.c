#include "GUI.h"
#include "stdio.h"
#include <string.h>
#include "DIALOG.h"
#include "sys_conf.h"
#include "em_main.h"
#include "display.h"

extern GUI_CONST_STORAGE GUI_FONT GUI_Fontsong16;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontyouyuan16;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontyouyuan18;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontyouyuan20;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontyouyuan24;



#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0     (GUI_ID_USER + 0x00 )
#define ID_TEXT_0       (GUI_ID_USER + 0x01 )
#define ID_TEXT_0_U     (GUI_ID_USER + 0x02 )
#define ID_TEXT_1       (GUI_ID_USER + 0x03 )
#define ID_TEXT_1_U     (GUI_ID_USER + 0x04 )
#define ID_TEXT_2       (GUI_ID_USER + 0x05 )
#define ID_TEXT_2_U     (GUI_ID_USER + 0x06 )
#define ID_TEXT_3       (GUI_ID_USER + 0x07 )
#define ID_TEXT_3_U     (GUI_ID_USER + 0x08 )
#define ID_TEXT_4       (GUI_ID_USER + 0x09 )
#define ID_TEXT_4_U     (GUI_ID_USER + 0x10 )
#define ID_TEXT_5       (GUI_ID_USER + 0x11 )
#define ID_TEXT_5_U     (GUI_ID_USER + 0x12 )
#define ID_TEXT_6       (GUI_ID_USER + 0x13 )
#define ID_TEXT_6_U     (GUI_ID_USER + 0x14 )
#define ID_TEXT_7       (GUI_ID_USER + 0x15 )
#define ID_TEXT_7_U     (GUI_ID_USER + 0x16 )
#define ID_TEXT_8       (GUI_ID_USER + 0x17 )
#define ID_TEXT_8_U     (GUI_ID_USER + 0x18 )
#define ID_TEXT_9       (GUI_ID_USER + 0x19 )
#define ID_TEXT_9_U     (GUI_ID_USER + 0x20 )
#define ID_TEXT_10      (GUI_ID_USER + 0x21 )
#define ID_TEXT_10_U    (GUI_ID_USER + 0x22 )
#define ID_TEXT_11      (GUI_ID_USER + 0x23 )
#define ID_TEXT_11_U    (GUI_ID_USER + 0x24 )
#define ID_TEXT_0_D    	(GUI_ID_USER + 0x25 )
#define ID_TEXT_1_D    	(GUI_ID_USER + 0x26 )
#define ID_TEXT_2_D    	(GUI_ID_USER + 0x27 )
#define ID_TEXT_3_D    	(GUI_ID_USER + 0x28 )
#define ID_TEXT_4_D    	(GUI_ID_USER + 0x29 )
#define ID_TEXT_5_D    	(GUI_ID_USER + 0x30 )
#define ID_TEXT_6_D    	(GUI_ID_USER + 0x31 )
#define ID_TEXT_7_D    	(GUI_ID_USER + 0x32 )
#define ID_TEXT_8_D    	(GUI_ID_USER + 0x33 )
#define ID_TEXT_9_D    	(GUI_ID_USER + 0x34 )
#define ID_TEXT_10_D    (GUI_ID_USER + 0x35 )
#define ID_TEXT_11_D    (GUI_ID_USER + 0x36 )

const char* name_space[] = 
{
    "hello",
    "stop",
    "start"
};

usr_label_st usr_label_inst;

static void usr_label_init(void)
{
    strcpy(usr_label_inst.usr_label[0].name,"abcd");
}

// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*///

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, 	"",    			ID_WINDOW_0, 	0, 		0, 		800, 	480, 	0, 0x0,  0 },
  { TEXT_CreateIndirect, 	"温度",        	ID_TEXT_0, 		20, 	100, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_0_D,	150, 	100, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"C",        	ID_TEXT_0_U, 	300, 	100, 	120, 	40, 	0, 0x64, 0 },  
  { TEXT_CreateIndirect, 	"大气压口",    	ID_TEXT_1, 		400, 	100, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_1_D,	550, 	100, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"mbar",        	ID_TEXT_1_U, 	700, 	100, 	120, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"酸碱度口",     ID_TEXT_2, 		20, 	150, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_2_D,	150, 	150, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"NA",        	ID_TEXT_2_U, 	300,	150, 	120, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"氧化还原",     ID_TEXT_3, 		400,	150, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_3_D,	550, 	150, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"mv",        	ID_TEXT_3_U, 	700,	150, 	120, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"电导率口",    	ID_TEXT_4, 		20, 	200, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_4_D,	150, 	200, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"uS/cm",       	ID_TEXT_4_U, 	300,	200, 	120, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"电阻率口",    	ID_TEXT_5, 		400,	200, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_5_D,	550, 	200, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"Ω.cm",        	ID_TEXT_5_U, 	700,	200, 	120, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"盐度",        	ID_TEXT_6, 		20, 	250, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_6_D,	150, 	250, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"PSU",        	ID_TEXT_6_U, 	300,	250, 	120, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"溶解固体",     ID_TEXT_7, 		400,	250, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_7_D,	550, 	250, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"mg/L",        	ID_TEXT_7_U, 	700,	250, 	120, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"溶解氧口",    	ID_TEXT_8, 		20, 	300, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_8_D,	150, 	300, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"mg/L",        	ID_TEXT_8_U, 	300,	300, 	120, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"深度",        	ID_TEXT_9, 		400,	300, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_9_D,	550, 	300, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"cm",        	ID_TEXT_9_U, 	700,	300, 	120, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"硝氮",        	ID_TEXT_10, 	20, 	350, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_10_D,	150, 	350, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"mg/L",        	ID_TEXT_10_U, 	300,	350, 	120, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"铵氮",        	ID_TEXT_11, 	400,	350, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_11_D,	550, 	350, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"mg/L",        	ID_TEXT_11_U, 	700,	350, 	120, 	40, 	0, 0x64, 0 }, 
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

static void gui_sprint(char* dbuf,uint32_t data,uint16_t dn)
{
    float   temp;
    if(dn>1)
    {
        temp = (float)data/dn;
        sprintf(dbuf,"%7.2f",temp);
    }
    else
    {
        temp = data;
        sprintf(dbuf,"%7d",(uint32_t)temp);
    }
}

static void gui_dupdate(WM_MESSAGE * pMsg)
{
    extern disp_sen_st disp_sen_inst;
    WM_HWIN hitem;
    char dbuf[32];
    static char i=0;
    
    for(i=0;i<11;i++)
    {
        hitem = WM_GetDialogItem(pMsg->hWin, (ID_TEXT_0_D+i));
        gui_sprint(dbuf,disp_sen_inst.sid[i].val,disp_sen_inst.sid[i].opt);
        TEXT_SetText(hitem, dbuf);
        TEXT_SetTextColor(hitem, (0x00FFFFFF));
        TEXT_SetTextAlign(hitem, GUI_TA_RIGHT | GUI_TA_VCENTER);
        TEXT_SetFont(hitem,&GUI_Font24_1);
    }
}

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  WM_HWIN hitem;
  static int i=0;
  char dbuf[32];
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Window'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, (0x00C08000));

	for(i=ID_TEXT_0;i<=ID_TEXT_11;i=i+2)
	{
      hItem = WM_GetDialogItem(pMsg->hWin, i);
      TEXT_SetTextColor(hItem, (0x00FFFFFF));
      TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
      TEXT_SetFont(hItem,&GUI_Fontyouyuan24);		
	}	
  
	for(i=ID_TEXT_0_U;i<=ID_TEXT_11_U;i=i+2)
	{
      hItem = WM_GetDialogItem(pMsg->hWin, i);
      TEXT_SetTextColor(hItem, (0x00FFFFFF));
      TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
      TEXT_SetFont(hItem,&GUI_Font24_1);		
	}  

    break;
  case WM_TIMER:
    WM_RestartTimer(pMsg->Data.v, 1000);
    gui_dupdate(pMsg);
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateWindow
*/
WM_HWIN CreateWindow(void);
WM_HWIN CreateWindow(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

/*
*********************************************************************************************************
*	函 数 名: MainTask
*	功能说明: GUI主函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MainTask(void) 
{
  WM_HWIN hwin;
  WM_HWIN hitem,htimer;

	/* 初始化 */
	GUI_Init();

	WM_MULTIBUF_Enable(1);

	/* 使能UTF-8编码 */     
	GUI_UC_SetEncodeUTF8(); 
	
	/* 调用此函数会自动的刷新桌面窗口 */
	WM_SetDesktopColor(GUI_WHITE); 

	/* 创建对话框 */
  hwin = CreateWindow();
//	GUI_CreateDialogBox(_aDialogCreate1, GUI_COUNTOF(_aDialogCreate1), &_cbCallback, 0, 0, 0);
  htimer = WM_CreateTimer(WM_GetClientWindow(hwin),0,1000,0);
	
	while(1) 
	{
		GUI_Delay(10);
	}
}

