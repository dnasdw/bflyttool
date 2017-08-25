#include "bflyt.h"

CBflyt::CBflyt()
	: m_bVerbose(false)
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

void CBflyt::SetVerbose(bool a_bVerbose)
{
	m_bVerbose = a_bVerbose;
}

bool CBflyt::ExportText()
{
	bool bResult = true;
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
				Char16_t* pTextStr = reinterpret_cast<Char16_t*>(pBflyt + uOffset + pTextBox->TextStrOffset);
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
				fu16printf(fp, L"No.%d\r\n", nIndex++);
				fu16printf(fp, L"--------------------------------------\r\n");
				fu16printf(fp, L"%ls\r\n", sTxt.c_str());
				fu16printf(fp, L"======================================\r\n");
				fu16printf(fp, L"%ls\r\n", sTxt.c_str());
				fu16printf(fp, L"--------------------------------------\r\n");
			}
		}
		else if (pBinaryBlockHeader->Signature == kSignatureParts)
		{
			// TODO: support later
		}
		uOffset += pBinaryBlockHeader->Size;
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
	fp = UFopen(m_sFileName.c_str(), USTR("wb"));
	if (fp == nullptr)
	{
		delete[] pBflyt;
		return 1;
	}
	fwrite(pLyt, sizeof(SLyt), 1, fp);
	wstring sTxt;
	n32 nIndex = 0;
	wstring::size_type uPos0 = 0;
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
				wstring sNum = Format(L"No.%d\r\n", nIndex++);
				uPos0 = sTxt.find(sNum, uPos0);
				if (uPos0 == wstring::npos)
				{
					fclose(fp);
					delete[] pBflyt;
					return 1;
				}
				uPos0 += sNum.size();
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
				for (n32 j = 0; j < static_cast<n32>(sStmt16.size()); j++)
				{
					Char16_t uU16Code = m_uFakeCharset[sStmt16[j]];
					if (uU16Code != 0)
					{
						sStmt16[j] = uU16Code;
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
