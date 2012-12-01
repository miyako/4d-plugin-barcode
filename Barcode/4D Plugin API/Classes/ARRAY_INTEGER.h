/*
 *  ARRAY_INTEGER.h
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#ifndef __ARRAY_INTEGER_H__
#define __ARRAY_INTEGER_H__ 1

#include "4DPluginAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef std::vector<short> CShortArray;	
	
class ARRAY_INTEGER
{

private:

	CShortArray* _CShortArray;
	
public:
 
	void fromParamAtIndex(PackagePtr pParams, uint16_t index);
	void toParamAtIndex(PackagePtr pParams, uint16_t index);	
	
	void appendIntValue(short shortValue);	
	
	void setShortValueAtIndex(short shortValue, uint32_t index);	
	int getShortValueAtIndex(uint32_t index);	
	
	uint32_t getSize();
	void setSize(uint32_t size);
	
	ARRAY_INTEGER();
	~ARRAY_INTEGER();

};

#ifdef __cplusplus
}
#endif

#endif