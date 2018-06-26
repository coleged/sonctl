/*        -Copyright notice from parent fork-
 *
 *      Copyright (C) 2018 Jean-Luc Barriere
 *
 *  This file is part of Noson-App
 *
 *  Noson-App is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Noson is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


//
//  main.cpp
//  sonctl
//
//  Adapted from noson-cli by Ed Cole on 14/06/2018.
//  Copyright © 2018 Ed Cole. All rights reserved.
//



#include "sonctl.h"

char version[] = "1.0.1";

SONOS::System * gSonos = 0;
bool debug = _DEBUG;
bool pflag,iflag,zflag=false;

/*
 * the main function
 */
int main(int argc, char** argv)
{
    int opt;
    std::string arg;
    int ret = 0;
#ifdef __WINDOWS__
    //Initialize Winsock
    WSADATA wsaData;
    if ((ret = WSAStartup(MAKEWORD(2, 2), &wsaData)))
        return ret;
#endif /* __WINDOWS__ */
    
    //Parse command line options
    while ((opt = getopt(argc, (char **)argv, "Dhtpizc:")) != -1){
        switch (opt){
            case 'D': // run in debug mode.
                debug=true;
                PRINT("Debug mode set\n");
                break;
                
            case 'c': // Connected Zone
                arg.assign(optarg);
                std::cout << arg << std::endl;
                break;
                
            case 't': // test mode
                debug = true;
                PRINT("test mode\n");
                break;
                
            case 'p': // players
                pflag = true;
                PRINT("test mode\n");
                break;
                
            case 'z': // zones
                zflag = true;
                PRINT("test mode\n");
                break;
                
            case 'i': // interactive
                iflag = true;
                PRINT("test mode\n");
                break;
                
            case 'h': // help
                printHelp();
                exit(EXIT_SUCCESS);
                break;
                
            default:
                printUsage();
                exit(EXIT_FAILURE);
                break;
                
        }//switch
    }//while opt parse
    
    if(debug) printf("sonctl (%s) using libnoson %s, Copyright (C) 2018 Jean-Luc Barriere /w Ed Cole\n",version, LIBVERSION);
    
    gSonos = new SONOS::System(0, handleEventCB);
    if(debug) ERROR("Searching... ");
    if (!gSonos->Discover())
    {
        ERROR("No SONOS zone found.\n");
        return EXIT_FAILURE;
    }
    if(debug) ERROR("Succeeded\n");
    /*
     * Print Players list
     */
    SONOS::ZonePlayerList players = gSonos->GetZonePlayerList();
    if(pflag){
        for (SONOS::ZonePlayerList::const_iterator it = players.begin(); it != players.end(); ++it){
        PRINT2("Found player '%s' with UUID '%s'\n", it->first.c_str(), it->second->GetUUID().c_str());
        }//for
    }//if
    SONOS::ZoneList zones = gSonos->GetZoneList();
    if ( zflag ){
        printZones();
    }//if
    
    if (iflag ){
        readStream(&std::cin);
    }//if
    delete gSonos;
    gSonos = 0;
    
#ifdef __WINDOWS__
    WSACleanup();
#endif /* __WINDOWS__ */
    return ret;
}


void readStream(std::istream *file)
{
    while (*file)
    {
        std::string sline;
        if (gSonos->IsConnected())
            PRINT1("%s >>> ", gSonos->GetConnectedZone()->GetZoneName().c_str());
        else
            PRINT(">>> ");
        getline(*file, sline);
        if (!parseCommand(sline))
            break;
    }
}

