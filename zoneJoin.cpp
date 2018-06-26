//
//  zoneJoin.cpp
//  sonctl
//
//  Created by Ed Cole on 14/06/2018.
//  Copyright © 2018 Ed Cole. All rights reserved.
//

#include "sonctl.h"

extern bool debug;
extern SONOS::System * gSonos;
extern bool connected;

void printZones(){
    SONOS::ZoneList zones = gSonos->GetZoneList();
    
    for (SONOS::ZoneList::const_iterator it = zones.begin(); it != zones.end(); ++it){
        PRINT3("Found zone %s '%s' with coordinator '%s'\n",
               it->first.c_str(),
               it->second->GetZoneName().c_str(),
               it->second->GetCoordinator()->c_str());
    }//for
}

bool standaloneAll(){
    SONOS::ZonePlayerList players = gSonos->GetZonePlayerList();
    for (SONOS::ZonePlayerList::const_iterator iz = players.begin();
         iz != players.end();
         ++iz){
        if(debug) printf("disconnecting %s\n",iz->first.c_str());
        SONOS::Player player(iz->second);
        player.BecomeStandalone();
    }//for
    return EXIT_SUCCESS;
}

bool standalone(std::string pname){
    SONOS::ZonePlayerList players = gSonos->GetZonePlayerList();
    for (SONOS::ZonePlayerList::const_iterator iz = players.begin();
         iz != players.end();
         ++iz){
        if (iz->first == pname){
            if(debug) printf("disconnecting %s\n",iz->first.c_str());
            SONOS::Player player(iz->second);
            player.BecomeStandalone();
            return true;
        }
    }//for
    return false;
}


bool joinPlayer(SONOS::ZonePtr zoneTo, SONOS::ZonePlayerPtr newPlayer){
    // joins newPlayer to zoneTo
    if (zoneTo && zoneTo->GetCoordinator() && newPlayer && newPlayer->IsValid())
    {
        SONOS::Player player(newPlayer);
        return player.JoinToGroup(zoneTo->GetCoordinator()->GetUUID());
    }
    else
        return false;
    
}

bool connectZone(std::string zone){
    std::string zoneStr;
    
    if (debug) ERROR1("Attempting connection to %s\n",zone.c_str());
    SONOS::ZoneList zones = gSonos->GetZoneList();
    for (SONOS::ZoneList::const_iterator iz = zones.begin(); iz != zones.end(); ++iz) // scan thru zones
    {
        zoneStr = iz->second->GetZoneName();
        if(zoneStr.find(zone,0) != std::string::npos)
            // look for zone name which might be substring of a grouped
            // zone. ISSUE. "Study" matches "William's Study" before "Study"
        {
            if (gSonos->ConnectZone(iz->second, 0, handleEventCB)){ // connect
                ERROR1("Connected to zone %s\n", gSonos->GetConnectedZone()->GetZoneName().c_str());
                connected=true;
                return true;
            }else
                ERROR("Failed to connect\n");
            break; // for
        }
    }//for
    return false;
}

