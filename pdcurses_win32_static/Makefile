CC:=i586-mingw32msvc-gcc
AR:=i586-mingw32msvc-ar
OBJCOPY:=i586-mingw32msvc-objcopy
STRIP:=i586-mingw32msvc-strip
LIB:=libpdcurses
SRC=$(wildcard pdcurses/*.c win32/*.c)

CFLAGS:=-O0 -gdwarf-2 -g3 -W -Wall -I.

SHLIB_CFLAGS:=-static

LDFLAGS:=

OBJ=$(SRC:%.c=%.o)

%.o: %.c 
#	i586-mingw32msvc-windres my.rc -O coff -o my.res
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB).a: $(OBJ)	
	$(AR) rcs $@ $^
#	$(CC) $(SHLIB_CFLAGS) -o $@ $^ $(LDFLAGS)
#	$(OBJCOPY) --only-keep-debug $(LIB).dll $(LIB).dwarf
#	$(STRIP) -s $(LIB).dll	
#	$(OBJCOPY) --add-gnu-debuglink=$(LIB).dwarf $(LIB).dll
#	upx -q9 $(LIB).dll	

clean:
	find -type f -regex ".*/.*\.\(o\|res\|a\|dll\|lib\|def\|dwarf\|unc-backup~\|md5~\\)" -delete

