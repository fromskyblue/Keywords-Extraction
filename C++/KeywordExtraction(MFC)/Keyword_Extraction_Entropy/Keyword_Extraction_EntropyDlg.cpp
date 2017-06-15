
// Keyword_Extraction_EntropyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Keyword_Extraction_Entropy.h"
#include "Keyword_Extraction_EntropyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKeyword_Extraction_EntropyDlg 对话框




CKeyword_Extraction_EntropyDlg::CKeyword_Extraction_EntropyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKeyword_Extraction_EntropyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_txtaddress = _T("");
	//  m_txtorigin = _T("");
}

void CKeyword_Extraction_EntropyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_txtaddress);
	//  DDX_Text(pDX, IDC_TXTORIGIN, m_txtorigin);
	DDX_Control(pDX, IDC_KEYWORD, m_keywordshow);

	DDX_Control(pDX, IDC_RICHEDIT21, m_richedit);
	DDX_Control(pDX, IDC_RICHEDIT22, m_status);
}

BEGIN_MESSAGE_MAP(CKeyword_Extraction_EntropyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CKeyword_Extraction_EntropyDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_KEYWORDSAVE, &CKeyword_Extraction_EntropyDlg::OnBnClickedKeywordsave)
	ON_BN_CLICKED(IDC_BUTTON2, &CKeyword_Extraction_EntropyDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_PRETREAMENT, &CKeyword_Extraction_EntropyDlg::OnBnClickedPretreament)
	ON_BN_CLICKED(IDC_KEYWORDEXTR, &CKeyword_Extraction_EntropyDlg::OnBnClickedKeywordextr)
	ON_BN_CLICKED(IDC_QUICK, &CKeyword_Extraction_EntropyDlg::OnBnClickedQuick)
	ON_EN_CHANGE(IDC_EDIT1, &CKeyword_Extraction_EntropyDlg::OnEnChangeEdit1)
//	ON_UPDATE_COMMAND_UI(ID_WIZFINISH, &CKeyword_Extraction_EntropyDlg::OnUpdateWizfinish)
	 ON_MESSAGE(WM_MyMessage, OnMyMessage)
	 ON_BN_CLICKED(IDC_STOPWORD, &CKeyword_Extraction_EntropyDlg::OnBnClickedStopword)
//	 ON_NOTIFY(NM_CLICK, IDC_KEYWORD, &CKeyword_Extraction_EntropyDlg::OnNMClickKeyword)
END_MESSAGE_MAP()


// CKeyword_Extraction_EntropyDlg 消息处理程序

BOOL CKeyword_Extraction_EntropyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
		CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
	cf.dwEffects = 0;
	cf.yHeight = 12*12;//文字高度
	cf.crTextColor = RGB(200, 100, 255); //文字颜色
	m_richedit.SetBackgroundColor(0,RGB(240,247,233));
	m_richedit.SetSel(1, 5); //设置处理区域
	m_richedit.SetSelectionCharFormat(cf);


	
	m_status.SetBackgroundColor(0,RGB(240,247,233));
	m_status.SetSel(1, 5); //设置处理区域
	m_status.SetSelectionCharFormat(cf);





	WORD dwStyle = m_keywordshow.GetExtendedStyle();                    //添加列表框的网格线！！！

	dwStyle |= LVS_EX_FULLROWSELECT;            
	dwStyle |= LVS_EX_GRIDLINES;   


	m_keywordshow.SetExtendedStyle(dwStyle);
	m_keywordshow.SetBkColor(RGB(240,247,233));             //选中此列是的颜色
	m_keywordshow.SetTextBkColor(RGB(240,247,233));

	m_keywordshow.InsertColumn(0,_T("序号"),LVCFMT_CENTER,37);
	m_keywordshow.InsertColumn(1,_T("词项"),LVCFMT_CENTER,80);
	m_keywordshow.InsertColumn(2,_T("EDnor"),LVCFMT_CENTER,80);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKeyword_Extraction_EntropyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CKeyword_Extraction_EntropyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CKeyword_Extraction_EntropyDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilter = _T("Data Files (*.txt)|*.txt||");
	CFileDialog fileDlg(
		true,//若为true则表明创建的是一个打开文件对话框，若为False则创建的是一个保存文件对话框
		NULL,//添加到没有扩展名文件上的扩展名
		_T(""),//初始选定的文件名
		NULL,//是否创建带有提示框的保存文件对话框
		strFilter//files of type 中的过滤参数
		);
	int result = fileDlg.DoModal();//创建一个打开文件对话框

	if(result == IDOK)//如果用户点击OK按钮
	{	string address;
	    address=change(fileDlg.GetFolderPath());
		if (address[address.size()-1]=='\\')
		{
			address=address+change(fileDlg.GetFileName());
		}
		else 
			address=address+"\\"+change(fileDlg.GetFileName());
		
		m_txtaddress=address.c_str();

	UpdateData(false);//更新与编辑框关联的成员函数m_openFileName的值
	GetDlgItem(IDC_EDIT1)->SetWindowText(m_txtaddress);//将m_openFileName的内容显示在编辑框中


	//CClientDC dc(this);
	//CRect rc;
	//GetClientRect(&rc); //获得当前对话框的rc
	////初始坐标点
	//MoveToEx(dc, 50,540, NULL);
	//LineTo(dc,602, 540);
	//MoveToEx(dc, 50,540, NULL);
	//LineTo(dc,50, 600);
	//MoveToEx(dc, 50,600, NULL);
	//LineTo(dc,601, 600);
	//MoveToEx(dc, 601,600, NULL);
	//LineTo(dc,601, 540);
	}
}


