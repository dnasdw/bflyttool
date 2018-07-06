#ifndef BFLYT_H_
#define BFLYT_H_

#include <sdw.h>

enum ETextColor
{
	kTextColorTOP,
	kTextColorBOTTOM,
	kTextColorMAX
};

enum EConst
{
	kConstResourceNameStrMax = 24,
	kConstUserDataStrMax = 8
};

enum ETextBoxFlag
{
	kTextBoxFlagSHADOW_ENABLED,
	kTextBoxFlagFORCE_ASSIGN_TEXT_LENGTH,
	kTextBoxFlagINVISIBLE_BORDER_ENABLED,
	kTextBoxFlagDOUBLE_DRAWN_BORDER_ENABLED,
	kTextBoxFlagPER_CHARACTER_TRANSFORM_ENABLED,
	kTextBoxFlagMAX
};

#include SDW_MSC_PUSH_PACKED
struct SBinaryFileHeader
{
	u32 Signature;
	u16 ByteOrder;
	u16 HeaderSize;
	u32 Version;
	u32 FileSize;
	u16 DataBlocks;
	u16 Reserved;
} SDW_GNUC_PACKED;

struct SBinaryBlockHeader
{
	u32 Signature;
	u32 Size;
} SDW_GNUC_PACKED;

struct SVec2
{
	f32 X;
	f32 Y;
} SDW_GNUC_PACKED;

struct SVec3
{
	f32 X;
	f32 Y;
	f32 Z;
} SDW_GNUC_PACKED;

struct SLyt
{
	SBinaryFileHeader FileHeader;
} SDW_GNUC_PACKED;

struct SColor
{
	u8 R;
	u8 G;
	u8 B;
	u8 A;
} SDW_GNUC_PACKED;

struct SPane
{
	SBinaryBlockHeader BlockHeader;
	u8 Flag;
	u8 BasePosition;
	u8 Alpha;
	u8 FlagEx;
	char Name[kConstResourceNameStrMax];
	char UserData[kConstUserDataStrMax];
	SVec3 Translate;
	SVec3 Rotate;
	SVec2 Scale;
	SVec2 Size;
} SDW_GNUC_PACKED;

struct SPerCharacterTransform
{
	f32 EvalTimeOffset;
	f32 EvalTimeWidth;
	u8 LoopType;
	u8 OriginV;
	u8 HasAnimationInfo;// TODO
	u8 Padding[1];
} SDW_GNUC_PACKED;

struct STextBox : public SPane
{
	u16 TextBufBytes;
	u16 TextStrBytes;
	u16 MaterialIdx;
	u16 FontIdx;
	u8 TextPosition;
	u8 TextAlignment;
	u8 TextBoxFlag;
	u8 Padding[1];
	f32 ItalicRatio;
	u32 TextStrOffset;
	SColor TextCols[kTextColorMAX];
	SVec2 FontSize;
	f32 CharSpace;
	f32 LineSpace;
	u32 TextIDOffset;
	SVec2 ShadowOffset;
	SVec2 ShadowScale;
	SColor ShadowCols[kTextColorMAX];
	f32 ShadowItalicRatio;
	u32 PerCharacterTransformOffset;	// 7.0.0.0 added
	//Char16_t Text[1];
} SDW_GNUC_PACKED;
#include SDW_MSC_POP_PACKED

class CBflyt
{
public:
	enum EEndianness
	{
		kEndianBig,
		kEndianLittle
	};
	enum ESignature
	{
		kSignatureFLYT = SDW_CONVERT_ENDIAN32('FLYT'),
		kSignatureTextBox = SDW_CONVERT_ENDIAN32('txt1'),
		kSignatureParts = SDW_CONVERT_ENDIAN32('prt1')
	};
	CBflyt();
	~CBflyt();
	void SetFileName(const UString& a_sFileName);
	void SetTxtName(const UString& a_sTxtName);
	void SetFakeCharsetName(const UString& a_sFakeCharset);
	void SetForce(bool a_bForce);
	void SetVerbose(bool a_bVerbose);
	bool ExportText();
	bool ImportText();
	static bool IsBflytFile(const UString& a_sFileName);
private:
	bool initExt();
	bool loadFakeCharset(bool a_bSwap);
	static const char s_nUnused[8];
	UString m_sFileName;
	UString m_sTxtName;
	UString m_sFakeCharsetName;
	bool m_bForce;
	bool m_bVerbose;
	EEndianness m_eEndianness;
	map<u32, U16String> m_mExt;
	map<U16String, u32> m_mExtStr;
	Char16_t m_uFakeCharset[0x10000];
};

#endif	// BFLYT_H_
