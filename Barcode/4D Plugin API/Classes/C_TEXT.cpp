/*
 *  C_TEXT.cpp
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#include "C_TEXT.h"

void C_TEXT::fromParamAtIndex(PackagePtr pParams, uint16_t index)
{
	if(index)
	{
		PA_Unistring *str = (PA_Unistring *)(pParams[index - 1]);
		*this->_CUTF16String = CUTF16String(str->fString, str->fLength);
	}
}

void C_TEXT::toParamAtIndex(PackagePtr pParams, uint16_t index)
{	
	if(index)
	{
		PA_SetUnistring(
						(PA_Unistring *)(pParams[index - 1]),
						(PA_Unichar *)this->_CUTF16String->c_str()
						);
	}	
}

void C_TEXT::setReturn(sLONG_PTR* pResult)
{	
	PA_SetUnistring(
					*(PA_Unistring **) pResult,
					(PA_Unichar *)this->_CUTF16String->c_str()
					);		
}

void C_TEXT::setUTF16String(CUTF16String* pString)
{
	*this->_CUTF16String = CUTF16String(pString->c_str(), pString->length());		
}

void C_TEXT::setUTF16String(const PA_Unichar* pString, uint32_t len)
{
	*this->_CUTF16String = CUTF16String(pString, (size_t)len);	
}

void C_TEXT::setUTF16String(const PA_Unistring* pString)
{
	*this->_CUTF16String = CUTF16String(pString->fString, pString->fLength);	
}

#if VERSIONMAC
#ifdef __OBJC__	
void C_TEXT::setUTF16String(NSString* pString)
{
	uint32_t len = [pString length];
	uint32_t size = (len * sizeof(PA_Unichar)) + sizeof(PA_Unichar);
	std::vector<uint8_t> buf(size);	
	
	if([pString getCString:(char *)&buf[0] maxLength:size encoding:NSUnicodeStringEncoding])
		this->setUTF16String((const PA_Unichar *)&buf[0], len);	
}

NSMutableString* C_TEXT::copyUTF16MutableString()
{
	return [[NSMutableString alloc]initWithCharacters:this->_CUTF16String->c_str() length:this->_CUTF16String->length()];
}

NSString* C_TEXT::copyUTF16String()
{
	return [[NSString alloc]initWithCharacters:this->_CUTF16String->c_str() length:this->_CUTF16String->length()];
}

NSString* C_TEXT::copyPath()
{
	NSString *path = @"";	
	
	NSURL *u = this->copyUrl();
	
	if(u){
		path = (NSString *)CFURLCopyFileSystemPath((CFURLRef)u, kCFURLPOSIXPathStyle);
		[u release];
	}
	
	return path;

}		

void C_TEXT::convertPath()
{
	NSString *posixPath = this->copyPath();
	this->setUTF16String(posixPath);
	[posixPath release];
}

NSURL *C_TEXT::copyUrl()
{
	NSURL *u = NULL;
	
	NSString *str = this->copyUTF16String();
	u = (NSURL *)CFURLCreateWithFileSystemPath(kCFAllocatorDefault, (CFStringRef)str, kCFURLHFSPathStyle, false);
	[str release];
	
	return u;
}

void C_TEXT::setPath(NSString* path)
{
	if(path){
		NSURL *u = (NSURL *)CFURLCreateWithFileSystemPath(kCFAllocatorDefault, (CFStringRef)path, kCFURLPOSIXPathStyle, false);
		
		if(u){
			NSString *str = (NSString *)CFURLCopyFileSystemPath((CFURLRef)u, kCFURLHFSPathStyle);
			this->setUTF16String(str);
			[str release];
			[u release];
		}

	}
}
#endif
#endif

void C_TEXT::convertFromUTF8(const CUTF8String* fromString, CUTF16String* toString)	
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

void C_TEXT::convertToUTF8(const CUTF16String* fromString, CUTF8String* toString)
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

void C_TEXT::setUTF8String(CUTF8String* pString)
{
	convertFromUTF8(pString, this->_CUTF16String);
}

void C_TEXT::setUTF8String(const uint8_t *pString, uint32_t len)
{
	CUTF8String u = CUTF8String(pString, len);
	convertFromUTF8(&u, this->_CUTF16String);	
}

const PA_Unichar *C_TEXT::getUTF16StringPtr()
{
	return this->_CUTF16String->c_str();
}

uint32_t C_TEXT::getUTF16Length()
{
	return (uint32_t)this->_CUTF16String->length();	
}

void C_TEXT::copyUTF16String(CUTF16String* pString)
{
	*pString = CUTF16String(this->_CUTF16String->c_str(), this->_CUTF16String->length());
}

void C_TEXT::copyUTF8String(CUTF8String* pString)
{
	convertToUTF8(this->_CUTF16String, pString);	
}

void C_TEXT::copyPath(CUTF8String* pString)
{
#if VERSIONMAC	
	NSString *path = this->copyPath();
	*pString = CUTF8String((const uint8_t *)[path UTF8String]);
	[path release];
#else
	this->copyUTF8String(pString);
#endif
}

C_TEXT::C_TEXT() : _CUTF16String(new CUTF16String)
{
	
}

C_TEXT::~C_TEXT()
{ 
	delete _CUTF16String; 
}

void CUTF8StringReplaceString(CUTF8String *src, const uint8_t *f, const uint8_t *t)
{
	CUTF8String from = CUTF8String(f);
	CUTF8String to = CUTF8String(t);
	
	CUTF8String::size_type pos = 0;
	
    while(pos = src->find(from, pos), pos != CUTF8String::npos) 
	{
        src->replace(pos, from.length(), to);
        pos += to.length();
    }
}