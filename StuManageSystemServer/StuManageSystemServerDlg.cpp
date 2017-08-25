
// StuManageSystemServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StuManageSystemServer.h"
#include "StuManageSystemServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define CONN_STR "Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=db_StuManageSystem;Data Source=."

class CRelease
{
public:
	CRelease()
	{
		//��ʼ��
		CoInitialize(NULL);
	}
	~CRelease()
	{
		//����ʼ��
		CoUninitialize();
	}
}g_Release;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CStuManageSystemServerDlg �Ի���



CStuManageSystemServerDlg::CStuManageSystemServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStuManageSystemServerDlg::IDD, pParent)
	, m_nPort(0)
{
	m_sServer = NULL;
	m_sClient = NULL;
	m_pContext = NULL;
	m_pStuInfoNoName = new tagStuInfoNoName;
	memset(m_pStuInfoNoName, 0, sizeof(tagStuInfoNoName));
	m_pszName = new char[CONFIELD_NAMELEN];
	memset(m_pszName, 0, CONFIELD_NAMELEN);
	m_pszSex = new char[5];
	memset(m_pszSex, 0, 5);
	m_bIsFirstFind = TRUE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStuManageSystemServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_IP, m_CtrlComboIPAddr);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
}

BEGIN_MESSAGE_MAP(CStuManageSystemServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BEGIN, &CStuManageSystemServerDlg::OnBnClickedButtonBegin)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_END, &CStuManageSystemServerDlg::OnBnClickedButtonEnd)
END_MESSAGE_MAP()


// CStuManageSystemServerDlg ��Ϣ�������

