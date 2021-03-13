set PATH=C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\bin;%PATH%
cmd /k "gcc.exe ./COMMON/SRC/*.c ./ILM_ASSEMBLER/SRC/*.c -lmingw32 -Wl,-subsystem,console -o ilm-assembler-bin.exe & exit"

