/*
 *  C_TIME.cpp
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#include "C_TIME.h"
	
void C_TIME::fromParamAtIndex(PackagePtr pParams, uint16_t index)
{	
	if(index)
		this->_seconds = *(uint32_t *)(pParams[index - 1]);	
}

void C_TIME::toParamAtIndex(PackagePtr pParams, uint16_t index)
{
	if(index)	
		*(uint32_t *)(pParams[index - 1]) = this->_seconds;
}

void C_TIME::setReturn(sLONG_PTR *pResult)
{	
	*(uint32_t *) pResult = this->_seconds;
}

uint32_t C_TIME::getSeconds()
{
	return this->_seconds;
}

void C_TIME::setSeconds(uint32_t seconds)
{
	this->_seconds = seconds;		
}

C_TIME::C_TIME() : _seconds(0)
{	
}

C_TIME::~C_TIME()
{ 
}