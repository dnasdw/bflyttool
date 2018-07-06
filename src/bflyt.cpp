#include "bflyt.h"

const char CBflyt::s_nUnused[8] = "@unused";

CBflyt::CBflyt()
	: m_bForce(false)
	, m_bVerbose(false)
	, m_eEndianness(kEndianLittle)
{
	memset(m_uFakeCharset, 0, sizeof(m_uFakeCharset));
}

CBflyt::~CBflyt()
{
}

void CBflyt::SetFileName(const UString& a_sFileName)
{
	m_sFileName = a_sFileName;
}

void CBflyt::SetTxtName(const UString& a_sPngName)
{
	m_sTxtName = a_sPngName;
}

void CBflyt::SetFakeCharsetName(const UString& a_sFakeCharset)
{
	m_sFakeCharsetName = a_sFakeCharset;
}

void CBflyt::SetForce(bool a_bForce)
{
	m_bForce = a_bForce;
}

void CBflyt::SetVerbose(bool a_bVerbose)
{
	m_bVerbose = a_bVerbose;
}

bool CBflyt::ExportText()
{
	bool bResult = true;
	if (!initExt())
	{
		return false;
	}
	if (!m_sFakeCharsetName.empty())
	{
		if (!loadFakeCharset(true))
		{
			return false;
		}
	}
	FILE* fp = UFopen(m_sFileName.c_str(), USTR("rb"));
	if (fp == nullptr)
	{
		return false;
	}
	fseek(fp, 0, SEEK_END);
	u32 uBflytSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	u8* pBflyt = new u8[uBflytSize];
	fread(pBflyt, 1, uBflytSize, fp);
	fclose(fp);
	SLyt* pLyt = reinterpret_cast<SLyt*>(pBflyt);
	if (pLyt->FileHeader.ByteOrder == 0xFFFE)
	{
		m_eEndianness = kEndianBig;
	}
	if (m_eEndianness == kEndianBig)
	{
		pLyt->FileHeader.ByteOrder = SDW_CONVERT_ENDIAN16(pLyt->FileHeader.ByteOrder);
		pLyt->FileHeader.HeaderSize = SDW_CONVERT_ENDIAN16(pLyt->FileHeader.HeaderSize);
		pLyt->FileHeader.Version = SDW_CONVERT_ENDIAN32(pLyt->FileHeader.Version);
		pLyt->FileHeader.FileSize = SDW_CONVERT_ENDIAN32(pLyt->FileHeader.FileSize);
		pLyt->FileHeader.DataBlocks = SDW_CONVERT_ENDIAN16(pLyt->FileHeader.DataBlocks);
		pLyt->FileHeader.Reserved = SDW_CONVERT_ENDIAN16(pLyt->FileHeader.Reserved);
	}
	u32 uOffset = pLyt->FileHeader.HeaderSize;
	fp = nullptr;
	n32 nIndex = 0;
	if (m_bForce)
	{
		u32* pBflytU32 = reinterpret_cast<u32*>(pBflyt);
		n32 nCount = (uBflytSize - sizeof(STextBox)) / 4;
		for (n32 i = 0; i < nCount; i++)
		{
			if (pBflytU32[i] == kSignatureTextBox)
			{
				uOffset = i * 4;
				STextBox* pTextBox = reinterpret_cast<STextBox*>(pBflytU32 + i);
				if (m_eEndianness == kEndianBig)
				{
					*reinterpret_cast<u32*>(&pTextBox->Translate.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.X));
					*reinterpret_cast<u32*>(&pTextBox->Translate.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.Y));
					*reinterpret_cast<u32*>(&pTextBox->Translate.Z) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.Z));
					*reinterpret_cast<u32*>(&pTextBox->Rotate.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.X));
					*reinterpret_cast<u32*>(&pTextBox->Rotate.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.Y));
					*reinterpret_cast<u32*>(&pTextBox->Rotate.Z) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.Z));
					*reinterpret_cast<u32*>(&pTextBox->Scale.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Scale.X));
					*reinterpret_cast<u32*>(&pTextBox->Scale.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Scale.Y));
					*reinterpret_cast<u32*>(&pTextBox->Size.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Size.X));
					*reinterpret_cast<u32*>(&pTextBox->Size.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Size.Y));
					pTextBox->TextBufBytes = SDW_CONVERT_ENDIAN16(pTextBox->TextBufBytes);
					pTextBox->TextStrBytes = SDW_CONVERT_ENDIAN16(pTextBox->TextStrBytes);
					pTextBox->MaterialIdx = SDW_CONVERT_ENDIAN16(pTextBox->MaterialIdx);
					pTextBox->FontIdx = SDW_CONVERT_ENDIAN16(pTextBox->FontIdx);
					*reinterpret_cast<u32*>(&pTextBox->ItalicRatio) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ItalicRatio));
					pTextBox->TextStrOffset = SDW_CONVERT_ENDIAN32(pTextBox->TextStrOffset);
					*reinterpret_cast<u32*>(&pTextBox->FontSize.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->FontSize.X));
					*reinterpret_cast<u32*>(&pTextBox->FontSize.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->FontSize.Y));
					*reinterpret_cast<u32*>(&pTextBox->CharSpace) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->CharSpace));
					*reinterpret_cast<u32*>(&pTextBox->LineSpace) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->LineSpace));
					pTextBox->TextIDOffset = SDW_CONVERT_ENDIAN32(pTextBox->TextIDOffset);
					*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.X));
					*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.Y));
					*reinterpret_cast<u32*>(&pTextBox->ShadowScale.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowScale.X));
					*reinterpret_cast<u32*>(&pTextBox->ShadowScale.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowScale.Y));
					*reinterpret_cast<u32*>(&pTextBox->ShadowItalicRatio) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowItalicRatio));
					if (pLyt->FileHeader.Version >= 0x07000000)
					{
						pTextBox->PerCharacterTransformOffset = SDW_CONVERT_ENDIAN32(pTextBox->PerCharacterTransformOffset);
					}
				}
				if (pTextBox->TextStrOffset != 0 && uOffset + pTextBox->TextStrOffset < uBflytSize)
				{
					n32 nTextBoxWidth = static_cast<n32>(pTextBox->Size.X);
					n32 nTextBoxHeight = static_cast<n32>(pTextBox->Size.Y);
					Char16_t* pTextStr = reinterpret_cast<Char16_t*>(pBflyt + uOffset + pTextBox->TextStrOffset);
					if (uOffset + pTextBox->TextStrOffset + sizeof(s_nUnused) > uBflytSize || memcmp(pTextStr, s_nUnused, sizeof(s_nUnused)) != 0)
					{
						if (m_eEndianness == kEndianBig)
						{
							for (n32 j = 0; j < pTextBox->TextStrBytes / 2; j++)
							{
								pTextStr[j] = SDW_CONVERT_ENDIAN16(pTextStr[j]);
							}
						}
						U16String sTextStr(pTextStr, pTextBox->TextStrBytes / 2 - 1);
						if (!m_sFakeCharsetName.empty())
						{
							for (n32 j = 0; j < static_cast<n32>(sTextStr.size()); j++)
							{
								Char16_t uU16Code = m_uFakeCharset[sTextStr[j]];
								if (uU16Code != 0)
								{
									sTextStr[j] = uU16Code;
								}
							}
						}
						for (u32 j = 0; j < static_cast<u32>(sTextStr.size()); j++)
						{
							Char16_t uU16Code = sTextStr[j];
							if (uU16Code >= 0xE000 && uU16Code <= 0xE757)
							{
								sTextStr.replace(j, 1, m_mExt[uU16Code]);
								j += static_cast<u32>(m_mExt[uU16Code].size()) - 1;
							}
						}
						wstring sTxt = U16ToW(sTextStr);
						wstring::size_type uPos = 0;
						uPos = sTxt.find(L"[No].");
						if (uPos != wstring::npos)
						{
							if (fp != nullptr)
							{
								fclose(fp);
							}
							delete[] pBflyt;
							return 1;
						}
						uPos = sTxt.find(L"[--------------------------------------]");
						if (uPos != wstring::npos)
						{
							if (fp != nullptr)
							{
								fclose(fp);
							}
							delete[] pBflyt;
							return 1;
						}
						uPos = sTxt.find(L"[======================================]");
						if (uPos != wstring::npos)
						{
							if (fp != nullptr)
							{
								fclose(fp);
							}
							delete[] pBflyt;
							return 1;
						}
						uPos = sTxt.find(L"<r>");
						if (uPos != wstring::npos)
						{
							if (fp != nullptr)
							{
								fclose(fp);
							}
							delete[] pBflyt;
							return 1;
						}
						sTxt = Replace(sTxt, L"No.", L"[No].");
						sTxt = Replace(sTxt, L"--------------------------------------", L"[--------------------------------------]");
						sTxt = Replace(sTxt, L"======================================", L"[======================================]");
						sTxt = Replace(sTxt, L'\r', L"<r>");
						sTxt = Replace(sTxt, L'\n', L"\r\n");
						if (fp == nullptr)
						{
							fp = UFopen(m_sTxtName.c_str(), USTR("wb"));
							if (fp == nullptr)
							{
								delete[] pBflyt;
								return 1;
							}
							fwrite("\xFF\xFE", 2, 1, fp);
						}
						else
						{
							fu16printf(fp, L"\r\n\r\n");
						}
						fu16printf(fp, L"No.%d %d,%d\r\n", nIndex++, nTextBoxWidth, nTextBoxHeight);
						fu16printf(fp, L"--------------------------------------\r\n");
						fu16printf(fp, L"%ls\r\n", sTxt.c_str());
						fu16printf(fp, L"======================================\r\n");
						fu16printf(fp, L"%ls\r\n", sTxt.c_str());
						fu16printf(fp, L"--------------------------------------\r\n");
					}
				}
			}
		}
	}
	else
	{
		for (n32 i = 0; i < pLyt->FileHeader.DataBlocks; i++)
		{
			SBinaryBlockHeader* pBinaryBlockHeader = reinterpret_cast<SBinaryBlockHeader*>(pBflyt + uOffset);
			if (m_eEndianness == kEndianBig)
			{
				pBinaryBlockHeader->Size = SDW_CONVERT_ENDIAN32(pBinaryBlockHeader->Size);
			}
			if (pBinaryBlockHeader->Signature == kSignatureTextBox)
			{
				STextBox* pTextBox = reinterpret_cast<STextBox*>(pBinaryBlockHeader);
				if (m_eEndianness == kEndianBig)
				{
					*reinterpret_cast<u32*>(&pTextBox->Translate.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.X));
					*reinterpret_cast<u32*>(&pTextBox->Translate.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.Y));
					*reinterpret_cast<u32*>(&pTextBox->Translate.Z) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.Z));
					*reinterpret_cast<u32*>(&pTextBox->Rotate.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.X));
					*reinterpret_cast<u32*>(&pTextBox->Rotate.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.Y));
					*reinterpret_cast<u32*>(&pTextBox->Rotate.Z) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.Z));
					*reinterpret_cast<u32*>(&pTextBox->Scale.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Scale.X));
					*reinterpret_cast<u32*>(&pTextBox->Scale.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Scale.Y));
					*reinterpret_cast<u32*>(&pTextBox->Size.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Size.X));
					*reinterpret_cast<u32*>(&pTextBox->Size.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Size.Y));
					pTextBox->TextBufBytes = SDW_CONVERT_ENDIAN16(pTextBox->TextBufBytes);
					pTextBox->TextStrBytes = SDW_CONVERT_ENDIAN16(pTextBox->TextStrBytes);
					pTextBox->MaterialIdx = SDW_CONVERT_ENDIAN16(pTextBox->MaterialIdx);
					pTextBox->FontIdx = SDW_CONVERT_ENDIAN16(pTextBox->FontIdx);
					*reinterpret_cast<u32*>(&pTextBox->ItalicRatio) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ItalicRatio));
					pTextBox->TextStrOffset = SDW_CONVERT_ENDIAN32(pTextBox->TextStrOffset);
					*reinterpret_cast<u32*>(&pTextBox->FontSize.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->FontSize.X));
					*reinterpret_cast<u32*>(&pTextBox->FontSize.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->FontSize.Y));
					*reinterpret_cast<u32*>(&pTextBox->CharSpace) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->CharSpace));
					*reinterpret_cast<u32*>(&pTextBox->LineSpace) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->LineSpace));
					pTextBox->TextIDOffset = SDW_CONVERT_ENDIAN32(pTextBox->TextIDOffset);
					*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.X));
					*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.Y));
					*reinterpret_cast<u32*>(&pTextBox->ShadowScale.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowScale.X));
					*reinterpret_cast<u32*>(&pTextBox->ShadowScale.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowScale.Y));
					*reinterpret_cast<u32*>(&pTextBox->ShadowItalicRatio) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowItalicRatio));
					if (pLyt->FileHeader.Version >= 0x07000000)
					{
						pTextBox->PerCharacterTransformOffset = SDW_CONVERT_ENDIAN32(pTextBox->PerCharacterTransformOffset);
					}
				}
				if (pTextBox->TextStrOffset != 0)
				{
					n32 nTextBoxWidth = static_cast<n32>(pTextBox->Size.X);
					n32 nTextBoxHeight = static_cast<n32>(pTextBox->Size.Y);
					Char16_t* pTextStr = reinterpret_cast<Char16_t*>(pBflyt + uOffset + pTextBox->TextStrOffset);
					if (memcmp(pTextStr, s_nUnused, sizeof(s_nUnused)) != 0)
					{
						if (m_eEndianness == kEndianBig)
						{
							for (n32 j = 0; j < pTextBox->TextStrBytes / 2; j++)
							{
								pTextStr[j] = SDW_CONVERT_ENDIAN16(pTextStr[j]);
							}
						}
						U16String sTextStr(pTextStr, pTextBox->TextStrBytes / 2 - 1);
						if (!m_sFakeCharsetName.empty())
						{
							for (n32 j = 0; j < static_cast<n32>(sTextStr.size()); j++)
							{
								Char16_t uU16Code = m_uFakeCharset[sTextStr[j]];
								if (uU16Code != 0)
								{
									sTextStr[j] = uU16Code;
								}
							}
						}
						for (u32 j = 0; j < static_cast<u32>(sTextStr.size()); j++)
						{
							Char16_t uU16Code = sTextStr[j];
							if (uU16Code >= 0xE000 && uU16Code <= 0xE757)
							{
								sTextStr.replace(j, 1, m_mExt[uU16Code]);
								j += static_cast<u32>(m_mExt[uU16Code].size()) - 1;
							}
						}
						wstring sTxt = U16ToW(sTextStr);
						wstring::size_type uPos = 0;
						uPos = sTxt.find(L"[No].");
						if (uPos != wstring::npos)
						{
							if (fp != nullptr)
							{
								fclose(fp);
							}
							delete[] pBflyt;
							return 1;
						}
						uPos = sTxt.find(L"[--------------------------------------]");
						if (uPos != wstring::npos)
						{
							if (fp != nullptr)
							{
								fclose(fp);
							}
							delete[] pBflyt;
							return 1;
						}
						uPos = sTxt.find(L"[======================================]");
						if (uPos != wstring::npos)
						{
							if (fp != nullptr)
							{
								fclose(fp);
							}
							delete[] pBflyt;
							return 1;
						}
						uPos = sTxt.find(L"<r>");
						if (uPos != wstring::npos)
						{
							if (fp != nullptr)
							{
								fclose(fp);
							}
							delete[] pBflyt;
							return 1;
						}
						sTxt = Replace(sTxt, L"No.", L"[No].");
						sTxt = Replace(sTxt, L"--------------------------------------", L"[--------------------------------------]");
						sTxt = Replace(sTxt, L"======================================", L"[======================================]");
						sTxt = Replace(sTxt, L'\r', L"<r>");
						sTxt = Replace(sTxt, L'\n', L"\r\n");
						if (fp == nullptr)
						{
							fp = UFopen(m_sTxtName.c_str(), USTR("wb"));
							if (fp == nullptr)
							{
								delete[] pBflyt;
								return 1;
							}
							fwrite("\xFF\xFE", 2, 1, fp);
						}
						else
						{
							fu16printf(fp, L"\r\n\r\n");
						}
						fu16printf(fp, L"No.%d %d,%d\r\n", nIndex++, nTextBoxWidth, nTextBoxHeight);
						fu16printf(fp, L"--------------------------------------\r\n");
						fu16printf(fp, L"%ls\r\n", sTxt.c_str());
						fu16printf(fp, L"======================================\r\n");
						fu16printf(fp, L"%ls\r\n", sTxt.c_str());
						fu16printf(fp, L"--------------------------------------\r\n");
					}
				}
			}
			else if (pBinaryBlockHeader->Signature == kSignatureParts)
			{
				// TODO: support later
			}
			uOffset += pBinaryBlockHeader->Size;
		}
	}
	if (fp != nullptr)
	{
		fclose(fp);
	}
	delete[] pBflyt;
	return bResult;
}

