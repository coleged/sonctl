//
//  errors.cpp
//  sonctl
//
//  Created by Ed Cole on 14/06/2018.
//  Copyright © 2018 Ed Cole. All rights reserved.
//

#include "sonctl.h"

void printUsage(){
    PRINT("usage: sonctl [-Dtpzih]\n");
    PRINT("          -D   Debug mode set\n");
    PRINT("          -t   test mode set\n");
    PRINT("          -p   show players found\n");
    PRINT("          -z   show zones found\n");
    PRINT("          -i   interactive mode\n");
    PRINT("          -h   help page for interactive commands\n");
}

void printHelp(){

    PRINT("EXIT/QUIT                   Exit\n");
    PRINT("CONNECT {zone name}         Connect to a zone for control\n");
    PRINT("ADD/GROUP {zone name}       Group to a zone for control\n");
    PRINT("UNJOIN/UNGROUP              Ungroup all zones\n");
    PRINT("STATUS                      Show the playing status\n");
    PRINT("ZONES                       Show zones\n");
    PRINT("PLAYURL {stream URL}        Play stream from URL\n");
    PRINT("PLAYFV {URL}                Play the given favorite\n");
    PRINT("PLAYSQ {URL}                Play the given playlist\n");
    PRINT("PLAYQUEUE                   Play queue\n");
    PRINT("PLAYLINEIN                  Play line-IN\n");
    PRINT("PLAYDIGITALIN               Play digital-IN/TV\n");
    PRINT("PLAY                        Press play\n");
    PRINT("PAUSE                       Press pause\n");
    PRINT("STOP                        Press stop\n");
    PRINT("PREVIOUS                    Press skip previous\n");
    PRINT("NEXT                        Press skip next\n");
    PRINT("SEEK 1..                    Seek to track number\n");
    PRINT("VOLUME 0..100               Set volume master\n");
    PRINT("VOLUME {player} 0..100      Set volume\n");
    PRINT("SLEEPTIMER 0..65535         Set sleep timer\n");
    PRINT("SHOWQUEUE                   Show queue content\n");
    PRINT("SHOWFV                      Show favorites\n");
    PRINT("SHOWSQ                      Show playlists\n");
    PRINT("HELP                        Print this help\n");
    PRINT("\n");
}
