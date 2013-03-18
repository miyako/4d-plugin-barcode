/*
 *  ARRAY_TEXT.h
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#ifndef __ARRAY_TEXT_H__
#define __ARRAY_TEXT_H__ 1

#include "4DPluginAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef std::vector<CUTF16String> CUTF16StringArray;
	typedef std::vector<CUTF8String> CUTF8StringArray;		
	
class ARRAY_TEXT
{

private:

	CUTF16StringArray* _CUTF16StringArray;
	
	void convertFromUTF8(const CUTF8String* fromString, CUTF16String* toString);		
	void convertToUTF8(const CUTF16String* fromString, CUTF8String* toString);
        
public:
 
	void fromParamAtIndex(PackagePtr pParams, uint32_t index);
	void toParamAtIndex(PackagePtr pParams, uint32_t index);	
	
	void appendUTF16String(CUTF16String* pString);	
	void appendUTF16String(const PA_Unichar* pString, uint32_t len);
	void appendUTF16String(const PA_Unichar* pString);	
	void appendUTF16String(const PA_Unistring* pString);	

	void appendUTF8String(CUTF8String* pString);	
	void appendUTF8String(const uint8_t* pString, uint32_t len);	
	
	void setUTF16StringAtIndex(CUTF16String* pString, uint32_t index);
	void setUTF16StringAtIndex(const PA_Unichar* pString, uint32_t index);		
	void setUTF16StringAtIndex(const PA_Unichar* pString, uint32_t len, uint32_t index);	
	void setUTF16StringAtIndex(const PA_Unistring* pString, uint32_t index);		
	
#if VERSIONMAC
#ifdef __OBJC__	
	void appendUTF16String(NSString* pString);		
	void setUTF16StringAtIndex(NSString* pString, uint32_t index);
	NSString *copyUTF16StringAtIndex(uint32_t index);		
	NSString *copyPathAtIndex(uint32_t index);		
	NSURL *copyUrlAtIndex(uint32_t index);		
#endif
#endif
	
	void copyUTF16StringAtIndex(CUTF16String* pString, uint32_t index);	
	void copyUTF8StringAtIndex(CUTF8String* pString, uint32_t index);	
	void copyPathAtIndex(CUTF8String* pString, uint32_t index);	
	
	uint32_t getSize();
	void setSize(uint32_t size);
	
	ARRAY_TEXT();
	~ARRAY_TEXT();

};

#ifdef __cplusplus
}
#endif

#endif