bool CBflyt::ImportText()
{
	bool bResult = true;
	if (!initExt())
	{
		return false;
	}
	if (!m_sFakeCharsetName.empty())
	{
		if (!loadFakeCharset(false))
		{
			return false;
		}
	}
	FILE* fp = UFopen(m_sFileName.c_str(), USTR("rb"));
	if (fp == nullptr)
	{
		return false;
	}
	fseek(fp, 0, SEEK_END);
	u32 uBflytSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	u8* pBflyt = new u8[uBflytSize];
	fread(pBflyt, 1, uBflytSize, fp);
	fclose(fp);
	SLyt* pLyt = reinterpret_cast<SLyt*>(pBflyt);
	if (pLyt->FileHeader.ByteOrder == 0xFFFE)
	{
		m_eEndianness = kEndianBig;
	}
	if (m_eEndianness == kEndianBig)
	{
		pLyt->FileHeader.ByteOrder = SDW_CONVERT_ENDIAN16(pLyt->FileHeader.ByteOrder);
		pLyt->FileHeader.HeaderSize = SDW_CONVERT_ENDIAN16(pLyt->FileHeader.HeaderSize);
		pLyt->FileHeader.Version = SDW_CONVERT_ENDIAN32(pLyt->FileHeader.Version);
		pLyt->FileHeader.FileSize = SDW_CONVERT_ENDIAN32(pLyt->FileHeader.FileSize);
		pLyt->FileHeader.DataBlocks = SDW_CONVERT_ENDIAN16(pLyt->FileHeader.DataBlocks);
		pLyt->FileHeader.Reserved = SDW_CONVERT_ENDIAN16(pLyt->FileHeader.Reserved);
	}
	u32 uOffset = pLyt->FileHeader.HeaderSize;
	wstring sTxt;
	n32 nIndex = 0;
	wstring::size_type uPos0 = 0;
	if (m_bForce)
	{
		vector<u8> vText;
		if (uBflytSize % 4 != 0)
		{
			vText.resize(4 - uBflytSize % 4);
		}
		u32* pBflytU32 = reinterpret_cast<u32*>(pBflyt);
		n32 nCount = (uBflytSize - sizeof(STextBox)) / 4;
		for (n32 i = 0; i < nCount; i++)
		{
			if (pBflytU32[i] == kSignatureTextBox)
			{
				uOffset = i * 4;
				STextBox* pTextBox = reinterpret_cast<STextBox*>(pBflytU32 + i);
				if (m_eEndianness == kEndianBig)
				{
					*reinterpret_cast<u32*>(&pTextBox->Translate.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.X));
					*reinterpret_cast<u32*>(&pTextBox->Translate.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.Y));
					*reinterpret_cast<u32*>(&pTextBox->Translate.Z) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.Z));
					*reinterpret_cast<u32*>(&pTextBox->Rotate.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.X));
					*reinterpret_cast<u32*>(&pTextBox->Rotate.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.Y));
					*reinterpret_cast<u32*>(&pTextBox->Rotate.Z) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.Z));
					*reinterpret_cast<u32*>(&pTextBox->Scale.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Scale.X));
					*reinterpret_cast<u32*>(&pTextBox->Scale.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Scale.Y));
					*reinterpret_cast<u32*>(&pTextBox->Size.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Size.X));
					*reinterpret_cast<u32*>(&pTextBox->Size.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Size.Y));
					pTextBox->TextBufBytes = SDW_CONVERT_ENDIAN16(pTextBox->TextBufBytes);
					pTextBox->TextStrBytes = SDW_CONVERT_ENDIAN16(pTextBox->TextStrBytes);
					pTextBox->MaterialIdx = SDW_CONVERT_ENDIAN16(pTextBox->MaterialIdx);
					pTextBox->FontIdx = SDW_CONVERT_ENDIAN16(pTextBox->FontIdx);
					*reinterpret_cast<u32*>(&pTextBox->ItalicRatio) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ItalicRatio));
					pTextBox->TextStrOffset = SDW_CONVERT_ENDIAN32(pTextBox->TextStrOffset);
					*reinterpret_cast<u32*>(&pTextBox->FontSize.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->FontSize.X));
					*reinterpret_cast<u32*>(&pTextBox->FontSize.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->FontSize.Y));
					*reinterpret_cast<u32*>(&pTextBox->CharSpace) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->CharSpace));
					*reinterpret_cast<u32*>(&pTextBox->LineSpace) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->LineSpace));
					pTextBox->TextIDOffset = SDW_CONVERT_ENDIAN32(pTextBox->TextIDOffset);
					*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.X));
					*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.Y));
					*reinterpret_cast<u32*>(&pTextBox->ShadowScale.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowScale.X));
					*reinterpret_cast<u32*>(&pTextBox->ShadowScale.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowScale.Y));
					*reinterpret_cast<u32*>(&pTextBox->ShadowItalicRatio) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowItalicRatio));
					if (pLyt->FileHeader.Version >= 0x07000000)
					{
						pTextBox->PerCharacterTransformOffset = SDW_CONVERT_ENDIAN32(pTextBox->PerCharacterTransformOffset);
					}
				}
				if (pTextBox->TextStrOffset != 0 && uOffset + pTextBox->TextStrOffset < uBflytSize)
				{
					Char16_t* pTextStr = reinterpret_cast<Char16_t*>(pBflyt + uOffset + pTextBox->TextStrOffset);
					if (uOffset + pTextBox->TextStrOffset + sizeof(s_nUnused) > uBflytSize || memcmp(pTextStr, s_nUnused, sizeof(s_nUnused)) != 0)
					{
						if (sTxt.empty())
						{
							FILE* fpTxt = UFopen(m_sTxtName.c_str(), USTR("rb"));
							if (fpTxt == nullptr)
							{
								delete[] pBflyt;
								return 1;
							}
							fseek(fpTxt, 0, SEEK_END);
							u32 uTxtSize = ftell(fpTxt);
							if (uTxtSize % 2 != 0)
							{
								fclose(fpTxt);
								delete[] pBflyt;
								return 1;
							}
							uTxtSize /= 2;
							fseek(fpTxt, 0, SEEK_SET);
							Char16_t* pTemp = new Char16_t[uTxtSize + 1];
							fread(pTemp, 2, uTxtSize, fpTxt);
							fclose(fpTxt);
							if (pTemp[0] != 0xFEFF)
							{
								delete[] pTemp;
								delete[] pBflyt;
								return 1;
							}
							pTemp[uTxtSize] = 0;
							sTxt = U16ToW(pTemp + 1);
							delete[] pTemp;
						}
						wstring sNum = Format(L"No.%d", nIndex++);
						uPos0 = sTxt.find(sNum, uPos0);
						if (uPos0 == wstring::npos)
						{
							delete[] pBflyt;
							return 1;
						}
						uPos0 += sNum.size();
						if (StartWith(sTxt, L" ", static_cast<u32>(uPos0)))
						{
							wstring::size_type uPos1 = sTxt.find(L"\r\n", uPos0);
							if (uPos1 == wstring::npos)
							{
								delete[] pBflyt;
								return 1;
							}
							wstring sTextBoxSize = Trim(sTxt.substr(uPos0, uPos1 - uPos0));
							vector<wstring> vTextBoxSize = Split(sTextBoxSize, L",");
							if (vTextBoxSize.size() != 2)
							{
								delete[] pBflyt;
								return 1;
							}
							n32 nTextBoxWidth = SToN32(vTextBoxSize[0]);
							n32 nTextBoxHeight = SToN32(vTextBoxSize[1]);
							if (nTextBoxWidth != static_cast<n32>(pTextBox->Size.X))
							{
								pTextBox->Size.X = static_cast<f32>(nTextBoxWidth);
							}
							if (nTextBoxHeight != static_cast<n32>(pTextBox->Size.Y))
							{
								pTextBox->Size.Y = static_cast<f32>(nTextBoxHeight);
							}
							uPos0 = uPos1;
						}
						uPos0 = sTxt.find(L"======================================\r\n", uPos0);
						if (uPos0 == wstring::npos)
						{
							delete[] pBflyt;
							return 1;
						}
						uPos0 += wcslen(L"======================================\r\n");
						wstring::size_type uPos1 = sTxt.find(L"\r\n--------------------------------------", uPos0);
						if (uPos1 == wstring::npos)
						{
							delete[] pBflyt;
							return 1;
						}
						wstring sStmt = sTxt.substr(uPos0, uPos1 - uPos0);
						uPos0 = uPos1 + wcslen(L"\r\n--------------------------------------");
						sStmt = Replace(sStmt, L"\r\n", L"\n");
						sStmt = Replace(sStmt, L"<r>", L"\r");
						sStmt = Replace(sStmt, L"[======================================]", L"======================================");
						sStmt = Replace(sStmt, L"[--------------------------------------]", L"--------------------------------------");
						sStmt = Replace(sStmt, L"[No].", L"No.");
						U16String sStmt16 = WToU16(sStmt);
						for (map<U16String, u32>::iterator it = m_mExtStr.begin(); it != m_mExtStr.end(); ++it)
						{
							const U16String& sPrivateChar = it->first;
							Char16_t uPrivateChar = it->second;
							uPos1 = 0;
							while ((uPos1 = sStmt16.find(sPrivateChar, uPos1)) != U16String::npos)
							{
								sStmt16.replace(uPos1, sPrivateChar.size(), 1, uPrivateChar);
								uPos1++;
							}
						}
						if (!m_sFakeCharsetName.empty())
						{
							for (n32 j = 0; j < static_cast<n32>(sStmt16.size()); j++)
							{
								Char16_t uU16Code = m_uFakeCharset[sStmt16[j]];
								if (uU16Code != 0)
								{
									sStmt16[j] = uU16Code;
								}
							}
						}
						u16 uTextStrBytes = pTextBox->TextStrBytes;
						pTextBox->TextStrBytes = static_cast<u16>(sStmt16.size() * 2 + 2);
						if (pTextBox->TextBufBytes > pTextBox->TextStrBytes)
						{
							pTextBox->TextBoxFlag |= SDW_BIT32(kTextBoxFlagFORCE_ASSIGN_TEXT_LENGTH);
						}
						else
						{
							pTextBox->TextBufBytes = pTextBox->TextStrBytes;
						}
						sStmt16.resize(static_cast<u32>(Align(sStmt16.size() + 1, 2)));
						if (m_eEndianness == kEndianBig)
						{
							for (n32 j = 0; j < static_cast<n32>(sStmt16.size()); j++)
							{
								sStmt16[j] = SDW_CONVERT_ENDIAN16(sStmt16[j]);
							}
						}
						if (Align(pTextBox->TextStrBytes, 4) <= Align(uTextStrBytes, 4))
						{
							memset(pTextStr, 0, uTextStrBytes);
							memcpy(pTextStr, &*sStmt16.begin(), sStmt16.size() * 2);
						}
						else
						{
							pTextBox->TextStrOffset = uBflytSize + static_cast<u32>(vText.size()) - uOffset;
							u32 uTextSize = static_cast<u32>(vText.size());
							vText.resize(vText.size() + static_cast<u32>(Align(pTextBox->TextStrBytes, 4)));
							memcpy(&*vText.begin() + uTextSize, &*sStmt16.begin(), sStmt16.size() * 2);
						}
					}
				}
				if (m_eEndianness == kEndianBig)
				{
					*reinterpret_cast<u32*>(&pTextBox->Translate.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.X));
					*reinterpret_cast<u32*>(&pTextBox->Translate.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.Y));
					*reinterpret_cast<u32*>(&pTextBox->Translate.Z) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.Z));
					*reinterpret_cast<u32*>(&pTextBox->Rotate.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.X));
					*reinterpret_cast<u32*>(&pTextBox->Rotate.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.Y));
					*reinterpret_cast<u32*>(&pTextBox->Rotate.Z) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.Z));
					*reinterpret_cast<u32*>(&pTextBox->Scale.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Scale.X));
					*reinterpret_cast<u32*>(&pTextBox->Scale.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Scale.Y));
					*reinterpret_cast<u32*>(&pTextBox->Size.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Size.X));
					*reinterpret_cast<u32*>(&pTextBox->Size.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Size.Y));
					pTextBox->TextBufBytes = SDW_CONVERT_ENDIAN16(pTextBox->TextBufBytes);
					pTextBox->TextStrBytes = SDW_CONVERT_ENDIAN16(pTextBox->TextStrBytes);
					pTextBox->MaterialIdx = SDW_CONVERT_ENDIAN16(pTextBox->MaterialIdx);
					pTextBox->FontIdx = SDW_CONVERT_ENDIAN16(pTextBox->FontIdx);
					*reinterpret_cast<u32*>(&pTextBox->ItalicRatio) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ItalicRatio));
					pTextBox->TextStrOffset = SDW_CONVERT_ENDIAN32(pTextBox->TextStrOffset);
					*reinterpret_cast<u32*>(&pTextBox->FontSize.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->FontSize.X));
					*reinterpret_cast<u32*>(&pTextBox->FontSize.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->FontSize.Y));
					*reinterpret_cast<u32*>(&pTextBox->CharSpace) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->CharSpace));
					*reinterpret_cast<u32*>(&pTextBox->LineSpace) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->LineSpace));
					pTextBox->TextIDOffset = SDW_CONVERT_ENDIAN32(pTextBox->TextIDOffset);
					*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.X));
					*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.Y));
					*reinterpret_cast<u32*>(&pTextBox->ShadowScale.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowScale.X));
					*reinterpret_cast<u32*>(&pTextBox->ShadowScale.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowScale.Y));
					*reinterpret_cast<u32*>(&pTextBox->ShadowItalicRatio) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowItalicRatio));
					if (pLyt->FileHeader.Version >= 0x07000000)
					{
						pTextBox->PerCharacterTransformOffset = SDW_CONVERT_ENDIAN32(pTextBox->PerCharacterTransformOffset);
					}
				}
			}
		}
		fp = UFopen(m_sFileName.c_str(), USTR("wb"));
		if (fp == nullptr)
		{
			delete[] pBflyt;
			return 1;
		}
		fwrite(pLyt, 1, uBflytSize, fp);
		if (!vText.empty())
		{
			fwrite(&*vText.begin(), 1, vText.size(), fp);
		}
		pLyt->FileHeader.FileSize = ftell(fp);
		if (m_eEndianness == kEndianBig)
		{
			pLyt->FileHeader.ByteOrder = SDW_CONVERT_ENDIAN16(pLyt->FileHeader.ByteOrder);
			pLyt->FileHeader.HeaderSize = SDW_CONVERT_ENDIAN16(pLyt->FileHeader.HeaderSize);
			pLyt->FileHeader.Version = SDW_CONVERT_ENDIAN32(pLyt->FileHeader.Version);
			pLyt->FileHeader.FileSize = SDW_CONVERT_ENDIAN32(pLyt->FileHeader.FileSize);
			pLyt->FileHeader.DataBlocks = SDW_CONVERT_ENDIAN16(pLyt->FileHeader.DataBlocks);
			pLyt->FileHeader.Reserved = SDW_CONVERT_ENDIAN16(pLyt->FileHeader.Reserved);
		}
		fseek(fp, 0, SEEK_SET);
		fwrite(pLyt, sizeof(SLyt), 1, fp);
		fclose(fp);
	}
	else
	{
		fp = UFopen(m_sFileName.c_str(), USTR("wb"));
		if (fp == nullptr)
		{
			delete[] pBflyt;
			return 1;
		}
		fwrite(pLyt, sizeof(SLyt), 1, fp);
		for (n32 i = 0; i < pLyt->FileHeader.DataBlocks; i++)
		{
			SBinaryBlockHeader* pBinaryBlockHeader = reinterpret_cast<SBinaryBlockHeader*>(pBflyt + uOffset);
			u32 uWriteSize = pBinaryBlockHeader->Size;
			if (m_eEndianness == kEndianBig)
			{
				uWriteSize = SDW_CONVERT_ENDIAN32(uWriteSize);
			}
			if (pBinaryBlockHeader->Signature == kSignatureTextBox)
			{
				STextBox* pTextBox = reinterpret_cast<STextBox*>(pBinaryBlockHeader);
				if (m_eEndianness == kEndianBig)
				{
					*reinterpret_cast<u32*>(&pTextBox->Translate.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.X));
					*reinterpret_cast<u32*>(&pTextBox->Translate.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.Y));
					*reinterpret_cast<u32*>(&pTextBox->Translate.Z) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.Z));
					*reinterpret_cast<u32*>(&pTextBox->Rotate.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.X));
					*reinterpret_cast<u32*>(&pTextBox->Rotate.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.Y));
					*reinterpret_cast<u32*>(&pTextBox->Rotate.Z) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.Z));
					*reinterpret_cast<u32*>(&pTextBox->Scale.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Scale.X));
					*reinterpret_cast<u32*>(&pTextBox->Scale.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Scale.Y));
					*reinterpret_cast<u32*>(&pTextBox->Size.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Size.X));
					*reinterpret_cast<u32*>(&pTextBox->Size.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Size.Y));
					pTextBox->TextBufBytes = SDW_CONVERT_ENDIAN16(pTextBox->TextBufBytes);
					pTextBox->TextStrBytes = SDW_CONVERT_ENDIAN16(pTextBox->TextStrBytes);
					pTextBox->MaterialIdx = SDW_CONVERT_ENDIAN16(pTextBox->MaterialIdx);
					pTextBox->FontIdx = SDW_CONVERT_ENDIAN16(pTextBox->FontIdx);
					*reinterpret_cast<u32*>(&pTextBox->ItalicRatio) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ItalicRatio));
					pTextBox->TextStrOffset = SDW_CONVERT_ENDIAN32(pTextBox->TextStrOffset);
					*reinterpret_cast<u32*>(&pTextBox->FontSize.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->FontSize.X));
					*reinterpret_cast<u32*>(&pTextBox->FontSize.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->FontSize.Y));
					*reinterpret_cast<u32*>(&pTextBox->CharSpace) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->CharSpace));
					*reinterpret_cast<u32*>(&pTextBox->LineSpace) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->LineSpace));
					pTextBox->TextIDOffset = SDW_CONVERT_ENDIAN32(pTextBox->TextIDOffset);
					*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.X));
					*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.Y));
					*reinterpret_cast<u32*>(&pTextBox->ShadowScale.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowScale.X));
					*reinterpret_cast<u32*>(&pTextBox->ShadowScale.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowScale.Y));
					*reinterpret_cast<u32*>(&pTextBox->ShadowItalicRatio) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowItalicRatio));
					if (pLyt->FileHeader.Version >= 0x07000000)
					{
						pTextBox->PerCharacterTransformOffset = SDW_CONVERT_ENDIAN32(pTextBox->PerCharacterTransformOffset);
					}
				}
				if (pTextBox->TextStrOffset != 0)
				{
					Char16_t* pTextStr = reinterpret_cast<Char16_t*>(pBflyt + uOffset + pTextBox->TextStrOffset);
					if (memcmp(pTextStr, s_nUnused, sizeof(s_nUnused)) != 0)
					{
						if (sTxt.empty())
						{
							FILE* fpTxt = UFopen(m_sTxtName.c_str(), USTR("rb"));
							if (fpTxt == nullptr)
							{
								fclose(fp);
								delete[] pBflyt;
								return 1;
							}
							fseek(fpTxt, 0, SEEK_END);
							u32 uTxtSize = ftell(fpTxt);
							if (uTxtSize % 2 != 0)
							{
								fclose(fpTxt);
								fclose(fp);
								delete[] pBflyt;
								return 1;
							}
							uTxtSize /= 2;
							fseek(fpTxt, 0, SEEK_SET);
							Char16_t* pTemp = new Char16_t[uTxtSize + 1];
							fread(pTemp, 2, uTxtSize, fpTxt);
							fclose(fpTxt);
							if (pTemp[0] != 0xFEFF)
							{
								delete[] pTemp;
								fclose(fp);
								delete[] pBflyt;
								return 1;
							}
							pTemp[uTxtSize] = 0;
							sTxt = U16ToW(pTemp + 1);
							delete[] pTemp;
						}
						wstring sNum = Format(L"No.%d", nIndex++);
						uPos0 = sTxt.find(sNum, uPos0);
						if (uPos0 == wstring::npos)
						{
							fclose(fp);
							delete[] pBflyt;
							return 1;
						}
						uPos0 += sNum.size();
						if (StartWith(sTxt, L" ", static_cast<u32>(uPos0)))
						{
							wstring::size_type uPos1 = sTxt.find(L"\r\n", uPos0);
							if (uPos1 == wstring::npos)
							{
								fclose(fp);
								delete[] pBflyt;
								return 1;
							}
							wstring sTextBoxSize = Trim(sTxt.substr(uPos0, uPos1 - uPos0));
							vector<wstring> vTextBoxSize = Split(sTextBoxSize, L",");
							if (vTextBoxSize.size() != 2)
							{
								fclose(fp);
								delete[] pBflyt;
								return 1;
							}
							n32 nTextBoxWidth = SToN32(vTextBoxSize[0]);
							n32 nTextBoxHeight = SToN32(vTextBoxSize[1]);
							if (nTextBoxWidth != static_cast<n32>(pTextBox->Size.X))
							{
								pTextBox->Size.X = static_cast<f32>(nTextBoxWidth);
							}
							if (nTextBoxHeight != static_cast<n32>(pTextBox->Size.Y))
							{
								pTextBox->Size.Y = static_cast<f32>(nTextBoxHeight);
							}
							uPos0 = uPos1;
						}
						uPos0 = sTxt.find(L"======================================\r\n", uPos0);
						if (uPos0 == wstring::npos)
						{
							fclose(fp);
							delete[] pBflyt;
							return 1;
						}
						uPos0 += wcslen(L"======================================\r\n");
						wstring::size_type uPos1 = sTxt.find(L"\r\n--------------------------------------", uPos0);
						if (uPos1 == wstring::npos)
						{
							fclose(fp);
							delete[] pBflyt;
							return 1;
						}
						wstring sStmt = sTxt.substr(uPos0, uPos1 - uPos0);
						uPos0 = uPos1 + wcslen(L"\r\n--------------------------------------");
						sStmt = Replace(sStmt, L"\r\n", L"\n");
						sStmt = Replace(sStmt, L"<r>", L"\r");
						sStmt = Replace(sStmt, L"[======================================]", L"======================================");
						sStmt = Replace(sStmt, L"[--------------------------------------]", L"--------------------------------------");
						sStmt = Replace(sStmt, L"[No].", L"No.");
						U16String sStmt16 = WToU16(sStmt);
						for (map<U16String, u32>::iterator it = m_mExtStr.begin(); it != m_mExtStr.end(); ++it)
						{
							const U16String& sPrivateChar = it->first;
							Char16_t uPrivateChar = it->second;
							uPos1 = 0;
							while ((uPos1 = sStmt16.find(sPrivateChar, uPos1)) != U16String::npos)
							{
								sStmt16.replace(uPos1, sPrivateChar.size(), 1, uPrivateChar);
								uPos1++;
							}
						}
						if (!m_sFakeCharsetName.empty())
						{
							for (n32 j = 0; j < static_cast<n32>(sStmt16.size()); j++)
							{
								Char16_t uU16Code = m_uFakeCharset[sStmt16[j]];
								if (uU16Code != 0)
								{
									sStmt16[j] = uU16Code;
								}
							}
						}
						bool bHasTextID = false;
						string sTextID;
						if (pTextBox->TextIDOffset != 0)
						{
							bHasTextID = true;
							sTextID = reinterpret_cast<char*>(pBflyt) + uOffset + pTextBox->TextIDOffset;
						}
						bool bHasPerCharacterTransform = false;
						if (pLyt->FileHeader.Version >= 0x07000000)
						{
							if (pTextBox->PerCharacterTransformOffset != 0)
							{
								bHasPerCharacterTransform = true;
							}
						}
						pTextBox->TextStrBytes = static_cast<u16>(sStmt16.size() * 2 + 2);
						if (pTextBox->TextBufBytes > pTextBox->TextStrBytes)
						{
							pTextBox->TextBoxFlag |= SDW_BIT32(kTextBoxFlagFORCE_ASSIGN_TEXT_LENGTH);
						}
						else
						{
							pTextBox->TextBufBytes = pTextBox->TextStrBytes;
						}
						sStmt16.resize(static_cast<u32>(Align(sStmt16.size() + 1, 2)));
						if (bHasTextID)
						{
							sTextID.resize(static_cast<u32>(Align(sTextID.size() + 1, 4)));
						}
						vector<u8> vPerCharacterTransform;
						if (pLyt->FileHeader.Version >= 0x07000000)
						{
							if (bHasPerCharacterTransform)
							{
								if (pTextBox->BlockHeader.Size - pTextBox->PerCharacterTransformOffset == sizeof(SPerCharacterTransform))
								{
									vPerCharacterTransform.resize(sizeof(SPerCharacterTransform));
								}
								else
								{
									vPerCharacterTransform.resize(pTextBox->BlockHeader.Size - pTextBox->PerCharacterTransformOffset);
								}
								memcpy(&*vPerCharacterTransform.begin(), pBflyt + uOffset + pTextBox->PerCharacterTransformOffset, vPerCharacterTransform.size());
							}
						}
						if (pLyt->FileHeader.Version >= 0x07000000)
						{
							pTextBox->TextStrOffset = sizeof(STextBox);
						}
						else
						{
							pTextBox->TextStrOffset = sizeof(STextBox) - 4;
						}
						pTextBox->BlockHeader.Size = pTextBox->TextStrOffset + static_cast<u32>(sStmt16.size() * 2);
						if (bHasTextID)
						{
							pTextBox->TextIDOffset = pTextBox->BlockHeader.Size;
							pTextBox->BlockHeader.Size += static_cast<u32>(sTextID.size());
						}
						if (pLyt->FileHeader.Version >= 0x07000000)
						{
							if (bHasPerCharacterTransform)
							{
								pTextBox->PerCharacterTransformOffset = pTextBox->BlockHeader.Size;
								pTextBox->BlockHeader.Size += static_cast<u32>(vPerCharacterTransform.size());
							}
						}
						if (m_eEndianness == kEndianBig)
						{
							pBinaryBlockHeader->Size = SDW_CONVERT_ENDIAN32(pBinaryBlockHeader->Size);
						}
						if (m_eEndianness == kEndianBig)
						{
							*reinterpret_cast<u32*>(&pTextBox->Translate.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.X));
							*reinterpret_cast<u32*>(&pTextBox->Translate.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.Y));
							*reinterpret_cast<u32*>(&pTextBox->Translate.Z) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.Z));
							*reinterpret_cast<u32*>(&pTextBox->Rotate.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.X));
							*reinterpret_cast<u32*>(&pTextBox->Rotate.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.Y));
							*reinterpret_cast<u32*>(&pTextBox->Rotate.Z) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.Z));
							*reinterpret_cast<u32*>(&pTextBox->Scale.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Scale.X));
							*reinterpret_cast<u32*>(&pTextBox->Scale.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Scale.Y));
							*reinterpret_cast<u32*>(&pTextBox->Size.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Size.X));
							*reinterpret_cast<u32*>(&pTextBox->Size.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Size.Y));
							pTextBox->TextBufBytes = SDW_CONVERT_ENDIAN16(pTextBox->TextBufBytes);
							pTextBox->TextStrBytes = SDW_CONVERT_ENDIAN16(pTextBox->TextStrBytes);
							pTextBox->MaterialIdx = SDW_CONVERT_ENDIAN16(pTextBox->MaterialIdx);
							pTextBox->FontIdx = SDW_CONVERT_ENDIAN16(pTextBox->FontIdx);
							*reinterpret_cast<u32*>(&pTextBox->ItalicRatio) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ItalicRatio));
							pTextBox->TextStrOffset = SDW_CONVERT_ENDIAN32(pTextBox->TextStrOffset);
							*reinterpret_cast<u32*>(&pTextBox->FontSize.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->FontSize.X));
							*reinterpret_cast<u32*>(&pTextBox->FontSize.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->FontSize.Y));
							*reinterpret_cast<u32*>(&pTextBox->CharSpace) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->CharSpace));
							*reinterpret_cast<u32*>(&pTextBox->LineSpace) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->LineSpace));
							pTextBox->TextIDOffset = SDW_CONVERT_ENDIAN32(pTextBox->TextIDOffset);
							*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.X));
							*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.Y));
							*reinterpret_cast<u32*>(&pTextBox->ShadowScale.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowScale.X));
							*reinterpret_cast<u32*>(&pTextBox->ShadowScale.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowScale.Y));
							*reinterpret_cast<u32*>(&pTextBox->ShadowItalicRatio) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowItalicRatio));
							if (pLyt->FileHeader.Version >= 0x07000000)
							{
								pTextBox->PerCharacterTransformOffset = SDW_CONVERT_ENDIAN32(pTextBox->PerCharacterTransformOffset);
							}
						}
						if (pLyt->FileHeader.Version >= 0x07000000)
						{
							fwrite(pTextBox, sizeof(STextBox), 1, fp);
						}
						else
						{
							fwrite(pTextBox, sizeof(STextBox) - 4, 1, fp);
						}
						if (m_eEndianness == kEndianBig)
						{
							for (n32 j = 0; j < static_cast<n32>(sStmt16.size()); j++)
							{
								sStmt16[j] = SDW_CONVERT_ENDIAN16(sStmt16[j]);
							}
						}
						fwrite(sStmt16.c_str(), 2, sStmt16.size(), fp);
						if (bHasTextID)
						{
							fwrite(sTextID.c_str(), 1, sTextID.size(), fp);
						}
						if (bHasPerCharacterTransform)
						{
							fwrite(&*vPerCharacterTransform.begin(), 1, vPerCharacterTransform.size(), fp);
						}
					}
					else
					{
						if (m_eEndianness == kEndianBig)
						{
							*reinterpret_cast<u32*>(&pTextBox->Translate.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.X));
							*reinterpret_cast<u32*>(&pTextBox->Translate.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.Y));
							*reinterpret_cast<u32*>(&pTextBox->Translate.Z) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.Z));
							*reinterpret_cast<u32*>(&pTextBox->Rotate.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.X));
							*reinterpret_cast<u32*>(&pTextBox->Rotate.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.Y));
							*reinterpret_cast<u32*>(&pTextBox->Rotate.Z) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.Z));
							*reinterpret_cast<u32*>(&pTextBox->Scale.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Scale.X));
							*reinterpret_cast<u32*>(&pTextBox->Scale.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Scale.Y));
							*reinterpret_cast<u32*>(&pTextBox->Size.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Size.X));
							*reinterpret_cast<u32*>(&pTextBox->Size.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Size.Y));
							pTextBox->TextBufBytes = SDW_CONVERT_ENDIAN16(pTextBox->TextBufBytes);
							pTextBox->TextStrBytes = SDW_CONVERT_ENDIAN16(pTextBox->TextStrBytes);
							pTextBox->MaterialIdx = SDW_CONVERT_ENDIAN16(pTextBox->MaterialIdx);
							pTextBox->FontIdx = SDW_CONVERT_ENDIAN16(pTextBox->FontIdx);
							*reinterpret_cast<u32*>(&pTextBox->ItalicRatio) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ItalicRatio));
							pTextBox->TextStrOffset = SDW_CONVERT_ENDIAN32(pTextBox->TextStrOffset);
							*reinterpret_cast<u32*>(&pTextBox->FontSize.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->FontSize.X));
							*reinterpret_cast<u32*>(&pTextBox->FontSize.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->FontSize.Y));
							*reinterpret_cast<u32*>(&pTextBox->CharSpace) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->CharSpace));
							*reinterpret_cast<u32*>(&pTextBox->LineSpace) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->LineSpace));
							pTextBox->TextIDOffset = SDW_CONVERT_ENDIAN32(pTextBox->TextIDOffset);
							*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.X));
							*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.Y));
							*reinterpret_cast<u32*>(&pTextBox->ShadowScale.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowScale.X));
							*reinterpret_cast<u32*>(&pTextBox->ShadowScale.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowScale.Y));
							*reinterpret_cast<u32*>(&pTextBox->ShadowItalicRatio) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowItalicRatio));
							if (pLyt->FileHeader.Version >= 0x07000000)
							{
								pTextBox->PerCharacterTransformOffset = SDW_CONVERT_ENDIAN32(pTextBox->PerCharacterTransformOffset);
							}
						}
						fwrite(pBinaryBlockHeader, 1, uWriteSize, fp);
					}
				}
				else
				{
					if (m_eEndianness == kEndianBig)
					{
						*reinterpret_cast<u32*>(&pTextBox->Translate.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.X));
						*reinterpret_cast<u32*>(&pTextBox->Translate.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.Y));
						*reinterpret_cast<u32*>(&pTextBox->Translate.Z) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Translate.Z));
						*reinterpret_cast<u32*>(&pTextBox->Rotate.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.X));
						*reinterpret_cast<u32*>(&pTextBox->Rotate.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.Y));
						*reinterpret_cast<u32*>(&pTextBox->Rotate.Z) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Rotate.Z));
						*reinterpret_cast<u32*>(&pTextBox->Scale.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Scale.X));
						*reinterpret_cast<u32*>(&pTextBox->Scale.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Scale.Y));
						*reinterpret_cast<u32*>(&pTextBox->Size.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Size.X));
						*reinterpret_cast<u32*>(&pTextBox->Size.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->Size.Y));
						pTextBox->TextBufBytes = SDW_CONVERT_ENDIAN16(pTextBox->TextBufBytes);
						pTextBox->TextStrBytes = SDW_CONVERT_ENDIAN16(pTextBox->TextStrBytes);
						pTextBox->MaterialIdx = SDW_CONVERT_ENDIAN16(pTextBox->MaterialIdx);
						pTextBox->FontIdx = SDW_CONVERT_ENDIAN16(pTextBox->FontIdx);
						*reinterpret_cast<u32*>(&pTextBox->ItalicRatio) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ItalicRatio));
						pTextBox->TextStrOffset = SDW_CONVERT_ENDIAN32(pTextBox->TextStrOffset);
						*reinterpret_cast<u32*>(&pTextBox->FontSize.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->FontSize.X));
						*reinterpret_cast<u32*>(&pTextBox->FontSize.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->FontSize.Y));
						*reinterpret_cast<u32*>(&pTextBox->CharSpace) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->CharSpace));
						*reinterpret_cast<u32*>(&pTextBox->LineSpace) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->LineSpace));
						pTextBox->TextIDOffset = SDW_CONVERT_ENDIAN32(pTextBox->TextIDOffset);
						*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.X));
						*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowOffset.Y));
						*reinterpret_cast<u32*>(&pTextBox->ShadowScale.X) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowScale.X));
						*reinterpret_cast<u32*>(&pTextBox->ShadowScale.Y) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowScale.Y));
						*reinterpret_cast<u32*>(&pTextBox->ShadowItalicRatio) = SDW_CONVERT_ENDIAN32(*reinterpret_cast<u32*>(&pTextBox->ShadowItalicRatio));
						if (pLyt->FileHeader.Version >= 0x07000000)
						{
							pTextBox->PerCharacterTransformOffset = SDW_CONVERT_ENDIAN32(pTextBox->PerCharacterTransformOffset);
						}
					}
					fwrite(pBinaryBlockHeader, 1, uWriteSize, fp);
				}
			}
			else if (pBinaryBlockHeader->Signature == kSignatureParts)
			{
				// TODO: support later
				fwrite(pBinaryBlockHeader, 1, uWriteSize, fp);
			}
			else
			{
				fwrite(pBinaryBlockHeader, 1, uWriteSize, fp);
			}
			uOffset += uWriteSize;
		}
		pLyt->FileHeader.FileSize = ftell(fp);
		if (m_eEndianness == kEndianBig)
		{
			pLyt->FileHeader.ByteOrder = SDW_CONVERT_ENDIAN16(pLyt->FileHeader.ByteOrder);
			pLyt->FileHeader.HeaderSize = SDW_CONVERT_ENDIAN16(pLyt->FileHeader.HeaderSize);
			pLyt->FileHeader.Version = SDW_CONVERT_ENDIAN32(pLyt->FileHeader.Version);
			pLyt->FileHeader.FileSize = SDW_CONVERT_ENDIAN32(pLyt->FileHeader.FileSize);
			pLyt->FileHeader.DataBlocks = SDW_CONVERT_ENDIAN16(pLyt->FileHeader.DataBlocks);
			pLyt->FileHeader.Reserved = SDW_CONVERT_ENDIAN16(pLyt->FileHeader.Reserved);
		}
		fseek(fp, 0, SEEK_SET);
		fwrite(pLyt, sizeof(SLyt), 1, fp);
		fclose(fp);
	}
	delete[] pBflyt;
	return bResult;
}

