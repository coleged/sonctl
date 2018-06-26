//
//  sonctl.h
//  sonctl
//
//  Created by Ed Cole on 14/06/2018.
//  Copyright © 2018 Ed Cole. All rights reserved.
//

#ifndef sonctl_h
#define sonctl_h

#define _DEBUG true

#if (defined(_WIN32) || defined(_WIN64))
#define __WINDOWS__
#endif

#ifdef __WINDOWS__
#include <winsock2.h>
#include <Windows.h>
#include <time.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

#include "../../noson-app/backend/lib/noson/noson/src/private/debug.h"
#include "../../noson-app/backend/lib/noson/noson/src/sonossystem.h"
#include "../../noson-app/backend/lib/noson/noson/src/contentdirectory.h"
#include "../../noson-app/backend/lib/noson/noson/src/didlparser.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

#include "../../noson-app/backend/lib/noson/noson/src/private/tokenizer.h"
#include "../../noson-app/backend/lib/noson/noson/src/private/builtin.h"

#define PRINT(a) fprintf(stdout, a)
#define PRINT1(a,b) fprintf(stdout, a, b)
#define PRINT2(a,b,c) fprintf(stdout, a, b, c)
#define PRINT3(a,b,c,d) fprintf(stdout, a, b, c, d)
#define PRINT4(a,b,c,d,e) fprintf(stdout, a, b, c, d, e)
#define ERROR(a) fprintf(stderr, a)
#define ERROR1(a,b) fprintf(stderr, a, b)
#define ERROR2(a,b,c) fprintf(stderr, a, b, c)
#define ERROR3(a,b,c,d) fprintf(stderr, a, b, c, d)

void readStream(std::istream*);
void printUsage();
void printHelp();
void printZones();
void printStatus();
bool parseCommand(const std::string&);
void handleEventCB(void*);
bool standaloneAll();
bool joinPlayer(SONOS::ZonePtr, SONOS::ZonePlayerPtr);
bool connectZone(std::string);
bool standalone(std::string);

#endif /* sonctl_h */
