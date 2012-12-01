/*
 *  C_PICTURE.cpp
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#include "C_PICTURE.h"

unsigned int CPicture::_GetCount()
{
	uint32_t i;
	PA_ErrorCode err = eER_NoErr;
	PA_Unistring type;
	
	this->_Types.clear();
	
	i = 0;
	
	while (err == eER_NoErr) 
	{
		type = PA_GetPictureData(this->_CPicture, ++i, NULL);
		err = PA_GetLastError();
		
		if(err == eER_NoErr)
		{
			uint32_t len;
			
			len = (uint32_t)(type.fLength * 4) + sizeof(uint8_t);
			std::vector<uint8_t> buf(len);
			
			PA_ConvertCharsetToCharset(
									   (char *)type.fString, 
									   type.fLength * sizeof(PA_Unichar), 
									   eVTC_UTF_16, 
									   (char *)&buf[0],
									   len, 
									   eVTC_UTF_8
									   );			
			CUTF8String uti;
			uti = CUTF8String((const uint8_t *)&buf[0]);
			CUTF8String typestring;
			
			size_t pos, found; 
			found = 0;
			
			for(pos = uti.find(';'); pos != CUTF8String::npos; pos = uti.find(';', found))
			{
				typestring = uti.substr(found, pos-found);
				found = pos + 1;
				this->_Types.insert(std::map<CUTF8String, uint32_t>::value_type(typestring, i));				
			}
			
			typestring = uti.substr(found, uti.length()-found);
			this->_Types.insert(std::map<CUTF8String, uint32_t>::value_type(typestring, i));				
			
		}
	}
	
	return (i-1);
}

bool CPicture::loadImageOfType(CUTF8String *pType)
{
	bool result = false;
	
	if(pType->length())
	{	
		
		std::map<CUTF8String, uint32_t>::iterator itr;
		
		itr = this->_Types.find(pType->c_str());
		
		PA_ErrorCode err = eER_IncompatibleTypes;
		
		if (itr != this->_Types.end()) 
		{
			uint32_t pos = itr->second;
			
			if(this->_TypeNum == pos)
			{
				result = true;
			}else{
				
				this->_TypeNum = pos;	
				
				PA_Handle h = PA_NewHandle(0);	
				
				err = eER_NoErr;
				PA_GetPictureData(this->_CPicture, pos, h);
				err = PA_GetLastError();
				
				if(err == eER_NoErr)
				{
					this->_CBytes->setBytes((const uint8_t *)PA_LockHandle(h), PA_GetHandleSize(h));
					PA_UnlockHandle(h);
					
					this->_Type = CUTF8String(pType->c_str());
					result = true;
				}
				PA_DisposeHandle(h);
			}	
			
		}
		
	}
	
	return result;
	
}

void CPicture::fromFileAtPath(CUTF8String *pFilePath)
{	
	C_TEXT filePath;
	filePath.setUTF8String(pFilePath);
	
	PA_Unistring _path = PA_CreateUnistring((PA_Unichar *)filePath.getUTF16StringPtr());
	
	PA_Variable args[2];
	args[0] = PA_CreateVariable(eVK_Unistring);
	PA_SetStringVariable(&args[0], &_path);
	args[1] = PA_CreateVariable(eVK_Picture);
	
	PA_ExecuteCommandByID(678, args, 2);
	
	PA_Picture p = PA_GetPictureVariable(args[1]);
	
	PA_DisposeUnistring(&_path);
	
	PA_ClearVariable(&args[0]);
	
	setPicture(p);
	
//	PA_ClearVariable(&args[1]);	
//	belongs to 4D? should not release
	
}

void CPicture::setBytes(const uint8_t *bytes, uint32_t len)
{
	PA_Picture p = PA_CreatePicture((void *)bytes, len);
	
	setPicture(p);	
}

const uint8_t *CPicture::getBytesPtr(CUTF8String *pType)
{
	if(this->loadImageOfType(pType))
	{
		return this->_CBytes->getBytesPtr();
	}else{
		return 0;
	}
}

uint32_t CPicture::getBytesLength(CUTF8String *pType)
{
	if(this->loadImageOfType(pType))
	{
		return this->_CBytes->getBytesLength();
	}else{
		return 0;
	}	
}

void CPicture::setPicture(PA_Picture picture)
{
	if(picture)
	{
		if(this->_CPicture != picture)
		{
			PA_DisposePicture(this->_CPicture);
			this->_CPicture = PA_DuplicatePicture(picture, 1);//1=retain
			this->_Count = _GetCount();
			this->_TypeNum = 0;
			this->_Type = CUTF8String();
			this->_CBytes->setBytes(NULL, 0);
		}
	}
}

void CPicture::fromParamAtIndex(PackagePtr pParams, uint32_t index)
{
	if(index)
	{
		PA_Picture p = *(PA_Picture *)(pParams[index - 1]);	
		setPicture(p);
	}
}

void CPicture::toParamAtIndex(PackagePtr pParams, uint32_t index)
{
	if(index)
	{
		PA_Picture *p = (PA_Picture *)(pParams[index - 1]);
		
		if (*p)
		{
			if(*p != this->_CPicture)
			{
				PA_DisposePicture(*p);
				*p = PA_DuplicatePicture(this->_CPicture, 1);//1=retain			
			}
		}else{
			
			//happens in compiled mode
			*p = PA_DuplicatePicture(this->_CPicture, 1);//1=retain	
			
		}
	}	
}

void CPicture::setReturn(sLONG_PTR *pResult)
{
	PA_Picture *p = (PA_Picture *)pResult;
	*p = PA_DuplicatePicture(this->_CPicture, 1);//1=retain	
}

PA_Picture CPicture::getPicture()
{
	return this->_CPicture;	
}

uint32_t CPicture::getCount()
{
	return this->_Count;		
}

#define CMU_TRANSFORM_PICTURE 988

PA_Picture CPicture::createGrayScale()
{
	PA_Picture p;
	
	p = PA_DuplicatePicture(this->_CPicture, 1);
	
	PA_Variable args[2];
	
	args[0] = PA_CreateVariable(eVK_Picture);
	args[1] = PA_CreateVariable(eVK_Longint);
	
	PA_SetPictureVariable(&args[0], p);	
	PA_SetLongintVariable(&args[1], 101);
	
	PA_ExecuteCommandByID(CMU_TRANSFORM_PICTURE, args, 2);
	
	p = PA_GetPictureVariable(args[0]);

	PA_SetPictureVariable(&args[0], NULL);
	
	PA_ClearVariable(&args[0]);	
	PA_ClearVariable(&args[1]);
	
	return p;	
}

#define CMU_PICTURE_PROPERTIES 457

void CPicture::getSize(unsigned int *width, unsigned int *height)
{
	PA_Variable args[3];
	
	args[0] = PA_CreateVariable(eVK_Picture);
	args[1] = PA_CreateVariable(eVK_Longint);		
	args[2] = PA_CreateVariable(eVK_Longint);	
	
	PA_SetPictureVariable(&args[0], this->_CPicture);
	
	PA_ExecuteCommandByID(CMU_PICTURE_PROPERTIES, args, 3);
	
	PA_SetPictureVariable(&args[0], NULL);	
	
	*width = PA_GetLongintVariable(args[1]);
	*height = PA_GetLongintVariable(args[2]);	
	
	PA_ClearVariable(&args[0]);		
	PA_ClearVariable(&args[1]);
	PA_ClearVariable(&args[2]);	
}

#define CMU_CREATE_THUMBNAIL 679

PA_Picture CPicture::createThumbnail(unsigned int width, unsigned int height)
{
	PA_Picture p;
	PA_Variable args[4];
	
	args[0] = PA_CreateVariable(eVK_Picture);
	args[1] = PA_CreateVariable(eVK_Picture);	
	args[2] = PA_CreateVariable(eVK_Longint);		
	args[3] = PA_CreateVariable(eVK_Longint);	
	
	PA_SetPictureVariable(&args[0], this->_CPicture);
	PA_SetLongintVariable(&args[2], width);
	PA_SetLongintVariable(&args[3], height);	
	
	PA_ExecuteCommandByID(CMU_CREATE_THUMBNAIL, args, 4);
	
	p = PA_GetPictureVariable(args[1]);
	
	PA_SetPictureVariable(&args[0], NULL);	
	PA_SetPictureVariable(&args[1], NULL);		
	
	PA_ClearVariable(&args[0]);		
	PA_ClearVariable(&args[1]);
	PA_ClearVariable(&args[2]);
	PA_ClearVariable(&args[3]);	
	
	return p;
}

CPicture::CPicture() : _CBytes(new CBytes)
{
	this->_CPicture = PA_CreatePicture(NULL, 0);
	this->_Count = 0;
	this->_TypeNum = 0;
	this->_Type = CUTF8String();
	this->_CBytes->setBytes(NULL, 0);
}

CPicture::~CPicture()
{
	PA_DisposePicture(this->_CPicture);
	delete _CBytes; 
}

void C_PICTURE::fromParamAtIndex(PackagePtr pParams, uint32_t index)
{
	this->_CPicture->fromParamAtIndex(pParams, index);
}

void C_PICTURE::toParamAtIndex(PackagePtr pParams, uint32_t index)
{
	this->_CPicture->toParamAtIndex(pParams, index);
}

void C_PICTURE::setReturn(sLONG_PTR *pResult)
{
	this->_CPicture->setReturn(pResult);
}

void C_PICTURE::setBytes(const uint8_t *bytes, uint32_t len)
{
	this->_CPicture->setBytes(bytes, len);	
}

PA_Picture C_PICTURE::getPicture()
{
	return this->_CPicture->getPicture();	
}

void C_PICTURE::setPicture(PA_Picture picture)
{
	return this->_CPicture->setPicture(picture);	
}

void C_PICTURE::fromFileAtPath(CUTF8String *pFilePath)
{
	this->_CPicture->fromFileAtPath(pFilePath);		
}

const uint8_t *C_PICTURE::getBytesPtr(CUTF8String *pType)
{	
	return this->_CPicture->getBytesPtr(pType);		
}

uint32_t C_PICTURE::getBytesLength(CUTF8String *pType)
{
	return this->_CPicture->getBytesLength(pType);	
}

PA_Picture C_PICTURE::createGrayScale()
{
	return this->_CPicture->createGrayScale();	
}

void C_PICTURE::getSize(unsigned int *width, unsigned int *height)
{
	this->_CPicture->getSize(width, height);	
}

PA_Picture C_PICTURE::createThumbnail(unsigned int width, unsigned int height)
{
	return this->_CPicture->createThumbnail(width, height);
}

C_PICTURE::C_PICTURE() : _CPicture(new CPicture)
{
	
}

C_PICTURE::~C_PICTURE()
{ 
	delete _CPicture; 
}