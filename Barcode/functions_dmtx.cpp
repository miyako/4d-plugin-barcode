#include "functions_dmtx.h"

// ---------------------------------- data matrix ---------------------------------


void DMTX_Read_image(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_PICTURE Param1;
	C_TEXT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	PA_Picture gs = Param1.createGrayScale();
	
#if VERSIONMAC	
	CGImageRef image = (CGImageRef)PA_CreateNativePictureForScreen(gs);
#else
	Gdiplus::Bitmap *image = (Gdiplus::Bitmap *)PA_CreateNativePictureForScreen(gs);
#endif	
	
	PA_DisposePicture(gs);
	
	if(image){
		
#if VERSIONMAC			
		
		size_t w = CGImageGetWidth(image);
		size_t h = CGImageGetHeight(image);
		
		size_t size = w * h;
		std::vector<uint8_t> buf(size);
		
		CGContextRef ctx = NULL;
		CGColorSpaceRef colorSpace = NULL;
		
		size_t bitmapBytesPerRow   = (w * 4);
		size_t bitmapByteCount     = (bitmapBytesPerRow * h);
		
		std::vector<uint8_t> bitmapData(bitmapByteCount);
		
		colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
		
		if (colorSpace){
			
			ctx = CGBitmapContextCreate ((void *)&bitmapData[0],
										 w,
										 h,
										 8,      // bits per component
										 bitmapBytesPerRow,
										 colorSpace,
										 kCGImageAlphaPremultipliedFirst);
			
			CGColorSpaceRelease(colorSpace);
			
		}
		
		if (ctx){
			
			CGRect rect = {{0,0},{w,h}};
			
			CGContextDrawImage(ctx, rect, image); 
			
			size_t *pixels = (size_t *)CGBitmapContextGetData (ctx);
			
			uint32_t pixel, y8;
			size_t i = 0;
			
			for(size_t y = 0; y < h; y++) {
				for(size_t x = 0; x < w; x++) {
					pixel = pixels[y*w+x];
					y8 = (pixel >> 24) & 0xFF;
					buf[i] = y8;
					i++;
				}
			}
			CGContextRelease(ctx); 
		}
		
#else
		size_t w = image->GetWidth();
		size_t h = image->GetHeight();
		
		size_t size = w * h;
		std::vector<uint8_t> buf(size);
		
		uint32_t y8;
		size_t i = 0;
		
		for(size_t y = 0; y < h; y++) {
			for(size_t x = 0; x < w; x++) {
				
				Gdiplus::Color c;
				image->GetPixel(x,y,&c);
				
				y8 = c.GetR();
				buf[i] = y8;
				i++;
			}
		}
#endif
	
		DmtxImage *img = dmtxImageCreate(&buf[0], w, h, DmtxPack8bppK);			
		
		if(img){
			DmtxDecode *dec = dmtxDecodeCreate(img, 1);
			if(dec){
				DmtxRegion *reg = dmtxRegionFindNext(dec, NULL);
				if(reg){
					DmtxMessage *msg = dmtxDecodeMatrixRegion(dec, reg, DmtxUndefined);
					if(msg){
						returnValue.setUTF8String(msg->output, msg->outputSize);
						dmtxMessageDestroy(&msg);
					}
					dmtxRegionDestroy(&reg);
				}
				dmtxDecodeDestroy(&dec);
			}
			dmtxImageDestroy(&img);
		}	
		
	}
	
	returnValue.setReturn(pResult);
}

void _convertToText(DmtxEncode *enc, C_TEXT &returnValue){
	
	if(enc)
	{
		
		if(enc->image){
	
			size_t width = dmtxImageGetProp(enc->image, DmtxPropWidth);
			size_t height = dmtxImageGetProp(enc->image, DmtxPropHeight);
			size_t bytesPerPixel = dmtxImageGetProp(enc->image, DmtxPropBytesPerPixel);
			
			uint8_t *p;
			size_t x, y, b = 0;
			
			p = enc->image->pxl;
			
			uint32_t size = ((width+1) * height) + 1;
			std::vector<uint8_t> data(size);
			
			void (*_PA_YieldAbsolute)(void) = PA_YieldAbsolute;
			
			for(y = 0; y < height; y++) 
			{
				for(x = 0; x < width; x++) 
				{
					(*_PA_YieldAbsolute)(); 
					
					if(*p&1)
					{
						data[b] = 49;
					}else{
						data[b] = 48;
					}
					
					p+=bytesPerPixel; 
					b++;
				}
				data[b] = 10;
				b++;
			}							
			
			returnValue.setUTF8String((const uint8_t*)&data[0], size);
			
		}
		
	}
			
}

