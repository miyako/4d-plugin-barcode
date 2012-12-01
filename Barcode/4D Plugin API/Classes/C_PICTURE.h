/*
 *  C_PICTURE.h
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#ifndef __C_PICTURE_H__
#define __C_PICTURE_H__ 1

#include "4DPluginAPI.h"

class CBytes;

#ifdef __cplusplus
extern "C" {
#endif
	
	class CPicture
	{
		
	private:
		
		std::map<CUTF8String, uint32_t> _Types;
		
		PA_Picture _CPicture;
		uint32_t _GetCount();
		
		uint32_t _Count;
		CUTF8String _Type;
		uint32_t	_TypeNum;
		CBytes* _CBytes;
		
		bool loadImageOfType(CUTF8String *pType);
		uint32_t getCount();
		
	public:
		
		void fromParamAtIndex(PackagePtr pParams, uint32_t index);
		void toParamAtIndex(PackagePtr pParams, uint32_t index);	
		void setReturn(sLONG_PTR *pResult);
		
		void setBytes(const uint8_t *bytes, uint32_t len);
		
		const uint8_t *getBytesPtr(CUTF8String *pType);
		uint32_t getBytesLength(CUTF8String *pType);
		
		void fromFileAtPath(CUTF8String *pFilePath);	
		
		PA_Picture getPicture();
		void setPicture(PA_Picture picture);
		
		PA_Picture createGrayScale();	
		
		
		
		
		void getSize(unsigned int *width, unsigned int *height);	
		PA_Picture createThumbnail(unsigned int width, unsigned int height);
		
		CPicture();	
		~CPicture();
		
	};
	
	class C_PICTURE
	{
		
	private:
		
		CPicture* _CPicture;

	public:
		
		void fromParamAtIndex(PackagePtr pParams, uint32_t index);
		void toParamAtIndex(PackagePtr pParams, uint32_t index);	
		void setReturn(sLONG_PTR *pResult);
		
		void setBytes(const uint8_t *bytes, uint32_t len);
		
		const uint8_t *getBytesPtr(CUTF8String *pType);
		uint32_t getBytesLength(CUTF8String *pType);
		
		void fromFileAtPath(CUTF8String *pFilePath);
		
		PA_Picture getPicture();
		void setPicture(PA_Picture picture);

		PA_Picture createGrayScale();
		
		
		
		
		void getSize(unsigned int *width, unsigned int *height);	
		PA_Picture createThumbnail(unsigned int width, unsigned int height);
		
		C_PICTURE();
		~C_PICTURE();
		
	};
	
#ifdef __cplusplus
}
#endif

#endif