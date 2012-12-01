/*
 *  C_BLOB.cpp
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#include "C_BLOB.h"

void CBytes::fromParamAtIndex(PackagePtr pParams, uint32_t index)
{
	if(index)
	{		
		PA_Handle h = *(PA_Handle *)(pParams[index - 1]);
		unsigned int size = PA_GetHandleSize(h);
		
		this->_CBytes.resize(size);
		
		PA_MoveBlock(PA_LockHandle(h), (char *)&this->_CBytes[0], size);		
		PA_UnlockHandle(h);
	}
}

void CBytes::toParamAtIndex(PackagePtr pParams, uint32_t index)
{
	if(index)
	{
		PA_Handle *h = (PA_Handle *)(pParams[index - 1]);
		
		if (*h) PA_DisposeHandle(*h);
				
		PA_Handle d = PA_NewHandle((unsigned int)this->_CBytes.size());
		PA_MoveBlock((char *)&this->_CBytes[0], PA_LockHandle(d), (unsigned int)this->_CBytes.size());
		PA_UnlockHandle(d);
		
		*h = d;
	}
}

void CBytes::setBytes(const uint8_t *bytes, unsigned int len)
{	
	if(bytes)
	{
		this->_CBytes.resize(len);
		PA_MoveBlock((void *)bytes, (char *)&this->_CBytes[0], len);	
	}
}

void CBytes::addBytes(const uint8_t *bytes, unsigned int len)
{	
	if(bytes)
	{
		unsigned int originalSize = this->_CBytes.size();
		this->_CBytes.resize(originalSize + len);
		PA_MoveBlock((void *)bytes, (char *)&this->_CBytes[originalSize], len);	
	}
}


void CBytes::setReturn(sLONG_PTR *pResult)
{
	PA_Handle *h = (PA_Handle *)pResult;
	
	PA_Handle d = PA_NewHandle((unsigned int)this->_CBytes.size());
	PA_MoveBlock((char *)&this->_CBytes[0], PA_LockHandle(d), (unsigned int)this->_CBytes.size());
	PA_UnlockHandle(d);
	
	*h = d;
}

const uint8_t *CBytes::getBytesPtr()
{
	return (const uint8_t *)&this->_CBytes[0];
}

uint32_t CBytes::getBytesLength()
{
	return (unsigned int)this->_CBytes.size();
}

const uint8_t *CBytes::getBytesPtrForSize(uint32_t *size)
{

	int len = this->_CBytes.size() - this->_cursorPosition;
	const uint8_t *ptr = NULL;
	
	if(len > 0){
	
		ptr = (const uint8_t *)&this->_CBytes[(this->_cursorPosition)];
		
		if((*size) > (unsigned int)len) {*size = len;}
		
		this->_cursorPosition = this->_cursorPosition + (*size);
		
	}else{
		*size = 0;
	}
	
	return ptr;
	
}

void CBytes::toHexText(C_TEXT *hex)
{
	
	CUTF8String u;
	
	std::vector<uint8_t> buf(3);
	const std::vector<uint8_t>::const_iterator binend = this->_CBytes.end();
	
	for (std::vector<uint8_t>::const_iterator i = this->_CBytes.begin(); i != binend; ++i) {
#if VERSIONMAC
		sprintf((char *)&buf[0], "%02x", *i);
#else
		sprintf_s((char *)&buf[0], 3, "%02x", *i);
#endif	
		u.append(&buf[0], 2);		
		
	}
	
	hex->setUTF8String(&u);
	
}

void CBytes::fromHexText(C_TEXT *hex)
{
	CUTF8String t;
	hex->copyUTF8String(&t);	
	
	CUTF8String v = CUTF8String((const uint8_t *)"0123456789ABCDEFabcdef -:");
	
	size_t pos; 
	BOOL data_in_buffer = false;
	int buf = 0;
	
	this->_CBytes.resize(0);
	
	for(pos = 0; pos < t.length(); pos++){
		
		size_t f = v.find(t[pos]);
		
		if(f == std::string::npos){
			this->_CBytes.resize(0);			
			break;
		} 
		
		if((f >= 0) && (f <= 15)){
			
			if(data_in_buffer){
				this->_CBytes.push_back(static_cast<uint8_t>((buf << 4) + f));
				data_in_buffer = false;
			}else{
				buf = f;
				data_in_buffer = true;
			}
			
		}else{
			
			if(f <= 21){
				
				if(data_in_buffer){
					this->_CBytes.push_back(static_cast<uint8_t>((buf << 4) + (f - 6)));
					data_in_buffer = false;
				}else{
					buf = f - 6;
					data_in_buffer = true;
				}
				
			}
			
		}
		
	}
	
}

static const char reverse_table[128] = {
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
	64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
	64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64
};

void CBytes::fromB64Text(C_TEXT *b64)
{
	CUTF8String t;
	b64->copyUTF8String(&t);
	
	const CUTF8String::const_iterator last = t.end();
	
	std::vector<uint8_t> buf(0);	
	
	int bits_collected = 0;
	unsigned int accumulator = 0;
	
	for (CUTF8String::const_iterator i = t.begin(); i != last; ++i) {
		const int c = *i;
		if (isspace(c) || c == '=') {
			// Skip whitespace and padding. Be liberal in what you accept.
			continue;
		}
		if ((c > 127) || (c < 0) || (reverse_table[c] > 63)) {
			// This contains characters not legal in a base64 encoded string.
			this->_CBytes.resize(0);
			break;
		}
		accumulator = (accumulator << 6) | reverse_table[c];
		bits_collected += 6;
		if (bits_collected >= 8) {
			bits_collected -= 8;
			this->_CBytes.push_back((uint8_t)((accumulator >> bits_collected) & 0xffu));
		}
		
	}
	
}

static const char b64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void CBytes::toB64Text(C_TEXT *b64)
{
	
	const ::std::size_t binlen = this->_CBytes.size();
	
	// Use = signs so the end is properly padded.
	CUTF8String retval((((binlen + 2) / 3) * 4), '=');
	
	::std::size_t outpos = 0;
	int bits_collected = 0;
	unsigned int accumulator = 0;
	
	const std::vector<uint8_t>::const_iterator binend = this->_CBytes.end();
	
	for (std::vector<uint8_t>::const_iterator i = this->_CBytes.begin(); i != binend; ++i) {
		accumulator = (accumulator << 8) | (*i & 0xffu);
		bits_collected += 8;
		while (bits_collected >= 6) {
			bits_collected -= 6;
			retval[outpos++] = b64_table[(accumulator >> bits_collected) & 0x3fu];
		}
	}
	
	if (bits_collected > 0) { // Any trailing bits that are missing.
		//	assert(bits_collected < 6);
		accumulator <<= 6 - bits_collected;
		retval[outpos++] = b64_table[accumulator & 0x3fu];
	}
	
	//	assert(outpos >= (retval.size() - 2));
	//	assert(outpos <= retval.size());
	
	b64->setUTF8String(&retval);
	
}

CBytes::CBytes() : _cursorPosition(0)
{	
}

CBytes::~CBytes()
{
}

void C_BLOB::fromParamAtIndex(PackagePtr pParams, uint32_t index)
{
	this->_CBytes->fromParamAtIndex(pParams, index);
}

void C_BLOB::toParamAtIndex(PackagePtr pParams, uint32_t index)
{
	this->_CBytes->toParamAtIndex(pParams, index);	
}

void C_BLOB::setReturn(sLONG_PTR *pResult)
{
	this->_CBytes->setReturn(pResult);
}

void C_BLOB::setBytes(const uint8_t *bytes, unsigned int len)
{
	this->_CBytes->setBytes(bytes, len);
}

void C_BLOB::addBytes(const uint8_t *bytes, unsigned int len)
{
	this->_CBytes->addBytes(bytes, len);
}

const uint8_t *C_BLOB::getBytesPtr()
{
	return this->_CBytes->getBytesPtr();
}

uint32_t C_BLOB::getBytesLength()
{
	return this->_CBytes->getBytesLength();	
}

const uint8_t *C_BLOB::getBytesPtrForSize(uint32_t *size)
{
	return this->_CBytes->getBytesPtrForSize(size);		
}

void C_BLOB::fromHexText(C_TEXT *hex)
{
	this->_CBytes->fromHexText(hex);	
}

void C_BLOB::fromB64Text(C_TEXT *b64)
{
	this->_CBytes->fromB64Text(b64);	
}

void C_BLOB::toHexText(C_TEXT *hex)
{
	this->_CBytes->toHexText(hex);	
}

void C_BLOB::toB64Text(C_TEXT *b64)
{
	this->_CBytes->toB64Text(b64);	
}


C_BLOB::C_BLOB() : _CBytes(new CBytes)
{
	
}

C_BLOB::~C_BLOB()
{ 
	delete _CBytes; 
}