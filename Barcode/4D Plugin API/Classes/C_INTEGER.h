/*
 *  C_INTEGER.h
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#ifndef __C_INTEGER_H__
#define __C_INTEGER_H__ 1

#include "4DPluginAPI.h"

#ifdef __cplusplus
extern "C" {
#endif


class C_INTEGER
{

private:

	short _shortValue;
	
public:
 	
	void fromParamAtIndex(PackagePtr pParams, uint16_t index);
	void toParamAtIndex(PackagePtr pParams, uint16_t index);	
	void setReturn(sLONG_PTR *pResult);
	
	short getShortValue();	
	void setShortValue(short shortValue);		
	
	C_INTEGER();
	~C_INTEGER();

};

#ifdef __cplusplus
}
#endif

#endif