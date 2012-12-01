/*
 *  ARRAY_REAL.cpp
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#include "ARRAY_REAL.h"

void ARRAY_REAL::fromParamAtIndex(PackagePtr pParams, uint16_t index)
{	
	if(index)		
	{
		PA_Variable arr = *((PA_Variable*) pParams[index - 1]);

		this->setSize(0);
		
		if(arr.fType == eVK_ArrayReal)
		{
			uint32_t i;
			
			double doubleValue;
			
			for(i = 0; i <= (uint32_t)arr.uValue.fArray.fNbElements; i++)
			{
				doubleValue = ( * (double**) (arr.uValue.fArray.fData) )[i];
				this->_CDoubleArray->push_back(doubleValue);				
			}
			
		}
		
	}
			
}

void ARRAY_REAL::toParamAtIndex(PackagePtr pParams, uint16_t index)
{
	if(index)		
	{
		PA_Variable arr = *((PA_Variable*) pParams[index - 1]);
		PA_Variable *param = ((PA_Variable *)pParams[index - 1]);
		
		switch (arr.fType) 
		{
			case eVK_ArrayReal:
				break;
			case eVK_Undefined:
				PA_ClearVariable(&arr);				
				arr = PA_CreateVariable(eVK_ArrayReal);
				param->fType = arr.fType;
				break;				
				
			default:
				break;
		}
		
		if(arr.fType == eVK_ArrayReal)
		{
			
			PA_ResizeArray(&arr, this->_CDoubleArray->empty() ? 0 : (uint32_t)(this->_CDoubleArray->size() - 1));
			
			uint32_t i;
			
			for(i = 0; i < this->_CDoubleArray->size(); i++)
			{
				PA_SetRealInArray(arr, i, this->_CDoubleArray->at(i));	
			}
			
			param->fFiller = 0;
			param->uValue.fArray.fCurrent = arr.uValue.fArray.fCurrent;
			param->uValue.fArray.fNbElements = arr.uValue.fArray.fNbElements;
			param->uValue.fArray.fData = arr.uValue.fArray.fData;	
			
		}
		
	}
				
}

void ARRAY_REAL::appendDoubleValue(double doubleValue)
{
	this->_CDoubleArray->push_back(doubleValue);	
}

void ARRAY_REAL::setDoubleValueAtIndex(double doubleValue, uint32_t index)
{
	if(index < this->_CDoubleArray->size())
	{
		
		std::vector<double>::iterator it = this->_CDoubleArray->begin();
		it += index;
		
		this->_CDoubleArray->insert(this->_CDoubleArray->erase(it), doubleValue);
		
	}
}

double ARRAY_REAL::getDoubleValueAtIndex(uint32_t index)	
{
	
	double doubleValue = 0;
	
	if(index < this->_CDoubleArray->size())
	{
		
		doubleValue = this->_CDoubleArray->at(index);
	}
	
	return doubleValue;
	
}

uint32_t ARRAY_REAL::getSize()
{
	return (uint32_t)this->_CDoubleArray->size();
}

void ARRAY_REAL::setSize(uint32_t size)
{
	this->_CDoubleArray->resize(size);
}

ARRAY_REAL::ARRAY_REAL() : _CDoubleArray(new CDoubleArray)
{
	
}

ARRAY_REAL::~ARRAY_REAL()
{ 
	delete _CDoubleArray; 
}