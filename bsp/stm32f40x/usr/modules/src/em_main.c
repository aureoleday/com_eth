#include <rtthread.h>
#include "drv_timer.h"
#include "GUI.h"
#include "stdio.h"
#include <string.h>
#include "DIALOG.h"
#include "sys_conf.h"
#include "em_main.h"
#include "display.h"
#include "bit_op.h"

typedef struct
{
    uint32_t    icon_stat;
}em_st;



extern GUI_CONST_STORAGE GUI_FONT GUI_Fontsong16;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontyouyuan16;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontyouyuan18;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontyouyuan20;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontyouyuan24;

extern const unsigned char _aclogop[2909UL + 1];
//extern const unsigned char _acbkg[117712UL + 1];
//GUI_MEMDEV_Handle hMemJPEG;

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_IMAGE_       (GUI_ID_USER + 0x100)
#define ID_IMAGE_0      (GUI_ID_USER + 0x101)
#define ID_IMAGE_1      (GUI_ID_USER + 0x102)
#define ID_IMAGE_2      (GUI_ID_USER + 0x103)
#define ID_IMAGE_3      (GUI_ID_USER + 0x104)
#define ID_IMAGE_4      (GUI_ID_USER + 0x105)

#define ID_WINDOW_2     (GUI_ID_USER + 81 )
#define ID_WINDOW_1     (GUI_ID_USER + 80 )
#define ID_WINDOW_0     (GUI_ID_USER + 0 )
#define ID_TEXT_0       (GUI_ID_USER + 1 )
#define ID_TEXT_0_U     (GUI_ID_USER + 2 )
#define ID_TEXT_1       (GUI_ID_USER + 3 )
#define ID_TEXT_1_U     (GUI_ID_USER + 4 )
#define ID_TEXT_2       (GUI_ID_USER + 5 )
#define ID_TEXT_2_U     (GUI_ID_USER + 6 )
#define ID_TEXT_3       (GUI_ID_USER + 7 )
#define ID_TEXT_3_U     (GUI_ID_USER + 8 )
#define ID_TEXT_4       (GUI_ID_USER + 9 )
#define ID_TEXT_4_U     (GUI_ID_USER + 10 )
#define ID_TEXT_5       (GUI_ID_USER + 11 )
#define ID_TEXT_5_U     (GUI_ID_USER + 12 )
#define ID_TEXT_6       (GUI_ID_USER + 13 )
#define ID_TEXT_6_U     (GUI_ID_USER + 14 )
#define ID_TEXT_7       (GUI_ID_USER + 15 )
#define ID_TEXT_7_U     (GUI_ID_USER + 16 )
#define ID_TEXT_8       (GUI_ID_USER + 17 )
#define ID_TEXT_8_U     (GUI_ID_USER + 18 )
#define ID_TEXT_9       (GUI_ID_USER + 19 )
#define ID_TEXT_9_U     (GUI_ID_USER + 20 )
#define ID_TEXT_10      (GUI_ID_USER + 21 )
#define ID_TEXT_10_U    (GUI_ID_USER + 22 )
#define ID_TEXT_11      (GUI_ID_USER + 23 )
#define ID_TEXT_11_U    (GUI_ID_USER + 24 )
#define ID_TEXT_0_D    	(GUI_ID_USER + 25 )
#define ID_TEXT_1_D    	(GUI_ID_USER + 26 )
#define ID_TEXT_2_D    	(GUI_ID_USER + 27 )
#define ID_TEXT_3_D    	(GUI_ID_USER + 28 )
#define ID_TEXT_4_D    	(GUI_ID_USER + 29 )
#define ID_TEXT_5_D    	(GUI_ID_USER + 30 )
#define ID_TEXT_6_D    	(GUI_ID_USER + 31 )
#define ID_TEXT_7_D    	(GUI_ID_USER + 32 )
#define ID_TEXT_8_D    	(GUI_ID_USER + 33 )
#define ID_TEXT_9_D    	(GUI_ID_USER + 34 )
#define ID_TEXT_10_D    (GUI_ID_USER + 35 )
#define ID_TEXT_11_D    (GUI_ID_USER + 36 )
#define ID_TEXT_TIME    (GUI_ID_USER + 37 )
#define ID_TEXT_VER    (GUI_ID_USER + 38 )

GUI_MEMDEV_Handle hMemdev;

