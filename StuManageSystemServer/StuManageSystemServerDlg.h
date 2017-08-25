
// StuManageSystemServerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "../../common/ClientManager.h"
//�����������ϣ�����ø�һ���ˣ���Ϊ·����ͬ
#import "C:\Windows\winsxs\x86_microsoft-windows-m..ents-mdac-ado15-dll_31bf3856ad364e35_6.1.7601.17857_none_0e101351cf067d1f\MSADO15.DLL" no_namespace rename("EOF", "MyEOF")

class CStuManageSystemServerDlg;
typedef BOOL (CStuManageSystemServerDlg::*pMemFun)(char* , int);

// CStuManageSystemServerDlg �Ի���
class CStuManageSystemServerDlg : public CDialogEx
{
// ����
public:
	CStuManageSystemServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

//�Զ������
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
//�Զ��庯��
	BOOL initFrame();
	BOOL OnComEnvi();
	BOOL CreateSocketAndAccept();
	static DWORD RecvFunc(LPVOID lpParam);
	BOOL OnGetReadBuf(SOCKET s, char*& pszBuf);
	BOOL SendPacket(SOCKET s, SOCKET sAdd, char* pszStr, DWORD StrLen, int nMsg);//SOCKET sAdd��������,StrLen��pszStr�������ֽڳ�
	
	/*BOOL OnMT_CLIENT_ADD(char* szBuf);*/
	//���������Ա�����������й��ܺ����鵽���º������ɼ��ٴ�����д
	BOOL OnOneKindDone(pMemFun pFunc, char* szBuf, DWORD dwLength);
	BOOL OnMT_CLIENT(pMemFun pFunc, char* szBuf);

	BOOL OnAddInfoStr(char* pszCmd, int nCmdLen);
	BOOL OnDelInfoStr(char* pszCmd, int nCmdLen);
	BOOL OnAlterInfoStr(char* pszCmd, int nCmdLen);
	//Ҫ�޸�����ʱ���ַ�������
	BOOL OnAlterInfoStrName(char* pszCmd, int nCmdLen);
	//Ҫ�޸����ʱ���ַ�������
	BOOL OnAlterInfoStrHeight(char* pszCmd, int nCmdLen);
	//Ҫ�޸��Ա�ʱ���ַ�������
	BOOL OnAlterInfoStrSex(char* pszCmd, int nCmdLen);
	//��Ϊ�޸ıȽ��鷳����дһ��������
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
// �Ի�������
	enum { IDD = IDD_STUMANAGESYSTEMSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