BOOL CStuManageSystemServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	BOOL bRet = initFrame();
	if (!bRet)
	{
		return FALSE;
	}
	bRet = OnComEnvi();
	if (!bRet)
	{
		return FALSE;
	}
	TurnEnableButton(TRUE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CStuManageSystemServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CStuManageSystemServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CStuManageSystemServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CStuManageSystemServerDlg::OnBnClickedButtonBegin()
{
	TurnEnableButton(FALSE);
	BOOL bRet = CreateSocketAndAccept();
	if (bRet == FALSE)
	{
		return;
	}
	//2.���ⴴ��һ���̣߳�ר�����ڽ�������
	AfxBeginThread((AFX_THREADPROC)RecvFunc,
				   (LPVOID)this);

}

BOOL CStuManageSystemServerDlg::OnComEnvi()
{
	//������m_conn��ȫ�̶��ã��ڹرմ���ʱ������ر�
	HRESULT hr = m_conn.CreateInstance("adodb.Connection");
	if (FAILED(hr))
	{
		OutputDebugString(TEXT("conn err"));
		return FALSE;
	}
	//��
	hr = m_conn->Open(CONN_STR, "", "", adConnectUnspecified);
	if (FAILED(hr))
	{
		OutputDebugString(TEXT("conn err"));
		return FALSE;
	}
	return TRUE;
}

BOOL CStuManageSystemServerDlg::initFrame()
{
	char szHostName[MAXBYTE] = { '\0' };
	int nRet = gethostname(szHostName, MAXBYTE);
	hostent FAR * pHost = { 0 };
	pHost = gethostbyname(szHostName);

	char* pszName = pHost->h_name;
	int n = (int)(pHost->h_name - pHost->h_addr_list[0]) / 4;

	m_CtrlComboIPAddr.AddString(TEXT("127.0.0.1"));
	for (int i = 0; i < n; i++)
	{
		//char* szIP = NULL;
		in_addr addr = { 0 };
		char* pIPAddr = pHost->h_addr_list[i];
		addr.S_un.S_addr = (DWORD&)*pIPAddr;//*(DWORD*)pIPAddr;//
		char* szIP = inet_ntoa(addr);
		CString str(szIP);
		m_CtrlComboIPAddr.AddString(str);
	}

	m_CtrlComboIPAddr.SetCurSel(0);
	m_nPort = 8889;
	UpdateData(FALSE);
	//TurnEnableButton(TRUE);
	return TRUE;
}

BOOL CStuManageSystemServerDlg::CreateSocketAndAccept()
{
	m_sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//u_long iMode = 1;  //non-blocking mode is enabled.
	//ioctlsocket(m_sServer, FIONBIO, &iMode); //����Ϊ������ģʽ
	if (m_sServer == INVALID_SOCKET)
	{
		return FALSE;
	}
	UpdateData(TRUE);
	sockaddr_in addr = { 0 };
	int nIndex = m_CtrlComboIPAddr.GetCurSel();
	WCHAR szText[MAXBYTE] = { '\0' };
	char szMulText[MAXBYTE] = { '\0' };
	//CString str;
	
#ifdef UNICODE
	int nByteGet = m_CtrlComboIPAddr.GetLBText(nIndex, (LPTSTR)szText);
	WideCharToMultiByte(CP_ACP, 0, szText, -1,
		szMulText, MAXBYTE, NULL, NULL);
#else
	int nByteGet = m_CtrlComboIPAddr.GetLBText(nIndex, (LPTSTR)szMulText);
#endif // UNICODE

	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_nPort);
	addr.sin_addr.S_un.S_addr = inet_addr(szMulText);//IP��ַ

	int nRet = bind(m_sServer, (sockaddr*)&addr, sizeof(sockaddr_in));

	if (nRet == SOCKET_ERROR)
	{
		return FALSE;
	}
	nRet = listen(m_sServer, SOMAXCONN);
	if (nRet == SOCKET_ERROR)
	{
		return FALSE;
	}

	int nLength = sizeof(sockaddr);
	sockaddr_in recvAddr = { 0 };
	recvAddr.sin_family = AF_INET;

	m_sClient = accept(m_sServer, (sockaddr*)&recvAddr, &nLength);
	if (m_sClient == SOCKET_ERROR)
	{
		return FALSE;
	}
	char szBuf[MAXBYTE] = {'\0'};
	(DWORD&)*szBuf = SF_SUCCESS;
	int nSize = sizeof(DWORD);
	BOOL bRet = SendPacket(m_sClient, NULL, szBuf, nSize, MT_SERVER_LOGIN_RLY);
	if (bRet == FALSE)
	{
		return FALSE;
	}
	//�����µ��׽��ְ������պͷ�������
	//�м�㷢�����ݸ������ʱ���ü��Ϸ��͵��׽��֣����㷢��ʱ֪��Ҫ�����Ǹ��ͻ���
	//Ϊʹ�м�㷢�ͺͽ����ֽ�ͬһ����Ҫ�ڷ��������ݸ��ͻ���ʱ��Ҳ��һ��ͬ�ֽ�����ռ��
	//Ϊ���ǲ�����ͳһ
	m_pContext = new CClientContext(m_sClient, recvAddr);
	if (m_pContext == NULL)
	{
		return FALSE;
	}
	return TRUE;
}