//static void usr_label_init(void)
//{
//    strcpy(usr_label_inst.usr_label[0].name,"abcd");
//}

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
//  { WINDOW_CreateIndirect, 	"",    			ID_WINDOW_1, 	60, 	100, 	310, 	300, 	0, 0x0,  0 },
//  { WINDOW_CreateIndirect, 	"",    			ID_WINDOW_2, 	460, 	100, 	310, 	300, 	0, 0x0,  0 },
  { IMAGE_CreateIndirect, 	"BKG", 			ID_IMAGE_, 		200, 	200, 	200, 	200, 	0, 0x0,  0 },
  { IMAGE_CreateIndirect, 	"LOGO", 		ID_IMAGE_0, 	10, 	10, 	150, 	36, 	0, 0, 	 0 },
  { IMAGE_CreateIndirect, 	"eth_con_ng", 	ID_IMAGE_1, 	760, 	10, 	150, 	40, 	0, 0, 	 0 },
  { IMAGE_CreateIndirect, 	"mb_con_ng", 	ID_IMAGE_2, 	720, 	10, 	30, 	30, 	0, 0, 	 0 },
  { TEXT_CreateIndirect, 	"大气压口",     ID_TEXT_0, 		70, 	100, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_0_D,	140, 	100, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"mbar",        	ID_TEXT_0_U, 	290, 	100, 	80, 	40, 	0, 0x64, 0 },  
  { TEXT_CreateIndirect, 	"温度",    		ID_TEXT_1, 		470, 	100, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_1_D,	540, 	100, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"°C",        	ID_TEXT_1_U, 	690, 	100, 	80, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"酸碱度口",     ID_TEXT_2, 		70, 	150, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_2_D,	140, 	150, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"NA",        	ID_TEXT_2_U, 	290,	150, 	80, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"氧化还原",     ID_TEXT_3, 		470,	150, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_3_D,	540, 	150, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"mv",        	ID_TEXT_3_U, 	690,	150, 	80, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"电导率口",    	ID_TEXT_4, 		70, 	200, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_4_D,	140, 	200, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"uS/cm",       	ID_TEXT_4_U, 	290,	200, 	80, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"电阻率口",    	ID_TEXT_5, 		470,	200, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_5_D,	540, 	200, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"O.cm",        	ID_TEXT_5_U, 	690,	200, 	80, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"盐度",        	ID_TEXT_6, 		70, 	250, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_6_D,	140, 	250, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"PSU",        	ID_TEXT_6_U, 	290,	250, 	80, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"溶解固体",     ID_TEXT_7, 		470,	250, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_7_D,	540, 	250, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"mg/L",        	ID_TEXT_7_U, 	690,	250, 	80, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"溶解氧口",    	ID_TEXT_8, 		70, 	300, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_8_D,	140, 	300, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"mg/L",        	ID_TEXT_8_U, 	290,	300, 	80, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"深度",        	ID_TEXT_9, 		470,	300, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_9_D,	540, 	300, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"cm",        	ID_TEXT_9_U, 	690,	300, 	80, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"硝氮",        	ID_TEXT_10, 	70, 	350, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_10_D,	140, 	350, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"mg/L",        	ID_TEXT_10_U, 	290,	350, 	80, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"铵氮",        	ID_TEXT_11, 	470,	350, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"",        		ID_TEXT_11_D,	540, 	350, 	120, 	40, 	0, 0x64, 0 },
  { TEXT_CreateIndirect, 	"mg/L",        	ID_TEXT_11_U, 	690,	350, 	80, 	40, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"",        	ID_TEXT_TIME, 	20,	450, 	300, 	30, 	0, 0x64, 0 }, 
  { TEXT_CreateIndirect, 	"",        	ID_TEXT_VER, 	720,	450, 	300, 	30, 	0, 0x64, 0 }, 
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
#define ID_IMAGE_0_IMAGE_   99
#define ID_IMAGE_0_IMAGE_0 0x00
#define ID_IMAGE_0_IMAGE_1 0x01
#define ID_IMAGE_0_IMAGE_2 0x02
#define ID_IMAGE_0_IMAGE_3 0x03
#define ID_IMAGE_0_IMAGE_4 0x04

extern const unsigned char _acmbng[1021UL + 1];
extern const unsigned char _acmbok[842UL + 1];
extern const unsigned char _acethng[636UL + 1];
extern const unsigned char _acethok[682UL + 1];

static const void * _GetImageById(U32 Id, U32 * pSize) {
  switch (Id) 
  {
//    case ID_IMAGE_0_IMAGE_:
//      *pSize = sizeof(_acbkg);
//      return (const void *)_acbkg;    
    case ID_IMAGE_0_IMAGE_0:
      *pSize = sizeof(_aclogop);
      return (const void *)_aclogop;
    case ID_IMAGE_0_IMAGE_1:
      *pSize = sizeof(_acmbng);
      return (const void *)_acmbng;
	case ID_IMAGE_0_IMAGE_2:
      *pSize = sizeof(_acmbok);
      return (const void *)_acmbok;  
	case ID_IMAGE_0_IMAGE_3:
      *pSize = sizeof(_acethng);
      return (const void *)_acethng;
	case ID_IMAGE_0_IMAGE_4:
      *pSize = sizeof(_acethok);
      return (const void *)_acethok;   
  }
  return NULL;
}

