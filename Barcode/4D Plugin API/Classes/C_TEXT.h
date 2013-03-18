/*
 *  C_TEXT.h
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#ifndef __C_TEXT_H__
#define __C_TEXT_H__ 1

#include "4DPluginAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef std::basic_string<PA_Unichar> CUTF16String;
	typedef std::basic_string<uint8_t> CUTF8String;	
	
	class C_TEXT
	{
		
	private:
		
		CUTF16String* _CUTF16String;
		
		void convertFromUTF8(const CUTF8String* fromString, CUTF16String* toString);		
		void convertToUTF8(const CUTF16String* fromString, CUTF8String* toString);
		
	public:
		
		void fromParamAtIndex(PackagePtr pParams, uint16_t index);
		void toParamAtIndex(PackagePtr pParams, uint16_t index);	
		void setReturn(sLONG_PTR* pResult);		
		
		void setUTF16String(CUTF16String* pString);
		void setUTF16String(const PA_Unichar* pString, uint32_t len);
		void setUTF16String(const PA_Unistring* pString);

#if VERSIONMAC
#ifdef __OBJC__	
		void setUTF16String(NSString* pString);
		NSMutableString* copyUTF16MutableString();
		NSString* copyUTF16String();
		NSString* copyPath();
		void convertPath();
		void setPath(NSString* path);		
		NSURL *copyUrl();
#endif
#endif
		
		void setUTF8String(CUTF8String* pString);
		void setUTF8String(const uint8_t* pString, uint32_t len);

		const PA_Unichar *getUTF16StringPtr();			
		uint32_t getUTF16Length();
		
		void copyUTF16String(CUTF16String* pString);			
		void copyUTF8String(CUTF8String* pString);
	
		void copyPath(CUTF8String* pString);
		
		C_TEXT();
		~C_TEXT();
		
	};
	
	void CUTF8StringReplaceString(CUTF8String *src, const uint8_t *f, const uint8_t *t);	
	
#ifdef __cplusplus
}
#endif

#endif