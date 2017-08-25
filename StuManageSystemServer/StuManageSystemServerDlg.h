
// StuManageSystemServerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "../../common/ClientManager.h"
//在其它电脑上，这个得改一改了，因为路径不同
#import "C:\Windows\winsxs\x86_microsoft-windows-m..ents-mdac-ado15-dll_31bf3856ad364e35_6.1.7601.17857_none_0e101351cf067d1f\MSADO15.DLL" no_namespace rename("EOF", "MyEOF")

class CStuManageSystemServerDlg;
typedef BOOL (CStuManageSystemServerDlg::*pMemFun)(char* , int);

// CStuManageSystemServerDlg 对话框
class CStuManageSystemServerDlg : public CDialogEx
{
// 构造
public:
	CStuManageSystemServerDlg(CWnd* pParent = NULL);	// 标准构造函数

//自定义变量
	SOCKET           m_sServer;
	SOCKET           m_sClient;
	CClientContext*  m_pContext;
	tagStuInfoNoName* m_pStuInfoNoName;
	char* m_pszName;
	char* m_pszSex;
	_ConnectionPtr m_conn;
	_RecordsetPtr m_rs;
	CMyPacket m_FindResultPkt;
	BOOL m_bIsFirstFind;
//自定义函数
	BOOL initFrame();
	BOOL OnComEnvi();
	BOOL CreateSocketAndAccept();
	static DWORD RecvFunc(LPVOID lpParam);
	BOOL OnGetReadBuf(SOCKET s, char*& pszBuf);
	BOOL SendPacket(SOCKET s, SOCKET sAdd, char* pszStr, DWORD StrLen, int nMsg);//SOCKET sAdd保留参数,StrLen是pszStr的内容字节长
	
	/*BOOL OnMT_CLIENT_ADD(char* szBuf);*/
	//打算用类成员函数，将所有功能函数归到以下函数，可减少代码书写
	BOOL OnOneKindDone(pMemFun pFunc, char* szBuf, DWORD dwLength);
	BOOL OnMT_CLIENT(pMemFun pFunc, char* szBuf);

	BOOL OnAddInfoStr(char* pszCmd, int nCmdLen);
	BOOL OnDelInfoStr(char* pszCmd, int nCmdLen);
	BOOL OnAlterInfoStr(char* pszCmd, int nCmdLen);
	//要修改姓名时的字符串输入
	BOOL OnAlterInfoStrName(char* pszCmd, int nCmdLen);
	//要修改身高时的字符串输入
	BOOL OnAlterInfoStrHeight(char* pszCmd, int nCmdLen);
	//要修改性别时的字符串输入
	BOOL OnAlterInfoStrSex(char* pszCmd, int nCmdLen);
	//因为修改比较麻烦，另写一个来操作
	BOOL OnMT_CLIENTALTER(char* pszBuf);

	BOOL OnFindInfoStr(char* pszCmd, int nCmdLen);
	BOOL OnMT_CLIENTFIND(char* pszBuf);
	BOOL OnFindStuInfo(char* szBuf, DWORD dwLength);
	BOOL OnPutInfoInBuf(char* szBuf, int nLength);
	/*BOOL OnGetFindResult(SOCKET s);*/
	BOOL OnGetListHeadData(SOCKET s);
	BOOL OnGetFindListInData(SOCKET s);
	//BOOL ConformPkt(SOCKET s, char* szBuf, int nHeadBytes, int nCountRow);

	BOOL OnSendListHead(SOCKET s);

	BOOL DoCommand(char* pszCmd);
	void ShowRecord(_RecordsetPtr& rs);
	BOOL TurnEnableButton(BOOL bConEnable);
// 对话框数据
	enum { IDD = IDD_STUMANAGESYSTEMSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_CtrlComboIPAddr;
	UINT m_nPort;
	afx_msg void OnBnClickedButtonBegin();
	afx_msg void OnClose();
//	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonEnd();
};