static void gui_sprint(char* dbuf,uint32_t data,uint16_t dn)
{
    float   temp;
    if(dn==100)
    {
        temp = (float)data/dn;
        sprintf(dbuf,"%.2f",temp);
    }
    else if(dn==10)
    {
        temp = (float)data/dn;
        sprintf(dbuf,"%.1f",temp);
    }
    else
    {
        temp = data;
        sprintf(dbuf,"%d",(uint32_t)temp);
    }
}

static void gui_print_time(WM_MESSAGE * pMsg)
{
    time_t  now;
    char    dbuf[64];    
    WM_HWIN hitem;
  
    now = time(RT_NULL);
    sprintf(dbuf,"%s",ctime(&now));
    hitem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TIME);
    TEXT_SetText(hitem, dbuf);
}

static void gui_ver(WM_MESSAGE * pMsg)
{
    WM_HWIN hitem;
    uint32_t main_ver,sub_ver;
    char    dbuf[32];   

    main_ver = SOFTWARE_VER>>16;
    sub_ver = SOFTWARE_VER&0x0000ffff;  
    hitem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_VER);
    TEXT_SetTextColor(hitem, (0x00FFFFFF));
    TEXT_SetTextAlign(hitem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetFont(hitem,&GUI_Font20_1);	  
    sprintf(dbuf,"V%d.%d",main_ver,sub_ver);
    TEXT_SetText(hitem, dbuf);
}

static void gui_dupdate(WM_MESSAGE * pMsg)
{
    extern disp_sen_st disp_sen_inst;
    WM_HWIN hitem;
    char dbuf[32];
    static char i=0;

    for(i=0;i<=11;i++)
    {
        hitem = WM_GetDialogItem(pMsg->hWin, (ID_TEXT_0_D+i));
        gui_sprint(dbuf,disp_sen_inst.sid[i].val,disp_sen_inst.sid[i].opt);
        TEXT_SetText(hitem, dbuf);
        GUI_Exec();
    }
    gui_print_time(pMsg); 
    GUI_Exec();
}

static void icon_update(WM_MESSAGE * pMsg)
{
    extern sys_reg_st  g_sys;
    WM_HWIN hitem;
    const void * pData ;
    U32          FileSize;   
    static uint32_t sts_bitmap = 0;
    uint8_t bit_sts = 0;
    bit_sts = bit_op_get(g_sys.stat.gen.status_bm,GBM_MBM);
    if(bit_sts != bit_op_get(sts_bitmap,GBM_MBM))
    {
        bit_op_set(&sts_bitmap,GBM_MBM,bit_sts);
        hitem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_1);
        WM_InvalidateWindow(hitem);
        if(bit_sts == 0)
            pData = _GetImageById(ID_IMAGE_0_IMAGE_1, &FileSize);
        else
            pData = _GetImageById(ID_IMAGE_0_IMAGE_2, &FileSize);
        IMAGE_SetPNG(hitem, pData, FileSize); 
    }
    
    bit_sts = bit_op_get(g_sys.stat.gen.status_bm,GBM_TCP);
    if(bit_sts != bit_op_get(sts_bitmap,GBM_TCP))
    {
        bit_op_set(&sts_bitmap,GBM_TCP,bit_sts);
        hitem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_2);
        WM_InvalidateWindow(hitem);
        if(bit_sts == 0)
            pData = _GetImageById(ID_IMAGE_0_IMAGE_3, &FileSize);
        else
            pData = _GetImageById(ID_IMAGE_0_IMAGE_4, &FileSize);
        IMAGE_SetPNG(hitem, pData, FileSize);
    }
}

static void gui_img_init(WM_MESSAGE * pMsg)
{
    WM_HWIN hItem;
    const void * pData ;
    U32          FileSize;
    
  
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, GUI_LIGHTBLUE);
//    GUI_Clear();
//    GUI_SetColor(GUI_GREEN);
//    GUI_DrawHLine(20, 0, 700);
//    hItem = WM_GetDialogItem(pMsg->hWin, ID_WINDOW_1);
//    WINDOW_SetBkColor(hItem, 0x00FFFFFF);

//    hItem = WM_GetDialogItem(pMsg->hWin, ID_WINDOW_2);
//    WINDOW_SetBkColor(hItem, 0x00FFFFFF);  
//    GUI_FillRect(80,120,160,200);
  
