#include "functions_p417.h"

void _convertToSVG(pdf417param *pdf417, C_TEXT &returnValue){
	
	if(pdf417)
	{
		if(!pdf417->error){
	
			char _x[33];
			char _y[33];
			
			CUTF8String svg;
			
			svg += (const uint8_t *)"<?xml version=\"1.0\" encoding=\"utf-8\" ?><svg>";
			
			char *p;
			int x, y = 0;
			
			p = pdf417->outBits;
			
			int fullBytes = pdf417->bitColumns / 8;
			int oddBytes = pdf417->bitColumns - (fullBytes * 8);
			
			void (*_PA_YieldAbsolute)(void) = PA_YieldAbsolute;
			
			for (int k = 0; k < pdf417->codeRows; ++k) {
				
				x = 0;
				
				sprintf(_y, "%d", y);
				
				for (int i = 0; i < fullBytes; ++i)	{
					
					(*_PA_YieldAbsolute)(); 
					
					for (int j = 0; j < 8; j++)	{
						
						sprintf(_x, "%d", x);

						if (*p & (0x80 >> j))
						{
							svg += (const uint8_t *)"<rect fill=\"black\" stroke=\"black\" height=\"30\" width=\"10\" ";
							svg += (const uint8_t *)"x=\"";
							svg += (const uint8_t *)_x;
							svg += (const uint8_t *)"\" ";
							svg += (const uint8_t *)"y=\"";
							svg += (const uint8_t *)_y;
							svg += (const uint8_t *)"\" />";
						}else{
							svg += (const uint8_t *)"<rect fill=\"white\" stroke=\"white\" height=\"30\" width=\"10\" ";
							svg += (const uint8_t *)"x=\"";
							svg += (const uint8_t *)_x;
							svg += (const uint8_t *)"\" ";
							svg += (const uint8_t *)"y=\"";
							svg += (const uint8_t *)_y;
							svg += (const uint8_t *)"\" />";
						}
						x = x + 10;
					}
					
					p++;	
				}
				
				for (int j = 0; j < oddBytes; j++)	{
					
					sprintf(_x, "%d", x);
					
					if (*p & (0x80 >> j))
					{
						svg += (const uint8_t *)"<rect fill=\"black\" stroke=\"black\" height=\"30\" width=\"10\" ";
						svg += (const uint8_t *)"x=\"";
						svg += (const uint8_t *)_x;
						svg += (const uint8_t *)"\" ";
						svg += (const uint8_t *)"y=\"";
						svg += (const uint8_t *)_y;
						svg += (const uint8_t *)"\" />";
					}else{
						svg += (const uint8_t *)"<rect fill=\"white\" stroke=\"white\" height=\"30\" width=\"10\" ";
						svg += (const uint8_t *)"x=\"";
						svg += (const uint8_t *)_x;
						svg += (const uint8_t *)"\" ";
						svg += (const uint8_t *)"y=\"";
						svg += (const uint8_t *)_y;
						svg += (const uint8_t *)"\" />";
					}
					x = x + 10;
				}
				
				p++;
			
				y = y + 30;
			}
			
			svg += (const uint8_t *)"</svg>";
			
			returnValue.setUTF8String(&svg);
		}	
		
	}
}

void _convertToText(pdf417param *pdf417, C_TEXT &returnValue){
	
	if(pdf417)
	{
		if(!pdf417->error){
			
			char *p;
			int b = 0;
			
			p = pdf417->outBits;
			
			
			int fullBytes = pdf417->bitColumns / 8;
			int oddBytes = pdf417->bitColumns - (fullBytes * 8);
			
			uint32_t size = ((pdf417->codeRows * pdf417->bitColumns) + pdf417->codeRows) + 1;
			std::vector<uint8_t> data(size);
			
			//NSLog(@"buffer size: %d, should be 1045", size);
			
			void (*_PA_YieldAbsolute)(void) = PA_YieldAbsolute;
			
			for (int k = 0; k < pdf417->codeRows; ++k) {
				
				for (int i = 0; i < fullBytes; ++i)	{
					
					(*_PA_YieldAbsolute)(); 
					
					for (int j = 0; j < 8; j++)	{
						
						if (*p & (0x80 >> j))
						{
							data[b] = 49;
						}else{
							data[b] = 48;
						}
						b++;
					}
				
					p++;	
				}
				
					for (int j = 0; j < oddBytes; j++)	{
						
						if (*p & (0x80 >> j))
						{
							data[b] = 49;
						}else{
							data[b] = 48;
						}
						b++;
					}
				
					p++;
					
				data[b] = 10;
				b++;
				
			}
			
			//NSLog(@"used size: %d, should be 1044", b);
			//NSLog(@"moved size: %d, should be %d", p - pdf417->outBits, pdf417->lenBits);			
			
			returnValue.setUTF8String((const uint8_t*)&data[0], size);
		}	
		
	}
}

#pragma mark -

void _PDF417(pdf417param *pdf417, C_LONGINT &Param2, C_LONGINT &Param3, C_TEXT &returnValue)
{
	if(pdf417){
		
		int errorLevel = Param3.getIntValue();
		
		if(!errorLevel){
			pdf417->options = PDF417_AUTO_ERROR_LEVEL;	
		}else{
			pdf417->options = PDF417_USE_ERROR_LEVEL;
			
			if(errorLevel < 0)
				errorLevel = 0;
			
			if(errorLevel > 4)
				errorLevel = 4;
			
			pdf417->errorLevel = errorLevel;
		}
		
		paintCode(pdf417);
		
		switch (Param2.getIntValue()) {
			case 1:
				_convertToSVG(pdf417, returnValue);				
				break;			
			default:
				_convertToText(pdf417, returnValue);		
				break;
		}	
		
		pdf417free(pdf417);
		
	}
	
}

#pragma mark -

void PDF417_Text(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_LONGINT Param2;
	C_LONGINT Param3;	
	C_TEXT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	
	if(Param1.getUTF16Length()){
	
		pdf417param pdf417;
		
		pdf417init(&pdf417);
		
		CUTF8String t;
		Param1.copyUTF8String(&t);
		
		pdf417.text = (char *)t.c_str();
		pdf417.lenText = t.size();
		
		_PDF417(&pdf417, Param2, Param3, returnValue);
		
	}
	
	returnValue.setReturn(pResult);
}



void PDF417_Data(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_BLOB Param1;
	C_LONGINT Param2;
	C_LONGINT Param3;
	C_TEXT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	
	if(Param1.getBytesLength()){
		
		pdf417param pdf417;
		
		pdf417init(&pdf417);
		
		pdf417.text = (char *)Param1.getBytesPtr();
		pdf417.lenText = Param1.getBytesLength();
		
		_PDF417(&pdf417, Param2, Param3, returnValue);

	}
	
	returnValue.setReturn(pResult);
}