/*
 *  ARRAY_TEXT.cpp
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#include "ARRAY_TEXT.h"

void ARRAY_TEXT::fromParamAtIndex(PackagePtr pParams, uint32_t index)
{	
	if(index)		
	{
		PA_Variable arr = *((PA_Variable*) pParams[index - 1]);
		
		this->setSize(0);		
		
		if(arr.fType == eVK_ArrayUnicode)
		{	
			uint32_t i;
			
			PA_Unistring str;
			
			for(i = 0; i <= (uint32_t)arr.uValue.fArray.fNbElements; i++)
			{				
				str = (*(PA_Unistring **) (arr.uValue.fArray.fData))[i];
				CUTF16String u = CUTF16String(str.fString, str.fLength);
				this->_CUTF16StringArray->push_back(u);
			}
			
		}

	}
	
}

void ARRAY_TEXT::toParamAtIndex(PackagePtr pParams, uint32_t index)
{
	if(index)		
	{
		PA_Variable arr = *((PA_Variable*) pParams[index - 1]);
		PA_Variable *param = ((PA_Variable *)pParams[index - 1]);
		
		switch (arr.fType) 
		{
			case eVK_ArrayUnicode:
				break;
			case eVK_Undefined:
				PA_ClearVariable(&arr);				
				arr = PA_CreateVariable(eVK_ArrayUnicode);
				param->fType = arr.fType;
				break;				
			
			default:
				break;
		}
		
		if(arr.fType == eVK_ArrayUnicode)
		{
			
			PA_ResizeArray(&arr, this->_CUTF16StringArray->empty() ? 0 : (uint32_t)(this->_CUTF16StringArray->size() - 1));
			
			uint32_t i;
			
			for(i = 0; i < this->_CUTF16StringArray->size(); i++)
			{
				PA_Unistring str = PA_CreateUnistring((PA_Unichar *)this->_CUTF16StringArray->at(i).c_str());
				PA_SetStringInArray(arr, i, &str);					
			}
			
			param->fFiller = 0;
			param->uValue.fArray.fCurrent = arr.uValue.fArray.fCurrent;
			param->uValue.fArray.fNbElements = arr.uValue.fArray.fNbElements;
			param->uValue.fArray.fData = arr.uValue.fArray.fData;			
			
		}
		
	}
	
}

void ARRAY_TEXT::convertFromUTF8(const CUTF8String* fromString, CUTF16String* toString)	
{
#ifdef _WIN32
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)fromString->c_str(), fromString->length(), NULL, 0);
	
	if(len){
		std::vector<uint8_t> buf((len + 1) * sizeof(PA_Unichar));
		if(MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)fromString->c_str(), fromString->length(), (LPWSTR)&buf[0], len)){
			*toString = CUTF16String((const PA_Unichar *)&buf[0]);
		}
    }else{
			*toString = CUTF16String((const PA_Unichar *)L"");
	}
           
#else
           CFStringRef str = CFStringCreateWithBytes(kCFAllocatorDefault, fromString->c_str(), fromString->length(), kCFStringEncodingUTF8, true);
           if(str){
               int len = CFStringGetLength(str);
               std::vector<uint8_t> buf((len+1) * sizeof(PA_Unichar));
               CFStringGetCharacters(str, CFRangeMake(0, len), (UniChar *)&buf[0]);
               *toString = CUTF16String((const PA_Unichar *)&buf[0]);
               CFRelease(str);
           }
#endif	
}

void ARRAY_TEXT::convertToUTF8(const CUTF16String* fromString, CUTF8String* toString)
{
#ifdef _WIN32
	int len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)fromString->c_str(), fromString->length(), NULL, 0, NULL, NULL);
	
	if(len){
		std::vector<uint8_t> buf(len + 1);
		if(WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)fromString->c_str(), fromString->length(), (LPSTR)&buf[0], len, NULL, NULL)){
			*toString = CUTF8String((const uint8_t *)&buf[0]);
		}
	}else{
			*toString = CUTF8String((const uint8_t *)"");
	}
           
#else
           CFStringRef str = CFStringCreateWithCharacters(kCFAllocatorDefault, (const UniChar *)fromString->c_str(), fromString->length());
           if(str){
               
               size_t size = CFStringGetMaximumSizeForEncoding(CFStringGetLength(str), kCFStringEncodingUTF8) + sizeof(uint8_t);
               std::vector<uint8_t> buf(size);
               CFIndex len = 0;
               CFStringGetBytes(str, CFRangeMake(0, CFStringGetLength(str)), kCFStringEncodingUTF8, 0, true, (UInt8 *)&buf[0], size, &len);
               
               *toString = CUTF8String((const uint8_t *)&buf[0], len);	
               CFRelease(str);
           }	
           
#endif
}

void ARRAY_TEXT::copyUTF8StringAtIndex(CUTF8String* pString, uint32_t index)
{	
	if(index < this->_CUTF16StringArray->size())
	{
		CUTF16String s = CUTF16String(this->_CUTF16StringArray->at(index));
		convertToUTF8(&s, pString);		
	}
}

void ARRAY_TEXT::copyUTF16StringAtIndex(CUTF16String* pString, uint32_t index)
{	
	if(index < this->_CUTF16StringArray->size())
	{
		*pString = CUTF16String(this->_CUTF16StringArray->at(index));
	}
}

void ARRAY_TEXT::setUTF16StringAtIndex(const PA_Unichar* pString, uint32_t len, uint32_t index)
{
	CUTF16String u;
	u = CUTF16String(pString, (size_t)len);	
	this->setUTF16StringAtIndex(&u, index);
}




void ARRAY_TEXT::copyPathAtIndex(CUTF8String* pString, uint32_t index)
{
#if VERSIONMAC
#ifdef __OBJC__	
	NSString *path = this->copyPathAtIndex(index);
	*pString = CUTF8String((const uint8_t *)[path UTF8String]);
	[path release];
#endif
#else
	this->copyUTF8StringAtIndex(pString, index);
#endif		
}
	
void ARRAY_TEXT::setUTF16StringAtIndex(CUTF16String* pString, uint32_t index)
{
	if(index < this->_CUTF16StringArray->size())
	{		
		CUTF16String str;
		
		if(pString->c_str())
		{
			str = CUTF16String(pString->c_str(), pString->length());
		}else{
			str = CUTF16String();		
		}
		
		std::vector<CUTF16String>::iterator it = this->_CUTF16StringArray->begin();
		it += index;
		this->_CUTF16StringArray->insert(this->_CUTF16StringArray->erase(it), str);	
	}
}

void ARRAY_TEXT::appendUTF8String(CUTF8String* pString)
{
	CUTF16String u16;	
	convertFromUTF8(pString, &u16);
	this->_CUTF16StringArray->push_back(u16);		
}

void ARRAY_TEXT::appendUTF8String(const uint8_t* pString, uint32_t len){

	CUTF8String u8 = CUTF8String(pString, len);
	CUTF16String u16;
	convertFromUTF8(&u8, &u16);
	this->_CUTF16StringArray->push_back(u16);		
}

void ARRAY_TEXT::appendUTF16String(const PA_Unichar* pString)
{
	CUTF16String u = CUTF16String(pString);
	this->_CUTF16StringArray->push_back(u);		
}

void ARRAY_TEXT::appendUTF16String(const PA_Unichar* pString, uint32_t len)
{
	CUTF16String u = CUTF16String(pString, len);
	this->_CUTF16StringArray->push_back(u);		
}

void ARRAY_TEXT::setUTF16StringAtIndex(const PA_Unichar* pString, uint32_t index)
{
	CUTF16String u = CUTF16String(pString);
	this->_CUTF16StringArray->push_back(u);		
}

void ARRAY_TEXT::appendUTF16String(CUTF16String* pString)
{
	CUTF16String u = CUTF16String(pString->c_str(), pString->length());	
	this->_CUTF16StringArray->push_back(u);		
}

void ARRAY_TEXT::appendUTF16String(const PA_Unistring* pString)
{
	CUTF16String u = CUTF16String(pString->fString, pString->fLength);
	this->_CUTF16StringArray->push_back(u);	
}

#if VERSIONMAC
#ifdef __OBJC__	
void ARRAY_TEXT::appendUTF16String(NSString* pString)
{	
	uint32_t len = [pString length];
	uint32_t size = (len * sizeof(PA_Unichar)) + sizeof(PA_Unichar);
	std::vector<uint8_t> buf(size);	
	
	if([pString getCString:(char *)&buf[0] maxLength:size encoding:NSUnicodeStringEncoding])
	{
		CUTF16String u;
		u = CUTF16String((const PA_Unichar *)&buf[0], len);
		this->_CUTF16StringArray->push_back(u);		
	}

}

void ARRAY_TEXT::setUTF16StringAtIndex(NSString* pString, uint32_t index)
{
	uint32_t len = [pString length];
	uint32_t size = (len * sizeof(PA_Unichar)) + sizeof(PA_Unichar);
	std::vector<uint8_t> buf(size);	
	
	if([pString getCString:(char *)&buf[0] maxLength:size encoding:NSUnicodeStringEncoding])
		this->setUTF16StringAtIndex((const PA_Unichar *)&buf[0], len, index);	
}

NSString *ARRAY_TEXT::copyUTF16StringAtIndex(uint32_t index)
{
	CUTF16String strUtf16;
	
	strUtf16 = CUTF16String(this->_CUTF16StringArray->at(index));
	
	return [[NSString alloc]initWithCharacters:strUtf16.c_str() length:strUtf16.length()];
}

NSString *ARRAY_TEXT::copyPathAtIndex(uint32_t index){
	
	NSString *path = @"";
	
	NSURL *u = this->copyUrlAtIndex(index);
	
	if(u){
		path = (NSString *)CFURLCopyFileSystemPath((CFURLRef)u, kCFURLPOSIXPathStyle);
		[u release];
	}
	
	return path;
	
}		
NSURL *ARRAY_TEXT::copyUrlAtIndex(uint32_t index){
	
	NSURL *u = NULL;
	
	NSString *str = this->copyUTF16StringAtIndex(index);
	u = (NSURL *)CFURLCreateWithFileSystemPath(kCFAllocatorDefault, (CFStringRef)str, kCFURLHFSPathStyle, false);
	[str release];
	
	return u;
	
}
#endif
#endif

uint32_t ARRAY_TEXT::getSize()
{
	return (uint32_t)this->_CUTF16StringArray->size();		
}

void ARRAY_TEXT::setSize(uint32_t size)
{	
	this->_CUTF16StringArray->resize(size);
}

ARRAY_TEXT::ARRAY_TEXT() : _CUTF16StringArray(new CUTF16StringArray)
{
	
}

ARRAY_TEXT::~ARRAY_TEXT()
{ 
	delete _CUTF16StringArray; 
}