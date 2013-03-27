#include "functions_qren.h"

QRecLevel _getErrorCorrectionLevel(int level){
	
	QRecLevel correctionLevel;
	
	switch (level) {
		case 1:
			correctionLevel = QR_ECLEVEL_M;
			break;
		case 2:
			correctionLevel = QR_ECLEVEL_Q;
			break;	
		case 3:
			correctionLevel = QR_ECLEVEL_H;
			break;				
		default:
			correctionLevel = QR_ECLEVEL_L;			
			break;
	}
	
	return correctionLevel;
	
}

QRecLevel _getErrorCorrectionLevelMicro(int level){
	
	QRecLevel correctionLevel;
	
	switch (level) {
		case 1:
			correctionLevel = QR_ECLEVEL_M;
			break;
		case 2:
			correctionLevel = QR_ECLEVEL_Q;
			break;				
		default:
			correctionLevel = QR_ECLEVEL_L;			
			break;
	}
	
	return correctionLevel;
	
}

#pragma mark -

void _convertToSVG(QRcode *qr, C_TEXT &returnValue){
	
	if(qr)
	{		
		char _x[33];
		char _y[33];
		
		CUTF8String svg;
		
		svg += (const uint8_t *)"<?xml version=\"1.0\" encoding=\"utf-8\" ?><svg>";
		
		uint8_t *p;
		int x, y = 0, i, j;
		
		p = qr->data;
		
		void (*_PA_YieldAbsolute)(void) = PA_YieldAbsolute;
		
		for(i = 0; i < qr->width; ++i) 
		{
			x = 0;
			
			sprintf(_y, "%d", y);
			
			for(j = 0; j < qr->width; ++j) 
			{
				sprintf(_x, "%d", x);
				
				(*_PA_YieldAbsolute)(); 
				
				if(*p&1)
				{
					svg += (const uint8_t *)"<rect fill=\"black\" stroke=\"black\" height=\"20\" width=\"20\" ";
					svg += (const uint8_t *)"x=\"";
					svg += (const uint8_t *)_x;
					svg += (const uint8_t *)"\" ";
					svg += (const uint8_t *)"y=\"";
					svg += (const uint8_t *)_y;
					svg += (const uint8_t *)"\" />";										
				}else{
					svg += (const uint8_t *)"<rect fill=\"white\" stroke=\"white\" height=\"20\" width=\"20\" ";
					svg += (const uint8_t *)"x=\"";
					svg += (const uint8_t *)_x;
					svg += (const uint8_t *)"\" ";
					svg += (const uint8_t *)"y=\"";
					svg += (const uint8_t *)_y;
					svg += (const uint8_t *)"\" />";
				}
				p++; 
				x = x + 20;
			}
			y = y + 20;
		}							
		
		svg += (const uint8_t *)"</svg>";
		
		returnValue.setUTF8String(&svg);
		
		QRcode_free(qr);
		
	}

}

void _convertToText(QRcode *qr, C_TEXT &returnValue){
	
	if(qr)
	{
		uint8_t *p;
		int x, y, b = 0;
		
		p = qr->data;
		
		uint32_t size = ((qr->width+1) * qr->width) + 1;
		std::vector<uint8_t> data(size);
		
		void (*_PA_YieldAbsolute)(void) = PA_YieldAbsolute;
		
		for(y = 0; y < qr->width; y++) 
		{
			for(x = 0; x < qr->width; x++) 
			{
				(*_PA_YieldAbsolute)(); 
				
				if(*p&1)
				{
					data[b] = 49;
				}else{
					data[b] = 48;
				}
				
				p++; 
				b++;
			}
			data[b] = 10;
			b++;
		}							
		
		returnValue.setUTF8String((const uint8_t*)&data[0], size);
		
		QRcode_free(qr);
		
	}	
	
}

#pragma mark -

