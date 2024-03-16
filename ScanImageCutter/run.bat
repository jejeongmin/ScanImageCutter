del /Q /F ..\image\dstVertical\*  > scanimagecutter.log
del /Q /F ..\image\dstHorizontal\* > scanimagecutter.log
del /Q /F ..\image\dstTrim\* > scanimagecutter.log

"../x64/bin/ScanImageCutterd.exe" "../image/src" "../image/dstVertical" divV > scanimagecutter.log
"../x64/bin/ScanImageCutterd.exe" "../image/dstVertical" "../image/dstHorizontal" divH > scanimagecutter.log
"../x64/bin/ScanImageCutterd.exe" "../image/dstHorizontal" "../image/dstTrim" trim > scanimagecutter.log
