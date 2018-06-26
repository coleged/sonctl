#*************************************************************************\
#                  Copyright (C) Ed Cole 2016.                            *
#				 colege@gmail.com                         *
#                                                                         *
# This program is free software. You may use, modify, and redistribute it *
# under the terms of the GNU General Public License as published by the   *
# Free Software Foundation, either version 3 or (at your option) any      *
# later version. This program is distributed without any warranty.  See   *
# the file COPYING.gpl-v3 for details.                                    *
#                                                                         *
#*************************************************************************/
TARGET = sonctl
INSTALL_DIR = /usr/local/bin/
LIBS =  -L/Users/ecole/src/noson-app/build/backend/lib/noson/noson \
	-lconfig -lz -lcrypto -lssl -lnoson
CC = g++
CFLAGS = -std=c++11 
IFLAGS = -I/Users/ecole/src/noson-app/build/backend/lib/noson/noson/include \
	 -I/Users/ecole/src/noson-app/backend/lib/noson/noson/src/private

.PHONY: default all clean install

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

%.o: %.cpp $(HEADERS) 
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): Makefile $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) $(IFLAGS) $(OBJECTS) \
		$(LIBS) -o $(TARGET)

clean:
	-rm -f *.o
	-rm -f $(TARGET)

install:

