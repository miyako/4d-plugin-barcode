/*
 *  C_INTEGER.cpp
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#include "C_INTEGER.h"

void C_INTEGER::fromParamAtIndex(PackagePtr pParams, uint16_t index)
{	
	if(index)	
		this->_shortValue = *(short *)(pParams[index - 1]);		
}

void C_INTEGER::toParamAtIndex(PackagePtr pParams, uint16_t index)
{
	if(index)	
		*(short *)(pParams[index - 1]) = this->_shortValue;
}

void C_INTEGER::setReturn(sLONG_PTR *pResult)
{
	*(short *) pResult = this->_shortValue;
}

void C_INTEGER::setShortValue(short shortValue)
{
	this->_shortValue = shortValue;
}

short C_INTEGER::getShortValue()
{
	return this->_shortValue;
}

C_INTEGER::C_INTEGER() : _shortValue(0)
{
	
}

C_INTEGER::~C_INTEGER()
{ 

}
