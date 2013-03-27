/*
 *  functions_zint.cpp
 *  Time
 *
 *  Created by miyako on 2012/10/30.
 *
 */

#include "functions_zint.h"

int _GetHorizontalOffset(zint_symbol *sym)
{
	int pixelWidth = sym->bitmap_width / sym->width;
	return pixelWidth * 3;//bytes per r:g:b pixel
}

int _GetVerticalOffset(zint_symbol *sym)
{			
	int offsetV, offsetH = _GetHorizontalOffset(sym);
	
	switch (sym->symbology) {
			
		case BARCODE_AZRUNE:
		case BARCODE_AZTEC:	
		case BARCODE_MICROQR:	
		case BARCODE_QRCODE:	
		case BARCODE_HIBC_QR:					
		case BARCODE_HIBC_DM:
		case BARCODE_DATAMATRIX:
		case BARCODE_CODEONE:
		case BARCODE_GRIDMATRIX:				
			offsetV = (offsetH * sym->width);
			break;
			
		case BARCODE_PDF417:
		case BARCODE_PDF417TRUNC:
		case BARCODE_HIBC_PDF:				
			offsetV = 5 * (offsetH * sym->width);
			break;	
			
		case BARCODE_MICROPDF417:
		case BARCODE_HIBC_MICPDF:				
			offsetV = 3 * (offsetH * sym->width);
			break;	
			
		case BARCODE_JAPANPOST:
		case BARCODE_ONECODE:	
		case BARCODE_KIX:	
		case BARCODE_AUSPOST:
		case BARCODE_AUSREPLY:		
		case BARCODE_AUSROUTE:
		case BARCODE_AUSREDIRECT:	
		case BARCODE_RM4SCC:
		case BARCODE_DAFT:				
			offsetV = 5 * (offsetH * sym->width);
			break;
			
		case BARCODE_PHARMA_TWO:				
			offsetV = 49 * (offsetH * sym->width);
			break;

		case BARCODE_POSTNET:	
		case BARCODE_PLANET:				
			offsetV = 11 * (offsetH * sym->width);
			break;
			
		default:
			offsetV = (offsetH * sym->width);
			break;
	}
	
	return offsetV;
}

#pragma mark -

void _convertToSVG(zint_symbol *sym, C_TEXT &returnValue){
	
	if(sym){
		
		char _x[33];
		char _y[33];
		
		char _xs[33];
		char _ys[33];
		
		CUTF8String svg;
		
		svg += (const uint8_t *)"<?xml version=\"1.0\" encoding=\"utf-8\" ?><svg>";
		
		char *p;
		int x, y = 0, i, j;
		
		int offsetH = _GetHorizontalOffset(sym);
		int offsetV = _GetVerticalOffset(sym);
				
		p = sym->bitmap;
		
		void (*_PA_YieldAbsolute)(void) = PA_YieldAbsolute;
		
		int x_size, y_size;
		
		switch (sym->symbology) {
			
			case BARCODE_JAPANPOST:
			case BARCODE_ONECODE:	
			case BARCODE_KIX:	
			case BARCODE_AUSPOST:
			case BARCODE_AUSREPLY:		
			case BARCODE_AUSROUTE:
			case BARCODE_AUSREDIRECT:	
			case BARCODE_RM4SCC:
			case BARCODE_DAFT:					
				x_size = 10;
				y_size = 30;				
				break;
				
			case BARCODE_AZRUNE:
			case BARCODE_AZTEC:		
			case BARCODE_MICROQR:	
			case BARCODE_QRCODE:
			case BARCODE_HIBC_QR:					
			case BARCODE_HIBC_DM:
			case BARCODE_DATAMATRIX:
			case BARCODE_CODEONE:
			case BARCODE_GRIDMATRIX:				
				x_size = 20;
				y_size = 20;				
				break;	
				
			case BARCODE_PDF417:
			case BARCODE_PDF417TRUNC:				
			case BARCODE_MICROPDF417:
			case BARCODE_HIBC_PDF:
			case BARCODE_HIBC_MICPDF:				
				x_size = 10;
				y_size = 20;	
				break;	
				
			case BARCODE_PHARMA_TWO:				
				x_size = 10;
				y_size = 200;
				break;
				
			case BARCODE_POSTNET:	
			case BARCODE_PLANET:				
				x_size = 10;
				y_size = 40;
				break;
				
			default:
				x_size = 2;
				y_size = 128;
				break;
		}
		
		sprintf(_xs, "%d", x_size);
		sprintf(_ys, "%d", y_size);
		
		for(i = 0; i < sym->rows; ++i) 
		{
			x = 0;
			
			sprintf(_y, "%d", y);
			
			for(j = 0; j < sym->width; ++j) 
			{
				sprintf(_x, "%d", x);
				
				(*_PA_YieldAbsolute)(); 
				
				if(*p==0x00)
				{
					svg += (const uint8_t *)"<rect fill=\"black\" stroke=\"black\" height=\"";
					svg += (const uint8_t *)_ys;
					svg += (const uint8_t *)"\" width=\"";
					svg += (const uint8_t *)_xs;
					svg += (const uint8_t *)"\" ";
					svg += (const uint8_t *)"x=\"";
					svg += (const uint8_t *)_x;
					svg += (const uint8_t *)"\" ";
					svg += (const uint8_t *)"y=\"";
					svg += (const uint8_t *)_y;
					svg += (const uint8_t *)"\" />";
				}else{
					svg += (const uint8_t *)"<rect fill=\"white\" stroke=\"white\" height=\"";
					svg += (const uint8_t *)_ys;
					svg += (const uint8_t *)"\" width=\"";
					svg += (const uint8_t *)_xs;
					svg += (const uint8_t *)"\" ";
					svg += (const uint8_t *)"x=\"";
					svg += (const uint8_t *)_x;
					svg += (const uint8_t *)"\" ";
					svg += (const uint8_t *)"y=\"";
					svg += (const uint8_t *)_y;
					svg += (const uint8_t *)"\" />";
				}
				
				p += offsetH; 
				x = x + x_size;
			}
			y = y + y_size;
			p += offsetV;
		}	
		
		svg += (const uint8_t *)"</svg>";
		
		returnValue.setUTF8String(&svg);
		
	}
	
}

