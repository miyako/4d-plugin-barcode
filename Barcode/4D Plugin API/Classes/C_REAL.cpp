/*
 *  C_REAL.cpp
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#include "C_REAL.h"

void C_REAL::fromParamAtIndex(PackagePtr pParams, uint16_t index)
{	
	if(index)
		this->_doubleValue = *(double *)(pParams[index - 1]);	
}

void C_REAL::toParamAtIndex(PackagePtr pParams, uint16_t index)
{
	if(index)	
		*(double *)(pParams[index - 1]) = this->_doubleValue;
}

void C_REAL::setReturn(sLONG_PTR *pResult)
{
	**(double **) pResult = this->_doubleValue;
}

void C_REAL::setDoubleValue(double doubleValue)
{
	this->_doubleValue = doubleValue;
}

double C_REAL::getDoubleValue()
{
	return this->_doubleValue;
}

C_REAL::C_REAL() : _doubleValue(0)
{
	
}

C_REAL::~C_REAL()
{ 
	
}