void QRCODE_Text(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_LONGINT Param2;
	C_LONGINT Param3;
	C_LONGINT Param4;
	C_TEXT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	Param4.fromParamAtIndex(pParams, 4);
	
	if(Param1.getUTF16Length()){
	
		PA_4DCharSet encoding;
		QRencodeMode mode;
		
		switch (Param2.getIntValue()) {
			case 3:
				encoding = eVTC_SHIFT_JIS;
				mode = QR_MODE_KANJI;				
				break;			
			default:
				encoding = eVTC_UTF_8;	
				mode = QR_MODE_8;			
				break;
		}
		
		QRecLevel correctionLevel = _getErrorCorrectionLevel(Param3.getIntValue());
		
		uint32_t size = (Param1.getUTF16Length() * sizeof(PA_Unichar) * 2)+ sizeof(uint8_t);	
		std::vector<uint8_t> buffer(size);
		
		PA_ConvertCharsetToCharset((char *)Param1.getUTF16StringPtr(),
								   Param1.getUTF16Length() * sizeof(PA_Unichar),
								   eVTC_UTF_16, 
								   (char *)&buffer[0],
								   size,
								   encoding);	
		
		QRcode *qr = NULL;
		
		qr = QRcode_encodeString((const char *)&buffer[0],
								 0,
								 correctionLevel,
								 mode, 1);				
		
		switch (Param4.getIntValue()) {
			case 1:
				_convertToSVG(qr, returnValue);				
				break;			
			default:
				_convertToText(qr, returnValue);		
				break;
		}
		
	}
	
	returnValue.setReturn(pResult);	
}

void QRCODE_Data(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_BLOB Param1;
	C_LONGINT Param2;
	C_LONGINT Param3;
	C_TEXT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	
	if(Param1.getBytesLength()){
		
		QRecLevel correctionLevel = _getErrorCorrectionLevel(Param2.getIntValue());
		
		QRcode *qr = NULL;
		
		qr = QRcode_encodeData(
							   (int)Param1.getBytesLength(),
							   (const unsigned char *)Param1.getBytesPtr(),
							   0,
							   correctionLevel);
		
		switch (Param3.getIntValue()) {
			case 1:
				_convertToSVG(qr, returnValue);				
				break;			
			default:
				_convertToText(qr, returnValue);		
				break;
		}
		
	}
	
	returnValue.setReturn(pResult);		
}

#pragma mark -

void MICRO_QRCODE_Data(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_BLOB Param1;
	C_LONGINT Param2;
	C_LONGINT Param3;
	C_LONGINT Param4;
	C_TEXT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	Param4.fromParamAtIndex(pParams, 4);
	
	if(Param1.getBytesLength()){
	
		QRecLevel correctionLevel = _getErrorCorrectionLevelMicro(Param2.getIntValue());
		
		QRcode *qr = NULL;
		
		qr = QRcode_encodeDataMQR(
								  (int)Param1.getBytesLength(),
								  (const unsigned char *)Param1.getBytesPtr(),
								  Param3.getIntValue(),
								  correctionLevel);	
		
		switch (Param4.getIntValue()) {
			case 1:
				_convertToSVG(qr, returnValue);				
				break;			
			default:
				_convertToText(qr, returnValue);		
				break;
		}
		
	}
	
	returnValue.setReturn(pResult);		
}

void MICRO_QRCODE_Text(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_LONGINT Param2;
	C_LONGINT Param3;
	C_LONGINT Param4;
	C_LONGINT Param5;
	C_TEXT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	Param4.fromParamAtIndex(pParams, 4);
	Param5.fromParamAtIndex(pParams, 5);
	
	if(Param1.getUTF16Length()){
	
		PA_4DCharSet encoding;
		QRencodeMode mode;
		
		switch (Param2.getIntValue()) {
			case 3:
				encoding = eVTC_SHIFT_JIS;
				mode = QR_MODE_KANJI;				
				break;			
			default:
				encoding = eVTC_UTF_8;	
				mode = QR_MODE_8;			
				break;
		}
		
		QRecLevel correctionLevel = _getErrorCorrectionLevelMicro(Param3.getIntValue());
		
		uint32_t size = (Param1.getUTF16Length() * sizeof(PA_Unichar) * 2)+ sizeof(uint8_t);	
		std::vector<uint8_t> buffer(size);
		
		PA_ConvertCharsetToCharset((char *)Param1.getUTF16StringPtr(),
								   Param1.getUTF16Length() * sizeof(PA_Unichar),
								   eVTC_UTF_16, 
								   (char *)&buffer[0],
								   size,
								   encoding);	
		
		QRcode *qr = NULL;	
		
		qr = QRcode_encodeStringMQR((const char *)&buffer[0],
									Param4.getIntValue(),
									correctionLevel,
									mode, 1);			
		
		switch (Param5.getIntValue()) {
			case 1:
				_convertToSVG(qr, returnValue);				
				break;			
			default:
				_convertToText(qr, returnValue);		
				break;
		}
		
	}
	
	returnValue.setReturn(pResult);	
}