void _convertToText(zint_symbol *sym, C_TEXT &returnValue){

	if(sym){
	
		char *p;
		int x, y, b = 0;
		
		int offsetH = _GetHorizontalOffset(sym);
		int offsetV = _GetVerticalOffset(sym);
		
		p = sym->bitmap;
		
		uint32_t size = ((sym->rows * sym->width) + sym->rows) + 1;
		
		std::vector<uint8_t> data(size);
		
		void (*_PA_YieldAbsolute)(void) = PA_YieldAbsolute;
		
		for(y = 0; y < sym->rows; ++y) 
		{
			for(x = 0; x < sym->width; ++x) 
			{
				(*_PA_YieldAbsolute)(); 
				
				if(*p==0x00)
				{
					data[b] = 49;
				}else{
					data[b] = 48;
				}
				
				p += offsetH; 
				b++;
			}
			data[b] = 10;
			p += offsetV; 
			b++;
		}	
	
		returnValue.setUTF8String((const uint8_t*)&data[0], size);
	
	}
	
}

#pragma mark -

void _ZINT(int n, unsigned char *s, C_LONGINT &Param2, C_LONGINT &Param3, C_TEXT &Param4, ARRAY_LONGINT &Param5, C_TEXT &returnValue)
{
	zint_symbol *sym = ZBarcode_Create();
	
	if(sym){
	
		sym->input_mode = UNICODE_MODE;
		
		sym->symbology = Param2.getIntValue();
		
		if(Param5.getSize() > 1)
			sym->option_1 = Param5.getIntValueAtIndex(1);
		
		if(Param5.getSize() > 2)
			sym->option_2 = Param5.getIntValueAtIndex(2);		
		
		if(Param5.getSize() > 3)
			sym->option_3 = Param5.getIntValueAtIndex(3);				
		
		sym->show_hrt = 0;	//no text
		
#ifdef DEBUG		
		sym->output_options = BARCODE_STDOUT;
		strncpy(sym->outfile, "dummy.txt", 10);
#endif		
		int err = ZBarcode_Encode(sym, s, n);
		
		if(!err){
#ifdef DEBUG			
			err = ZBarcode_Print(sym, 0);
#endif			
				
			err = ZBarcode_Buffer(sym, 0);
			
			if(!err){
				
				switch (Param3.getIntValue()) {
					case 1:
						_convertToSVG(sym, returnValue);				
						break;			
					default:
						_convertToText(sym, returnValue);		
						break;
				}
				
			}
			
		}else{
			CUTF8String err = (const uint8_t *)sym->errtxt;
			Param4.setUTF8String(&err);
		}
		
		ZBarcode_Delete(sym);
		
	}

}

void BARCODE_Text(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_LONGINT Param2;
	C_LONGINT Param3;
	C_TEXT Param4;
	ARRAY_LONGINT Param5;
	C_TEXT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	Param5.fromParamAtIndex(pParams, 5);
	
	if(Param1.getUTF16Length()){
	
		CUTF8String str;
		Param1.copyUTF8String(&str);
		
		int n = str.size();
		unsigned char *s = (unsigned char *)str.c_str();
		
		_ZINT(n, s, Param2, Param3, Param4, Param5, returnValue);
		
	}
	
	Param4.toParamAtIndex(pParams, 4);
	returnValue.setReturn(pResult);
}

void BARCODE_Data(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_BLOB Param1;
	C_LONGINT Param2;
	C_LONGINT Param3;
	C_TEXT Param4;
	ARRAY_LONGINT Param5;
	C_TEXT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	Param5.fromParamAtIndex(pParams, 5);
	
	if(Param1.getBytesLength()){
		
		int n = Param1.getBytesLength();
		unsigned char *s = (unsigned char *)Param1.getBytesPtr();
		
		_ZINT(n, s, Param2, Param3, Param4, Param5, returnValue);
		
	}
	
	Param4.toParamAtIndex(pParams, 4);
	returnValue.setReturn(pResult);
}