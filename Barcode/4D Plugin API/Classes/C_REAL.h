/*
 *  C_REAL.h
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#ifndef __C_REAL_H__
#define __C_REAL_H__ 1

#include "4DPluginAPI.h"

#ifdef __cplusplus
extern "C" {
#endif
	
	
	class C_REAL
	{
		
	private:
		
		double _doubleValue;
		
	public:
		
		void fromParamAtIndex(PackagePtr pParams, uint16_t index);
		void toParamAtIndex(PackagePtr pParams, uint16_t index);
		void setReturn(sLONG_PTR *pResult);	
		
		double getDoubleValue();	
		void setDoubleValue(double doubleValue);	
		
		C_REAL();
		~C_REAL();
		
	};
	
#ifdef __cplusplus
}
#endif

#endif