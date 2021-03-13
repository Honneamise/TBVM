
@ECHO OFF

SET msvc_ver=2019

SET msvc=C:\Program Files (x86)\Microsoft Visual Studio\%msvc_ver%\Community\VC\Auxiliary\Build\vcvars32.bat

SET compile=cl.exe /MTd /W3 /TC COMMON/SRC/*.c ILM_ASSEMBLER/SRC/*.c /link /SUBSYSTEM:CONSOLE /MACHINE:X86 /OUT:ilm-assembler-debug.exe

SET clean=del *.obj

"%msvc%" && cmd /k "%compile% && %clean% && pause && exit"

