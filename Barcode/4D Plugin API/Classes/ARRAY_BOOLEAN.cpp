/*
 *  ARRAY_BOOLEAN.cpp
 *  4D Plugin
 *
 *  Created by miyako on 11/01/03.
 *
 */

#include "ARRAY_BOOLEAN.h"

static unsigned char tabTrue [ 8 ] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

static bool GetBitInTable( unsigned char *bitTable, long index)
{
	return (bool) ( ( *( bitTable + ( index >> 3 ) ) & tabTrue[ index & 7 ] ) ? 1 : 0 );
}

void ARRAY_BOOLEAN::fromParamAtIndex(PackagePtr pParams, uint16_t index)
{	
	if(index)		
	{
		PA_Variable arr = *((PA_Variable*) pParams[index - 1]);

		this->setSize(0);
		
		if(arr.fType == eVK_ArrayBoolean)
		{
			uint32_t i;
			
			bool booleanValue;
			
			for(i = 0; i <= (uint32_t)arr.uValue.fArray.fNbElements; i++)
			{
				booleanValue = GetBitInTable( *(unsigned char**) (arr.uValue.fArray.fData), i );
				this->_CboolArray->push_back(booleanValue);				
			}
			
		}
		
	}
			
}

void ARRAY_BOOLEAN::toParamAtIndex(PackagePtr pParams, uint16_t index)
{
	if(index)		
	{
		PA_Variable arr = *((PA_Variable*) pParams[index - 1]);
		PA_Variable *param = ((PA_Variable *)pParams[index - 1]);
		
		switch (arr.fType) 
		{
			case eVK_ArrayBoolean:
				break;
			case eVK_Undefined:
				PA_ClearVariable(&arr);				
				arr = PA_CreateVariable(eVK_ArrayBoolean);
				param->fType = arr.fType;
				break;				
				
			default:
				break;
		}
		
		if(arr.fType == eVK_ArrayBoolean)
		{
			
			PA_ResizeArray(&arr, this->_CboolArray->empty() ? 0 : (uint32_t)(this->_CboolArray->size() - 1));
			
			uint32_t i;
			
			for(i = 0; i < this->_CboolArray->size(); i++)
			{
				PA_SetBooleanInArray(arr, i, this->_CboolArray->at(i));	
			}
			
			param->fFiller = 0;
			param->uValue.fArray.fCurrent = arr.uValue.fArray.fCurrent;
			param->uValue.fArray.fNbElements = arr.uValue.fArray.fNbElements;
			param->uValue.fArray.fData = arr.uValue.fArray.fData;	
			
		}
		
	}
				
}

void ARRAY_BOOLEAN::appendBooleanValue(bool booleanValue)
{
	this->_CboolArray->push_back(booleanValue);	
}

void ARRAY_BOOLEAN::setBooleanValueAtIndex(bool booleanValue, uint32_t index)
{
	if(index < this->_CboolArray->size())
	{
		
		std::vector<bool>::iterator it = this->_CboolArray->begin();
		it += index;
		
		this->_CboolArray->insert(this->_CboolArray->erase(it), booleanValue);
		
	}
}

bool ARRAY_BOOLEAN::getBooleanValueAtIndex(uint32_t index)	
{
	
	bool booleanValue = 0;
	
	if(index < this->_CboolArray->size())
	{
		
		booleanValue = this->_CboolArray->at(index);
	}
	
	return booleanValue;
	
}

uint32_t ARRAY_BOOLEAN::getSize()
{
	return (uint32_t)this->_CboolArray->size();
}

void ARRAY_BOOLEAN::setSize(uint32_t size)
{
	this->_CboolArray->resize(size);
}

ARRAY_BOOLEAN::ARRAY_BOOLEAN() : _CboolArray(new CboolArray)
{
	
}

ARRAY_BOOLEAN::~ARRAY_BOOLEAN()
{ 
	delete _CboolArray; 
}