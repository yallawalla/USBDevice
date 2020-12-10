del   ..\bin\*.bin, ..\bin\*.hex
copy  object\*.hex  ..\bin\*.hex
srec_cat.exe ..\bin\USBDevice.hex -Intel -crop 0x8000000 -offset -0x8000000 -o ..\bin\USBDevice.bin -Binary