DWORD CStuManageSystemServerDlg::RecvFunc(LPVOID lpParam)
{
	CStuManageSystemServerDlg* pDlg = (CStuManageSystemServerDlg*)lpParam;
	BOOL bRet = FALSE;
	while (TRUE)
	{
		char* pszBuf = NULL;
		bRet = pDlg->OnGetReadBuf(pDlg->m_sClient, pszBuf);
		if (bRet == FALSE)
		{
			continue;
		}
		ASSERT(pszBuf != NULL);
		switch (pszBuf[4])
		{
		case MT_CLIENT_LOGIN:
			{
				
			}
			break;
		case MT_CLIENT_ADD:
			{
				//bRet = pDlg->OnMT_CLIENT_ADD(pszBuf);
				bRet = pDlg->OnMT_CLIENT(&CStuManageSystemServerDlg::OnAddInfoStr,
					pszBuf);
			}
			break;
		case MT_CLIENT_DEL:
			{
				bRet = pDlg->OnMT_CLIENT(&CStuManageSystemServerDlg::OnDelInfoStr,
										pszBuf);
			}
			break;
		case MT_CLIENT_ALTER:
			{
				bRet = pDlg->OnMT_CLIENTALTER(pszBuf);					
			}
			break;
		case MT_CLIENT_FIND:
			{
				BOOL bRet = FALSE;
				DWORD dwIsFirstFind = (DWORD&)*(pszBuf + 9);
				DWORD dwLength = (DWORD&)(*pszBuf);
				if (dwIsFirstFind == FI_FIRST)
				{
					SOCKET s = (SOCKET&)*(pszBuf + 5);
					bRet = pDlg->OnSendListHead(s);
				}
				bRet = pDlg->OnFindStuInfo(pszBuf + 5, dwLength - 5);
			}
			break;
		}
		DWORD dwRequiredSize = (DWORD&)*pszBuf;
		CClientContext* pContext = pDlg->m_pContext;
		ASSERT(pContext != NULL);
		memmove(pContext->m_Packet.m_pBuf,
			pContext->m_Packet.m_pBuf + dwRequiredSize,
			pContext->m_Packet.m_nLength - dwRequiredSize);
		pContext->m_Packet.m_nLength -= dwRequiredSize;
	}
	return 0;
}

//���º�������ûȫ�����ܣ����ܻ���ַ���������ȫ�������꣬
//���ܽ��ܵ��ַ���,��˸ú����õ�ͦ��Σ��ѽ��յ��ַ�������
//Ȼ�����������ѭ�����õĹ���д�����º����и���
BOOL CStuManageSystemServerDlg::OnGetReadBuf(SOCKET s, char*& pszBuf)
{
	int i = 0;//5�ζ�û�㶨�����Զ��˳�
	while (i < 5)
	{
		int nRecvedBytes = recv(s,
			(char*)m_pContext->m_szRecvBuf,
			RECV_BYTES,
			0);//��������װ�Ľ��պ��������ﻹ����д
		//2���յ������ݼ��뵽��������,ÿ���׽����ж����ַ�������
		BOOL bRet = FALSE;
		if (nRecvedBytes > 0)
		{
			m_pContext->m_Packet.Append((char*)m_pContext->m_szRecvBuf,
				nRecvedBytes);
			//3��黺�����Ĵ�С,������ȴﵽĿ��,��ô��ʼ���ô�����
			bRet = m_pContext->m_Packet.Handle(s);
		}
		if (bRet == TRUE)
		{
			pszBuf = m_pContext->m_Packet.GetBuf();
			break ;
		}
		i++;
		if (i == 4)
		{
			return FALSE;
		}
	}
	return TRUE;
}

//SOCKET sAdd��������,StrLen��pszStr�������ֽڳ�
BOOL CStuManageSystemServerDlg::SendPacket(SOCKET s, SOCKET sAdd, char* pszStr,
	DWORD StrLen, int nMsg)
{
	if (s == NULL)
	{
		return FALSE;
	}
	CMySocket sendSkt(s);
	CMyPacket Sendpkt;

	char szHead[9] = { 0 };//ͷ�������׽��ֹ��Ÿ��ֽ�
	DWORD dwSize = sizeof(DWORD)+sizeof(BYTE)+(StrLen + 1) * sizeof(char)+sizeof(SOCKET);//����Ϣ�������һ���׽��֣���ҪΪ��Ӧ�м��
	(DWORD&)*szHead = dwSize;
	szHead[4] = nMsg;
	(SOCKET&)*(szHead + 5) = sAdd;//���ն˻�������

	BOOL bRet = Sendpkt.Append(szHead, sizeof(szHead));
	bRet = Sendpkt.Append((char*)pszStr, (StrLen + 1) * sizeof(char));

	if (bRet == FALSE)
	{
		return bRet;
	}

	int nRet = sendSkt.SendPkt(Sendpkt);
	if (nRet != SOCKET_SUCCESS)
	{
		return bRet;
	}
	return TRUE;
}

