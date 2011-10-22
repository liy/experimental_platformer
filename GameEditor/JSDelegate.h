#pragma once

typedef struct _awe_jsarray awe_jsarray;
class UIView;

class GenericClass{};

typedef void(GenericClass::*FuncPtr)(UIView* uiview, const std::string& jsObjName, const std::string& jsFuncName, const awe_jsarray* jsArgs);

class JSDelegate
{
public:
	JSDelegate(void* instance, FuncPtr funcPtr){
		_funcPtr = funcPtr;
		_instance = instance;
	}
	~JSDelegate(void);

	void Invoke(UIView* uiview, const std::string& jsObjName, const std::string& jsFuncName, const awe_jsarray* jsArgs){
		(static_cast<GenericClass*>(_instance)->*_funcPtr)(uiview, jsObjName, jsFuncName, jsArgs);
	}

protected:
	void* _instance;
	FuncPtr _funcPtr;
};

