/*
 *  functions_dtmx.h
 *  Time
 *
 *  Created by miyako on 2012/10/30.
 *
 */

#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "dmtx.h"

// --- data matrix
void DMTX_Read_image(sLONG_PTR *pResult, PackagePtr pParams);
void DMTX_Text(sLONG_PTR *pResult, PackagePtr pParams);
void DMTX_Data(sLONG_PTR *pResult, PackagePtr pParams);