void _convertToSVG(DmtxEncode *enc, C_TEXT &returnValue){
	
	if(enc)
	{	
		
		if(enc->image){
		
			size_t width = dmtxImageGetProp(enc->image, DmtxPropWidth);
			size_t height = dmtxImageGetProp(enc->image, DmtxPropHeight);
			size_t bytesPerPixel = dmtxImageGetProp(enc->image, DmtxPropBytesPerPixel);
			
			char _x[33];
			char _y[33];
			
			CUTF8String svg;
			
			svg += (const uint8_t *)"<?xml version=\"1.0\" encoding=\"utf-8\" ?><svg>";
			
			uint8_t *p;
			size_t x, y = 0, i, j;
			
			p = enc->image->pxl;
			
			void (*_PA_YieldAbsolute)(void) = PA_YieldAbsolute;
			
			for(i = 0; i < height; ++i) 
			{
				x = 0;
				
				sprintf(_y, "%d", (int)y);
				
				for(j = 0; j < width; ++j) 
				{
					sprintf(_x, "%d", (int)x);
					
					(*_PA_YieldAbsolute)(); 
					
					if(*p&1)
					{
						svg += (const uint8_t *)"<rect fill=\"white\" stroke=\"white\" height=\"20\" width=\"20\" ";
						svg += (const uint8_t *)"x=\"";
						svg += (const uint8_t *)_x;
						svg += (const uint8_t *)"\" ";
						svg += (const uint8_t *)"y=\"";
						svg += (const uint8_t *)_y;
						svg += (const uint8_t *)"\" />";										
					}else{
						svg += (const uint8_t *)"<rect fill=\"black\" stroke=\"black\" height=\"20\" width=\"20\" ";
						svg += (const uint8_t *)"x=\"";
						svg += (const uint8_t *)_x;
						svg += (const uint8_t *)"\" ";
						svg += (const uint8_t *)"y=\"";
						svg += (const uint8_t *)_y;
						svg += (const uint8_t *)"\" />";
					}
					p+=bytesPerPixel; 
					x = x + 20;
				}
				y = y + 20;
			}							
			
			svg += (const uint8_t *)"</svg>";
			
			returnValue.setUTF8String(&svg);	
			
		}
		
	}
	
}

void DMTX_Text(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_LONGINT Param2;
	C_LONGINT Param3;
	C_TEXT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);

	if(Param1.getUTF16Length()){
	
		DmtxEncode *enc = dmtxEncodeCreate();
		
		DmtxScheme scheme;
		
		switch (Param2.getIntValue()) {
			case 0:
				scheme = DmtxSchemeAscii;
				break;
			case 1:
				scheme = DmtxSchemeC40;
				break;
			case 2:
				scheme = DmtxSchemeText;
				break;
			case 3:
				scheme = DmtxSchemeX12;
				break;
			case 4:
				scheme = DmtxSchemeEdifact;
				break;			
			case 5:
				scheme = DmtxSchemeBase256;
				break;
			default:
				scheme = DmtxSchemeAutoBest;
				break;
		}
		
		dmtxEncodeSetProp(enc, DmtxPropScheme, scheme);	
		dmtxEncodeSetProp(enc, DmtxPropMarginSize, 0);
		dmtxEncodeSetProp(enc, DmtxPropModuleSize, 1);
		
		CUTF8String str;
		Param1.copyUTF8String(&str);
		
		dmtxEncodeDataMatrix(enc, str.size(), (unsigned char *)str.c_str());
		
		switch (Param3.getIntValue()) {
			case 1:
				_convertToSVG(enc, returnValue);				
				break;			
			default:
				_convertToText(enc, returnValue);		
				break;
		}
		
		dmtxEncodeDestroy(&enc);

	}
	
	returnValue.setReturn(pResult);
}

void DMTX_Data(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_BLOB Param1;
	C_LONGINT Param2;
	C_LONGINT Param3;
	C_TEXT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	
	if(Param1.getBytesLength()){
	
		DmtxEncode *enc = dmtxEncodeCreate();
		
		DmtxScheme scheme;
		
		switch (Param2.getIntValue()) {
			case 0:
				scheme = DmtxSchemeAscii;
				break;
			case 1:
				scheme = DmtxSchemeC40;
				break;
			case 2:
				scheme = DmtxSchemeText;
				break;
			case 3:
				scheme = DmtxSchemeX12;
				break;
			case 4:
				scheme = DmtxSchemeEdifact;
				break;			
			case 5:
				scheme = DmtxSchemeBase256;
				break;
			default:
				scheme = DmtxSchemeAutoBest;
				break;
		}
		
		dmtxEncodeSetProp(enc, DmtxPropScheme, scheme);	
		dmtxEncodeSetProp(enc, DmtxPropMarginSize, 0);
		dmtxEncodeSetProp(enc, DmtxPropModuleSize, 1);
		
		dmtxEncodeDataMatrix(enc, Param1.getBytesLength(), (unsigned char *)Param1.getBytesPtr());
		
		switch (Param3.getIntValue()) {
			case 1:
				_convertToSVG(enc, returnValue);				
				break;			
			default:
				_convertToText(enc, returnValue);		
				break;
		}
		
		dmtxEncodeDestroy(&enc);
	
	}
	
	returnValue.setReturn(pResult);
}
