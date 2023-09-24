#include <YSI_Coding\y_hooks>

forward CopchaseUpdate();
public CopchaseUpdate()
{
    new string[128];
    if(CopchaseInfo[CC_Started] == true)
    {
        if(CopchaseInfo[CC_TraceDelay])
        {
            CopchaseInfo[CC_TraceDelay]--;
            if(CopchaseInfo[CC_TraceDelay] <= 1)
            {
                CopchaseInfo[CC_TraceDelay] = 0;
            }
        }
        foreach(new i : Player)
        {
            if(IsPlayerConnected(i) && GetPlayerGameMode(i) == GAME_MODE_COPCHASE)
            {
                format(string, sizeof string, "TIME      %02d:%02d", CopchaseInfo[CC_Time] / 60, CopchaseInfo[CC_Time] % 60);
                PlayerTextDrawSetString(i, ui_ProgressInfo[i], string);
            }
        }
        if(CopchaseInfo[CC_Time] == 380 || CopchaseInfo[CC_Time] == 360 || CopchaseInfo[CC_Time] == 300 || CopchaseInfo[CC_Time] == 240 || CopchaseInfo[CC_Time] == 180 || CopchaseInfo[CC_Time] == 60 || CopchaseInfo[CC_Time] == 30)
        {
            foreach(new id : Player)
            {
                if(PlayerInfo[GetRandomFugitive()][Property] != -1)
                {
                    if(GetPlayerGameMode(id) == GAME_MODE_COPCHASE && PlayerInfo[id][CC_Team] == TEAM_LSPD) SendClientMessageEx(id, COLOR_RADIO, "** [CH: 911 S: 1] Suspect last seen at %s.", PropertyInfo[PlayerInfo[GetRandomFugitive()][Property]][PropName]);
                }
                else if(GetPlayerGameMode(id) == GAME_MODE_COPCHASE && PlayerInfo[id][CC_Team] == TEAM_LSPD) SendClientMessageEx(id, COLOR_RADIO, "** [CH: 911 S: 1] Suspect last seen at %s.", ReturnLocation(GetRandomFugitive()));
            }
        }
        if(CopchaseInfo[CC_Time] == 179)
        {
            SendGameMessage(GAME_MODE_COPCHASE, COLOR_RED, "[ ! ] {FFFFFF}3 minutes left, all criminal has the shot tag!");
            foreach(new id : Player)
            {
                if(GetPlayerGameMode(id) == GAME_MODE_COPCHASE && PlayerInfo[id][CC_Team] == TEAM_FUGITIVE) 
                {
                    SetPlayerWantedLevel(id, 3);
                }
            }
        }
        if(CopchaseInfo[CC_InteriorDelay] && CopchaseInfo[CC_InteriorDelay] < 100 && CopchaseInfo[CC_EnableInterior] == true) CopchaseInfo[CC_InteriorDelay]--;
        else if(!CopchaseInfo[CC_InteriorDelay] && CopchaseInfo[CC_EnableInterior] == true)
        {
            CopchaseInfo[CC_InteriorDelay] = 105;
            foreach(new id : Player)
            {
                if(GetPlayerGameMode(id) == GAME_MODE_COPCHASE) SendClientMessageEx(id, COLOR_RED, "[ ! ]{FFFFFF} The cops can now use /enter!");
            }
        }
        if(CopchaseInfo[CC_Time])
        {
            CopchaseInfo[CC_Time]--;
        }
        else if(!CopchaseInfo[CC_Time])
        {
            EndCopchase();
            // Set the fugitives as the winner
            return 1;
        }

        if(!GetFugitivesCount() || !GetCopsCount())
        {
            EndCopchase();
            return 1;
        }
    }
    return 1;
}

forward CopchasePlayerCheck();
public CopchasePlayerCheck()
{
    if(CopchaseInfo[CC_Started] == true || CopchaseInfo[CC_Starting] == true) return 1;

    if(GetLobbyPlayersCount() < 3)
    {
        foreach(new i : Player)
        {
            if(IsPlayerConnected(i))
            {
                if(GetPlayerGameMode(i) == GAME_MODE_LOBBY && PlayerInfo[i][IsLoggedIn] == true && IsPlayerInCopchaseLobby(i))
                {
                    SendClientMessageEx(i, 0x7289daFF, "COPCHASE:{FFFFFF} %d more %s required to start the copchase!", 3 - GetLobbyPlayersCount(), (3 - GetLobbyPlayersCount() == 1) ? ("player is") : ("players are"));
                }
            }
        }
    }
    else
    {
        CopchaseInfo[CC_Starting] = true;
        CopchaseInfo[CC_StartingTime] = 10;
        SendClientMessageToAll(COLOR_RED, "[ ! ] {FFFFFF}The copchase is about to start. Use /copchase to join!");
    }
    return 1;
}

forward CopchaseStarting();
public CopchaseStarting()
{
    new string[64];
    if(CopchaseInfo[CC_Started] == true) return 1;

    if(CopchaseInfo[CC_Starting] == true)
    {
        foreach(new i : Player)
        {
            if(IsPlayerConnected(i) && PlayerInfo[i][IsLoggedIn] == true && GetPlayerGameMode(i) == GAME_MODE_LOBBY && IsPlayerInCopchaseLobby(i))
            {
                format(string, sizeof string, "~w~STARTING IN~n~%d", CopchaseInfo[CC_StartingTime]);
		        GameTextForPlayer(i, string, 2500, 3);
            }
        }
        if(CopchaseInfo[CC_StartingTime]) CopchaseInfo[CC_StartingTime]--;
        if(GetLobbyPlayersCount() < 3)
        {
            CopchaseInfo[CC_Starting] = false;
            SendGameMessage(GAME_MODE_LOBBY, 0x7289daFF, "COPCHASE:{FFFFFF} %d more %s required to start the copchase!", 3 - GetLobbyPlayersCount(), (3 - GetLobbyPlayersCount() == 1) ? ("player is") : ("players are"));
            return 1;
        }
        else
        {
            if(!CopchaseInfo[CC_StartingTime])
            {
                CopchaseInfo[CC_Starting] = false;
                StartCopchase();
            }
            return 1;
        }
    }
    return 1;
}

forward CopchaseCollision();
public CopchaseCollision()
{
    if(CopchaseInfo[CC_Started] == false) return 1;

    foreach(new i : Player)
    {
        if(IsPlayerConnected(i))
        {
            if(PlayerInfo[i][IsLoggedIn] == true && GetPlayerGameMode(i) == GAME_MODE_COPCHASE)
            {
                DisableRemoteVehicleCollisions(i, 0);
                switch(PlayerInfo[i][CC_Team])
                {
                    case TEAM_LSPD:
                    {
                        GivePlayerWeapon(i, 31, 250);
                        GivePlayerWeapon(i, 24, 45);
                        for(new wep; wep < 12; wep++)
                        {
                            GetPlayerWeaponData(i, wep, PlayerInfo[i][CC_Weapon][wep], PlayerInfo[i][CC_WeaponAmmo][wep]);
                        }
                        if(PlayerHasPerk(i, 3)) GivePlayerWeapon(i, 33, 4);
                    }
                    case TEAM_FUGITIVE:
                    {
                        GivePlayerWeapon(i, 30, 200);
                        GivePlayerWeapon(i, 24, 45);
                        for(new wep; wep < 12; wep++)
                        {
                            GetPlayerWeaponData(i, wep, PlayerInfo[i][CC_Weapon][wep], PlayerInfo[i][CC_WeaponAmmo][wep]);
                        }
                        if(PlayerHasPerk(i, 3)) GivePlayerWeapon(i, 33, 4);
                    }
                }
            }
        }
    }
    CopchaseInfo[CC_WeaponEnabled] = true;
    SendGameMessage(GAME_MODE_COPCHASE, COLOR_GREEN, "Vehicle collision has been enabled.", CopchaseInfo[CC_Time]);
    SendGameMessage(GAME_MODE_COPCHASE, COLOR_RED, "> {FFFFFF}Weapon deployed!", CopchaseInfo[CC_Time]);
    return 1;
}