void CKeyword_Extraction_EntropyDlg::OnBnClickedKeywordsave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilter = _T("Data Files (*.txt)|*.txt||");
	CFileDialog fileDlg(
		false,//若为true则表明创建的是一个打开文件对话框，若为False则创建的是一个保存文件对话框
		NULL,//添加到没有扩展名文件上的扩展名
		_T(""),//初始选定的文件名
		NULL,//是否创建带有提示框的保存文件对话框
		strFilter//files of type 中的过滤参数
		);
	int result = fileDlg.DoModal();//创建一个打开文件对话框

	if(result == IDOK)//如果用户点击OK按钮
	{string address;
	address=change(fileDlg.GetFolderPath());
	if (address[address.size()-1]=='\\')
	{
		address=address+change(fileDlg.GetFileName());
	}
	else 
		address=address+"\\"+change(fileDlg.GetFileName());

	savekeyword(address);
	m_status.SetWindowText(_T("关键词保存成功！"));
	
	}	
}


void CKeyword_Extraction_EntropyDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	
	string doc_address=change(m_txtaddress);
	string origintext=LoadDoc(doc_address);
	if(!origintext.empty())
	{doc=origintext;
	doc_pretreament=origintext;
	m_status.SetWindowText(_T("txt读取成功！"));
	GetDlgItem(IDC_KEYWORDEXTR)->EnableWindow(TRUE);
	GetDlgItem(IDC_PRETREAMENT)->EnableWindow(TRUE);
	GetDlgItem(IDC_QUICK)->EnableWindow(TRUE);

	CString strText;
	
	strText=doc.c_str();
	m_richedit.SetWindowText(strText );
	//InvalidateRect(NULL, TRUE);
	m_keywordshow.DeleteAllItems();
	}
	else
		m_status.SetWindowText(_T("输入的路径有误或txt内没有内容！请重新输入！"));
	
}


void CKeyword_Extraction_EntropyDlg::OnBnClickedPretreament()
{
	// TODO: 在此添加控件通知处理程序代码
	doc_pretreament=pre_treatment(doc);
	m_status.SetWindowText(_T("预处理完成！"));
}


void CKeyword_Extraction_EntropyDlg::OnBnClickedKeywordextr()
{
	// TODO: 在此添加控件通知处理程序代码
	m_status.SetWindowText(_T("正在进行关键词提取，请耐心等待（一篇19万字文章，需2—4分钟）"));
	UpdateData(TRUE);
	
	GetDlgItem(IDC_KEYWORDEXTR)->EnableWindow(FALSE);
	GetDlgItem(IDC_PRETREAMENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOPWORD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_QUICK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_QUICK)->EnableWindow(FALSE);
	GetDlgItem(IDC_KEYWORDSAVE)->EnableWindow(FALSE);
	m_keywordshow.DeleteAllItems();
	pThread=AfxBeginThread(ThreadFunc,0,0,0,NULL);

	
	
}


