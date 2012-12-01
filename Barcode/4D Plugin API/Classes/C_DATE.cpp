/*
 *  C_DATE.cpp
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#include "C_DATE.h"

void C_DATE::fromParamAtIndex(PackagePtr pParams, uint16_t index)
{	
	if(index)
	{
		PA_Date *date = (PA_Date *)(pParams[index - 1]);
		
		this->_year = date->fYear;
		this->_month = date->fMonth;
		this->_day = date->fDay;		
	}
}

void C_DATE::toParamAtIndex(PackagePtr pParams, uint16_t index)
{
	if(index)	
	{
		PA_Date *date = (PA_Date *)(pParams[index - 1]);
		
		date->fYear = this->_year;
		date->fMonth = this->_month;
		date->fDay = this->_day;		
	}
}

void C_DATE::setReturn(sLONG_PTR *pResult)
{
	PA_Date *date = *(PA_Date **) pResult;
	
	date->fYear = this->_year;
	date->fMonth = this->_month;
	date->fDay = this->_day;	
}

uint16_t C_DATE::getYear()
{
	return this->_year;
}

uint16_t C_DATE::getMonth()
{
	return this->_month;
}

uint16_t C_DATE::getDay()
{
	return this->_day;
}

void C_DATE::getYearMonthDay(uint16_t *pYear, uint16_t *pMonth, uint16_t *pDay)
{
	*pYear = this->_year;
	*pMonth = this->_month;
	*pDay = this->_day;		
}

void C_DATE::setYearMonthDay(uint16_t year, uint16_t month, uint16_t day)
{
	this->_year = year;
	this->_month = month;
	this->_day = day;		
}

C_DATE::C_DATE() : _year(0), _month(0), _day(0)
{	
}

C_DATE::~C_DATE()
{ 
}

#if VERSIONMAC
#ifdef __OBJC__	
NSDate *CreateNSDateFromDateTime(C_DATE *date, C_TIME *time)
{
	CFGregorianDate d;
	d.year		= date->getYear();
	d.month		= date->getMonth();
	d.day		= date->getDay();
	d.hour		= 0;
	d.minute	= 0;
	d.second	= 0;
	
	CFGregorianUnits h;
	h.years		= 0;
	h.months	= 0;
	h.days		= 0;	
	h.minutes	= 0;
	h.hours		= 0;
	h.seconds	= time->getSeconds();
	
	NSTimeZone *z = [NSTimeZone localTimeZone];
	
	NSDate *nd = NULL;
	
	if(CFGregorianDateIsValid(d, kCFGregorianUnitsYears+kCFGregorianUnitsMonths+kCFGregorianUnitsDays))
	{
		CFAbsoluteTime at = CFGregorianDateGetAbsoluteTime(d, (CFTimeZoneRef)z);
		CFAbsoluteTime s = CFAbsoluteTimeAddGregorianUnits(at, (CFTimeZoneRef)z, h);		
		nd= (NSDate *)CFDateCreate(kCFAllocatorDefault, s);
	}	
	
	return nd;
}
#endif	
#endif
