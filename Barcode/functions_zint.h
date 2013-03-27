/*
 *  functions_zint.h
 *  Time
 *
 *  Created by miyako on 2012/10/30.
 *
 */

#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "zint.h"

#if VERSIONWIN
#include <gdiplus.h>
#endif

// --- zint
void BARCODE_Data(sLONG_PTR *pResult, PackagePtr pParams);
void BARCODE_Text(sLONG_PTR *pResult, PackagePtr pParams);
