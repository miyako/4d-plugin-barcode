#include "functions_zbar.h"

void BARCODE_Read_image(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_PICTURE Param1;
	ARRAY_TEXT Param2;
	ARRAY_TEXT Param3;
	C_TEXT returnValue;	
	
	Param1.fromParamAtIndex(pParams, 1);
	
	PA_Picture gs = Param1.createGrayScale();
	
	void (*_PA_YieldAbsolute)(void) = PA_YieldAbsolute;
	
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
				(*_PA_YieldAbsolute)(); 
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
			(*_PA_YieldAbsolute)(); 
			for(size_t x = 0; x < w; x++) {
				
				Gdiplus::Color c;
				image->GetPixel(x,y,&c);
				
				y8 = c.GetR();
				buf[i] = y8;
				i++;
			}
		}
#endif			
		
		//http://sourceforge.net/apps/mediawiki/zbar/index.php?title=HOWTO:_Scan_images_using_the_API		
		
		using namespace zbar;
		
		zbar_image_scanner_t *zScanner = zbar_image_scanner_create();
		
		zbar_image_scanner_set_config(zScanner, ZBAR_NONE, ZBAR_CFG_ENABLE, 1); 		
		
		zbar_image_t *zImage = zbar_image_create();
		zbar_image_set_format(zImage, *(int*)"Y800");	
		zbar_image_set_size(zImage, w, h);
		zbar_image_set_data(zImage, &buf[0], w * h, NULL);
		
		unsigned int n = zbar_scan_image(zScanner, zImage);
		unsigned int count = 0;
		
		if(n){
			
			Param2.setSize(1);
			Param3.setSize(1);
			
			const zbar_symbol_t *symbol = zbar_image_first_symbol(zImage);
			
			for(; symbol; symbol = zbar_symbol_next(symbol)) {
				
				(*_PA_YieldAbsolute)(); 
				
				count++;
				
				const char *zData = zbar_symbol_get_data(symbol);
				const char *zName = zbar_get_symbol_name(zbar_symbol_get_type(symbol));
				
				CUTF8String uData = CUTF8String((const uint8_t *)zData);
				CUTF8String uName = CUTF8String((const uint8_t *)zName);
				
				Param2.appendUTF8String(&uData);
				Param3.appendUTF8String(&uName);
				
				if(count == 1){
					returnValue.setUTF8String(&uData);
				}
				
			}
			
		}
		
		zbar_image_scanner_destroy(zScanner); zScanner = NULL;
		zbar_image_destroy(zImage); zImage = NULL;
		
#if VERSIONMAC	
		CGImageRelease(image); image = NULL; 
#else
		delete image; image = NULL; 
#endif		
		
	}

	Param2.toParamAtIndex(pParams, 2);
	Param3.toParamAtIndex(pParams, 3);
	returnValue.setReturn(pResult);
}