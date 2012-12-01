/*
 *  C_TIME.h
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#ifndef __C_TIME_H__
#define __C_TIME_H__ 1

#include "4DPluginAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

	class C_TIME
	{
		
	private:
		
		uint32_t _seconds;

	public:
		
		void fromParamAtIndex(PackagePtr pParams, uint16_t index);
		void toParamAtIndex(PackagePtr pParams, uint16_t index);
		void setReturn(sLONG_PTR *pResult);	
		
		uint32_t getSeconds();		
		void setSeconds(uint32_t seconds);	

		C_TIME();
		~C_TIME();
		
	};	
	
#ifdef __cplusplus
}
#endif

#endif