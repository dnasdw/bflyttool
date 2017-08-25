#include "bflyttool.h"
#include "bflyt.h"

CBflytTool::SOption CBflytTool::s_Option[] =
{
	{ USTR("export-text"), USTR('e'), USTR("export text from the target file") },
	{ USTR("import-text"), USTR('i'), USTR("import text to the target file") },
	{ USTR("file"), USTR('f'), USTR("the target file") },
	{ USTR("txt"), USTR('t'), USTR("the txt file for the target file") },
	{ USTR("fake-charset"), 0, USTR("fake charset file for some game with the japanese encoding") },
	{ USTR("verbose"), USTR('v'), USTR("show the info") },
	{ USTR("help"), USTR('h'), USTR("show this help") },
	{ nullptr, 0, nullptr }
};

CBflytTool::CBflytTool()
	: m_eAction(kActionNone)
	, m_bVerbose(false)
{
}

CBflytTool::~CBflytTool()
{
}

int CBflytTool::ParseOptions(int a_nArgc, UChar* a_pArgv[])
{
	if (a_nArgc <= 1)
	{
		return 1;
	}
	for (int i = 1; i < a_nArgc; i++)
	{
		int nArgpc = static_cast<int>(UCslen(a_pArgv[i]));
		if (nArgpc == 0)
		{
			continue;
		}
		int nIndex = i;
		if (a_pArgv[i][0] != USTR('-'))
		{
			UPrintf(USTR("ERROR: illegal option\n\n"));
			return 1;
		}
		else if (nArgpc > 1 && a_pArgv[i][1] != USTR('-'))
		{
			for (int j = 1; j < nArgpc; j++)
			{
				switch (parseOptions(a_pArgv[i][j], nIndex, a_nArgc, a_pArgv))
				{
				case kParseOptionReturnSuccess:
					break;
				case kParseOptionReturnIllegalOption:
					UPrintf(USTR("ERROR: illegal option\n\n"));
					return 1;
				case kParseOptionReturnNoArgument:
					UPrintf(USTR("ERROR: no argument\n\n"));
					return 1;
				case kParseOptionReturnOptionConflict:
					UPrintf(USTR("ERROR: option conflict\n\n"));
					return 1;
				}
			}
		}
		else if (nArgpc > 2 && a_pArgv[i][1] == USTR('-'))
		{
			switch (parseOptions(a_pArgv[i] + 2, nIndex, a_nArgc, a_pArgv))
			{
			case kParseOptionReturnSuccess:
				break;
			case kParseOptionReturnIllegalOption:
				UPrintf(USTR("ERROR: illegal option\n\n"));
				return 1;
			case kParseOptionReturnNoArgument:
				UPrintf(USTR("ERROR: no argument\n\n"));
				return 1;
			case kParseOptionReturnOptionConflict:
				UPrintf(USTR("ERROR: option conflict\n\n"));
				return 1;
			}
		}
		i = nIndex;
	}
	return 0;
}

int CBflytTool::CheckOptions()
{
	if (m_eAction == kActionNone)
	{
		UPrintf(USTR("ERROR: nothing to do\n\n"));
		return 1;
	}
	if (m_eAction != kActionHelp)
	{
		if (m_sFileName.empty())
		{
			UPrintf(USTR("ERROR: no --file option\n\n"));
			return 1;
		}
		if (m_sTxtName.empty())
		{
			UPrintf(USTR("ERROR: no --txt option\n\n"));
			return 1;
		}
		if (!CBflyt::IsBflytFile(m_sFileName))
		{
			UPrintf(USTR("ERROR: %") PRIUS USTR(" is not a bflyt file\n\n"), m_sFileName.c_str());
			return 1;
		}
	}
	return 0;
}

int CBflytTool::Help()
{
	UPrintf(USTR("bflyttool %") PRIUS USTR(" by dnasdw\n\n"), AToU(BFLYTTOOL_VERSION).c_str());
	UPrintf(USTR("usage: bflyttool [option...] [option]...\n"));
	UPrintf(USTR("sample:\n"));
	UPrintf(USTR("  bflyttool -evft title.bflyt title.l10n.txt\n"));
	UPrintf(USTR("  bflyttool -ivft title.bflyt title_new.l10n.txt\n"));
	UPrintf(USTR("  bflyttool -ivft title.bflyt title_new.l10n.txt --fake-charset fake-charset.bin\n"));
	UPrintf(USTR("\n"));
	UPrintf(USTR("option:\n"));
	SOption* pOption = s_Option;
	while (pOption->Name != nullptr || pOption->Doc != nullptr)
	{
		if (pOption->Name != nullptr)
		{
			UPrintf(USTR("  "));
			if (pOption->Key != 0)
			{
				UPrintf(USTR("-%c,"), pOption->Key);
			}
			else
			{
				UPrintf(USTR("   "));
			}
			UPrintf(USTR(" --%-8") PRIUS, pOption->Name);
			if (UCslen(pOption->Name) >= 8 && pOption->Doc != nullptr)
			{
				UPrintf(USTR("\n%16") PRIUS, USTR(""));
			}
		}
		if (pOption->Doc != nullptr)
		{
			UPrintf(USTR("%") PRIUS, pOption->Doc);
		}
		UPrintf(USTR("\n"));
		pOption++;
	}
	return 0;
}

