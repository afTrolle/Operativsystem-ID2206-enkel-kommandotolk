# Operativsystem-ID2206-enkel-kommandotolk

complied with following commands:

Signal method:
gcc InputHandler.c InputHandler.h Main.h Main.c signalHandler.c signalHandler.h -o a.out -D SIGNALDETECTION=1 -Wall

Pooling method:
gcc InputHandler.c InputHandler.h Main.h Main.c signalHandler.c signalHandler.h -o a.out -Wall

start program by:
./a.out

known issue:
To launch a background process it is required to have a space between program and &:
Example: "firefox &" -correct way , "firefox&"-wrong
"gedit test.txt &"-correct, "gedit test.txt&"-wrong 