void CKeyword_Extraction_EntropyDlg::OnBnClickedQuick()
{
	// TODO: 在此添加控件通知处理程序代码
	
	m_status.SetWindowText(_T("正在进行关键词提取，请耐心等待（一篇19万字文章，需2—4分钟）"));
	UpdateData(TRUE);

	GetDlgItem(IDC_KEYWORDEXTR)->EnableWindow(FALSE);
	GetDlgItem(IDC_PRETREAMENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOPWORD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_QUICK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_QUICK)->EnableWindow(FALSE);
	GetDlgItem(IDC_KEYWORDSAVE)->EnableWindow(FALSE);

	m_keywordshow.DeleteAllItems();
	pThread=AfxBeginThread(ThreadFunc1,0,0,0,NULL);
	
	
}


void CKeyword_Extraction_EntropyDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	string address=change(m_txtaddress);
	 if (address.empty())
	 {
		 GetDlgItem(IDC_KEYWORDEXTR)->EnableWindow(FALSE);
		 GetDlgItem(IDC_PRETREAMENT)->EnableWindow(FALSE);
		 GetDlgItem(IDC_STOPWORD)->EnableWindow(FALSE);
		 GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		 GetDlgItem(IDC_QUICK)->EnableWindow(FALSE);
	 }
	 else 
	 GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	 
	// TODO:  在此添加控件通知处理程序代码
}


//void CKeyword_Extraction_EntropyDlg::OnUpdateWizfinish(CCmdUI *pCmdUI)
//{
//	// TODO: 在此添加命令更新用户界面处理程序代码
//	
//}

LRESULT CKeyword_Extraction_EntropyDlg::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	//MessageBox("recv msg success");
	//添加自己的消息处理
	int i=0;
	CString str;
	vector<Node> result_show=getresult();
	
    for(i=0;i<result_show.size();i++){
		str.Format(_T("%d"),i+1);
		m_keywordshow.InsertItem(i,str);

		str=result_show[i].word.c_str();
		m_keywordshow.SetItemText(i,1,str);

		str.Format(_T("%lf"),result_show[i].EDnor);
		m_keywordshow.SetItemText(i,2,str);
	}
	m_status.SetWindowText(_T("关键词提取完成！"));
	GetDlgItem(IDC_KEYWORDEXTR)->EnableWindow(TRUE);
	GetDlgItem(IDC_PRETREAMENT)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOPWORD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	GetDlgItem(IDC_QUICK)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	GetDlgItem(IDC_QUICK)->EnableWindow(TRUE);
	GetDlgItem(IDC_KEYWORDSAVE)->EnableWindow(TRUE);
	return 0;    
}

void CKeyword_Extraction_EntropyDlg::OnBnClickedStopword()
{
	// TODO: 在此添加控件通知处理程序代码
	vector<Node> delet_result;

	CString strFilter = _T("Data Files (*.txt)|*.txt||");
	CFileDialog fileDlg(
		true,//若为true则表明创建的是一个打开文件对话框，若为False则创建的是一个保存文件对话框
		NULL,//添加到没有扩展名文件上的扩展名
		_T(""),//初始选定的文件名
		NULL,//是否创建带有提示框的保存文件对话框
		strFilter//files of type 中的过滤参数
		);
	int result = fileDlg.DoModal();//创建一个打开文件对话框

	if(result == IDOK)//如果用户点击OK按钮
	{	string address;
	address=change(fileDlg.GetFolderPath());
	if (address[address.size()-1]=='\\')
	{
		address=address+change(fileDlg.GetFileName());
	}
	else 
		address=address+"\\"+change(fileDlg.GetFileName());
  

	int tag=delet_stopword(address,delet_result);
	if(tag==1) 
	{   
		m_keywordshow.DeleteAllItems();
		CString str;
		for(int i=0;i<delet_result.size();i++){
			str.Format(_T("%d"),i+1);
			m_keywordshow.InsertItem(i,str);

			str=delet_result[i].word.c_str();
			m_keywordshow.SetItemText(i,1,str);

			str.Format(_T("%lf"),delet_result[i].EDnor);
			m_keywordshow.SetItemText(i,2,str);
		}
		m_status.SetWindowText(_T("停用词去除成功！"));
	}
	   else
	   m_status.SetWindowText(_T("输入的路径有误或txt内没有内容！请重新输入！"));
    }
}

//void CKeyword_Extraction_EntropyDlg::OnNMClickKeyword(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	POSITION pos=m_ClusterAll.GetFirstSelectedItemPosition();
//
//	if(pos!=NULL){
//		int tag=(int)m_ClusterAll.GetNextSelectedItem(pos);
//		vector<Node> result_show;
//		result_show=getresult();
//		result_show[tag]
//	}
//	*pResult = 0;
//}