//    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_);
//    pData = _GetImageById(ID_IMAGE_0_IMAGE_0, &FileSize);
//    IMAGE_SetPNG(hItem, pData, FileSize);
  
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
    pData = _GetImageById(ID_IMAGE_0_IMAGE_0, &FileSize);
    IMAGE_SetPNG(hItem, pData, FileSize);

//    GUI_JPEG_INFO JpegInfo;
//  GUI_JPEG_GetInfo(_acbkg2,sizeof(_acbkg2), &JpegInfo);
//    hMemdev = GUI_MEMDEV_CreateEx(0,0,800,480,GUI_MEMDEV_HASTRANS);
//    GUI_MEMDEV_Select(hMemdev);
//    GUI_MEMDEV_Select(0);
  
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_1);
    pData = _GetImageById(ID_IMAGE_0_IMAGE_1, &FileSize);
    IMAGE_SetPNG(hItem, pData, FileSize); 
  
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_2);
    pData = _GetImageById(ID_IMAGE_0_IMAGE_3, &FileSize);
    IMAGE_SetPNG(hItem, pData, FileSize);   
}

static void gui_text_init(WM_MESSAGE * pMsg)
{
    WM_HWIN hItem;
    int   i;    
    for(i=ID_TEXT_0;i<=ID_TEXT_11;i=i+2)
    {
        hItem = WM_GetDialogItem(pMsg->hWin, i);
        TEXT_SetTextColor(hItem, (GUI_WHITE));
        TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        TEXT_SetFont(hItem,&GUI_Fontyouyuan24);		
    }	
    
    for(i=ID_TEXT_0_D;i<=ID_TEXT_11_D;i++)
    {
        hItem = WM_GetDialogItem(pMsg->hWin, i);
        TEXT_SetTextColor(hItem, (GUI_LIGHTYELLOW));
        TEXT_SetTextAlign(hItem, GUI_TA_RIGHT | GUI_TA_VCENTER);
//        TEXT_SetWrapMode(hItem,GUI_WRAPMODE_WORD);
        TEXT_SetFont(hItem,&GUI_Font24_1);		
    }  
    
    for(i=ID_TEXT_0_U;i<=ID_TEXT_11_U;i=i+2)
    {
        hItem = WM_GetDialogItem(pMsg->hWin, i);
        TEXT_SetTextColor(hItem, GUI_WHITE);
        TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        TEXT_SetFont(hItem,&GUI_Font24_1);		
    }   
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_TIME);
    TEXT_SetTextColor(hItem, (0x00FFFFFF));
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetFont(hItem,&GUI_Font20_1);	
    
    gui_ver(pMsg);
}

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
//  WM_HWIN hItem;
//  const void * pData ;
//  U32          FileSize;
//  
//  static int i=0;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
    case WM_PAINT:
      break;
    case WM_INIT_DIALOG:    
      gui_img_init(pMsg);
      gui_text_init(pMsg);
      break;
    case WM_TIMER:
      WM_RestartTimer(pMsg->Data.v, 1000);
      gui_dupdate(pMsg);
      icon_update(pMsg);
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
//  extern const unsigned char _acbkg2[73191UL + 1];
//  GUI_MEMDEV_Handle hMemJPEG;
	/* 初始化 */
	GUI_Init();

	WM_MULTIBUF_Enable(1);

	/* 使能UTF-8编码 */     
	GUI_UC_SetEncodeUTF8(); 
	
	/* 调用此函数会自动的刷新桌面窗口 */
//	WM_SetDesktopColor(GUI_TRANSPARENT); 
  
	/* 创建对话框 */   
  
//  GUI_JPEG_INFO JpegInfo;
//  GUI_JPEG_GetInfo(_acbkg2,sizeof(_acbkg2), &JpegInfo);
//  hMemJPEG = GUI_MEMDEV_CreateEx(0,0,JpegInfo.XSize,JpegInfo.YSize,GUI_MEMDEV_HASTRANS);
//  GUI_MEMDEV_Select(hMemJPEG);
//  GUI_JPEG_Draw(_acbkg2,sizeof(_acbkg2),0,0);
//  GUI_MEMDEV_Select(0);
//  GUI_MEMDEV_WriteAt(hMemJPEG,0,0);      
  
  hwin = CreateWindow();
//	GUI_CreateDialogBox(_aDialogCreate1, GUI_COUNTOF(_aDialogCreate1), &_cbCallback, 0, 0, 0);
  WM_CreateTimer(WM_GetClientWindow(hwin),0,1000,0);
  
	while(1) 
	{
		GUI_Delay(10);
	}
}