int CBflytTool::Action()
{
	if (m_eAction == kActionExportText)
	{
		if (!exportText())
		{
			UPrintf(USTR("ERROR: export text failed\n\n"));
			return 1;
		}
	}
	if (m_eAction == kActionImportText)
	{
		if (!importText())
		{
			UPrintf(USTR("ERROR: import text failed\n\n"));
			return 1;
		}
	}
	if (m_eAction == kActionHelp)
	{
		return Help();
	}
	return 0;
}

CBflytTool::EParseOptionReturn CBflytTool::parseOptions(const UChar* a_pName, int& a_nIndex, int a_nArgc, UChar* a_pArgv[])
{
	if (UCscmp(a_pName, USTR("export-text")) == 0)
	{
		if (m_eAction == kActionNone)
		{
			m_eAction = kActionExportText;
		}
		else if (m_eAction != kActionExportText && m_eAction != kActionHelp)
		{
			return kParseOptionReturnOptionConflict;
		}
	}
	else if (UCscmp(a_pName, USTR("import-text")) == 0)
	{
		if (m_eAction == kActionNone)
		{
			m_eAction = kActionImportText;
		}
		else if (m_eAction != kActionImportText && m_eAction != kActionHelp)
		{
			return kParseOptionReturnOptionConflict;
		}
	}
	else if (UCscmp(a_pName, USTR("file")) == 0)
	{
		if (a_nIndex + 1 >= a_nArgc)
		{
			return kParseOptionReturnNoArgument;
		}
		m_sFileName = a_pArgv[++a_nIndex];
	}
	else if (UCscmp(a_pName, USTR("txt")) == 0)
	{
		if (a_nIndex + 1 >= a_nArgc)
		{
			return kParseOptionReturnNoArgument;
		}
		m_sTxtName = a_pArgv[++a_nIndex];
	}
	else if (UCscmp(a_pName, USTR("fake-charset")) == 0)
	{
		if (a_nIndex + 1 >= a_nArgc)
		{
			return kParseOptionReturnNoArgument;
		}
		m_sFakeCharsetName = a_pArgv[++a_nIndex];
	}
	else if (UCscmp(a_pName, USTR("verbose")) == 0)
	{
		m_bVerbose = true;
	}
	else if (UCscmp(a_pName, USTR("help")) == 0)
	{
		m_eAction = kActionHelp;
	}
	return kParseOptionReturnSuccess;
}

CBflytTool::EParseOptionReturn CBflytTool::parseOptions(int a_nKey, int& a_nIndex, int m_nArgc, UChar* a_pArgv[])
{
	for (SOption* pOption = s_Option; pOption->Name != nullptr || pOption->Key != 0 || pOption->Doc != nullptr; pOption++)
	{
		if (pOption->Key == a_nKey)
		{
			return parseOptions(pOption->Name, a_nIndex, m_nArgc, a_pArgv);
		}
	}
	return kParseOptionReturnIllegalOption;
}

bool CBflytTool::exportText()
{
	CBflyt bflyt;
	bflyt.SetFileName(m_sFileName);
	bflyt.SetTxtName(m_sTxtName);
	bflyt.SetFakeCharsetName(m_sFakeCharsetName);
	bflyt.SetVerbose(m_bVerbose);
	return bflyt.ExportText();
}

bool CBflytTool::importText()
{
	CBflyt bflyt;
	bflyt.SetFileName(m_sFileName);
	bflyt.SetTxtName(m_sTxtName);
	bflyt.SetFakeCharsetName(m_sFakeCharsetName);
	bflyt.SetVerbose(m_bVerbose);
	return bflyt.ImportText();
}

int UMain(int argc, UChar* argv[])
{
	CBflytTool tool;
	if (tool.ParseOptions(argc, argv) != 0)
	{
		return tool.Help();
	}
	if (tool.CheckOptions() != 0)
	{
		return 1;
	}
	return tool.Action();
}
