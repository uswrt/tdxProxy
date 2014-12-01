// dllmain.h : 模块类的声明。

class CTdxProxyModule : public ATL::CAtlDllModuleT< CTdxProxyModule >
{
public :
	DECLARE_LIBID(LIBID_TdxProxyLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TDXPROXY, "{043E0195-2462-4BC0-9068-62FA210D32FE}")
};

extern class CTdxProxyModule _AtlModule;
