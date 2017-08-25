#ifndef BFLYTTOOL_H_
#define BFLYTTOOL_H_

#include <sdw.h>

class CBflytTool
{
public:
	enum EParseOptionReturn
	{
		kParseOptionReturnSuccess,
		kParseOptionReturnIllegalOption,
		kParseOptionReturnNoArgument,
		kParseOptionReturnOptionConflict
	};
	enum EAction
	{
		kActionNone,
		kActionExportText,
		kActionImportText,
		kActionHelp
	};
	struct SOption
	{
		const UChar* Name;
		int Key;
		const UChar* Doc;
	};
	CBflytTool();
	~CBflytTool();
	int ParseOptions(int a_nArgc, UChar* a_pArgv[]);
	int CheckOptions();
	int Help();
	int Action();
	static SOption s_Option[];
private:
	EParseOptionReturn parseOptions(const UChar* a_pName, int& a_nIndex, int a_nArgc, UChar* a_pArgv[]);
	EParseOptionReturn parseOptions(int a_nKey, int& a_nIndex, int a_nArgc, UChar* a_pArgv[]);
	bool exportText();
	bool importText();
	EAction m_eAction;
	UString m_sFileName;
	UString m_sTxtName;
	UString m_sFakeCharsetName;
	bool m_bVerbose;
};

#endif	// BFLYTTOOL_H_
