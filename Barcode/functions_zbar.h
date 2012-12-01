/*
 *  functions_zbar.h
 *  Time
 *
 *  Created by miyako on 2012/10/30.
 *
 */

#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "zbar.h"

#if VERSIONWIN
#include <gdiplus.h>
#endif

// --- zbar
void BARCODE_Read_image(sLONG_PTR *pResult, PackagePtr pParams);
