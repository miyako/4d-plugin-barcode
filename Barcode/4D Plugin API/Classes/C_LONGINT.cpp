/*
 *  C_LONGINT.cpp
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#include "C_LONGINT.h"

void C_LONGINT::fromParamAtIndex(PackagePtr pParams, uint16_t index)
{	
	if(index)
		this->_intValue = *(int *)(pParams[index - 1]);	
}

void C_LONGINT::toParamAtIndex(PackagePtr pParams, uint16_t index)
{
	if(index)	
		*(int *)(pParams[index - 1]) = this->_intValue;
}

void C_LONGINT::setReturn(sLONG_PTR *pResult)
{
	*(int *) pResult = this->_intValue;
}

void C_LONGINT::setIntValue(int intValue)
{
	this->_intValue = intValue;
}

int C_LONGINT::getIntValue()
{
	return this->_intValue;
}

C_LONGINT::C_LONGINT() : _intValue(0)
{
	
}

C_LONGINT::~C_LONGINT()
{ 
	
}
