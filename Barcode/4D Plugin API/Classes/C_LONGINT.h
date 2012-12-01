/*
 *  C_LONGINT.h
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#ifndef __C_LONGINT_H__
#define __C_LONGINT_H__ 1

#include "4DPluginAPI.h"

#ifdef __cplusplus
extern "C" {
#endif
	
	
	class C_LONGINT
	{
		
	private:
		
		int _intValue;
		
	public:
			
		void fromParamAtIndex(PackagePtr pParams, uint16_t index);
		void toParamAtIndex(PackagePtr pParams, uint16_t index);
		void setReturn(sLONG_PTR *pResult);	
		
		int getIntValue();	
		void setIntValue(int intValue);	
		
		C_LONGINT();
		~C_LONGINT();
		
	};
	
#ifdef __cplusplus
}
#endif

#endif