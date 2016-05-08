4d-plugin-barcode
=================

libqrencode, libzbar implementation for 4D

***Deprecated***

Please use 

https://github.com/miyako/4d-plugin-zint

https://github.com/miyako/4d-plugin-qrencode

https://github.com/miyako/4d-plugin-data-matrix

https://github.com/miyako/4d-plugin-zbar

##Platform

| carbon | cocoa | win32 | win64 |
|:------:|:-----:|:---------:|:---------:|
|🆗|🚫|🆗|🆗|

Commands
---

```c
// --- data matrix
DMTX_Read_image
DMTX_Text
DMTX_Data

// --- pdf417
PDF417_Data
PDF417_Text

// --- qrencode
MICRO_QRCODE_Data
MICRO_QRCODE_Text
QRCODE_Data
QRCODE_Text

// --- zbar
BARCODE_Read_image

// --- zint
BARCODE_Data
BARCODE_Text
```