#define ADDINFOSTR "INSERT INTO ѧ����Ϣ�� (ѧ��ѧ��, ѧ������, ѧ���Ա�, ѧ�����)\
VALUES('%d', '%s', '%s', %.2f)", m_pStuInfoNoName->m_dwID, m_pszName, \
m_pszSex, m_pStuInfoNoName->m_fHeight

//ɾ��һ��ѧ�� �� ����ѧ��
#define DELINFOSTRONE "DELETE ѧ����Ϣ�� WHERE ѧ��ѧ�� = '%d' and ѧ������ = '%s'",\
m_pStuInfoNoName->m_dwID, m_pszName
#define DELINFOSTRONEALL "DELETE ѧ����Ϣ��"

//�޸�ѧ����������
#define ALTERINFOSTRONENAME "UPDATE ѧ����Ϣ�� SET ѧ������ = '%s'\
WHERE ѧ��ѧ�� = '%d'", m_pszName, m_pStuInfoNoName->m_dwID
//��ߡ�
#define ALTERINFOSTRONEHEIGHT "UPDATE ѧ����Ϣ�� SET ѧ����� = '%.2f'\
WHERE ѧ��ѧ�� = '%d'", m_pStuInfoNoName->m_fHeight, m_pStuInfoNoName->m_dwID
//�Ա�
#define ALTERINFOSTRONESEX "UPDATE ѧ����Ϣ�� SET ѧ���Ա� = '%s'\
WHERE ѧ��ѧ�� = '%d'", m_pszSex, m_pStuInfoNoName->m_dwID

BOOL CStuManageSystemServerDlg::OnAddInfoStr(char* pszCmd, int nCmdLen)
{
	sprintf_s(pszCmd, nCmdLen, ADDINFOSTR);
	return TRUE;
}

BOOL CStuManageSystemServerDlg::OnDelInfoStr(char* pszCmd, int nCmdLen)
{
	if (m_pStuInfoNoName->m_dwID != 0 && strlen(m_pszName) != 0)
	{
		sprintf_s(pszCmd, nCmdLen, DELINFOSTRONE);
	}
	else if (m_pStuInfoNoName->m_dwID == 0 && strlen(m_pszName) == 0)
	{
		sprintf_s(pszCmd, nCmdLen, DELINFOSTRONEALL);
	}
	return TRUE;
}

//Ҫ�޸�����ʱ���ַ�������
BOOL CStuManageSystemServerDlg::OnAlterInfoStrName(char* pszCmd, int nCmdLen)
{
	ASSERT(m_pStuInfoNoName->m_dwID != 0);
	if (strlen(m_pszName) != 0)
	{
		sprintf_s(pszCmd, nCmdLen, ALTERINFOSTRONENAME);
		return TRUE;
	}
	return FALSE;
}

//Ҫ�޸����ʱ���ַ�������
BOOL CStuManageSystemServerDlg::OnAlterInfoStrHeight(char* pszCmd, int nCmdLen)
{
	ASSERT(m_pStuInfoNoName->m_dwID != 0);
	if (m_pStuInfoNoName->m_fHeight != 0)
	{
		sprintf_s(pszCmd, nCmdLen, ALTERINFOSTRONEHEIGHT);
		return TRUE;
	}
	return FALSE;
}

//Ҫ�޸��Ա�ʱ���ַ�������
BOOL CStuManageSystemServerDlg::OnAlterInfoStrSex(char* pszCmd, int nCmdLen)
{
	ASSERT(m_pStuInfoNoName->m_dwID != 0);
	if (m_pStuInfoNoName->m_dwSex != 0)
	{
		sprintf_s(pszCmd, nCmdLen, ALTERINFOSTRONESEX);
		return TRUE;
	}
	return FALSE;
}

BOOL CStuManageSystemServerDlg::OnFindInfoStr(char* pszCmd, int nCmdLen)
{
	return TRUE;
}

