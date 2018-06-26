//
//  parser.cpp
//  sonctl
//
//  Created by Ed Cole on 14/06/2018.
//  Copyright © 2018 Ed Cole. All rights reserved.
//

#include "sonctl.h"

bool connected = false;

extern bool debug;
extern SONOS::System * gSonos;

std::string& upstr(std::string& str)
{
    std::string::iterator c = str.begin();
    while (c != str.end())
    {
        *c = toupper(*c);
        ++c;
    }
    return str;
}

bool parseCommand(const std::string& line)
{
    
    std::vector<std::string> tokens;
    tokenize(line, " ", tokens, true);
    std::vector<std::string>::const_iterator it = tokens.begin();
    
    if (it != tokens.end()) //
    {
        std::string token(*it);
        upstr(token);
        //=========================================
        if (token == "EXIT" || token == "QUIT")
            return false;
        else if (token == "") // how would this ever happen?
        {}
        //=========================================
        else if (token == "HELP")
            printHelp();
        //=========================================
        else if (token == "ZONES")
            printZones();
        //=========================================
        else if (token == "CONNECT") // connect <player name>
        {
            if (++it != tokens.end()){
                std::string param(*it); // param = zone to connect to
                while(++it != tokens.end())
                    param.append(" ").append(*it); // handle zone name w/ multi words
                if(!connectZone(param)){
                    ERROR("connectZone(): Failed to connect\n");
                }
            }
            else
                ERROR("Error: Missing arguments.\n");
            if (!gSonos->IsConnected())
                {
                    ERROR("Error: Not connected.\n");
                    connected = false;
                }
        }
        //=========================================
        else if (token == "DISCONNECT")// DISCONNECT <player name>
        {
            if (++it != tokens.end()){
                std::string param(*it); // param = player to disconnect
                while(++it != tokens.end())
                    param.append(" ").append(*it); // handle player name w/ multi words
                if(!standalone(param)){
                    ERROR("standalone(): Failed to disconnect\n");
                }
            }
            else
                ERROR("Error: Missing arguments.\n");
        }
        // ADD
        //=========================================
        else if (token == "ADD" || token == "GROUP"){
            if (gSonos->IsConnected()){
                SONOS::ZonePtr toZone = gSonos->GetConnectedZone();
                printf("Current zone: %s\n",toZone->GetZoneName().c_str());
                if (++it != tokens.end())
                {
                    std::string param(*it);
                    while(++it != tokens.end())
                        param.append(" ").append(*it);
                    SONOS::ZonePlayerList players = gSonos->GetZonePlayerList();
                    for (SONOS::ZonePlayerList::const_iterator iz = players.begin(); iz != players.end(); ++iz)
                    {
                        if (iz->first.c_str() == param)
                        {
                            if (joinPlayer(toZone, iz->second)){
                                if (debug) printf("Connected %s to %s zone \n",
                                                  iz->first.c_str(),
                                                  gSonos->GetConnectedZone()->GetZoneName().c_str());
                            }else
                                ERROR("Failed\n");
                            break;
                        }
                    }//for
                }//if
                else{
                    ERROR("Error: Missing arguments.\n");
                }
            }else{
                ERROR("Error: Not connected.\n");
            }
        }
        //=========================================
        else if (token == "STATUS")
            printStatus();
        //=========================================
        else if (token == "SLEEPTIMER")
        {
            if (++it != tokens.end())
            {
                std::string param(*it);
                uint16_t value = 0;
                string_to_uint16(param.c_str(), &value);
                if (gSonos->GetPlayer()->ConfigureSleepTimer((unsigned)value))
                    ERROR("Succeeded\n");
                else
                    ERROR("Failed\n");
            }
            else
                ERROR("Error: Missing arguments.\n");
        }
        //=========================================
        else if (token == "PLAY")
        {
            if (gSonos->GetPlayer()->Play())
                ERROR("Succeeded\n");
            else
                ERROR("Failed\n");
        }
        //=========================================
        else if (token == "STOP")
        {
            if (gSonos->GetPlayer()->Stop())
                ERROR("Succeeded\n");
            else
                ERROR("Failed\n");
        }
        //=========================================
        else if (token == "UNJOIN" || token == "UNGROUP")
        {
            standaloneAll();
        }
        //=========================================
        else if (token == "PAUSE")
        {
            if (gSonos->GetPlayer()->Pause())
                ERROR("Succeeded\n");
            else
                ERROR("Failed\n");
        }
        //=========================================
        else if (token == "PREVIOUS")
        {
            if (gSonos->GetPlayer()->Previous())
                ERROR("Succeeded\n");
            else
                ERROR("Failed\n");
        }
        //=========================================
        else if (token == "NEXT")
        {
            if (gSonos->GetPlayer()->Next())
                ERROR("Succeeded\n");
            else
                ERROR("Failed\n");
        }
        //=========================================
        else if (token == "SEEK")
        {
            if (++it != tokens.end())
            {
                uint32_t value;
                string_to_uint32(it->c_str(), &value);
                if (gSonos->GetPlayer()->SeekTrack(value))
                    ERROR("Succeeded\n");
                else
                    ERROR("Failed\n");
            }
            else
                ERROR("Error: Missing arguments.\n");
        }
        //=========================================
        else if (token == "PLAYURL")
        {
            if (++it != tokens.end())
            {
                std::string param(*it);
                while(++it != tokens.end())
                    param.append(" ").append(*it);
                if (gSonos->GetPlayer()->PlayStream(param, ""))
                    ERROR("Succeeded\n");
                else
                    ERROR("Failed\n");
            }
            else
                ERROR("Error: Missing arguments.\n");
        }
        //=========================================
        else if (token == "VOLUME") // VOLUME <player>
        {
            if (gSonos->IsConnected()){
                if (++it != tokens.end())
                {
                    bool all = true;
                    std::string param(*it);
                    std::string param2;
                    while(++it != tokens.end())
                    {
                        all = false;
                        if ((it + 1) == tokens.end())
                            param2.append(*it);
                        else
                            param.append(" ").append(*it);
                    }
                    if (all)
                        param2.assign(param);
                    SONOS::ZonePtr pl = gSonos->GetConnectedZone();
                    for (SONOS::Zone::iterator ip = pl->begin(); ip != pl->end(); ++ip)
                    {
                        if (all || (param == **ip))
                        {
                            uint8_t value = 0;
                            string_to_uint8(param2.c_str(), &value);
                            if (gSonos->GetPlayer()->SetVolume((*ip)->GetUUID(), value)){
                                if(debug) ERROR3("%s [%s]: volume %u\n", (*ip)->c_str(), (*ip)->GetUUID().c_str(), value);
                            }else{
                                ERROR("Failed\n");
                                }
                        }
                        
                    }
                }
                else{
                    ERROR("Error: Missing arguments.\n");
                }
                
            }else{
                ERROR("Error: Not connected to play zone.\n");
            }
            
        }
        //=========================================
        else if (token == "SHOWQUEUE")
        {
            SONOS::ContentDirectory mycontent(gSonos->GetPlayer()->GetHost(), gSonos->GetPlayer()->GetPort());
            SONOS::ContentList bdir(mycontent, "Q:0");
            PRINT1("UpdateID  : %u\n", bdir.GetUpdateID());
            PRINT1("Item count: %u\n", bdir.size());
            SONOS::ContentList::iterator ic = bdir.begin();
            int i = 0;
            while (ic != bdir.end())
            {
                PRINT3("%d: [%s] [%s]\n", ++i, (*ic)->GetValue("dc:title").c_str(), (*ic)->GetValue("res").c_str());
                ++ic;
            }
        }//=========================================
        else if (token == "PLAYQUEUE")
        {
            if (gSonos->GetPlayer()->PlayQueue(true))
                ERROR("Succeeded\n");
            else
                ERROR("Failed\n");
        }
        //=========================================
        else if (token == "PLAYLINEIN")
        {
            if (gSonos->GetPlayer()->PlayLineIN())
                ERROR("Succeeded\n");
            else
                ERROR("Failed\n");
        }
        //=========================================
        else if (token == "PLAYDIGITALIN")
        {
            if (gSonos->GetPlayer()->PlayDigitalIN())
                ERROR("Succeeded\n");
            else
                ERROR("Failed\n");
        }
        //=========================================
        else if (token == "SHOWFV")
        {
            if (gSonos->IsConnected()){
                SONOS::ContentDirectory mycontent(gSonos->GetPlayer()->GetHost(), gSonos->GetPlayer()->GetPort());
                SONOS::ContentList bdir(mycontent, "FV:2");
                PRINT1("UpdateID  : %u\n", bdir.GetUpdateID());
                PRINT1("Item count: %u\n", bdir.size());
                SONOS::ContentList::iterator ic = bdir.begin();
                int i = 0;
                while (ic != bdir.end())
                {
                    PRINT3("%d: [%s] [%s]\n", ++i, (*ic)->GetValue("dc:title").c_str(), (*ic)->GetValue("res").c_str());
                    ++ic;
                }
            }else{
                ERROR("Not connected\n");
            }
        }
        //=========================================
        else if (token == "PLAYSQ")
        {
            if (++it != tokens.end())
            {
                SONOS::ContentDirectory mycontent(gSonos->GetPlayer()->GetHost(), gSonos->GetPlayer()->GetPort());
                SONOS::ContentList bdir(mycontent, "SQ:");
                SONOS::ContentList::iterator ic = bdir.begin();
                while (ic != bdir.end())
                {
                    if ((*ic)->GetValue("res") == (*it))
                    {
                        SONOS::PlayerPtr player = gSonos->GetPlayer();
                        PRINT2("Playing item [%s] [%s]\n", (*ic)->GetValue("dc:title").c_str(), (*ic)->GetValue("res").c_str());
                        if (player->RemoveAllTracksFromQueue() && player->PlayQueue(false) && player->AddURIToQueue(*ic, 1) && player->SeekTrack(1) && player->Play())
                            ERROR("Succeeded\n");
                        else
                            ERROR("Failed\n");
                    }
                    ++ic;
                }
            }
            else
                ERROR("Error: Missing arguments.\n");
        }
        //=========================================
        else if (token == "PLAYFV")
        {
            if (++it != tokens.end())
            {
                SONOS::ContentDirectory mycontent(gSonos->GetPlayer()->GetHost(), gSonos->GetPlayer()->GetPort());
                SONOS::ContentList bdir(mycontent, "FV:2");
                SONOS::ContentList::iterator ic = bdir.begin();
                while (ic != bdir.end())
                {
                    if ((*ic)->GetValue("res") == (*it))
                    {
                        SONOS::DigitalItemPtr item;
                        if (SONOS::System::ExtractObjectFromFavorite((*ic), item))
                        {
                            SONOS::PlayerPtr player = gSonos->GetPlayer();
                            if (SONOS::System::CanQueueItem(item))
                            {
                                PRINT2("Playing item [%s] [%s]\n", (*ic)->GetValue("dc:title").c_str(), (*ic)->GetValue("res").c_str());
                                if (player->RemoveAllTracksFromQueue() && player->PlayQueue(false) && player->AddURIToQueue(item, 1) && player->SeekTrack(1) && player->Play())
                                    ERROR("Succeeded\n");
                                else
                                    ERROR("Failed\n");
                            }
                            else if (player->SetCurrentURI(item) && player->Play())
                                ERROR("Succeeded\n");
                            else
                                ERROR("Failed\n");
                        }
                        else
                            ERROR("Failed\n");
                    }
                    ++ic;
                }
            }
            else
                ERROR("Error: Missing arguments.\n");
        }
        //=========================================
        else if (token == "SHOWSQ")
        {
            SONOS::ContentDirectory mycontent(gSonos->GetPlayer()->GetHost(), gSonos->GetPlayer()->GetPort());
            SONOS::ContentList bdir(mycontent, "SQ:");
            PRINT1("UpdateID  : %u\n", bdir.GetUpdateID());
            PRINT1("Item count: %u\n", bdir.size());
            SONOS::ContentList::iterator ic = bdir.begin();
            int i = 0;
            while (ic != bdir.end())
            {
                PRINT3("%d: [%s] [%s]\n", ++i, (*ic)->GetValue("dc:title").c_str(), (*ic)->GetValue("res").c_str());
                ++ic;
            }
        }
        //=========================================
        else if (token == "PLAYDIGITALIN")
        {
            if (gSonos->GetPlayer()->PlayDigitalIN())
                ERROR("Succeeded\n");
            else
                ERROR("Failed\n");
        }
        else
        {
            ERROR("Error: Command invalid.\n");
        }
        
        
    }
    return true;
}

