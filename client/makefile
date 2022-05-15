

CC=$(CROSS)g++
LD=$(CROSS)ld
AR=$(CROSS)ar
PKG_CONFIG=$(CROSS)pkg-config

FILES=Snake.cpp Visual.cpp Server.cpp Client.cpp main.cpp

PKGS=sdl2 SDL2_net SDL2_ttf

CFLAGS=-Wall -Wextra -pedantic -std=c++11 `$(PKG_CONFIG) --cflags $(PKGS)`
CRFLAGS=-Wall -Wextra -pedantic -O2 -std=c++11  `$(PKG_CONFIG) --cflags --static $(PKGS)`
LIBS=`$(PKG_CONFIG) --libs $(PKGS)`

net: $(FILES)
	$(CC) $(CFLAGS) -o net $(FILES) $(LIBS)
netter: $(FILES)
	$(CC) $(CRFLAGS) -o netter $(FILES) $(LIBS)
clean:
	rm -f net netter
