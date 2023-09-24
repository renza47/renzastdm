#include <YSI_Coding\y_hooks>

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(PRESSED(KEY_SPRINT))
    {
        if(PlayerInfo[playerid][BrokenLeg] == true && PlayerInfo[playerid][State] == PLAYER_STATE_ALIVE)
        {
            if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
            {
                ApplyAnimation(playerid, "PED", "FALL_COLLAPSE", 4.1, 0, 1, 1, 0, 0, 1);   
                ApplyAnimation(playerid, "PED", "FALL_COLLAPSE", 4.1, 0, 1, 1, 0, 0, 1);   
            }
        }
    }
    if(PRESSED(KEY_JUMP))
    {
        if(PlayerInfo[playerid][BrokenLeg] == true && PlayerInfo[playerid][State] == PLAYER_STATE_ALIVE)
        {
            if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
            {
                ApplyAnimation(playerid, "GYMNASIUM", "gym_jog_falloff",4.1,0,1,1,0,0);
				ApplyAnimation(playerid, "GYMNASIUM", "gym_jog_falloff",4.1,0,1,1,0,0);
            }
        }
    }
    return 1;
}

public OnPlayerShootDynamicObject(playerid, weaponid, objectid, Float:x, Float:y, Float:z)
{
    return CallLocalFunction("OnPlayerWeaponShot", "iiiifff", playerid, weaponid, BULLET_HIT_TYPE_OBJECT, objectid, x, y, z);
}

hook OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    if(IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_PASSENGER)
    {
        if(GetVehicleDriver(GetPlayerVehicleID(playerid)) == INVALID_PLAYER_ID)
        {
            RemovePlayerFromVehicle(playerid);
            GameTextForPlayer(playerid, "~r~No drive by w/o driver!", 2000, 6);
        }
    }

    if(hittype == BULLET_HIT_TYPE_VEHICLE)
    {
        if(GetPlayerGameMode(playerid) == GAME_MODE_COPCHASE)
        {
            if(PlayerInfo[playerid][CC_Team] == TEAM_LSPD && PlayerInfo[GetVehicleDriver(hitid)][CC_Team] == TEAM_FUGITIVE)
            {
                if(!GetPlayerWantedLevel(GetVehicleDriver(hitid)))
                {
                    new Float:vehiclehp, panels, doors, lights, tires;
                    GetVehicleHealth(hitid, vehiclehp);
                    GetVehicleDamageStatus(hitid, panels, doors, lights, tires);

                    SetVehicleHealth(hitid, vehiclehp);
                    UpdateVehicleDamageStatus(hitid, panels, doors, lights, tires);

                    GameTextForPlayer(playerid, "~r~THAT PLAYER HASN'T SHOT YET!", 2000, 6);
                    return 0;
                }
            }
            if(PlayerInfo[playerid][CC_Team] == PlayerInfo[GetVehicleDriver(hitid)][CC_Team])
            {
                new Float:vehiclehp, panels, doors, lights, tires;
                GetVehicleHealth(hitid, vehiclehp);
                GetVehicleDamageStatus(hitid, panels, doors, lights, tires);

                SetVehicleHealth(hitid, vehiclehp);
                UpdateVehicleDamageStatus(hitid, panels, doors, lights, tires);

                GameTextForPlayer(playerid, "~r~Do not team killing!", 2000, 6);
                return 0;
            }
        }
    }
    
    if(GetPlayerGameMode(playerid) == GAME_MODE_TDM)
    {
        
        if(PlayerInfo[playerid][Team] != TEAM_LSPD)
        {
            foreach(new i : Player)
            {
                if(PlayerInfo[i][Team] == TEAM_LSPD)
                {
                    if(IsPlayerNearPlayer(playerid, i, 35.0))
                    {
                        if(!GetPlayerWantedLevel(playerid))
                        {
                            SetPlayerWantedLevel(playerid, 3);
                            GameTextForPlayer(playerid, "~r~~n~~n~~n~~n~~n~~n~~n~~n~~n~WANTED LEVEL INCREASED!", 1500, 3);
                            //SendPoliceMessage(COLOR_RADIO, "** [DISPATCH] Shots fired at %s.", ReturnLocation(playerid));
                        }
                    }
                }
            }
        }
    }
    else if(GetPlayerGameMode(playerid) == GAME_MODE_COPCHASE)
    {
        if(GetPlayerWeapon(playerid) == 23 && PlayerInfo[playerid][Taser] == true) ApplyAnimation(playerid, "COLT45", "colt45_reload", 4.1, 0, 0, 0, 0, 0);
        
        if(PlayerInfo[playerid][CC_Team] != TEAM_LSPD)
        {
            if(hittype == BULLET_HIT_TYPE_VEHICLE || hittype == BULLET_HIT_TYPE_NONE)
            {
                foreach(new i : Player)
                {
                    if(PlayerInfo[i][CC_Team] == TEAM_LSPD)
                    {
                        if(IsPlayerNearPlayer(playerid, i, 35.0) || GetVehicleDriver(hitid) == i)
                        {
                            if(!GetPlayerWantedLevel(playerid))
                            {
                                SetPlayerWantedLevel(playerid, 3);
                                GameTextForPlayer(playerid, "~r~~n~~n~~n~~n~~n~~n~~n~~n~~n~WANTED LEVEL INCREASED!", 1500, 3);
                                foreach(new id : Player)
                                {
                                    if(GetPlayerGameMode(id) == GAME_MODE_COPCHASE && PlayerInfo[id][CC_Team] == TEAM_LSPD) SendClientMessageEx(id, COLOR_RADIO, "** [DISPATCH] Shots fired at %s.", ReturnLocation(playerid));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}