bool CBflyt::IsBflytFile(const UString& a_sFileName)
{
	FILE* fp = UFopen(a_sFileName.c_str(), USTR("rb"));
	if (fp == nullptr)
	{
		return false;
	}
	u32 uSignature = 0;
	fread(&uSignature, sizeof(uSignature), 1, fp);
	fclose(fp);
	return uSignature == kSignatureFLYT;
}

bool CBflyt::initExt()
{
	m_mExt[0xE000] = WToU16(L"DS_A");
	m_mExt[0xE001] = WToU16(L"DS_B");
	m_mExt[0xE002] = WToU16(L"DS_X");
	m_mExt[0xE003] = WToU16(L"DS_Y");
	m_mExt[0xE004] = WToU16(L"DS_L");
	m_mExt[0xE005] = WToU16(L"DS_R");
	m_mExt[0xE006] = WToU16(L"DS_DPAD");
	m_mExt[0xE007] = WToU16(L"DS_ALARM_CLOCK");
	m_mExt[0xE008] = WToU16(L"DS_SMILE_FACE");
	m_mExt[0xE009] = WToU16(L"DS_ANGRY_FACE");
	m_mExt[0xE00A] = WToU16(L"DS_DISAPPOINTED_FACE");
	m_mExt[0xE00B] = WToU16(L"DS_EXPRESSIONLESS_FACE");
	m_mExt[0xE00C] = WToU16(L"DS_SUN");
	m_mExt[0xE00D] = WToU16(L"DS_CLOUD");
	m_mExt[0xE00E] = WToU16(L"DS_UNBRELLA");
	m_mExt[0xE00F] = WToU16(L"DS_SNOWMAN");
	m_mExt[0xE010] = WToU16(L"DS_EXCLAMATION_MARK");
	m_mExt[0xE011] = WToU16(L"DS_QUESTION_MARK");
	m_mExt[0xE012] = WToU16(L"DS_ENVELOPE");
	m_mExt[0xE013] = WToU16(L"DS_MOBILE_PHONE");
	m_mExt[0xE014] = WToU16(L"DS_HOSPITAL");
	m_mExt[0xE015] = WToU16(L"DS_SPADE");
	m_mExt[0xE016] = WToU16(L"DS_DIAMOND");
	m_mExt[0xE017] = WToU16(L"DS_HEART");
	m_mExt[0xE018] = WToU16(L"DS_CLUB");
	m_mExt[0xE019] = WToU16(L"DS_ARROW_RIGHT");
	m_mExt[0xE01A] = WToU16(L"DS_ARROW_LEFT");
	m_mExt[0xE01B] = WToU16(L"DS_ARROW_UP");
	m_mExt[0xE01C] = WToU16(L"DS_ARROW_DOWN");
	m_mExt[0xE01D] = WToU16(L"DS_CALIBRATION");
	m_mExt[0xE01E] = WToU16(L"DS_CAMERA");
	m_mExt[0xE01F] = WToU16(L"DS_BOXED_CROSS_E01F");
	m_mExt[0xE020] = WToU16(L"DS_LOADING_TOP_LEFT");
	m_mExt[0xE021] = WToU16(L"DS_LOADING_TOP");
	m_mExt[0xE022] = WToU16(L"DS_LOADING_TOP_RIGHT");
	m_mExt[0xE023] = WToU16(L"DS_LOADING_RIGHT");
	m_mExt[0xE024] = WToU16(L"DS_LOADING_BOTTOM_RIGHT");
	m_mExt[0xE025] = WToU16(L"DS_LOADING_BOTTOM");
	m_mExt[0xE026] = WToU16(L"DS_LOADING_BOTTOM_LEFT");
	m_mExt[0xE027] = WToU16(L"DS_LOADING_LEFT");
	m_mExt[0xE028] = WToU16(L"DS_CROSS");
	m_mExt[0xE029] = WToU16(L"DS_PICTOCHAT_ROOM_A");
	m_mExt[0xE02A] = WToU16(L"DS_PICTOCHAT_ROOM_B");
	m_mExt[0xE02B] = WToU16(L"DS_PICTOCHAT_ROOM_C");
	m_mExt[0xE02C] = WToU16(L"DS_PICTOCHAT_ROOM_D");
	m_mExt[0xE02D] = WToU16(L"DS_BOOT_AUTOMATIC");
	m_mExt[0xE02E] = WToU16(L"DS_BOOT_MANUAL");
	m_mExt[0xE02F] = WToU16(L"DS_BOXED_CROSS_E02F");
	m_mExt[0xE030] = WToU16(L"DS_PICTOCHAT_LOGO_P");
	m_mExt[0xE031] = WToU16(L"DS_PICTOCHAT_LOGO_I");
	m_mExt[0xE032] = WToU16(L"DS_PICTOCHAT_LOGO_C");
	m_mExt[0xE033] = WToU16(L"DS_PICTOCHAT_LOGO_T");
	m_mExt[0xE034] = WToU16(L"DS_PICTOCHAT_LOGO_H");
	m_mExt[0xE035] = WToU16(L"DS_PICTOCHAT_LOGO_A");
	m_mExt[0xE036] = WToU16(L"DS_BOXED_CROSS_E036");
	m_mExt[0xE037] = WToU16(L"DS_BOXED_CROSS_E037");
	m_mExt[0xE038] = WToU16(L"DS_BOXED_CROSS_E038");
	m_mExt[0xE039] = WToU16(L"DS_BOXED_CROSS_E039");
	m_mExt[0xE03A] = WToU16(L"DS_BOXED_CROSS_E03A");
	m_mExt[0xE03B] = WToU16(L"DS_BOXED_CROSS_E03B");
	m_mExt[0xE03C] = WToU16(L"DS_BOXED_CROSS_E03C");
	m_mExt[0xE03D] = WToU16(L"DS_BOXED_CROSS_E03D");
	m_mExt[0xE03E] = WToU16(L"DS_BOXED_INVERTED_SMALL_CROSS");
	m_mExt[0xE03F] = WToU16(L"DS_BOXED_INVERTED_LARGE_CROSS");
	m_mExt[0xE040] = WToU16(L"WII_POWER");
	m_mExt[0xE041] = WToU16(L"WII_DPAD");
	m_mExt[0xE042] = WToU16(L"WII_A");
	m_mExt[0xE043] = WToU16(L"WII_B");
	m_mExt[0xE044] = WToU16(L"WII_HOME");
	m_mExt[0xE045] = WToU16(L"WII_+START");
	m_mExt[0xE046] = WToU16(L"WII_-SELECT");
	m_mExt[0xE047] = WToU16(L"WII_1");
	m_mExt[0xE048] = WToU16(L"WII_2");
	m_mExt[0xE049] = WToU16(L"WII_STICK");
	m_mExt[0xE04A] = WToU16(L"WII_C");
	m_mExt[0xE04B] = WToU16(L"WII_Z");
	m_mExt[0xE04C] = WToU16(L"WII_a");
	m_mExt[0xE04D] = WToU16(L"WII_b");
	m_mExt[0xE04E] = WToU16(L"WII_x");
	m_mExt[0xE04F] = WToU16(L"WII_y");
	m_mExt[0xE050] = WToU16(L"WII_L_STICK");
	m_mExt[0xE051] = WToU16(L"WII_R_STICK");
	m_mExt[0xE052] = WToU16(L"WII_L");
	m_mExt[0xE053] = WToU16(L"WII_R");
	m_mExt[0xE054] = WToU16(L"WII_ZL");
	m_mExt[0xE055] = WToU16(L"WII_ZR");
	m_mExt[0xE056] = WToU16(L"WII_ENTER");
	m_mExt[0xE057] = WToU16(L"WII_SPACE");
	m_mExt[0xE058] = WToU16(L"WII_POINTER");
	m_mExt[0xE059] = WToU16(L"WII_POINTER_1");
	m_mExt[0xE05A] = WToU16(L"WII_POINTER_2");
	m_mExt[0xE05B] = WToU16(L"WII_POINTER_3");
	m_mExt[0xE05C] = WToU16(L"WII_POINTER_4");
	m_mExt[0xE05D] = WToU16(L"WII_POINTER_GRAB");
	m_mExt[0xE05E] = WToU16(L"WII_POINTER_GRAB_1");
	m_mExt[0xE05F] = WToU16(L"WII_POINTER_GRAB_2");
	m_mExt[0xE060] = WToU16(L"WII_POINTER_GRAB_3");
	m_mExt[0xE061] = WToU16(L"WII_POINTER_GRAB_4");
	m_mExt[0xE062] = WToU16(L"WII_POINTER_OPEN");
	m_mExt[0xE063] = WToU16(L"WII_POINTER_OPEN_1");
	m_mExt[0xE064] = WToU16(L"WII_POINTER_OPEN_2");
	m_mExt[0xE065] = WToU16(L"WII_POINTER_OPEN_3");
	m_mExt[0xE066] = WToU16(L"WII_POINTER_OPEN_4");
	m_mExt[0xE067] = WToU16(L"WII_Wii_LOGO");
	m_mExt[0xE068] = WToU16(L"WII_SUPERSCRIPT_er");
	m_mExt[0xE069] = WToU16(L"WII_SUPERSCRIPT_re");
	m_mExt[0xE06A] = WToU16(L"WII_SUPERSCRIPT_e");
	m_mExt[0xE06B] = WToU16(L"WII_ALTERNATE_CHARACTERS");
	m_mExt[0xE06C] = WToU16(L"WII_NULL_E06C");
	m_mExt[0xE06D] = WToU16(L"WII_NULL_E06D");
	m_mExt[0xE06E] = WToU16(L"WII_NULL_E06E");
	m_mExt[0xE06F] = WToU16(L"WII_NULL_E06F");
	m_mExt[0xE070] = WToU16(L"3DS_BOXED_CROSS");
	m_mExt[0xE071] = WToU16(L"3DS_BOXED_INVERTED_CROSS");
	m_mExt[0xE072] = WToU16(L"3DS_BACK");
	m_mExt[0xE073] = WToU16(L"3DS_HOME");
	m_mExt[0xE074] = WToU16(L"3DS_STEPS_TAKEN");
	m_mExt[0xE075] = WToU16(L"3DS_PLAY_COINS");
	m_mExt[0xE076] = WToU16(L"3DS_MOVIE_CAMERA");
	m_mExt[0xE077] = WToU16(L"3DS_CIRCLE_PAD");
	m_mExt[0xE078] = WToU16(L"3DS_POWER");
	m_mExt[0xE079] = WToU16(L"3DS_DPAD_UP");
	m_mExt[0xE07A] = WToU16(L"3DS_DPAD_DOWN");
	m_mExt[0xE07B] = WToU16(L"3DS_DPAD_LEFT");
	m_mExt[0xE07C] = WToU16(L"3DS_DPAD_RIGHT");
	m_mExt[0xE07D] = WToU16(L"3DS_DPAD_UP_DOWN");
	m_mExt[0xE07E] = WToU16(L"3DS_DPAD_LEFT_RIGHT");
	m_mExt[0xE07F] = WToU16(L"3DS_NULL");
	m_mExt[0xE080] = WToU16(L"WIIU_STICK");
	m_mExt[0xE081] = WToU16(L"WIIU_L_STICK");
	m_mExt[0xE082] = WToU16(L"WIIU_R_STICK");
	m_mExt[0xE083] = WToU16(L"WIIU_L");
	m_mExt[0xE084] = WToU16(L"WIIU_R");
	m_mExt[0xE085] = WToU16(L"WIIU_ZL");
	m_mExt[0xE086] = WToU16(L"WIIU_ZR");
	m_mExt[0xE087] = WToU16(L"WIIU_GAMEPAD");
	m_mExt[0xE088] = WToU16(L"WIIU_WII_REMOTE");
	m_mExt[0xE089] = WToU16(L"WIIU_TV");
	m_mExt[0xE08A] = WToU16(L"WIIU_L_STICK_PRESS");
	m_mExt[0xE08B] = WToU16(L"WIIU_R_STICK_PRESS");
	m_mExt[0xE08C] = WToU16(L"WIIU_ARROW_LEFT_RIGHT");
	m_mExt[0xE08D] = WToU16(L"WIIU_ARROW_UP_DOWN");
	m_mExt[0xE08E] = WToU16(L"WIIU_ARROW_CLOCKWISE");
	m_mExt[0xE08F] = WToU16(L"WIIU_ARROW_COUNTER_CLOCKWISE");
	m_mExt[0xE090] = WToU16(L"WIIU_ARROW_RIGHT");
	m_mExt[0xE091] = WToU16(L"WIIU_ARROW_LEFT");
	m_mExt[0xE092] = WToU16(L"WIIU_ARROW_UP");
	m_mExt[0xE093] = WToU16(L"WIIU_ARROW_DOWN");
	m_mExt[0xE094] = WToU16(L"WIIU_ARROW_UP_RIGHT");
	m_mExt[0xE095] = WToU16(L"WIIU_ARROW_DOWN_RIGHT");
	m_mExt[0xE096] = WToU16(L"WIIU_ARROW_DOWN_LEFT");
	m_mExt[0xE097] = WToU16(L"WIIU_ARROW_UP_LEFT");
	m_mExt[0xE098] = WToU16(L"WIIU_CROSS");
	m_mExt[0xE099] = WToU16(L"WIIU_NFC");
	for (u32 i = 0xE000; i <= 0xE757; i++)
	{
		m_mExt.insert(make_pair(i, WToU16(Format(L"%04X", i))));
		m_mExt[i] = WToU16(Format(L"<P=%ls>", U16ToW(m_mExt[i]).c_str()));
	}
	for (map<u32, U16String>::iterator it = m_mExt.begin(); it != m_mExt.end(); ++it)
	{
		if (!m_mExtStr.insert(make_pair(it->second, it->first)).second)
		{
			UPrintf(USTR("ERROR: extended characters %") PRIUS USTR(" more than 1\n\n"), U16ToU(it->second).c_str());
			return false;
		}
	}
	return true;
}

bool CBflyt::loadFakeCharset(bool a_bSwap)
{
	FILE* fp = UFopen(m_sFakeCharsetName.c_str(), USTR("rb"));
	if (fp == nullptr)
	{
		return false;
	}
	if (a_bSwap)
	{
		Char16_t uFakeCharset[0x10000] = {};
		fread(uFakeCharset, 2, 0x10000, fp);
		for (n32 i = 0; i < 0x10000; i++)
		{
			Char16_t uU16CodeNew = uFakeCharset[i];
			if (uU16CodeNew != 0 && m_uFakeCharset[uU16CodeNew] == 0)
			{
				m_uFakeCharset[uU16CodeNew] = static_cast<Char16_t>(i);
			}
		}
	}
	else
	{
		fread(m_uFakeCharset, 2, 0x10000, fp);
	}
	fclose(fp);
	return true;
}
