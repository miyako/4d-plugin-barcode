/*
 *  ARRAY_BOOLEAN.h
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#ifndef __ARRAY_BOOLEAN_H__
#define __ARRAY_BOOLEAN_H__ 1

#include "4DPluginAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef std::vector<bool> CboolArray;	
	
class ARRAY_BOOLEAN
{

private:

	CboolArray* _CboolArray;
	
public:
 
	void fromParamAtIndex(PackagePtr pParams, uint16_t index);
	void toParamAtIndex(PackagePtr pParams, uint16_t index);	
	
	void appendBooleanValue(bool booleanValue);	
	
	void setBooleanValueAtIndex(bool booleanValue, uint32_t index);	
	bool getBooleanValueAtIndex(uint32_t index);	
	
	uint32_t getSize();
	void setSize(uint32_t size);
	
	ARRAY_BOOLEAN();
	~ARRAY_BOOLEAN();

};

#ifdef __cplusplus
}
#endif

#endif