BOOL CStuManageSystemServerDlg::OnOneKindDone(pMemFun pFunc, char* szBuf, DWORD dwLength)
{
	DWORD dwNameLen = (DWORD&)*szBuf;
	//char szName[MAXBYTE] = { '\0' };
	memset(m_pszName, 0, CONFIELD_NAMELEN);
	memmove(m_pszName, szBuf + 4, dwNameLen);//��ȡ����
	memmove(m_pStuInfoNoName, szBuf + 4 + dwNameLen, sizeof(tagStuInfoNoName));//��ȡ�������ֶ���Ϣ
	//������ݵ����ݿ���
	ASSERT(dwLength <= MAXBYTE);
	char pszCmd[MAXBYTE] = { '\0' };
	//char szSex[10] = { '\0' };
	if (m_pStuInfoNoName == NULL)
	{
		return FALSE;
	}
	memset(m_pszSex, 0, 5);
	if (m_pStuInfoNoName->m_dwSex == SK_MAN)
	{
		strcpy_s(m_pszSex, 5, "��");
	}
	else if (m_pStuInfoNoName->m_dwSex == SK_WOMAN)
	{
		strcpy_s(m_pszSex, 5, "Ů");
	}
	//��Ҫ����Ĳ���ȫ��д��һ������
	BOOL bRet = (this->*pFunc)(pszCmd, MAXBYTE);
	if (!bRet)
	{
		return FALSE;
	}
	//sprintf_s(pszCmd, MAXBYTE, ADDINFOSTR);
	bRet = DoCommand(pszCmd);
	if (!bRet)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CStuManageSystemServerDlg::OnMT_CLIENT(pMemFun pFunc, char* pszBuf)
{
	SOCKET s = (SOCKET&)*(pszBuf + 5);
	DWORD dwLength = (DWORD&)*pszBuf;
	//BOOL bRet = OnAddStu(pszBuf + 9, dwLength - 9);
	BOOL bRet = OnOneKindDone(pFunc, pszBuf + 9, dwLength - 9);
	char szBuf[MAXBYTE] = { '\0' };
	int nSize = 0;
	if (bRet)
	{
		(DWORD&)*szBuf = SF_SUCCESS;
	}
	else if (!bRet)
	{
		(DWORD&)*szBuf = SF_FAIL;
	}
	nSize = sizeof(DWORD);
	SendPacket(m_sClient, s, szBuf,
		nSize, MT_SERVER_EXEC_RLY);

	return bRet;
}

BOOL CStuManageSystemServerDlg::OnMT_CLIENTALTER(char* pszBuf)
{
	SOCKET s = (SOCKET&)*(pszBuf + 5);
	DWORD dwLength = (DWORD&)*pszBuf;
	BOOL bRetTot = FALSE;
	BOOL bRet = OnOneKindDone(&CStuManageSystemServerDlg::OnAlterInfoStrName, pszBuf + 9, dwLength - 9);
	if (bRet == TRUE)
	{
		bRetTot = bRet;
	}
	
	bRet = OnOneKindDone(&CStuManageSystemServerDlg::OnAlterInfoStrHeight, pszBuf + 9, dwLength - 9);
	if (bRet == TRUE)
	{
		bRetTot = bRet;
	}
	bRet = OnOneKindDone(&CStuManageSystemServerDlg::OnAlterInfoStrSex, pszBuf + 9, dwLength - 9);
	if (bRet == TRUE)
	{
		bRetTot = bRet;
	}
	char szBuf[MAXBYTE] = { '\0' };
	int nSize = 0;
	if (bRetTot)
	{
		(DWORD&)*szBuf = SF_SUCCESS;
	}
	else if (!bRetTot)
	{
		(DWORD&)*szBuf = SF_FAIL;
	}
	nSize = sizeof(DWORD);
	SendPacket(m_sClient, s, szBuf,
		nSize, MT_SERVER_EXEC_RLY);
	return bRet;
}

BOOL CStuManageSystemServerDlg::OnFindStuInfo(char* szBufWiths, DWORD dwLengthWiths)
{
	SOCKET s = (SOCKET&)(*szBufWiths);
	char* szBuf = szBufWiths + 4 + 4;//�м仹�и����
	DWORD dwLength = dwLengthWiths - 4 -4;//�м仹�и����
	DWORD dwNameLen = (DWORD&)*szBuf;
	//char szName[MAXBYTE] = {'\0'};
	memset(m_pszName, 0, CONFIELD_NAMELEN);
	memmove(m_pszName, szBuf + 4, dwNameLen);//��ȡ����
	memmove(m_pStuInfoNoName, szBuf + 4 + dwNameLen, sizeof(tagStuInfoNoName));//��ȡ�������ֶ���Ϣ
	//������ݵ����ݿ���
	ASSERT(dwLength <= MAXBYTE);
	char pszCmd[MAXBYTE] = {'\0'};
	memset(m_pszSex, 0, 5);
	if (m_pStuInfoNoName == NULL)
	{
		return FALSE;
	}
	if (m_pStuInfoNoName->m_dwSex == SK_MAN)
	{
		strcpy_s(m_pszSex, 5, "��");
	}
	else if (m_pStuInfoNoName->m_dwSex == SK_WOMAN)
	{
		strcpy_s(m_pszSex, 5, "Ů");
	}
	//��Ҫ����Ĳ���ȫ��д��һ������
	//sprintf_s(pszCmd, MAXBYTE, ADDINFOSTR);
	OnPutInfoInBuf(pszCmd, MAXBYTE);
	BOOL bRet = DoCommand(pszCmd);
	if (!bRet)
	{
		return FALSE;
	}

	bRet = OnGetFindListInData(s);//�ڸú��������з�������
	if (!bRet)
	{
		return FALSE;
	}
	return TRUE;
}

#define PSZCMD "SELECT * FROM ѧ����Ϣ�� \
WHERE ѧ������ %s and ѧ��ѧ�� %s and ѧ����� %s and ѧ���Ա� %s",\
szName, szID, szHeight, szSex

BOOL CStuManageSystemServerDlg::OnPutInfoInBuf(char* szBuf, int nLength)
{
	char szID[MAXBYTE] = "> 0";
	char szName[MAXBYTE] = "LIKE '%%'";
	char szHeight[MAXBYTE] = "> 0";
	char szSex[MAXBYTE] = "LIKE '%%'";
	if (m_pStuInfoNoName == NULL)
	{
		return FALSE;
	}
	if (m_pStuInfoNoName->m_dwID > 0)
	{
		sprintf_s(szID, MAXBYTE, "= %d", m_pStuInfoNoName->m_dwID);
	}
	if (strlen(m_pszName) > 0)
	{
		sprintf_s(szName, MAXBYTE, "LIKE '%s'", m_pszName);
	}
	if ((m_pStuInfoNoName->m_fHeight > 0) && (m_pStuInfoNoName->m_fHeightMax > 0) &&\
		(m_pStuInfoNoName->m_fHeightMax > m_pStuInfoNoName->m_fHeight))
	{
		sprintf_s(szHeight, MAXBYTE, "BETWEEN %f AND %f", \
			m_pStuInfoNoName->m_fHeight, m_pStuInfoNoName->m_fHeightMax);
	}
	if (strlen(m_pszSex) > 0)
	{
		sprintf_s(szSex, MAXBYTE, "='%s'", m_pszSex);
	}
	
	sprintf_s(szBuf, nLength, PSZCMD);
	return FALSE;
}


BOOL CStuManageSystemServerDlg::OnGetListHeadData(SOCKET s)
{
	int nPtrBuf = 0;
	int nCount = m_rs->Fields->GetCount();
	char pszName[CONFIELD_NAMELEN] = {'\0'};
	memset(pszName, 0, CONFIELD_NAMELEN);
		
	char* szBuf = new char[CONFIELD_ONEROWLEN];
	memset(szBuf, 0, CONFIELD_ONEROWLEN);
	nPtrBuf = 8;
	for (int i = 0; i < nCount; i++)
	{
		memset(pszName, 0, CONFIELD_NAMELEN);
		sprintf_s(pszName, CONFIELD_NAMELEN, "%s", (char*)m_rs->Fields->Item[(long)i]->Name);
		int nNameLen = strlen(pszName);
		szBuf[nPtrBuf] = nNameLen;
		nPtrBuf += 1;
		sprintf_s(szBuf + nPtrBuf, CONFIELD_ONEROWLEN - nPtrBuf, "%s", pszName);
		nPtrBuf += nNameLen;
	}
	(DWORD&)*szBuf = FI_FIRST;//
	(DWORD&)*(szBuf + 4) = nPtrBuf - 8;//�洢��ʵ���ݵĳ���
	BOOL BRet = SendPacket(m_sClient, s, szBuf, nPtrBuf, MT_SERVER_FIND_RLY);
	if (szBuf != NULL)
	{
		delete szBuf;
		szBuf = NULL;
	}
	return BRet;
	//return 
	
}


BOOL CStuManageSystemServerDlg::OnGetFindListInData(SOCKET s)
{
	/*memset(m_FindResultPkt.GetBuf(), 0, m_FindResultPkt.m_nSize);*/
	char szBufRow[CONFIELD_ONEROWLEN] = {'\0'};//�����������
	memset(szBufRow, 0, CONFIELD_ONEROWLEN);
	char pszValue[CONFIELD_NAMELEN] = { '\0' };
	int nCountRow = 0;

	int nCount = m_rs->Fields->GetCount();//�ֶ���
	//BOOL bIsFirstAppend = TRUE;
	CString StrBuf;
	while (!m_rs->MyEOF)
	{
		int nPtrRow = 0;
		memset(szBufRow, 0, CONFIELD_ONEROWLEN);
		//����н���ķ�ʽ�ǣ� 1�ֽڴ泤�� ��� ������
		for (int i = 0; i < nCount; i++)
		{
			memset(pszValue, 0, CONFIELD_VALUELEN);
			sprintf_s(pszValue, 10, "%s", (char*)(_bstr_t)m_rs->Fields->Item[(long)i]->Value);

			int nValueLen = strlen(pszValue);
			szBufRow[nPtrRow] = nValueLen;
			nPtrRow += 1;
			sprintf_s(szBufRow + nPtrRow, 20, "%s", pszValue);
			nPtrRow += nValueLen;
		}
		StrBuf += szBufRow;
		/*if (bIsFirstAppend == TRUE)
		{
		m_FindResultPkt.SetBuf(g_szBufRow, nPtrRow * 3, nPtrRow);
		bIsFirstAppend = FALSE;
		}
		else
		{
		m_FindResultPkt.Append(g_szBufRow, nPtrRow);
		}*/
	
		m_rs->MoveNext();
		nCountRow++;
	}
	/*if (g_szBufRow != NULL)
	{
		delete g_szBufRow;
		g_szBufRow = NULL;
	}*/
	//�����ϰ������ͳ�ȥ
	int nTotalRsltBytes = 0;
	DWORD dwDataLen = StrBuf.GetLength();
	nTotalRsltBytes = 4 + 4 + 4 + dwDataLen;//���ϴ�Ÿ��е��ֽ���,4�ֽ���������������,�������ĸ��ֽ������ݳ���,4�ֽڱ��
	char* pszResult = new char[nTotalRsltBytes + 1];
	memset(pszResult, 0, nTotalRsltBytes + 1);

	(DWORD&)*pszResult = FI_NOTFIRST;
	(DWORD&)*(pszResult + 4) = dwDataLen;//����й���ѧ�����������ֽڳ�
	(DWORD&)*(pszResult + 8) = nCountRow;//�����ѧ���ĸ���

	memmove(pszResult + 12, StrBuf.GetBuffer(0), StrBuf.GetLength());

	SendPacket(m_sClient, s, pszResult, nTotalRsltBytes + 1, MT_SERVER_FIND_RLY);
	if (pszResult != NULL)
	{
		delete pszResult;
		pszResult = NULL;
	}

	return TRUE;
}

//BOOL CStuManageSystemServerDlg::ConformPkt(SOCKET s, char* szBuf, int nHeadBytes, int nCountRow)
//{
//	
//	return TRUE;
//}


//���º�������ִ�и�������
BOOL CStuManageSystemServerDlg::DoCommand(char* pszCmd)
{
	try
	{
		//����ʵ����
		_variant_t t;
		m_rs = m_conn->Execute(pszCmd, &t, adCmdText);
		//AfxMessageBox(TEXT("�����ɹ���"));ͳһ�����洦��
		return TRUE;
	}
	catch (_com_error e)
	{
		_bstr_t bstr = e.Description();
		TRACE("%d", e.WCode());
		//AfxMessageBox(TEXT("����ʧ�ܣ�"));
		return FALSE;
	}
}

BOOL CStuManageSystemServerDlg::OnMT_CLIENTFIND(char* pszBuf)
{
	return FALSE;
}

void CStuManageSystemServerDlg::ShowRecord(_RecordsetPtr& rs)
{
	int nCount = rs->Fields->GetCount();
	//��ӡ��
	for (int i = 0; i < nCount; i++)
	{
		printf("%s ", (char*)rs->Fields->Item[(long)i]->Name);
	}

	printf("\r\n");
	//��ӡ����
	while (!rs->MyEOF)
	{
		for (int i = 0; i < nCount; i++)
		{
			printf("%s ", (char*)(_bstr_t)rs->Fields->Item[(long)i]->Value);
		}
		printf("\r\n");

		rs->MoveNext();
	}
}

//��һ�β�ѯʱʱ����ͷ����Ϣ
BOOL CStuManageSystemServerDlg::OnSendListHead(SOCKET s)
{
	int nTest = sizeof(_RecordsetPtr);
	char szBuf[MAXBYTE] = "SELECT * FROM ѧ����Ϣ�� WHERE ѧ��ѧ�� = 0";
	BOOL bRet = DoCommand(szBuf);
	return OnGetListHeadData(s);
}

//void CStuManageSystemServerDlg::OnBnClickedButton1()
//{
//	SOCKET s = 345;
//	//OnGetFindResult(s);
//}

BOOL CStuManageSystemServerDlg::TurnEnableButton(BOOL bConEnable)
{
	BOOL bRuptEnable = TRUE;
	if (bConEnable == TRUE)
	{
		bRuptEnable = FALSE;
	}

	CWnd *pWndStart = GetDlgItem(IDC_BUTTON_BEGIN); // ȡ�ÿؼ���ָ��  
	HWND hWndStart = pWndStart->GetSafeHwnd(); // ȡ�ÿؼ��ľ��  
	BOOL bRet = ::EnableWindow(hWndStart, bConEnable);

	CWnd *pWndEnd = GetDlgItem(IDC_BUTTON_END); // ȡ�ÿؼ���ָ��  
	HWND hWndEnd = pWndEnd->GetSafeHwnd(); // ȡ�ÿؼ��ľ��  
	bRet = ::EnableWindow(hWndEnd, bRuptEnable);
	return TRUE;
}

void CStuManageSystemServerDlg::OnBnClickedButtonEnd()
{
	TurnEnableButton(TRUE);
	closesocket(m_sServer);
	closesocket(m_sClient);
}

void CStuManageSystemServerDlg::OnClose()
{
	//�ͷŵ�new����Դ
	if (m_pContext != NULL)
	{
		delete m_pContext;
		m_pContext = NULL;
	}
	if (m_pStuInfoNoName != NULL)
	{
		delete m_pStuInfoNoName;
		m_pStuInfoNoName = NULL;
	}
	if (m_pszName != NULL)
	{
		delete m_pszName;
		m_pszName = NULL;
	}
	if (m_pszSex != NULL)
	{
		delete m_pszSex;
		m_pszSex = NULL;
	}
	//�ر��׽�����Դ
	closesocket(m_sServer);
	closesocket(m_sClient);
	//m_rs->Close();

	CDialogEx::OnClose();
}
