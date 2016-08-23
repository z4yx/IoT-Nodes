@echo off
ESP8266\esptool --port COM11 write_flash 0x0 dump_with_lua_512K.bin
pause