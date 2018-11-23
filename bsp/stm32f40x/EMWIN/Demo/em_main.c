#include "GUI.h"
#include "stdio.h"
#include "DIALOG.h"

extern GUI_CONST_STORAGE GUI_FONT GUI_Fontsong16;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontyouyuan16;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontyouyuan18;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontyouyuan20;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontyouyuan24;
///*
//*********************************************************************************************************
//*	                                  对话框资源列表
//*********************************************************************************************************
//*/
//static const GUI_WIDGET_CREATE_INFO _aDialogCreate1[] = 
//{
//    {WINDOW_CreateIndirect,     "", 0,  0,  0,  800,480,0,0},
//    {BUTTON_CreateIndirect,    "hi",          GUI_ID_BUTTON0,          350,20,420,150,0,0},
//    {TEXT_CreateIndirect,      "硝酸",        GUI_ID_TEXT0,            5, 10, 300, 33, 0,0},
//    {TEXT_CreateIndirect,      "温度",        GUI_ID_TEXT1,            5, 40,250, 50, 0,0},
//    {TEXT_CreateIndirect,      "氨氮",        GUI_ID_TEXT2,            5, 100,360, 90, 0,0},
//    {TEXT_CreateIndirect,      "温度",        GUI_ID_TEXT3,            5, 220,144, 144, 0,0},
//    {TEXT_CreateIndirect,      "气压",        GUI_ID_TEXT4,            205, 230,144, 144, 0,0},
//    {TEXT_CreateIndirect,      "电导率口",      GUI_ID_TEXT5,          405, 230,144, 144, 0,0},
//    {TEXT_CreateIndirect,      "溶解氧口",      GUI_ID_TEXT6,          605, 230,144, 144, 0,0},
//};


///*
//*********************************************************************************************************
//*	函 数 名: PaintDialog
//*	功能说明: 对话框重绘函数
//*	形    参：pMsg   消息指针
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//void PaintDialog(WM_MESSAGE   * pMsg)
//{
////    WM_HWIN hWin = pMsg->hWin;
//	
//}

///*
//*********************************************************************************************************
//*	函 数 名: 对话框初始化
//*	功能说明: 对话框初始化
//*	形    参: pMsg   消息指针
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//void InitDialog(WM_MESSAGE * pMsg)
//{
//    WM_HWIN hWin = pMsg->hWin;
//	
//    //
//    // 配置FrameWin
//    //
//    FRAMEWIN_SetFont(hWin,&GUI_Font32B_ASCII);
//    FRAMEWIN_SetTextAlign(hWin,GUI_TA_VCENTER|GUI_TA_CENTER);
//    FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
//    FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
//    FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
//    FRAMEWIN_SetTitleHeight(hWin,35);
//	
//    //
//    // 按钮的字体是4倍抗锯齿，144点阵
//    //                                                               
//    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON0),&GUI_Fontsong16);                    
//	
//    //
//    // 分别用16点阵，32点阵和72点阵字体显示 安富莱电子 五个字。
//    //
//    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT0), GUI_RED);
//    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT0),&GUI_Fontsong16);
//    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT1), GUI_GREEN);
//    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT1),&GUI_Fontsong16);
//    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT2), GUI_BLUE);
//    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT2),&GUI_Fontsong16);

//    //
//    // 分别用144点阵汉字，144点阵的扩展模式且4倍抗锯齿汉字，144点阵的4倍抗锯齿汉字和
//	// 144点阵的扩展模式且带边框汉字。
//    //
//    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT3),&GUI_Fontsong16);
//    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT4),&GUI_Fontsong16);
//    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT5),&GUI_Fontsong16);
//    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT6),&GUI_Fontsong16);
//}

///*
//*********************************************************************************************************
//*	函 数 名: _cbCallback
//*	功能说明: 对话框回调函数
//*	形    参: pMsg   消息指针
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//static void _cbCallback(WM_MESSAGE * pMsg) 
//{
//    int NCode, Id;
//    WM_HWIN hWin = pMsg->hWin;
//    switch (pMsg->MsgId) 
//    {
//        case WM_PAINT:
//            PaintDialog(pMsg);
//            break;
//		
//        case WM_INIT_DIALOG:
//            InitDialog(pMsg);
//            break;
//		
//        case WM_KEY:
//            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
//            {
//                case GUI_KEY_ESCAPE:
//                    GUI_EndDialog(hWin, 1);
//                    break;
//                case GUI_KEY_ENTER:
//                    GUI_EndDialog(hWin, 0);
//                    break;
//            }
//            break;
//			
//        case WM_NOTIFY_PARENT:
//            Id = WM_GetId(pMsg->hWinSrc); 
//            NCode = pMsg->Data.v;        
//            switch (Id) 
//            {
//                case GUI_ID_OK:
//                    if(NCode==WM_NOTIFICATION_RELEASED)
//                        GUI_EndDialog(hWin, 0);
//                    break;
//					
//                case GUI_ID_CANCEL:
//                    if(NCode==WM_NOTIFICATION_RELEASED)
//                        GUI_EndDialog(hWin, 0);
//                    break;

//            }
//            break;
//			
//        default:
//            WM_DefaultProc(pMsg);
//    }
//}


#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0   (GUI_ID_USER + 0x00)
#define ID_TEXT_0   (GUI_ID_USER + 0x01)
#define ID_TEXT_1   (GUI_ID_USER + 0x02)
#define ID_TEXT_2   (GUI_ID_USER + 0x03)


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
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 800, 480, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "温度", ID_TEXT_0, 20, 40, 80, 20, 0, 0x64, 0 },  
  { TEXT_CreateIndirect, "气压", ID_TEXT_1, 20, 65, 80, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "硝酸", ID_TEXT_2, 80, 40, 80, 20, 0, 0x64, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Window'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, (0x00C08000));
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
//    TEXT_SetText(hItem, "hello");
    TEXT_SetTextColor(hItem, (0x00FFFFFF));
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetFont(hItem,&GUI_Fontyouyuan18);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
//    TEXT_SetText(hItem, "wolrd");
    TEXT_SetTextColor(hItem, (0x00FFFFFF));
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetFont(hItem,&GUI_Fontyouyuan18);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
//    TEXT_SetText(hItem, "wolrd");
    TEXT_SetTextColor(hItem, (0x00FFFFFF));
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, &GUI_Font16_ASCII);
//    TEXT_SetFont(hItem,&GUI_Fontsong16);    
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
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
  WM_HWIN hitem;
  int i=0;
  char dbuf[32];
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
	
	while(1) 
	{
    hitem = WM_GetDialogItem(hwin, ID_TEXT_2);
    sprintf(dbuf,"V%d\r",i);
    TEXT_SetText(hitem, dbuf);
    i++;
		GUI_Delay(1000);
	}
}