hook OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	new Float:hp, Float:ar;
    GetPlayerHP(damagedid, hp);
    GetPlayerArmor(damagedid, ar);
    if(GetPlayerGameMode(playerid) == GAME_MODE_COPCHASE)
    {
        if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE)
        {
            amount = 0;
            SetPlayerArmor(damagedid, ar);
            return SetPlayerHP(damagedid, hp);
        }
        
        if(PlayerInfo[playerid][CC_Team] == PlayerInfo[damagedid][CC_Team])
        {
            amount = 0;
            GameTextForPlayer(playerid, "~r~NO TEAM KILL", 2000, 6);
            SetPlayerArmor(damagedid, ar);
            return SetPlayerHP(damagedid, hp);
        }

        if(gettime() - PlayerInfo[damagedid][RecentlySpawned] < 3)
        {
            GameTextForPlayer(playerid, "~r~NO SPAWN KILL", 2000, 6);
            amount = 0;
            SetPlayerArmor(damagedid, ar);
            return SetPlayerHP(damagedid, hp);
        }

        if(PlayerInfo[playerid][CC_Team] == TEAM_LSPD && !GetPlayerWantedLevel(damagedid) && PlayerInfo[playerid][Taser] == false)
        {
            GameTextForPlayer(playerid, "~r~THAT PLAYER HASN'T SHOT YET!", 2000, 6);
            amount = 0;
            SetPlayerArmor(damagedid, ar);
            return SetPlayerHP(damagedid, hp);
        }

        if(weaponid == WEAPON_VEHICLE)
        {
            amount = 0;
            new Float:RamPosX, Float:RamPosY, Float:RamPosZ;
            GameTextForPlayer(playerid, "~r~Do not ramming!", 2000, 6);
            GetPlayerPos(playerid, RamPosX, RamPosY, RamPosZ);
            SetVehiclePos(GetPlayerVehicleID(playerid), RamPosX, RamPosY, RamPosZ);
            SetPlayerArmor(damagedid, ar);
            return SetPlayerHP(damagedid, hp);
        }

        if(PlayerInfo[damagedid][AdminDuty] == true)
        {
            amount = 0;
            SetPlayerArmor(damagedid, ar);
            return SetPlayerHP(damagedid, hp);
        }

        if(PlayerInfo[damagedid][DamageFix]) 
        {
            amount = 0;
            SetPlayerArmor(damagedid, ar);
            return SetPlayerHP(damagedid, 30);
        }

        PlayerInfo[damagedid][LastDamage] = gettime();
        UpdateNameTag(damagedid, true);

        if(PlayerInfo[damagedid][CC_Team] == TEAM_LSPD)
        {
            if(PlayerInfo[damagedid][CC_Team] != PlayerInfo[playerid][CC_Team])
            {
                if(!GetPlayerWantedLevel(playerid))
                {
                    SetPlayerWantedLevel(playerid, 3);
                    GameTextForPlayer(playerid, "~r~~n~~n~~n~~n~~n~~n~~n~~n~~n~Wanted level increased!", 1500, 3);
                }
            }
        }

        if(GetPlayerWeapon(playerid) == 23 && PlayerInfo[playerid][Taser] == true)
        {
            if(GetPlayerWantedLevel(damagedid) >= 3 || PlayerInfo[damagedid][Tased] || PlayerInfo[damagedid][State] != PLAYER_STATE_ALIVE || GetPlayerState(damagedid) != PLAYER_STATE_ONFOOT)
            {
                amount = 0;
                SetPlayerArmor(damagedid, ar);
                return SetPlayerHP(damagedid, hp);
            }
            else
            {
                if(IsPlayerNearPlayer(playerid, damagedid, float(randomEx(10, 13))))
                {
                    TogglePlayerControllable(damagedid, false);
                    PlayerInfo[damagedid][Tased] = 13;
                    SendNearbyMessage(damagedid, 30.0, COLOR_EMOTE, "* %s falls on the ground after being hit by %s's taser.", ReturnName(damagedid), ReturnName(playerid));
				    GameTextForPlayer(damagedid, "~r~You Are Tasered", 3000, 3);
                    ApplyAnimationEx(damagedid,"PED","KO_shot_front",4.1,0,1,1,1,1);
                }

                amount = 0;
                SetPlayerArmor(damagedid, ar);
                return SetPlayerHP(damagedid, hp);
            }
        }

        
        if(PlayerInfo[damagedid][State] == PLAYER_STATE_ALIVE)
        {
            if(bodypart == 7 || bodypart == 8)
            {
                if(PlayerInfo[damagedid][BrokenLeg] == false)
                {
                    if(amount >= 10.0)
                    {
                        PlayerInfo[damagedid][BrokenLeg] = true;
                        SendClientMessage(damagedid, COLOR_RED, "-> You were shot in the legs, you are not able to jump and sprint.");
                    }
                }
            }

            switch(weaponid)
            {
                case 0:
                {
                    //registered gta damage
                }
                case 25, 27:
                {
                    amount = amount + WeaponDamage[weaponid][WepDamage]; //registered gta damage + script damage
                }
                default:
                {
                    amount = WeaponDamage[weaponid][WepDamage]; //script damage
                }
            }
            
            if(bodypart == 9) amount += float(floatround(amount / 2, floatround_ceil));

            if(PlayerInfo[playerid][HitSound]) PlayerPlaySound(playerid, 17802, 0.0, 0.0, 0.0);
        
            HitMarkerForPlayer(playerid, damagedid, weaponid, amount, false);

            if(ar > 1)
            {
                if(bodypart == 3)
                {
                    SetPlayerArmor(damagedid, ar-amount);
                    if(ar - amount <= 6)
                    {
                        SetPlayerArmor(damagedid, 0);
                    }
                }
                else
                {
                    SetPlayerArmor(damagedid, ar);
                    SetPlayerHP(damagedid, hp-amount);
                    AddPlayerDamage(damagedid, PlayerInfo[playerid][Name], weaponid, amount, bodypart);
                }
            }
            else
            {
                SetPlayerArmor(damagedid, ar);
                SetPlayerHP(damagedid, hp-amount);
                AddPlayerDamage(damagedid, PlayerInfo[playerid][Name], weaponid, amount, bodypart);
            }
        }

        if(PlayerInfo[damagedid][State] == PLAYER_STATE_WOUNDED && !PlayerInfo[damagedid][DamageFix])
        {
            new Float:DeadPos[4];
            GetPlayerPos(damagedid, DeadPos[0], DeadPos[1], DeadPos[2]);
            GetPlayerFacingAngle(damagedid, DeadPos[3]);

            PlayerInfo[damagedid][Actor] = CreateDynamicActor(GetPlayerSkin(damagedid), DeadPos[0], DeadPos[1], DeadPos[2], DeadPos[3], 1, 100.0, GetPlayerVirtualWorld(damagedid));
            ApplyDynamicActorAnimation(PlayerInfo[damagedid][Actor], "WUZI", "CS_Dead_Guy", 4.1, 0, 0, 0, 1, 0);

            RemovePlayerFromCopchase(damagedid);
            return 1;
        }

        if(hp - amount <= 6)
        {
            if(PlayerInfo[damagedid][State] == PLAYER_STATE_ALIVE)
            {
                new rand = randomEx(5, 10);
                if(GetPlayerState(damagedid) == PLAYER_STATE_ONFOOT) ClearAnimations(damagedid);
                TogglePlayerControllable(damagedid, false);
                PlayerInfo[damagedid][State] = PLAYER_STATE_WOUNDED;
                PlayerInfo[damagedid][DamageFix] = 5;
                PlayerInfo[damagedid][DeathTime] = 15;
                SendClientMessage(damagedid, COLOR_RED, "(( You were brutally wounded. Use /damages to check your injuries, /acceptdeath to respawn. ))");
                foreach(new i : Player)
                {
                    if(GetPlayerGameMode(i) == GetPlayerGameMode(playerid)) SendDeathMessageToPlayer(i, playerid, damagedid, weaponid);
                }
                UpdatePlayerDeaths(damagedid);
                UpdatePlayerKills(playerid);	
                if(GetPlayerState(damagedid) != PLAYER_STATE_ONFOOT)
                {
                    TogglePlayerControllable(damagedid, false);
                    ApplyAnimationEx(damagedid, "ped", "CAR_dead_LHS", 4.1, 0, 0, 0, 1, 0, 1);	
                    SetPlayerArmedWeapon(damagedid, 0);
                    if(GetPlayerState(damagedid) == PLAYER_STATE_DRIVER) 
                    {
                        new vehicleid = GetPlayerVehicleID(damagedid), engine, lights, alarm, doors, bonnet, boot, objective;

                        GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

                        SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);
                    }
                }
                else  
                {
                    ApplyAnimationEx(damagedid, "WUZI", "CS_Dead_Guy", 4.1, 0, 0, 0, 1, 0, 1);	
                }
                GameTextForPlayer(damagedid, "~r~BRUTALLY WOUNDED", 3000, 4);

                if(PlayerInfo[damagedid][CC_Team] == TEAM_LSPD) SendGameMessage(GAME_MODE_COPCHASE, COLOR_RED, "Police Officer %s has been downed by %s.", ReturnName(damagedid), ReturnName(playerid));
                else if(PlayerInfo[damagedid][CC_Team] == TEAM_FUGITIVE) SendGameMessage(GAME_MODE_COPCHASE, COLOR_COP, "Felon %s has been downed by %s.", ReturnName(damagedid), ReturnName(playerid));

                SetPlayerHP(damagedid, 30);
                PlayerInfo[playerid][Score] += 5;
                PlayerInfo[playerid][Cash] += rand;
                new Float:arm;
                GetPlayerArmor(playerid, arm);
                if(PlayerHasPerk(playerid, 7))
                {
                    if(arm < 75.0)
                    {
                        SetPlayerArmor(playerid, arm += 15.0);
                        if(arm > 75.0)
                        {
                            SetPlayerArmor(playerid, 75.0);
                        }
                    }
                }

                new str[128];
                if(PlayerInfo[playerid][CC_Team] == TEAM_LSPD)
                {
                    if(!GetFugitivesCount()) format(str, sizeof(str), "You killed %s (%d)~n~~g~+5 score~n~+$%d cash", ReturnName(damagedid), damagedid, rand);
                    else format(str, sizeof(str), "You killed %s (%d)~n~~g~+5 score~n~+$%d cash~n~~r~%d fugitive(s) left", ReturnName(damagedid), damagedid, rand, GetFugitivesCount());
                }
                else
                {
                    if(!GetCopsCount()) format(str, sizeof(str), "You killed %s (%d)~n~~g~+5 score~n~+$%d cash", ReturnName(damagedid), damagedid, rand);
                    else format(str, sizeof(str), "You killed %s (%d)~n~~g~+5 score~n~+$%d cash~n~~b~%d cop(s) left", ReturnName(damagedid), damagedid, rand, GetCopsCount());
                }

                PlayerTextDrawSetString(playerid, PlayerInfo[playerid][i_Textdraw][9], str);
                PlayerTextDrawShow(playerid, PlayerInfo[playerid][i_Textdraw][9]);

                if(PlayerInfo[playerid][i_ExpireTimer] != -1)
                {
                    KillTimer(PlayerInfo[playerid][i_ExpireTimer]);
                    PlayerInfo[playerid][i_ExpireTimer] = -1;
                }

                PlayerInfo[playerid][i_ExpireTimer] = SetTimerEx("OnPlayerSignatureExpire", 2 * 1000, false, "i", playerid);
                return 1;
            }
        }
    }
	return 1;
}

hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    if(!ispassenger && GetVehicleDriver(vehicleid) != INVALID_PLAYER_ID)
    {
        ClearAnimations(playerid);
        new Float:X, Float:Y, Float:Z;
        GetPlayerPos(playerid, X, Y, Z);
        SetPlayerPos(playerid, X, Y, Z + 1.0);
        GameTextForPlayer(playerid, "~r~Do not car jacking!", 2000, 6);
        return 0;
    }

    if(GetPlayerGameMode(playerid) == GAME_MODE_COPCHASE)
    {
        if(ispassenger && GetVehicleDriver(vehicleid) != INVALID_PLAYER_ID && PlayerInfo[GetVehicleDriver(vehicleid)][CC_Team] != PlayerInfo[playerid][CC_Team])
        {
            ClearAnimations(playerid);
            new Float:X, Float:Y, Float:Z;
            GetPlayerPos(playerid, X, Y, Z);
            SetPlayerPos(playerid, X, Y, Z + 1.0);
            GameTextForPlayer(playerid, "~r~Do not teaming!", 2000, 6);
            return 0;
        }
    }
    return 1;
}

Menu:MENU_VEHICLE_TRUNK(playerid, response, listitem)
{
    new engine, lights, alarm, doors, bonnet, boot, objective, vehID = GetClosestVehicle(playerid);
    if (response == MENU_RESPONSE_SELECT)
    {
        if(GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command can only be used in copchase.");

        if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE) return SendErrorMessage(playerid, "You must be alive to use this command.");

        if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "You must be onfoot to use this command.");

        new Float:vehPos[3];
        GetVehiclePos(vehID, vehPos[0], vehPos[1], vehPos[2]);

        if (GetPlayerDistanceFromPoint(playerid, vehPos[0], vehPos[1], vehPos[2]) > 3.0) return SendErrorMessage(playerid, "You're not near a trunk.");

        if(IsValidWeapon(VehicleInfo[vehID][vGun][listitem]))
        {
            SendNearbyMessage(playerid, 30.0, 0xC2A2DAFF, "* %s {C2A2DA}takes out a(n) %s from the %s.", ReturnName(playerid), ReturnWeaponNameEx(VehicleInfo[vehID][vGun][listitem]), GetVehicleNameByModel(GetVehicleModel(vehID)));
            GivePlayerWeapon(playerid, VehicleInfo[vehID][vGun][listitem], VehicleInfo[vehID][vGunAmmo][listitem]);

            VehicleInfo[vehID][vGun][listitem] = 0;
            VehicleInfo[vehID][vGunAmmo][listitem] = 0;
        }

        GetVehicleParamsEx(vehID, engine, lights, alarm, doors, bonnet, boot, objective);
        SetVehicleParamsEx(vehID, engine, lights, alarm, doors, bonnet, VEHICLE_PARAMS_OFF, objective);
        TogglePlayerControllable(playerid, true);
    }
    else if(response == MENU_RESPONSE_CLOSE)
    {
        GetVehicleParamsEx(vehID, engine, lights, alarm, doors, bonnet, boot, objective);
        SetVehicleParamsEx(vehID, engine, lights, alarm, doors, bonnet, VEHICLE_PARAMS_OFF, objective);
        TogglePlayerControllable(playerid, true);
    }
    return 1;
}

