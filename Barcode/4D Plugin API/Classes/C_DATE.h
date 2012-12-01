/*
 *  C_DATE.h
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#ifndef __C_DATE_H__
#define __C_DATE_H__ 1

#include "4DPluginAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

	class C_DATE
	{
		
	private:
		
		uint16_t _year;
		uint16_t _month;
		uint16_t _day;
		
	public:
		
		void fromParamAtIndex(PackagePtr pParams, uint16_t index);
		void toParamAtIndex(PackagePtr pParams, uint16_t index);
		void setReturn(sLONG_PTR *pResult);	
		
		uint16_t getYear();	
		uint16_t getMonth();
		uint16_t getDay();
		void getYearMonthDay(uint16_t *pYear, uint16_t *pMonth, uint16_t *pDay);			
		void setYearMonthDay(uint16_t year, uint16_t month, uint16_t day);	
		
		C_DATE();
		~C_DATE();
		
	};
	
#if VERSIONMAC
#ifdef __OBJC__	
//need this for GCC 4.0 and 10.4u sdk support	
	class C_TIME;
	NSDate *CreateNSDateFromDateTime(C_DATE *date, C_TIME *time);
#endif	
#endif	
	
#ifdef __cplusplus
}
#endif

#endif