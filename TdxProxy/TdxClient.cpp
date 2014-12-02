// TdxClient.cpp : CTdxClient 的实现

#include "stdafx.h"
#include "TdxClient.h"


// CTdxClient

/// <summary>
/// 初始化通达信实例
/// </summary>
typedef void(__stdcall* Tdx_InitDelegate)();
/// <summary>
/// 退出通达信实例
/// </summary>
typedef void(__stdcall* Tdx_ExitDelegate)();

/// <summary>
/// 交易账户登录
/// </summary>
/// <param name="IP">交易服务器IP, 可通过券商通达信登录界面里的通讯设置查询哪个交易服务器速度最快</param>
/// <param name="Port">交易服务器端口</param>
/// <param name="Version">用来设置通达信的版本号，如果设的太低，券商服务器会认为使用的通达信软件太旧要升级</param>
/// <param name="AccountNo">资金账号</param>
/// <param name="JyPassword">交易密码</param>
/// <param name="TxPassword">通讯密码</param>
/// <param name="ErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。</param>
/// <returns>客户端ID，失败时返回-1</returns>
typedef int(__stdcall* Tdx_LoginDelegate)(BSTR IP, short Port, BSTR Version, BSTR AccountNo, BSTR JyPassword, BSTR TxPassword, BSTR* ErrInfo);

/// <summary>
/// 交易账户注销
/// </summary>
/// <param name="ClientID">客户端ID</param>
typedef void(__stdcall* Tdx_LogoutDelegate)(int ClientID);

/// <summary>
/// 查询各种交易数据
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Category">表示查询信息的种类，0资金  1股份   2当日委托  3当日成交     4可撤单   5股东代码  6融资余额   7融券余额  8可融证券</param>
/// <param name="Result">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
/// <param name="ErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。请注意演示版也用此显示功能限制信息。</param>
typedef void(__stdcall* Tdx_QueryDataDelegate)(int ClientID, int Category, char* Result, char* ErrInfo);

//比如查询股东代码时返回的结果字符串就是 "股东代码\t股东名称\t帐号类别\t保留信息\n0000064567\t\t0\t\nA000064567\t\t1\t\n2000064567\t\t2\t\nB000064567\t\t3\t"
//查得此数据之后，通过分割字符串， 可以恢复为几行几列的表格形式的数据


/// <summary>
/// 属于批量版功能,查询各种历史数据
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Category">表示查询信息的种类，0历史委托  1历史成交   2交割单</param>
/// <param name="StartDate">表示开始日期，格式为yyyyMMdd,比如2014年3月1日为  20140301
/// <param name="EndDate">表示结束日期，格式为yyyyMMdd,比如2014年3月1日为  20140301
/// <param name="Result">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
/// <param name="ErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。请注意演示版也用此显示功能限制信息。</param>
typedef void(__stdcall* Tdx_QueryHistoryDataDelegate)(int ClientID, int Category, char* StartDate, char* EndDate, char* Result, char* ErrInfo);

/// <summary>
/// 属于批量版功能,批量查询各种交易数据,用数组传入每个委托的参数，数组第i个元素表示第i个查询的相应参数
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Category">信息的种类的数组, 第i个元素表示第i个查询的信息种类，0资金  1股份   2当日委托  3当日成交     4可撤单   5股东代码  6融资余额   7融券余额  8可融证券</param>
/// <param name="Count">查询的个数，即数组的长度</param>
/// <param name="Result">返回数据的数组, 第i个元素表示第i个委托的返回信息. 此API执行返回后，Result[i]内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
/// <param name="ErrInfo">错误信息的数组，第i个元素表示第i个委托的错误信息. 此API执行返回后，如果出错，ErrInfo[i]保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。请注意演示版也用此显示功能限制信息。</param>
typedef void(__stdcall* Tdx_QueryBatDataDelegate)(int ClientID, int Category[], int Count, char* Result[], char* ErrInfo[]);



/// <summary>
/// 下委托交易证券
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Category">表示委托的种类，0买入 1卖出  2融资买入  3融券卖出   4买券还券   5卖券还款  6现券还券</param>
/// <param name="PriceType">表示报价方式 0  上海限价委托 深圳限价委托 1深圳对方最优价格  2深圳本方最优价格  3深圳即时成交剩余撤销  4上海五档即成剩撤 深圳五档即成剩撤 5深圳全额成交或撤销 6上海五档即成转限价
/// <param name="Gddm">股东代码, 交易上海股票填上海的股东代码；交易深圳的股票填入深圳的股东代码</param>
/// <param name="Zqdm">证券代码</param>
/// <param name="Price">委托价格</param>
/// <param name="Quantity">委托数量</param>
/// <param name="Result">此API执行返回后，Result内保存了返回的委托编号等数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
/// <param name="ErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。请注意演示版也用此显示功能限制信息。</param>
typedef void(__stdcall* Tdx_SendOrderDelegate)(int ClientID, int Category, int PriceType, char* Gddm, char* Zqdm, float Price, int Quantity, char* Result, char* ErrInfo);



