/*
 *  ARRAY_LONGINT.h
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#ifndef __ARRAY_LONGINT_H__
#define __ARRAY_LONGINT_H__ 1

#include "4DPluginAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef std::vector<int> CIntArray;	
	
class ARRAY_LONGINT
{

private:

	CIntArray* _CIntArray;
	
public:
 
	void fromParamAtIndex(PackagePtr pParams, uint32_t index);
	void toParamAtIndex(PackagePtr pParams, uint32_t index);	
	
	void appendIntValue(int intValue);	
	
	void setIntValueAtIndex(int intValue, uint32_t index);	
	int getIntValueAtIndex(uint32_t index);	
	
	uint32_t getSize();
	void setSize(uint32_t size);
	
	ARRAY_LONGINT();
	~ARRAY_LONGINT();

};

#ifdef __cplusplus
}
#endif

#endif