void printStatus(){
    while (gSonos->GetPlayer()->TransportPropertyEmpty())
        sleep(1);
    SONOS::AVTProperty props = gSonos->GetPlayer()->GetTransportProperty();
    PRINT1("TransportStatus = %s\n", props.TransportStatus.c_str());
    PRINT1("TransportState = %s\n", props.TransportState.c_str());
    PRINT1("AVTransportURI = [%s]\n", props.AVTransportURI.c_str());
    PRINT1("AVTransportTitle = [%s]\n", props.AVTransportURIMetaData ? props.AVTransportURIMetaData->GetValue("dc:title").c_str() : "null");
    PRINT1("CurrentTrack = %d\n", props.CurrentTrack);
    PRINT1("CurrentTrackDuration = %s\n", props.CurrentTrackDuration.c_str());
    PRINT1("CurrentTrackURI = [%s]\n", props.CurrentTrackURI.c_str());
    PRINT1("CurrentTrackTitle = [%s]\n", props.CurrentTrackMetaData ? props.CurrentTrackMetaData->GetValue("dc:title").c_str() : "null");
    PRINT1("CurrentCrossfadeMode = %s\n", props.CurrentCrossfadeMode.c_str());
    PRINT1("CurrentPlayMode = %s\n", props.CurrentPlayMode.c_str());
    PRINT1("CurrentTransportActions = %s\n", props.CurrentTransportActions.c_str());
    PRINT1("NumberOfTracks = %d\n", props.NumberOfTracks);
    SONOS::ElementList vars;
    if (gSonos->GetPlayer()->GetRemainingSleepTimerDuration(vars))
    {
        PRINT1("RemainingSleepTimerDuration = %s\n", vars.GetValue("RemainingSleepTimerDuration").c_str());
    }
}