public OnPlayerStartRobbery(playerid, actorid, bool:robbed_recently)
{
    if(PlayerInfo[playerid][State] == PLAYER_STATE_WOUNDED) return 0;

    if(PlayerInfo[playerid][CC_Team] == TEAM_LSPD || PlayerInfo[playerid][Team] == TEAM_LSPD) return 0;

    foreach(new i : Player)
    {
        if(IsPlayerConnected(i))
        {
            if(IsValidDynamicActor(PlayerInfo[i][Actor]) && actorid == PlayerInfo[i][Actor]) return 0;
        }
    }

    if(robbed_recently)  return 0;

    SendNearbyMessage(playerid, 20.0, COLOR_EMOTE, "* %s{C2A2DA} aims at the cashier.", ReturnName(playerid));

    if(GetPlayerGameMode(playerid) == GAME_MODE_TDM)
    {
        if(random(100) <= 50)
        {
            foreach(new i : Player)
            {
                if(GetPlayerGameMode(i) == GAME_MODE_TDM && PlayerInfo[i][Team] == TEAM_LSPD)
                {
                    SendClientMessageEx(i, COLOR_RADIO, "** [CH: 911 S: 1] DISPATCH: A store has been robbed at %s.", ReturnLocation(playerid));
                }
            }
        }
    }
    return 1;
}


public OnPlayerFinishRobbery(playerid, actorid, robbedmoney, type)
{
    if(PlayerInfo[playerid][State] == PLAYER_STATE_WOUNDED) return 0;

    if(PlayerInfo[playerid][CC_Team] == TEAM_LSPD || PlayerInfo[playerid][Team] == TEAM_LSPD) return 0;

    new string[128];

    switch(type)
    {
        case TYPE_SUCCESS:
        {
            format(string, sizeof(string), "~w~You stole ~G~$%i~W~ from the cashier", robbedmoney);

            GameTextForPlayer_(playerid, string, 5000, 9);  

            PlayerInfo[playerid][Cash] += robbedmoney;
        }
        case TYPE_FAILED:
        {
            GameTextForPlayer_(playerid, "~r~Cashier refused to give money", 5000, 9);
        }
        case TYPE_UNFINISHED:
        {
            GameTextForPlayer_(playerid, "~r~Robbery Failed", 5000, 9);
        }
    }
    return 1;
}