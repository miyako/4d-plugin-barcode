/*
 *  ARRAY_INTEGER.cpp
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#include "ARRAY_INTEGER.h"

void ARRAY_INTEGER::fromParamAtIndex(PackagePtr pParams, uint16_t index)
{	
	if(index)		
	{
		PA_Variable arr = *((PA_Variable*) pParams[index - 1]);

		this->setSize(0);
		
		if(arr.fType == eVK_ArrayInteger)
		{
			uint32_t i;
			
			short shortValue;
			
			for(i = 0; i <= (uint32_t)arr.uValue.fArray.fNbElements; i++)
			{
				shortValue = ( * (short**) (arr.uValue.fArray.fData) )[i];
				this->_CShortArray->push_back(shortValue);				
			}
			
		}
		
	}
			
}

void ARRAY_INTEGER::toParamAtIndex(PackagePtr pParams, uint16_t index)
{
	if(index)		
	{
		PA_Variable arr = *((PA_Variable*) pParams[index - 1]);
		PA_Variable *param = ((PA_Variable *)pParams[index - 1]);
		
		switch (arr.fType) 
		{
			case eVK_ArrayInteger:
				break;
			case eVK_Undefined:
				PA_ClearVariable(&arr);
				arr = PA_CreateVariable(eVK_ArrayInteger);
				param->fType = arr.fType;
				break;				
				
			default:
				break;
		}
		
		if(arr.fType == eVK_ArrayInteger)
		{
			
			PA_ResizeArray(&arr, this->_CShortArray->empty() ? 0 : (uint32_t)(this->_CShortArray->size() - 1));
			
			uint32_t i;
			
			for(i = 0; i < this->_CShortArray->size(); i++)
			{
				PA_SetIntegerInArray(arr, i, this->_CShortArray->at(i));	
			}
			
			param->fFiller = 0;
			param->uValue.fArray.fCurrent = arr.uValue.fArray.fCurrent;
			param->uValue.fArray.fNbElements = arr.uValue.fArray.fNbElements;
			param->uValue.fArray.fData = arr.uValue.fArray.fData;	
			
		}
		
	}
				
}

void ARRAY_INTEGER::appendIntValue(short shortValue)
{
	this->_CShortArray->push_back(shortValue);	
}

void ARRAY_INTEGER::setShortValueAtIndex(short shortValue, uint32_t index)
{
	if(index < this->_CShortArray->size())
	{
		
		std::vector<short>::iterator it = this->_CShortArray->begin();
		it += index;
		
		this->_CShortArray->insert(this->_CShortArray->erase(it), shortValue);
		
	}
}

int ARRAY_INTEGER::getShortValueAtIndex(uint32_t index)	
{
	
	short shortValue = 0;
	
	if(index < this->_CShortArray->size())
	{
		
		shortValue = this->_CShortArray->at(index);
	}
	
	return shortValue;
	
}

uint32_t ARRAY_INTEGER::getSize()
{
	return (uint32_t)this->_CShortArray->size();
}

void ARRAY_INTEGER::setSize(uint32_t size)
{
	this->_CShortArray->resize(size);
}

ARRAY_INTEGER::ARRAY_INTEGER() : _CShortArray(new CShortArray)
{
	
}

ARRAY_INTEGER::~ARRAY_INTEGER()
{ 
	delete _CShortArray; 
}