/*
 *  ARRAY_REAL.h
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#ifndef __ARRAY_REAL_H__
#define __ARRAY_REAL_H__ 1

#include "4DPluginAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef std::vector<double> CDoubleArray;	
	
class ARRAY_REAL
{

private:

	CDoubleArray* _CDoubleArray;
	
public:
 
	void fromParamAtIndex(PackagePtr pParams, uint16_t index);
	void toParamAtIndex(PackagePtr pParams, uint16_t index);	
	
	void appendDoubleValue(double doubleValue);	
	
	void setDoubleValueAtIndex(double doubleValue, uint32_t index);	
	double getDoubleValueAtIndex(uint32_t index);	
	
	uint32_t getSize();
	void setSize(uint32_t size);
	
	ARRAY_REAL();
	~ARRAY_REAL();

};

#ifdef __cplusplus
}
#endif

#endif