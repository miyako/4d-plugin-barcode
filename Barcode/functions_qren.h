/*
 *  functions_qren.h
 *  
 *
 *  Created by miyako on 2012/10/30.
 *
 */

#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "qrencode.h"

// --- qrencode
void MICRO_QRCODE_Data(sLONG_PTR *pResult, PackagePtr pParams);
void MICRO_QRCODE_Text(sLONG_PTR *pResult, PackagePtr pParams);
void QRCODE_Data(sLONG_PTR *pResult, PackagePtr pParams);
void QRCODE_Text(sLONG_PTR *pResult, PackagePtr pParams);