/// <summary>
/// 属于批量版功能,批量下委托交易证券，用数组传入每个委托的参数，数组第i个元素表示第i个委托的相应参数
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Category">委托种类的数组，第i个元素表示第i个委托的种类，0买入 1卖出  2融资买入  3融券卖出   4买券还券   5卖券还款  6现券还券</param>
/// <param name="PriceType">表示报价方式的数组,  第i个元素表示第i个委托的报价方式, 0  上海限价委托 深圳限价委托 1深圳对方最优价格  2深圳本方最优价格  3深圳即时成交剩余撤销  4上海五档即成剩撤 深圳五档即成剩撤 5深圳全额成交或撤销 6上海五档即成转限价
/// <param name="Gddm">股东代码数组，第i个元素表示第i个委托的股东代码，交易上海股票填上海的股东代码；交易深圳的股票填入深圳的股东代码</param>
/// <param name="Zqdm">证券代码数组，第i个元素表示第i个委托的证券代码</param>
/// <param name="Price">委托价格数组，第i个元素表示第i个委托的委托价格</param>
/// <param name="Quantity">委托数量数组，第i个元素表示第i个委托的委托数量</param>
/// <param name="Count">委托的个数，即数组的长度</param>
/// <param name="Result">返回数据的数组，第i个元素表示第i个委托的返回信息。此API执行返回后，Result内保存了返回的委托编号等数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
/// <param name="ErrInfo">错误信息的数组，第i个元素表示第i个委托的错误信息。此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。请注意演示版也用此显示功能限制信息。</param>
typedef void(__stdcall* Tdx_SendBatOrdersDelegate)(int ClientID, int Category[], int PriceType[], char* Gddm[], char* Zqdm[], float Price[], int Quantity[], int Count, char* Result[], char* ErrInfo[]);


/// <summary>
/// 撤委托
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="hth">表示要撤的目标委托的编号</param>
/// <param name="Result">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
/// <param name="ErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。请注意演示版也用此显示功能限制信息。</param>
typedef void(__stdcall* Tdx_CancelOrderDelegate)(int ClientID, char* hth, char* Result, char* ErrInfo);//招商证券信用版本




/// <summary>
/// 属于批量版功能,批量撤委托, 用数组传入每个委托的参数，数组第i个元素表示第i个撤委托的相应参数
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="hth">表示要撤的目标委托的编号</param>
/// <param name="Count">撤委托的个数，即数组的长度</param>
/// <param name="Result">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
/// <param name="ErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。请注意演示版也用此显示功能限制信息。</param>
typedef void(__stdcall* Tdx_CancelBatOrdersDelegate)(int ClientID, char* hth[], int Count, char* Result[], char* ErrInfo[]);


/// <summary>
/// 获取证券的实时五档行情
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Zqdm">证券代码</param>
/// <param name="Result">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
/// <param name="ErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。请注意演示版也用此显示功能限制信息。</param>
typedef void(__stdcall* Tdx_GetSSHQDelegate)(int ClientID, char* Zqdm, char* Result, char* ErrInfo);


/// <summary>
/// 属于批量版功能,批量获取证券的实时五档行情
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Zqdm">证券代码</param>
/// <param name="Result">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
/// <param name="ErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。请注意演示版也用此显示功能限制信息。</param>
typedef void(__stdcall* Tdx_GetBatSSHQDelegate)(int ClientID, char* Zqdm[], int Count, char* Result[], char* ErrInfo[]);


/// <summary>
/// 融资融券直接还款
/// </summary>
/// <param name="ClientID">客户端ID</param>
/// <param name="Amount">还款金额</param>
/// <param name="Result">此API执行返回后，Result内保存了返回的查询数据, 形式为表格数据，行数据之间通过\n字符分割，列数据之间通过\t分隔。一般要分配1024*1024字节的空间。出错时为空字符串。</param>
/// <param name="ErrInfo">此API执行返回后，如果出错，保存了错误信息说明。一般要分配256字节的空间。没出错时为空字符串。请注意演示版也用此显示功能限制信息。</param>
typedef void(__stdcall* Tdx_RefundMoneyDelegate)(int ClientID, char* Amount, char* Result, char* ErrInfo);


/// <summary>
/// 获取api适用的券商账户类型
/// </summary>
/// <param name="Result">此API执行返回后，保存了返回的账户类型结果，为一字符串。一般要分配256字节的空间。</param>
typedef void(__stdcall* Tdx_GetAccountEditionDelegate)(char* Result);


STDMETHODIMP CTdxClient::Tdx_Login(BSTR IP, SHORT Port, BSTR version, BSTR AccountNo, BSTR JyPassword, BSTR TxPassword, BSTR* ErrInfo, LONG* ClientID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO:  在此添加实现代码
	__try{
		Tdx_LoginDelegate Tdx_Login = (Tdx_LoginDelegate)GetProcAddress(hDLL, "TC_Login");
		LONG clientId = (LONG)Tdx_Login(IP, Port, version, AccountNo, JyPassword, TxPassword, ErrInfo);
		ClientID = &clientId;
	}
	__except (1)
	{
	 _EXCEPTION_POINTERS a=	GetExceptionInformation();
	}
	return S_OK;
}


STDMETHODIMP CTdxClient::Tdx_Init()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// TODO:  在此添加实现代码
	LPCWSTR libdll = L"Tc.dll";
	hDLL = LoadLibrary(libdll);//加载动态链接库MyDll.dll文件； 
	Tdx_InitDelegate	 Tdx_Init = (Tdx_InitDelegate)GetProcAddress(hDLL, "TC_Init_Environ");
	Tdx_Init();
	return S_OK;
}


STDMETHODIMP CTdxClient::Tdx_Exit()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO:  在此添加实现代码
	Tdx_ExitDelegate Tdx_Exit = (Tdx_ExitDelegate)GetProcAddress(hDLL, "TC_Uninit");
	Tdx_Exit();
	FreeLibrary(hDLL);
	return S_OK;
}
