/*
 *  C_BLOB.h
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#ifndef __C_BLOB_H__
#define __C_BLOB_H__ 1

#include "4DPluginAPI.h"

class C_TEXT;

#ifdef __cplusplus
extern "C" {
#endif
	
	class CBytes
	{
		
	private:
		
		std::vector<uint8_t> _CBytes;		
		uint32_t _cursorPosition;
		
	public:
		
		void fromParamAtIndex(PackagePtr pParams, uint32_t index);
		void toParamAtIndex(PackagePtr pParams, uint32_t index);	
		void setReturn(sLONG_PTR *pResult);	
		
		void setBytes(const uint8_t *bytes, uint32_t len);
		void addBytes(const uint8_t *bytes, uint32_t len);
		
		const uint8_t *getBytesPtr();
		uint32_t getBytesLength();	

		const uint8_t *getBytesPtrForSize(uint32_t *size);		
		
		void fromHexText(C_TEXT *hex);
		void fromB64Text(C_TEXT *b64);
		
		void toHexText(C_TEXT *hex);
		void toB64Text(C_TEXT *b64);		
		
		CBytes();	
		~CBytes();
		
	};
	
	class C_BLOB
	{
		
	private:
		
		CBytes* _CBytes;
		
	public:
		
		void fromParamAtIndex(PackagePtr pParams, uint32_t index);
		void toParamAtIndex(PackagePtr pParams, uint32_t index);	
		void setReturn(sLONG_PTR *pResult);		
		
		void setBytes(const uint8_t *bytes, uint32_t len);
		void addBytes(const uint8_t *bytes, uint32_t len);
		
		const uint8_t *getBytesPtr();
		uint32_t getBytesLength();		
		
		const uint8_t *getBytesPtrForSize(uint32_t *size);
	
		void fromHexText(C_TEXT *hex);
		void fromB64Text(C_TEXT *b64);

		void toHexText(C_TEXT *hex);
		void toB64Text(C_TEXT *b64);
		
		C_BLOB();
		~C_BLOB();
		
	};
	
#ifdef __cplusplus
}
#endif

#endif