alias:pm("msg", "message")
alias:changeteam("ct", "switchteam")
alias:weapon("weapons", "loadout")
alias:radio("team", "f", "faction", "r")
alias:setarmed("setaw", "sw", "setweapon", "switchweapon")
alias:help("cmds", "helpme")
alias:toggle("tog")
alias:global("o", "ooc")

alias:shout("s")
alias:cw("carwhisper", "whisper")
alias:b("ooc")
alias:megaphone("m")
alias:taser("ts", "tz", "tazer")
alias:cuff("handcuff")
alias:local("l", "low")
alias:dragout("drag", "takeout")
alias:trace("tr", "find", "track")
alias:nos("nitro", "buynitro")
alias:acceptdeath("respawnme", "respawn")

alias:report("re")

alias:vehicle("veh")
alias:world("vw")
alias:health("hp")
alias:armour("ar")

cmd:help(playerid, params[])
{
    new option[32];
    if(sscanf(params, "s[32]", option)) 
    {
        SendUsageMessage(playerid, "/help [section]");
        return SendClientMessage(playerid, -1, "Section: general, mode, TDM, copchase, freeroam");
    }

    if(strcmp(params, "general", true) == 0)
    {
        SendClientMessage(playerid, 0xBADA55FF, "General");
        SendClientMessage(playerid, 0xBADA55FF, "* /lobby /stats /global /pm /hitsound /hitmarker /duel /signature /toggle /attachment *");
        SendClientMessage(playerid, 0xBADA55FF, "* /votekick /voteyes /id /report /perks /selfrevive /flipveh /fixveh *");
    }
    else if(strcmp(params, "mode", true) == 0)
    {
        SendClientMessage(playerid, 0xBADA55FF, "Mode");
        SendClientMessage(playerid, 0xBADA55FF, "* /tdm /dm /copchase /freeroam *");
    }
    else if(strcmp(params, "TDM", true) == 0)
    {
        SendClientMessage(playerid, 0xBADA55FF, "Team Deathmatch");
        SendClientMessage(playerid, 0xBADA55FF, "* /radio /medkit /acceptdeath /skin /buy /revive /weapon /damages /setarmed /fugitives *");
    }
    else if(strcmp(params, "copchase", true) == 0)
    {
        SendClientMessage(playerid, 0xBADA55FF, "Copchase");
        SendClientMessage(playerid, 0xBADA55FF, "* /car [cop/fugitive] /pullincar /taser /trunk /radio /medkit /acceptdeath /revive /damages /setarmed /fugitives /dragout *");
        SendClientMessage(playerid, 0xBADA55FF, "* /lock /trace /nos *");
    }
    else if(strcmp(params, "freeroam", true) == 0)
    {
        SendClientMessage(playerid, 0xBADA55FF, "Freeroam");
        SendClientMessage(playerid, 0xBADA55FF, "* /vehicle /weapon /health /armour /world *");
    }
    return 1;
}

cmd:toggle(playerid, params[])
{
    new option[32], temp_str[1024], primary_str[1024];
    if(sscanf(params, "s[32]", option)) return SendUsageMessage(playerid, "/toggle [login/hud]");

    if(strcmp(params, "login", true) == 0)
    {
        switch(PlayerInfo[playerid][Tog][0])
        {
            case 0:
            {
                PlayerInfo[playerid][Tog][0] = 1;
                SendClientMessage(playerid, COLOR_GREY, "You disabled login message.");
            }
            case 1:
            {
                PlayerInfo[playerid][Tog][0] = 0;
                SendClientMessage(playerid, COLOR_GREY, "You enabled login message.");
            }
        }
    }
    else if(strcmp(params, "hud", true) == 0)
    {
        strcat(primary_str, "Name\tStatus\n");
        format(temp_str, sizeof temp_str, "Server Logo\t%s{FFFFFF}\n", (!PlayerInfo[playerid][Tog][1]) ? ("{00FF00}Enabled") : ("{FF0000}Disabled"));
        strcat(primary_str, temp_str);
        format(temp_str, sizeof temp_str, "Statistics\t%s{FFFFFF}\n", (!PlayerInfo[playerid][Tog][2]) ? ("{00FF00}Enabled") : ("{FF0000}Disabled"));
        strcat(primary_str, temp_str);
        ShowPlayerDialog(playerid, DIALOG_HUD, DIALOG_STYLE_TABLIST_HEADERS, "User Interfaces", primary_str, "Set", "Abort");
    }
    return 1;
}

cmd:car(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_LOBBY) return SendErrorMessage(playerid, "You must be in lobby to use this command.");

    new item[32];
    if(sscanf(params, "s[32]", item)) return SendUsageMessage(playerid, "/car [cop/fugitive]");
    if(strcmp(params, "cop", true) == 0)
    {
        ShowModelSelectionMenu(playerid, "Cop Vehicle", MODEL_SELECTION_COP_VEHICLE, lspdVeh, sizeof(lspdVeh), -16.0, 0.0, -55.0);
    }
    else if(strcmp(params, "fugitive", true) == 0)
    {
        ShowModelSelectionMenu(playerid, "Fugitive Vehicle", MODEL_SELECTION_FUG_VEHICLE, gCriminalVeh, sizeof(gCriminalVeh), -16.0, 0.0, -55.0);
    }
    return 1;
}

cmd:enter(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command can only be used in copchase.");

    if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE) return SendErrorMessage(playerid, "You must be alive to use this command.");

    if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "You must be onfoot to use this command.");

    if(CopchaseInfo[CC_EnableInterior] == false && PlayerInfo[playerid][CC_Team] == TEAM_LSPD) return SendErrorMessage(playerid, "Thats too early!");

    if(CopchaseInfo[CC_InteriorDelay] && CopchaseInfo[CC_InteriorDelay] < 100 && PlayerInfo[playerid][CC_Team] == TEAM_LSPD) return SendErrorMessage(playerid, "Try again after %d second(s)", CopchaseInfo[CC_InteriorDelay]);

    if(gettime() - PlayerInfo[playerid][EnterExitDelay] < 4) return SendErrorMessage(playerid, "Please wait for a few seconds to use this command again.");

    for(new i; i < MAX_PROPERTIES; i++)
    {
        if(IsPlayerInRangeOfPoint(playerid, 3.0, PropertyInfo[i][PropEntrance][0], PropertyInfo[i][PropEntrance][1], PropertyInfo[i][PropEntrance][2]) && IsPlayerInDynamicCP(playerid, PropertyInfo[i][PropCheckpoint]))
        {
            PlayerInfo[playerid][EnterExitDelay] = gettime();

            SetPlayerPos(playerid, PropertyInfo[i][PropExit][0], PropertyInfo[i][PropExit][1], PropertyInfo[i][PropExit][2]);
            SetPlayerVirtualWorld(playerid, PropertyInfo[i][PropWorld]);
            SetPlayerInterior(playerid, PropertyInfo[i][PropInterior]);

            PlayerInfo[playerid][Property] = i;

            PlayerInfo[playerid][ObjectLoad] = 3;
            TogglePlayerControllable(playerid, 0);

            SendClientMessageEx(playerid, -1, "Welcome to %s!", PropertyInfo[i][PropName]);

            SendClientMessage(playerid, COLOR_GREY, "Press Y to immediately unfreeze.");

            PlayerInfo[playerid][RecentlySpawned] = gettime();

            if(PlayerInfo[playerid][CC_Team] == TEAM_FUGITIVE)
            {
                if(CopchaseInfo[CC_EnableInterior] == false)
                {
                    CopchaseInfo[CC_EnableInterior] = true;
                    CopchaseInfo[CC_InteriorDelay] = 15;
                    foreach(new id : Player)
                    {
                        if(GetPlayerGameMode(id) == GAME_MODE_COPCHASE && PlayerInfo[id][CC_Team] == TEAM_LSPD) SendClientMessageEx(id, COLOR_RADIO, "** [CH: 911 S: 1] The suspect entered %s.", PropertyInfo[i][PropName]);
                    }
                }
                if(GetPlayerWantedLevel(playerid) < 3)
                {
                    SetPlayerWantedLevel(playerid, 3);
                    GameTextForPlayer(playerid, "~r~~n~~n~~n~~n~~n~~n~~n~~n~~n~Wanted level increased!", 1500, 3);
                }
            }
        }
    }
    return 1;
}

cmd:pullincar(playerid, params[])
{
    new target, seatid, vehicleid = GetPlayerVehicleID(playerid);
    if(GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command can only be used in copchase.");

    if(PlayerInfo[playerid][CC_Team] != TEAM_LSPD) return SendErrorMessage(playerid, "You must be a cop to use this command.");

    if(PlayerInfo[playerid][State] == PLAYER_STATE_WOUNDED) return SendErrorMessage(playerid, "You must be alive to use this command.");

    if(!IsPlayerInAnyVehicle(playerid)) return SendErrorMessage(playerid, "You aren't in a vehicle.");

    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendErrorMessage(playerid, "You aren't the driver.");

    if(sscanf(params, "ud", target, seatid)) return SendUsageMessage(playerid, "/pullincar [playerid/name] [seatid]");

    if(!IsPlayerConnected(target)) return SendErrorMessage(playerid, "The player you specified isn't connected.");

    if(target == playerid) return SendErrorMessage(playerid, "The player you specified isn't connected.");

    if(PlayerInfo[playerid][CC_Team] != PlayerInfo[target][CC_Team]) return SendErrorMessage(playerid, "The player you specified isn't in your team.");

    if(!IsPlayerNearPlayer(playerid, target, 7.0)) return SendErrorMessage(playerid, "You are too far away from that player.");

    if(PlayerInfo[target][State] != PLAYER_STATE_WOUNDED) return SendErrorMessage(playerid, "That player is alive.");

    if(GetPlayerState(target) != PLAYER_STATE_ONFOOT) return SendErrorMessage(target, "That player isn't onfoot.");

    if(seatid <= 0 || seatid >= 4) return SendErrorMessage(playerid, "Invalid seat ID!");

    if(IsVehicleSeatTaken(vehicleid, seatid)) return SendErrorMessage(playerid, "That seat is already taken.");

    PutPlayerInVehicle(target, vehicleid, seatid);
    SendNearbyMessage(playerid, 25.0, COLOR_EMOTE, "* %s pulls %s into the %s.", ReturnName(playerid), ReturnName(target), GetVehicleNameByModel(GetVehicleModel(vehicleid)));
    return 1;
}

cmd:taser(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command can only be used in copchase.");

    if(PlayerInfo[playerid][CC_Team] != TEAM_LSPD) return SendErrorMessage(playerid, "You must be a cop to use this command.");

    if(PlayerInfo[playerid][State] == PLAYER_STATE_WOUNDED) return SendErrorMessage(playerid, "You must be alive to use this command.");

    if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "You must be onfoot to use this command.");

    if(CopchaseInfo[CC_WeaponEnabled] == false) return SendErrorMessage(playerid, "Please wait until the weapon deployed.");

    if(PlayerInfo[playerid][Taser] == false)
    {
        PlayerInfo[playerid][AntiCheatPause] = 2;
        for(new wep; wep < 12; wep++)
        {
            GetPlayerWeaponData(playerid, wep, PlayerInfo[playerid][CC_Weapon][wep], PlayerInfo[playerid][CC_WeaponAmmo][wep]);
        }
        PlayerInfo[playerid][Taser] = true;
        ResetPlayerWeapons(playerid);
        GivePlayerWeapon(playerid, 23, 5);
        SendNearbyMessage(playerid, 20.0, COLOR_EMOTE, "* %s takes out their taser.", ReturnName(playerid));
    }
    else if(PlayerInfo[playerid][Taser] == true)
    {
        PlayerInfo[playerid][AntiCheatPause] = 2;
        ResetPlayerWeapons(playerid);
        PlayerInfo[playerid][Taser] = false;

        for(new wep; wep < 12; wep++)
        {
            if(IsValidWeapon(PlayerInfo[playerid][CC_Weapon][wep]) && PlayerInfo[playerid][CC_WeaponAmmo][wep]) GivePlayerWeapon(playerid, PlayerInfo[playerid][CC_Weapon][wep], PlayerInfo[playerid][CC_WeaponAmmo][wep]);
        }
        SendNearbyMessage(playerid, 20.0, COLOR_EMOTE, "* %s puts their taser away.", ReturnName(playerid));
    }
    return 1;
}

cmd:trace(playerid, params[])
{
    new target;
    if(GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command can only be used in copchase.");

    if(PlayerInfo[playerid][CC_Team] != TEAM_LSPD) return SendErrorMessage(playerid, "You must be a cop to use this command.");

    if(PlayerInfo[playerid][State] == PLAYER_STATE_WOUNDED) return SendErrorMessage(playerid, "You must be alive to use this command.");

    if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "You must be in a car to access your track device.");

    if(CopchaseInfo[CC_WeaponEnabled] == false) return SendErrorMessage(playerid, "Please wait until the weapon deployed.");

    if(sscanf(params, "u", target)) return SendUsageMessage(playerid, "/trace [playerid/name]");

    if(!IsPlayerConnected(target)) return SendErrorMessage(playerid, "The player you specified isn't connected.");

    if(target == playerid) return SendErrorMessage(playerid, "The player you specified isn't connected.");

    if(PlayerInfo[target][CC_Team] == TEAM_LSPD) return SendErrorMessage(playerid, "The player you specified is in your team.");

    if(CopchaseInfo[CC_TraceDelay]) return SendErrorMessage(playerid, "Someone recently used this command. Try again later.");

    foreach(new i : Player)
    {
        if(GetPlayerGameMode(i) == GAME_MODE_COPCHASE && PlayerInfo[i][CC_Team] == TEAM_LSPD)
        {
            SendClientMessageEx(i, COLOR_RADIO, "** [CH: 911 S: 1] Felon %s spotted at %s.",  PlayerInfo[target][Name], ReturnLocation(target));
        }
    }

    PlayerInfo[target][Traced] = 10;

    CopchaseInfo[CC_TraceDelay] = 20;
    return 1;
}

cmd:cuff(playerid, params[])
{
    new target;
    if(GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command can only be used in copchase.");

    if(PlayerInfo[playerid][CC_Team] != TEAM_LSPD) return SendErrorMessage(playerid, "You must be a cop to use this command.");

    if(PlayerInfo[playerid][State] == PLAYER_STATE_WOUNDED) return SendErrorMessage(playerid, "You must be alive to use this command.");

    if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "You must be onfoot to use this command.");

    if(CopchaseInfo[CC_WeaponEnabled] == false) return SendErrorMessage(playerid, "Please wait until the weapon deployed.");

    if(sscanf(params, "u", target)) return SendUsageMessage(playerid, "/cuff [playerid/name]");

    if(!IsPlayerConnected(target)) return SendErrorMessage(playerid, "The player you specified isn't connected.");

    if(target == playerid) return SendErrorMessage(playerid, "The player you specified isn't connected.");

    if(PlayerInfo[target][CC_Team] == TEAM_LSPD) return SendErrorMessage(playerid, "The player you specified is in your team.");

    if(!IsPlayerNearPlayer(playerid, target, 3.0)) return SendErrorMessage(playerid, "You are too far away from the target.");

    if(GetPlayerWantedLevel(target) >= 3) return SendNearbyMessage(playerid, 20.0, COLOR_EMOTE, "* %s tries to handcuff %s but failed.", ReturnName(playerid), ReturnName(target));

    if(GetPlayerState(target) != PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "That player isn't onfoot. Use /dragout to drag him out.");

    if(PlayerInfo[playerid][CuffTime]) return SendErrorMessage(playerid, "You already reviving someone.");

    PlayerInfo[playerid][CuffID] = target;
    PlayerInfo[playerid][CuffTime] = 8;

    SendNearbyMessage(playerid, 20.0, COLOR_EMOTE, "* %s tries to handcuff %s.", ReturnName(playerid), ReturnName(target));
    return 1;
}

cmd:dragout(playerid, params[])
{
    new target;
    if(GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command can only be used in copchase.");

    if(PlayerInfo[playerid][CC_Team] != TEAM_LSPD) return SendErrorMessage(playerid, "You must be a cop to use this command.");

    if(PlayerInfo[playerid][State] == PLAYER_STATE_WOUNDED) return SendErrorMessage(playerid, "You must be alive to use this command.");

    if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "You must be onfoot to use this command.");

    if(CopchaseInfo[CC_WeaponEnabled] == false) return SendErrorMessage(playerid, "Please wait until the weapon deployed.");

    if(sscanf(params, "u", target)) return SendUsageMessage(playerid, "/dragout [playerid/name]");

    if(!IsPlayerConnected(target)) return SendErrorMessage(playerid, "The player you specified isn't connected.");

    if(target == playerid) return SendErrorMessage(playerid, "The player you specified isn't connected.");

    if(PlayerInfo[target][CC_Team] == TEAM_LSPD) return SendErrorMessage(playerid, "The player you specified is in your team.");

    if(!IsPlayerNearPlayer(playerid, target, 3.0)) return SendErrorMessage(playerid, "You are too far away from the target.");

    if(GetPlayerWantedLevel(target) >= 3) return SendNearbyMessage(playerid, 20.0, COLOR_EMOTE, "* %s tries to drag out %s but failed.", ReturnName(playerid), ReturnName(target));

    if(GetPlayerState(target) == PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "That player isn't in vehicle.");

    if(PlayerInfo[playerid][DragTime]) return SendErrorMessage(playerid, "You already dragging someone.");

    PlayerInfo[playerid][DragID] = target;
    PlayerInfo[playerid][DragTime] = 5;

    SendNearbyMessage(playerid, 20.0, COLOR_EMOTE, "* %s tries to drag out %s.", ReturnName(playerid), ReturnName(target));
    return 1;
}

cmd:nos(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command can only be used in copchase.");

    if(PlayerInfo[playerid][CC_Team] != TEAM_FUGITIVE) return SendErrorMessage(playerid, "You must be a fugitive to use this command.");

    if(PlayerInfo[playerid][State] == PLAYER_STATE_WOUNDED) return SendErrorMessage(playerid, "You must be alive to use this command.");

    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendErrorMessage(playerid, "You must be in vehicle to use this command.");

    if(PlayerInfo[playerid][Cash] < 50) return SendErrorMessage(playerid, "You don't have enough money.");

    AddVehicleComponent(GetPlayerVehicleID(playerid), 1010); // Nitro
    SendClientMessage(playerid, COLOR_GREEN, "You've bought a x10 nitro for $50.");

    PlayerInfo[playerid][Cash] -= 50;
    return 1;
}

cmd:trunk(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command can only be used in copchase.");

    if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE) return SendErrorMessage(playerid, "You must be alive to use this command.");

    if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "You must be onfoot to use this command.");

    new Float:vehPos[3], vehID = GetClosestVehicle(playerid), string[512], engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehiclePos(vehID, vehPos[0], vehPos[1], vehPos[2]);

	if (GetPlayerDistanceFromPoint(playerid, vehPos[0], vehPos[1], vehPos[2]) > 3.0) return SendErrorMessage(playerid, "You're not near a trunk.");

    GetVehicleParamsEx(vehID, engine, lights, alarm, doors, bonnet, boot, objective);

    if(doors) return SendErrorMessage(playerid, "The vehicle is locked.");

    if(CopchaseInfo[CC_WeaponEnabled] == false) return SendErrorMessage(playerid, "Please wait until the weapon deployed.");

    ShowPlayerMenu(playerid, MENU_VEHICLE_TRUNK, "Vehicle Trunk");
    TogglePlayerControllable(playerid, false);

    for(new i; i < 10; i++)
    {
        format(string, sizeof string, "%s", ReturnWeaponNameEx(VehicleInfo[vehID][vGun][i]));
        AddPlayerMenuItem(playerid, string, "Press ~k~~PED_SPRINT~ to select, press ~k~~VEHICLE_ENTER_EXIT~ to cancel.");
    }

    GetVehicleParamsEx(vehID, engine, lights, alarm, doors, bonnet, boot, objective);
    SetVehicleParamsEx(vehID, engine, lights, alarm, doors, bonnet, VEHICLE_PARAMS_ON, objective);
    return 1;
}

cmd:exit(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command can only be used in copchase.");

    if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE) return SendErrorMessage(playerid, "You must be alive to use this command.");

    if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "You must be onfoot to use this command.");

    if(gettime() - PlayerInfo[playerid][EnterExitDelay] < 4) return SendErrorMessage(playerid, "Please wait for a few seconds to use this command again.");

    for(new i; i < MAX_PROPERTIES; i++)
    {
        if(IsPlayerInRangeOfPoint(playerid, 3.0, PropertyInfo[i][PropExit][0], PropertyInfo[i][PropExit][1], PropertyInfo[i][PropExit][2]) && GetPlayerVirtualWorld(playerid) == PropertyInfo[i][PropWorld] && GetPlayerInterior(playerid) == PropertyInfo[i][PropInterior])
        {
            PlayerInfo[playerid][EnterExitDelay] = gettime();

            SetPlayerPos(playerid, PropertyInfo[i][PropEntrance][0], PropertyInfo[i][PropEntrance][1], PropertyInfo[i][PropEntrance][2]);
            SetPlayerVirtualWorld(playerid, 2);
            SetPlayerInterior(playerid, 0);

            PlayerInfo[playerid][Property] = -1;

            PlayerInfo[playerid][ObjectLoad] = 3;
            TogglePlayerControllable(playerid, 0);
        }
    }
    return 1;
}

cmd:votekick(playerid, params[])
{
    new playerb, reason[128];
    if(PlayerInfo[playerid][Score] < 750) return SendErrorMessage(playerid, "You must have atleast 750 score to use this command.");

    if (sscanf(params, "us[126]", playerb, reason)) return SendClientMessage(playerid, 0xf6b26bFF, "USAGE: /votekick [playerid/name] [reason]");

	if (!IsPlayerConnected(playerb) || playerb == INVALID_PLAYER_ID) return SendClientMessage(playerid, 0xe06666FF, "That player isn't connected.");

    if (playerb == playerid) return SendClientMessage(playerid, 0xe06666FF, "You can't do that to yourself.");

    if (PlayerInfo[playerb][AdminLevel] > PlayerInfo[playerid][AdminLevel]) return SendClientMessage(playerid, 0xe06666FF, "You can't vote kick that player.");

    if(VoteInfo[VoteID] != INVALID_PLAYER_ID) return SendErrorMessage(playerid, "There is already an active vote.");

    if(GetPlayerCount() < 4) return SendErrorMessage(playerid, "There must be atleast 4 players online to use this command.");

    if(VoteInfo[VoteCooldown]) return SendErrorMessage(playerid, "Please wait for %d second(s) to use this command again.", VoteInfo[VoteCooldown]);

    VoteInfo[VoteID] = playerb;
    VoteInfo[VoteAmount] = 0;
    VoteInfo[VoteIssuer] = playerid;

    if(GetPlayerCount() < 5) VoteInfo[VoteMaxAmount] = (GetPlayerCount() / 2);
    else VoteInfo[VoteMaxAmount] = 5;

    VoteInfo[VoteTime] = 60;
    format(VoteInfo[VoteReason], 126, reason);

    foreach(new i : Player)
    {
        PlayerInfo[i][Vote] = 0;
    }

    SendClientMessageToAll(COLOR_RED, "---------------------------------");
    SendClientMessageToAllEx(-1, "{FF6347}%s {FFFFFF}has started a kick vote againts {FF6347}%s{FFFFFF}.", PlayerInfo[playerid][Name], PlayerInfo[playerb][Name]);
    SendClientMessageToAllEx(-1, "Reason: %s", reason);
    SendClientMessageToAllEx(-1, "Vote needed to kick: %d", VoteInfo[VoteMaxAmount]);
    SendClientMessageToAllEx(-1, "Use '{FF6347}/voteyes{FFFFFF}' to vote.");
    SendClientMessageToAll(COLOR_RED, "---------------------------------");
    return 1;
}

cmd:voteyes(playerid, params[])
{
    if(VoteInfo[VoteID] == INVALID_PLAYER_ID) return SendErrorMessage(playerid, "There is no active vote.");

    if(VoteInfo[VoteIssuer] == playerid) return SendErrorMessage(playerid, "No, don't do this.");

    if(PlayerInfo[playerid][Vote]) return SendErrorMessage(playerid, "You already voted.");

    PlayerInfo[playerid][Vote] = 1;
    VoteInfo[VoteAmount]++;
    SendClientMessageEx(playerid, COLOR_GREEN, "You voted yes.");
    SendClientMessageToAllEx(COLOR_RED, "VOTEKICK: {FFFFFF}%s voted yes.", PlayerInfo[playerid][Name]);

    if(VoteInfo[VoteAmount] >= VoteInfo[VoteMaxAmount])
    {
        new ip[126];
        GetPlayerIp(VoteInfo[VoteID], ip, sizeof ip);
        SendClientMessageToAllEx(COLOR_RED, "VOTEKICK: %s has been kicked from the server.", PlayerInfo[VoteInfo[VoteID]][Name]);

        BlockIpAddress(ip, 30 * 1000);
        
        DelayedKick(VoteInfo[VoteID]);

        VoteInfo[VoteID] = INVALID_PLAYER_ID;
        VoteInfo[VoteAmount] = 0;
        VoteInfo[VoteIssuer] = INVALID_PLAYER_ID;
        format(VoteInfo[VoteReason], 126, "NaN");
        VoteInfo[VoteMaxAmount] = 0;
        VoteInfo[VoteTime] = 0;
        foreach(new i : Player)
        {
            PlayerInfo[i][Vote] = 0;
        }
        VoteInfo[VoteCooldown] = 30;
    }
    return 1;
}

CMD:id(playerid, params[])
{
    new playerb;
    if (sscanf(params, "u", playerb))
        return SendUsageMessage(playerid, "/id [playerid/name]");

    if (strlen(params) < 3 && !IsNumeric(params))
        return SendErrorMessage(playerid, "You must specify at least 3 characters.");

    if(!IsNumeric(params))
    {
        new count;
        foreach (new i : Player)
        {
            if (strfind(ReturnName(i), params, true) != -1)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "* %s - (ID: %d)", ReturnName(i, false), i);
                count++;
            }
        }
        if (!count)
            return SendErrorMessage(playerid, "No players found with these criteria: \"%s\".", params);
    }
    else
    {
        if(!PlayerInfo[playerb][IsLoggedIn]) return SendErrorMessage(playerid, "There is no such player.");
        
        SendClientMessageEx(playerid, COLOR_GREY, "* %s - (ID: %d)", ReturnName(playerb, false), playerb);
    }
    return 1;
}

cmd:hitmarker(playerid, params[])
{
    if(PlayerInfo[playerid][HitMarker])
    {
        PlayerInfo[playerid][HitMarker] = 0;
        SendClientMessage(playerid, COLOR_YELLOW, "Hit marker togged off.");
    }
    else if(!PlayerInfo[playerid][HitMarker])
    {
        PlayerInfo[playerid][HitMarker] = 1;
        SendClientMessage(playerid, COLOR_YELLOW, "Hit marker togged on.");
    }
    return 1;
}

cmd:hitsound(playerid, params[])
{
    if(PlayerInfo[playerid][HitSound])
    {
        PlayerInfo[playerid][HitSound] = 0;
        SendClientMessage(playerid, COLOR_YELLOW, "Hit sound togged off.");
    }
    else if(!PlayerInfo[playerid][HitSound])
    {
        PlayerInfo[playerid][HitSound] = 1;
        SendClientMessage(playerid, COLOR_YELLOW, "Hit sound togged on.");
    }
    return 1;
}

cmd:lobby(playerid, params[])
{
    if(GetPlayerGameMode(playerid) == GAME_MODE_LOBBY && !IsPlayerInCopchaseLobby(playerid)) return SendErrorMessage(playerid, "You can't use this while in lobby.");

    if(gettime() - PlayerInfo[playerid][LastDamage] < 10) return SendErrorMessage(playerid, "You took damage recently.");

    if (PlayerInfo[playerid][SelectingTeam] == true) return 1;

    if(GetPlayerGameMode(playerid) == GAME_MODE_DUEL)
	{
	    PlayerInfo[DuelInfo[playerid][DuelEnemy]][Cash] += DuelInfo[playerid][DuelBet];
	    PlayerInfo[playerid][Cash] -= DuelInfo[playerid][DuelBet];
        SendGameMessage(GAME_MODE_LOBBY, COLOR_RED, "DUEL: {FFFFFF} %s defeated %s in a duel. (%s)", ReturnName(DuelInfo[playerid][DuelEnemy]), ReturnName(playerid), ConvertTime(GetTickCount() - DuelInfo[playerid][DuelTick]));
		SendGameMessage(GAME_MODE_DM, COLOR_RED, "DUEL: {FFFFFF} %s defeated %s in a duel. (%s)", ReturnName(DuelInfo[playerid][DuelEnemy]), ReturnName(playerid), ConvertTime(GetTickCount() - DuelInfo[playerid][DuelTick]));
        SendGameMessage(GAME_MODE_DUEL, COLOR_RED, "DUEL: {FFFFFF} %s defeated %s in a duel. (%s)", ReturnName(DuelInfo[playerid][DuelEnemy]), ReturnName(playerid), ConvertTime(GetTickCount() - DuelInfo[playerid][DuelTick]));

        SendPlayerToLobby(DuelInfo[playerid][DuelEnemy]);
	    DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelEnemy] = INVALID_PLAYER_ID;
	    DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelWeapon] = 0;
	    DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelWeapon2] = 0;
	    DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelBet] = 0;
	    DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelMap] = 0;

        SendPlayerToLobby(playerid);
        DuelInfo[playerid][DuelEnemy] = INVALID_PLAYER_ID;
        DuelInfo[playerid][DuelWeapon] = 0;
        DuelInfo[playerid][DuelWeapon2] = 0;
        DuelInfo[playerid][DuelBet] = 0;
        DuelInfo[playerid][DuelMap] = 0;
	}

    ResetPlayerWeapons(playerid);
    ClearAnimations(playerid);
    PlayerInfo[playerid][State] = PLAYER_STATE_ALIVE;
    SetPlayerHP(playerid, 100);
    PlayerInfo[playerid][KillStreak] = 0;

    PlayerInfo[playerid][Team] = -1;
    PlayerInfo[playerid][DM] = -1;

    PlayerInfo[playerid][MedkitTime] = 0;

    SendPlayerToLobby(playerid);

    GameTextForPlayer(playerid, "_", 100, 3);
    return 1;
}

cmd:startcopchase(playerid, params[])
{
    if(PlayerInfo[playerid][AdminLevel] < 8) return 1;

    StartCopchase();
    SendClientMessageToAllEx(0xe06666FF, "Admin %s has started the copchase match.", PlayerInfo[playerid][Name]);
    return 1;
}

cmd:endcopchase(playerid, params[])
{
    if(PlayerInfo[playerid][AdminLevel] < 8) return 1;

    EndCopchase();
    SendClientMessageToAllEx(0xe06666FF, "The copchase match has been ended by %s.", PlayerInfo[playerid][Name]);
    return 1;
}

cmd:copchase(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_LOBBY) return SendErrorMessage(playerid, "This command is only available in lobby.");

    if(IsPlayerInCopchaseLobby(playerid)) return SendErrorMessage(playerid, "You already in a copchase lobby.");

    SetPlayerPos(playerid, 246.40, 110.84, 1003.22);
    SetPlayerInterior(playerid, 10);
    SendGameMessage(GAME_MODE_LOBBY, COLOR_RED, "[ # ]{AFAFAF} %s has joined copchase (/copchase)", ReturnName(playerid));
    return 1;
}

/*cmd:nocopchase(playerid, params[])
{
    switch(PlayerInfo[playerid][NoCopchase])
    {
        case 0:
        {
            PlayerInfo[playerid][NoCopchase] = 1;
            SendClientMessage(playerid, COLOR_RED, "You disabled auto joining copchase.");
        }
        case 1:
        {
            PlayerInfo[playerid][NoCopchase] = 0;
            SendClientMessage(playerid, COLOR_GREEN, "You enabled auto joining copchase.");
        }
    }
    return 1;
}*/

cmd:tdm(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_LOBBY) return SendErrorMessage(playerid, "You must be in lobby to use this command.");

    if(PlayerInfo[playerid][Team] != -1) return SendErrorMessage(playerid, "An error has occured.");

    SendGameMessage(GAME_MODE_LOBBY, COLOR_RED, "[ # ]{AFAFAF} %s has joined TDM (/tdm)", ReturnName(playerid));
	SendGameMessage(GAME_MODE_DM, COLOR_RED, "[ # ]{AFAFAF} %s has joined TDM (/tdm)", ReturnName(playerid));
    SendPlayerToTeamSelection(playerid);
    return 1;
}

cmd:dm(playerid, params[])
{
    new temp_str[1096], primary_str[1096], arena;

    if(GetPlayerGameMode(playerid) != GAME_MODE_LOBBY) return SendErrorMessage(playerid, "You must be in lobby to use this command.");

    if(PlayerInfo[playerid][DM] != -1) return SendErrorMessage(playerid, "An error has occured.");

    if(sscanf(params, "s[32]", arena))
    {
        strcat(primary_str, "Arena\tPlayers\n");
        format(temp_str, sizeof temp_str, "LVPD\t%d/10\n", GameInfo[0][GamePlayers]);
        strcat(primary_str, temp_str);
        format(temp_str, sizeof temp_str, "Warehouse\t%d/10\n", GameInfo[1][GamePlayers]);
        strcat(primary_str, temp_str);
        format(temp_str, sizeof temp_str, "Warehouse 2\t%d/10\n", GameInfo[2][GamePlayers]);
        strcat(primary_str, temp_str);
        format(temp_str, sizeof temp_str, "RC Battleground\t%d/10\n", GameInfo[3][GamePlayers]);
        strcat(primary_str, temp_str);
        format(temp_str, sizeof temp_str, "Ghost Town (headshot only)\t%d/10\n", GameInfo[4][GamePlayers]);
        strcat(primary_str, temp_str);
        ShowPlayerDialog(playerid, DIALOG_ARENA, DIALOG_STYLE_TABLIST_HEADERS, "DM Arena", primary_str, "Join", "Cancel");
    }
    else
    {
        if(strcmp(params, "lvpd", true) == 0 || strcmp(params, "1", true) == 0)
        {
            if((GameInfo[GAME_LVPD][GamePlayers] + 1) >= 10) return SendErrorMessage(playerid, "Max arena player reached. Returning to lobby.");

            SendGameMessage(GAME_MODE_LOBBY, COLOR_RED, "[ # ]{AFAFAF} %s has joined DM arena (/dm 1)", ReturnName(playerid));
	        SendGameMessage(GAME_MODE_DM, COLOR_RED, "[ # ]{AFAFAF} %s has joined DM arena (/dm 1)", ReturnName(playerid));
			SendPlayerToDMArena(playerid, GAME_LVPD);
        }
        else if(strcmp(params, "warehouse", true) == 0 || strcmp(params, "2", true) == 0)
        {
            if((GameInfo[GAME_WAREHOUSE][GamePlayers] + 1) >= 10) return SendErrorMessage(playerid, "Max arena player reached. Returning to lobby.");

            SendGameMessage(GAME_MODE_LOBBY, COLOR_RED, "[ # ]{AFAFAF} %s has joined DM arena (/dm 2)", ReturnName(playerid));
	        SendGameMessage(GAME_MODE_DM, COLOR_RED, "[ # ]{AFAFAF} %s has joined DM arena (/dm 2)", ReturnName(playerid));
			SendPlayerToDMArena(playerid, GAME_WAREHOUSE);
        }
        else if(strcmp(params, "warehouse2", true) == 0 || strcmp(params, "3", true) == 0)
        {
            if((GameInfo[GAME_WAREHOUSE2][GamePlayers] + 1) >= 10) return SendErrorMessage(playerid, "Max arena player reached. Returning to lobby.");

            SendGameMessage(GAME_MODE_LOBBY, COLOR_RED, "[ # ]{AFAFAF} %s has joined DM arena (/dm 3)", ReturnName(playerid));
	        SendGameMessage(GAME_MODE_DM, COLOR_RED, "[ # ]{AFAFAF} %s has joined DM arena (/dm 3)", ReturnName(playerid));
			SendPlayerToDMArena(playerid, GAME_WAREHOUSE2);
        }
        else if(strcmp(params, "battleground", true) == 0 || strcmp(params, "4", true) == 0)
        {
            if((GameInfo[GAME_RCBG][GamePlayers] + 1) >= 10) return SendErrorMessage(playerid, "Max arena player reached. Returning to lobby.");

            SendGameMessage(GAME_MODE_LOBBY, COLOR_RED, "[ # ]{AFAFAF} %s has joined DM arena (/dm 4)", ReturnName(playerid));
	        SendGameMessage(GAME_MODE_DM, COLOR_RED, "[ # ]{AFAFAF} %s has joined DM arena (/dm 4)", ReturnName(playerid));
			SendPlayerToDMArena(playerid, GAME_RCBG);
        }
        else if(strcmp(params, "gt", true) == 0 || strcmp(params, "5", true) == 0)
        {
            if((GameInfo[GAME_GHOST_TOWN][GamePlayers] + 1) >= 10) return SendErrorMessage(playerid, "Max arena player reached. Returning to lobby.");

            SendGameMessage(GAME_MODE_LOBBY, COLOR_RED, "[ # ]{AFAFAF} %s has joined DM arena (/dm 5)", ReturnName(playerid));
	        SendGameMessage(GAME_MODE_DM, COLOR_RED, "[ # ]{AFAFAF} %s has joined DM arena (/dm 5)", ReturnName(playerid));
			SendPlayerToDMArena(playerid, GAME_GHOST_TOWN);
        }
    }
    return 1;
}

cmd:freeroam(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_LOBBY) return SendErrorMessage(playerid, "You must be in lobby to use this command.");

    SendPlayerToFreeroam(playerid);
    return 1;
}

CMD:signature(playerid)
{
	if(PlayerInfo[playerid][b_Toggled])
	{
		ShowPlayerSignature(playerid, playerid);
		ShowPlayerDialog(playerid, DIALOG_SIGN, DIALOG_STYLE_LIST, "Signature", "Disable signature\nEdit Signature\nRestore Default Settings", "Select", "Close");
	}
	else
	{
		HidePlayerSignature(playerid);
		ShowPlayerDialog(playerid, DIALOG_SIGN, DIALOG_STYLE_LIST, "Signature", "Enable signature", "Select", "Close");
	}
	return 1;
}

cmd:stats(playerid, params[]) 
{
    new targetid;
    if(PlayerInfo[playerid][AdminLevel] >= 3)
    {
        if(sscanf(params, "u", targetid))
        {
            ShowPlayerStats(playerid, playerid);
        }
        else
        {
            if(!IsPlayerConnected(targetid)) return 1;
            if(PlayerInfo[targetid][IsLoggedIn] == false) return 1;

            ShowPlayerStats(playerid, targetid);
        }
    }
    else
    {
        ShowPlayerStats(playerid, playerid);
    }
    return 1;
}

cmd:radio(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_TDM && GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command is only available in TDM.");

    if(PlayerInfo[playerid][Muted]) return SendErrorMessage(playerid, "You're muted!");

    if(sscanf(params, "s[256]", params)) return SendUsageMessage(playerid, "/radio [message]");

    if(GetPlayerGameMode(playerid) == GAME_MODE_TDM)
    {
        foreach(new i : Player)
        {
            if(GetPlayerGameMode(i) == GAME_MODE_TDM && PlayerInfo[i][Team] == PlayerInfo[playerid][Team])
            {
                SendClientMessageEx(i, ReturnTeamColorEx(PlayerInfo[playerid][Team]), "** %s %s: %s", ReturnTeamTag(PlayerInfo[playerid][Team]), PlayerInfo[playerid][Name], params);
            }
        }
    }
    else if(GetPlayerGameMode(playerid) == GAME_MODE_COPCHASE)
    {
        switch(PlayerInfo[playerid][CC_Team])
        {
            case TEAM_LSPD:
            {
                foreach(new i : Player)
                {
                    if(GetPlayerGameMode(i) == GAME_MODE_COPCHASE && PlayerInfo[i][CC_Team] == PlayerInfo[playerid][CC_Team])
                    {
                        SendClientMessageEx(i, COLOR_RADIO, "** [CH: 911 S: 1] %s: %s",  PlayerInfo[playerid][Name], params);
                    }
                }
            }
            case TEAM_FUGITIVE:
            {
                foreach(new i : Player)
                {
                    if(GetPlayerGameMode(i) == GAME_MODE_COPCHASE && PlayerInfo[i][CC_Team] == PlayerInfo[playerid][CC_Team])
                    {
                        SendClientMessageEx(i, COLOR_RADIO, "** [CH: 7398 S: 1] %s: %s",  PlayerInfo[playerid][Name], params);
                    }
                }
            }
        }
    }
    return 1;
}

cmd:tag(playerid, params[])
{
    new text[32], color[32];
    if(!PlayerInfo[playerid][Donator]) return SendErrorMessage(playerid, "You can't use this command.");

    if(sscanf(params, "s[32]s[32]", color, text)) return SendUsageMessage(playerid, "/tag [color in hex] [text (\"none\" to reset)]");

    if(strcmp(text, "none", true) == 0)
    {
        format(PlayerInfo[playerid][DonatorTag], 32, "");
        SendClientMessageEx(playerid, -1, "You've changed removed your tag");
    }
    else
    {
        format(PlayerInfo[playerid][DonatorTag], 32, "{%s}%s", color, text);
        SendClientMessageEx(playerid, -1, "You've changed your donator tag to \"{%s}%s\"", color, text);
    }
    return 1;
}

cmd:global(playerid, params[])
{
    if(GlobalChat == false && !PlayerInfo[playerid][AdminLevel]) return SendErrorMessage(playerid, "The global chat has been disabled by admin.");

    if(PlayerInfo[playerid][Muted]) return SendErrorMessage(playerid, "You're muted!");

    if(isnull(params)) return SendUsageMessage(playerid, "/global [message]");

    if(PlayerInfo[playerid][AdminDuty]) SendClientMessageToAllEx(COLOR_RED, "[GLOBAL] %s %s: %s", ReturnAdminRank(playerid), PlayerInfo[playerid][Name], params);
    else 
    {
        if(PlayerInfo[playerid][Donator])
        {
            if(isnull(PlayerInfo[playerid][DonatorTag])) SendClientMessageToAllEx(-1, "[GLOBAL] %s {%06x}%s{FFFFFF}: %s", ReturnDonorRank(playerid), GetPlayerColor(playerid) >>> 8, PlayerInfo[playerid][Name], params);
            else SendClientMessageToAllEx(-1, "[GLOBAL] %s %s{FFFFFF}: %s", PlayerInfo[playerid][DonatorTag], PlayerInfo[playerid][Name], params);
        }
        else SendClientMessageToAllEx(-1, "[GLOBAL] {%06x}%s{FFFFFF}: %s", GetPlayerColor(playerid) >>> 8, PlayerInfo[playerid][Name], params);
    }
    return 1;
}

cmd:pm(playerid, params[])
{
	new
		playerb,
		text[144]
	;

    if(PlayerInfo[playerid][Muted]) return SendErrorMessage(playerid, "You're muted!");
		
	if(sscanf(params, "us[156]", playerb, text))
		return SendUsageMessage(playerid, "/pm [playerid] [message]");
		
	if(!IsPlayerConnected(playerb))
		return SendErrorMessage(playerid, "The player you specified isn't connected.");
		
	if(PlayerInfo[playerid][AdminDuty])
	{
		SendClientMessageEx(playerb, COLOR_PMRECEIVED, "Private message from {FF9900}%s{FFDC18}(%d):{FFFFFF} %s", PlayerInfo[playerid][Name], playerid, text); 
        GameTextForPlayer(playerb, "~y~~n~~n~~n~~n~~n~~n~~n~~n~~n~NEW MESSAGE!", 1500, 3);
        PlayerPlaySound(playerb, 1052, 0.0, 0.0, 0.0);
		
		if(!PlayerInfo[playerb][AdminDuty])
			SendClientMessageEx(playerid, COLOR_PMSENT, "Private message sent to %s(%d):{FFFFFF} %s", PlayerInfo[playerb][Name], playerb, text); 
			
		else SendClientMessageEx(playerid, COLOR_PMSENT, "Private message sent to {FF9900}%s{EEE854}(%d):{FFFFFF} %s", PlayerInfo[playerb][Name], playerb, text); 
	}
	else
	{
		if(PlayerInfo[playerb][AdminDuty])
		{
			SendClientMessageEx(playerb, COLOR_PMRECEIVED, "Private message from %s(%d):{FFFFFF} %s", PlayerInfo[playerid][Name], playerid, text); 
			SendClientMessageEx(playerid, COLOR_PMSENT, "Private message sent to {FF9900}%s{EEE854}(%d):{FFFFFF} %s", PlayerInfo[playerb][Name], playerb, text); 

            GameTextForPlayer(playerb, "~y~~n~~n~~n~~n~~n~~n~~n~~n~~n~NEW MESSAGE!", 1500, 3);
            PlayerPlaySound(playerb, 1052, 0.0, 0.0, 0.0);
		}
		else
		{
			SendClientMessageEx(playerb, COLOR_PMRECEIVED, "Private message from %s(%d):{FFFFFF} %s", PlayerInfo[playerid][Name], playerid, text); 
			SendClientMessageEx(playerid, COLOR_PMSENT, "Private message sent to %s(%d):{FFFFFF} %s", PlayerInfo[playerb][Name], playerb, text); 

            GameTextForPlayer(playerb, "~y~~n~~n~~n~~n~~n~~n~~n~~n~~n~NEW MESSAGE!", 1500, 3);
            PlayerPlaySound(playerb, 1052, 0.0, 0.0, 0.0);   
		}
	}
	return 1;
}

cmd:report(playerid, params[])
{
    new reason[128];

    if(sscanf(params, "s[128]", reason)) return SendUsageMessage(playerid, "/report [reason]");

    if(strlen(reason) < 3) return SendErrorMessage(playerid, "Please write an actual message.");

    new idx, bool:reported;

    for(new i = 1; i < MAX_REPORTS; i++)
    {
        if(ReportInfo[i][rID] && ReportInfo[i][rIssuer] == playerid)
        {
            reported = true;
            break;
        }
    }
	
    for (new i = 1; i < MAX_REPORTS; i ++)
    {
        if (!ReportInfo[i][rID])
        {
            idx = i;
            break; 
        }
    }

    if(!idx) return 1;

    if(reported == true) return SendErrorMessage(playerid, "You already have an active report.");

    ReportInfo[idx][rID] = idx;
    ReportInfo[idx][rIssuer] = playerid;
    ReportInfo[idx][rDate] = ReturnDate();
    format(ReportInfo[idx][rReason], 128, reason);

    SendClientMessageEx(playerid, COLOR_RED, "Your report was sent to all online administrators.");
    SendClientMessageEx(playerid, COLOR_GREY, "\"%s\"", reason);

    foreach (new i : Player) 
	{
		if (PlayerInfo[i][AdminLevel])
		{
			SendClientMessageEx(i, COLOR_RED, "[Report #%i] %s (%i): \"%s\"", idx, ReturnName(playerid, false), playerid, reason);
		}
	}
    return 1;
}

cmd:acceptdeath(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_TDM && GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command is only available in TDM/Copchase.");

    if(PlayerInfo[playerid][State] != PLAYER_STATE_WOUNDED) return SendErrorMessage(playerid, "You aren't wounded!");

    if(PlayerInfo[playerid][DeathTime]) return SendErrorMessage(playerid, "You have to wait for %d second(s) to be able to respawn.", PlayerInfo[playerid][DeathTime]);

    if(GetPlayerGameMode(playerid) == GAME_MODE_TDM)
    {
        ResetPlayerWeapons(playerid);
        PlayerInfo[playerid][State] = PLAYER_STATE_ALIVE;
        PlayerInfo[playerid][DeathTime] = 0;
        if(!PlayerHasPerk(playerid, 2)) SetPlayerHP(playerid, 150.0);
	    else SetPlayerHP(playerid, 200.0);

        SetPlayerChatBubble(playerid, "(( Respawned ))", COLOR_GREY, 30.0, 500); 

        ClearAnimations(playerid);

        SetPlayerInterior(playerid, 0);
        SetPlayerVirtualWorld(playerid, 0);

        PlayerInfo[playerid][KillStreak] = 0;

        PlayerInfo[playerid][MedkitTime] = 0;

        ClearPlayerDamages(playerid);

        SetPlayerPos(playerid, PlayerInfo[playerid][X_Pos], PlayerInfo[playerid][Y_Pos], PlayerInfo[playerid][Z_Pos]);
        SetPlayerFacingAngle(playerid, PlayerInfo[playerid][A_Pos]);

        SetCameraBehindPlayer(playerid);

        SetPlayerSkin(playerid, PlayerInfo[playerid][Skin]);

        SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, 0);
        SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, 0);
        SetPlayerSkillLevel(playerid, WEAPONSKILL_SPAS12_SHOTGUN, 0);

        if (PlayerInfo[playerid][Team] == -1) 
        {
            SendPlayerToTeamSelection(playerid);
        }
        else
        {
            SetPlayerPosition(playerid);
        }
    }
    else if(GetPlayerGameMode(playerid) == GAME_MODE_COPCHASE)
    {
        RemovePlayerFromCopchase(playerid);
    }
    return 1;
}

cmd:changeteam(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_TDM) return SendErrorMessage(playerid, "This command is only available in TDM.");

    if(gettime() - PlayerInfo[playerid][LastDamage] < 10) return SendErrorMessage(playerid, "You took damage recently."); 

    if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE) return SendErrorMessage(playerid, "You can't do this right now!");

    if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "You can't do this right now!");

    ResetPlayerWeapons(playerid);
    ClearAnimations(playerid);
    PlayerInfo[playerid][State] = PLAYER_STATE_ALIVE;
    SetPlayerHP(playerid, 100);
    PlayerInfo[playerid][KillStreak] = 0;

    PlayerInfo[playerid][Team] = -1;

    PlayerInfo[playerid][MedkitTime] = 0;

    SetPlayerName(playerid, PlayerInfo[playerid][Name]);
    SetPlayerColor(playerid, COLOR_GREY);

    SelectTextDraw(playerid, 0xAFAFAFFF);

    SendPlayerToTeamSelection(playerid);
    return 1;
}

cmd:skin(playerid, params[])
{
    if(!PlayerInfo[playerid][Team]) return 1;

    if(GetPlayerGameMode(playerid) == GAME_MODE_TDM) ShowPlayerDialog(playerid, DIALOG_SKIN, DIALOG_STYLE_LIST, "Skin Changer", "Team skin", "Proceed", "Cancel");
    else if(GetPlayerGameMode(playerid) == GAME_MODE_LOBBY || GetPlayerGameMode(playerid) == GAME_MODE_FREEROAM)
    {
        new item[32];
        if(sscanf(params, "s[32]", item)) return SendUsageMessage(playerid, "/skin [lobby/cop]");
        if(strcmp(params, "cop", true) == 0)
        {
            ShowModelSelectionMenu(playerid, "Cop Skins", MODEL_SELECTION_COP_SKIN, lspdSkin, sizeof(lspdSkin), -16.0, 0.0, -55.0);
        }
        else if(strcmp(params, "lobby", true) == 0)
        {
            ShowModelSelectionMenu(playerid, "Lobby/Fugitive Skins", MODEL_SELECTION_SAMP_SKIN, gSAMPSkins, sizeof(gSAMPSkins), -16.0, 0.0, -55.0);
        }
    }
    return 1;
}

cmd:revive(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_TDM && GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command is only available in TDM.");

    new target;
    if(PlayerHasPerk(playerid, 4) && PlayerInfo[playerid][MedkitRevive] < 3 && PlayerInfo[playerid][Medkit])
    {
        if(sscanf(params, "u", target)) return SendUsageMessage(playerid, "/revive [targetid]");
        if(!IsPlayerConnected(target)) return SendErrorMessage(playerid, "The player you specified isn't connected.");
        if(!PlayerInfo[target][IsLoggedIn]) return SendErrorMessage(playerid, "The player you specified isn't logged in.");
        if(PlayerInfo[playerid][Team] != PlayerInfo[target][Team] && PlayerInfo[playerid][CC_Team] != PlayerInfo[target][CC_Team]) return SendErrorMessage(playerid, "That player isn't in your team.");
        if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE) return SendErrorMessage(playerid, "You must be alive to be able to use this command.");
        if(!IsPlayerNearPlayer(playerid, target, 5.0)) return SendErrorMessage(playerid, "You aren't near that player."); 
        if(PlayerInfo[target][State] != PLAYER_STATE_WOUNDED) return SendErrorMessage(playerid, "That player is alive.");
        if(PlayerInfo[playerid][Reviving] != -1) return SendErrorMessage(playerid, "You're reviving someone.");

        TogglePlayerControllable(playerid, false);
        PlayerInfo[playerid][Reviving] = target;
        PlayerInfo[playerid][RevivingTime] = 1;

        PlayerInfo[playerid][MedkitRevive]++;	
        PlayerInfo[playerid][Medkit]--;	

        SendClientMessageEx(playerid, COLOR_ACTION, "Attempting to revive %s.", PlayerInfo[target][Name]);
        SendClientMessageEx(target, COLOR_ACTION, "You're being revived by %s.", PlayerInfo[playerid][Name]);

        ApplyAnimationEx(playerid, "MEDIC","CPR", 4.1, 0, 1, 1, 1, 0, 1);

        GameTextForPlayer(playerid, "~r~~n~~n~~n~~n~~n~~n~~n~~n~~n~Reviving..", 2000, 3);	
    }
    else
    {
        if(sscanf(params, "u", target)) return SendUsageMessage(playerid, "/revive [targetid]");
        if(!IsPlayerConnected(target)) return SendErrorMessage(playerid, "The player you specified isn't connected.");
        if(!PlayerInfo[target][IsLoggedIn]) return SendErrorMessage(playerid, "The player you specified isn't logged in.");
        if(PlayerInfo[playerid][Team] != PlayerInfo[target][Team] && PlayerInfo[playerid][CC_Team] != PlayerInfo[target][CC_Team]) return SendErrorMessage(playerid, "That player isn't in your team.");
        if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE) return SendErrorMessage(playerid, "You must be alive to be able to use this command.");
        if(!IsPlayerNearPlayer(playerid, target, 5.0)) return SendErrorMessage(playerid, "You aren't near that player."); 
        if(PlayerInfo[target][State] != PLAYER_STATE_WOUNDED) return SendErrorMessage(playerid, "That player is alive.");
        if(PlayerInfo[playerid][Reviving] != -1) return SendErrorMessage(playerid, "You're reviving someone.");

        TogglePlayerControllable(playerid, false);
        PlayerInfo[playerid][Reviving] = target;
        PlayerInfo[playerid][RevivingTime] = 6;

        SendClientMessageEx(playerid, COLOR_ACTION, "Attempting to revive %s.", PlayerInfo[target][Name]);
        SendClientMessageEx(target, COLOR_ACTION, "You're being revived by %s.", PlayerInfo[playerid][Name]);

        ApplyAnimationEx(playerid, "MEDIC","CPR", 4.1, 0, 1, 1, 1, 0, 1);

        GameTextForPlayer(playerid, "~r~~n~~n~~n~~n~~n~~n~~n~~n~~n~Reviving..", 5000, 3);	
    }
    return 1;
}

cmd:selfrevive(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_TDM && GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command is only available in TDM.");

    if(!PlayerHasPerk(playerid, 1)) return SendErrorMessage(playerid, "You can't use this!");
 
    if(PlayerInfo[playerid][State] == PLAYER_STATE_ALIVE) return SendErrorMessage(playerid, "You aren't dead!");

    if(random(100) >= 50) return SendErrorMessage(playerid, "Try again later!");

    if(PlayerInfo[playerid][SelfRevive]) return SendErrorMessage(playerid, "You can only use this command one time!");

    new bool:playerFound = false;

    foreach(new i : Player)
    {
        if(IsPlayerNearPlayer(playerid, i, 25.0)) 
        {
            playerFound = true;
            break;
        }
    }

    if(playerFound == true) return SendErrorMessage(playerid, "Can't use this when there are players nearby!");

    SendNearbyMessage(playerid, 30.0, 0xC2A2DAFF, "* %s {C2A2DA}has revived themselves.", ReturnName(playerid));

    PlayerInfo[playerid][SelfRevive] = 1;	

    if(IsPlayerInAnyVehicle(playerid)) RemovePlayerFromVehicle(playerid);

    PlayerInfo[playerid][State] = PLAYER_STATE_ALIVE;
    ClearAnimations(playerid);
    TogglePlayerControllable(playerid, true);

    SetPlayerChatBubble(playerid, "(( Used /selfrevive ))", COLOR_GREY, 30.0, 500); 

    SendClientMessage(playerid, COLOR_RED, "You are revived!");
    return 1;
}

cmd:mask(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_TDM && GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command is only available in TDM.");

    if(!PlayerHasPerk(playerid, 5)) return SendErrorMessage(playerid, "You can't use this!");

    if(PlayerInfo[playerid][Mask]) return SendErrorMessage(playerid, "You already used this command recently, try again later!");
 
    if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE) return SendErrorMessage(playerid, "You can't use this while dead!");

    SendNearbyMessage(playerid, 30.0, 0xC2A2DAFF, "* %s {C2A2DA}has their mask on.", ReturnName(playerid));

    PlayerInfo[playerid][Mask] = 60;	

    GameTextForPlayer(playerid, "~P~MASK ON", 3000, 4);
    return 1;
}

cmd:fixveh(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_TDM && GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command is only available in TDM.");

    if(!PlayerHasPerk(playerid, 6)) return SendErrorMessage(playerid, "You can't use this!");

    if(PlayerInfo[playerid][FixVeh]) return SendErrorMessage(playerid, "You already used this command recently, try again later!");
 
    if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE) return SendErrorMessage(playerid, "You can't use this while dead!");

    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "You must be in a vehicle.");

    SendNearbyMessage(playerid, 30.0, -1, "{FFFF00}%s {FFFFFF}has fixed their vehicle.", ReturnName(playerid));

    PlayerInfo[playerid][FixVeh] = 1;

    RepairVehicle(GetPlayerVehicleID(playerid));

    GameTextForPlayer(playerid, "~G~VEHICLE FIXED", 3000, 4);
    return 1;
}

cmd:flipveh(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_TDM && GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command is only available in TDM.");

    if(!PlayerHasPerk(playerid, 6)) return SendErrorMessage(playerid, "You can't use this!");

    if(PlayerInfo[playerid][FlipVeh]) return SendErrorMessage(playerid, "You already used this command recently, try again later!");
 
    if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE) return SendErrorMessage(playerid, "You can't use this while dead!");

    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) return SendErrorMessage(playerid, "You must be in a vehicle.");

    SendNearbyMessage(playerid, 30.0, -1, "{FFFF00}%s {FFFFFF}has flipped their vehicle.", ReturnName(playerid));

    PlayerInfo[playerid][FlipVeh] = 1;

    new Float:vPos[3];

    GetVehiclePos(GetPlayerVehicleID(playerid),vPos[0], vPos[1], vPos[2]);
    SetVehiclePos(GetPlayerVehicleID(playerid),vPos[0], vPos[1], vPos[2]);

    GameTextForPlayer(playerid, "~G~VEHICLE FLIPPED", 3000, 4);
    return 1;
}

CMD:weapon(playerid, params[]) 
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_TDM && GetPlayerGameMode(playerid) != GAME_MODE_FREEROAM) return SendErrorMessage(playerid, "This command is only available in TDM/Freeroam.");

    if(GetPlayerGameMode(playerid) == GAME_MODE_TDM)
    {
        new string[256];
        format(string, sizeof string, "Slot\tCurrent Weapon\nWeapon slot 1\t%s\nWeapon slot 2\t%s", GetWeaponNameEx(PlayerInfo[playerid][Weapon][0]), GetWeaponNameEx(PlayerInfo[playerid][Weapon][1]));
        ShowPlayerDialog(playerid, DIALOG_WEAPON, DIALOG_STYLE_TABLIST_HEADERS, "Weapon Loadout", string, "Edit", "Close");
    }
    else if(GetPlayerGameMode(playerid) == GAME_MODE_FREEROAM)
    {
        new weaponid;
        if(sscanf(params, "i", weaponid)) return SendUsageMessage(playerid, "/weapon [weaponid]");

        if(weaponid < 1 || weaponid > 46) return SendErrorMessage(playerid, "Invalid weapon ID!");

        GivePlayerWeapon(playerid, weaponid, 500);
        SendClientMessageEx(playerid, COLOR_CYAN, "You've been given a %s.", ReturnWeaponNameEx(weaponid));
    }
    return 1;
}

CMD:health(playerid, params[]) 
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_FREEROAM) return SendErrorMessage(playerid, "This command is only available in Freeroam.");

    new Float:health;
    if(sscanf(params, "f", health)) return SendUsageMessage(playerid, "/health [amount]");

    if(health < 1.0 || health > 100.0) return SendErrorMessage(playerid, "Invalid input!");

    SetPlayerHealth(playerid, health);
    SendClientMessageEx(playerid, COLOR_CYAN, "You've set your HP to %.2f.", health);
    return 1;
}

CMD:armour(playerid, params[]) 
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_FREEROAM) return SendErrorMessage(playerid, "This command is only available in Freeroam.");
    
    new Float:armour;
    if(sscanf(params, "f", armour)) return SendUsageMessage(playerid, "/armour [amount]");

    if(armour < 0.0 || armour > 100.0) return SendErrorMessage(playerid, "Invalid input!");

    SetPlayerArmour(playerid, armour);
    SendClientMessageEx(playerid, COLOR_CYAN, "You've set your armour to %.2f.", armour);
    return 1;
}

CMD:vehicle(playerid, params[]) 
{
    new modelid = 0, Float:Pos[4];
    if(GetPlayerGameMode(playerid) != GAME_MODE_FREEROAM) return SendErrorMessage(playerid, "This command is only available in Freeroam.");

    if(isnull(params)) return SendUsageMessage(playerid, "/vehicle [modelid/name]");

    if(!isNumeric(params))
    {
        for(new i = 0; i < sizeof(vehName); i ++)
	    {
	        if(strfind(params, vehName[i], true)) continue;
	        modelid = i + 400;
	        break;
	    }

        if(modelid == 0) return SendErrorMessage(playerid, "Invalid model ID!");

        if(IsValidVehicle(PlayerInfo[playerid][FreeroamVehicle])) 
        {
            DestroyVehicle(PlayerInfo[playerid][FreeroamVehicle]);
            PlayerInfo[playerid][FreeroamVehicle] = INVALID_VEHICLE_ID;
        }

        GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);
        GetPlayerFacingAngle(playerid, Pos[3]);
        PlayerInfo[playerid][FreeroamVehicle] = CreateVehicle(modelid, Pos[0], Pos[1], Pos[2], Pos[3], random(100), random(100), -1, 0);
        SetVehicleVirtualWorld(PlayerInfo[playerid][FreeroamVehicle], GetPlayerVirtualWorld(playerid));
        PutPlayerInVehicle(playerid, PlayerInfo[playerid][FreeroamVehicle], 0);
        SendClientMessageEx(playerid, COLOR_CYAN, "You've spawned a %s.", GetVehicleNameByModel(modelid));
    }
    else
    {
        modelid = strval(params);

        if(modelid < 400 || modelid > sizeof(vehName)) return SendErrorMessage(playerid, "Invalid model ID!");

        if(IsValidVehicle(PlayerInfo[playerid][FreeroamVehicle])) 
        {
            DestroyVehicle(PlayerInfo[playerid][FreeroamVehicle]);
            PlayerInfo[playerid][FreeroamVehicle] = INVALID_VEHICLE_ID;
        }

        GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);
        GetPlayerFacingAngle(playerid, Pos[3]);
        PlayerInfo[playerid][FreeroamVehicle] = CreateVehicle(modelid, Pos[0], Pos[1], Pos[2], Pos[3], random(100), random(100), -1, 0);
        SetVehicleVirtualWorld(PlayerInfo[playerid][FreeroamVehicle], GetPlayerVirtualWorld(playerid));
        PutPlayerInVehicle(playerid, PlayerInfo[playerid][FreeroamVehicle], 0);
        SendClientMessageEx(playerid, COLOR_CYAN, "You've spawned a %s.", GetVehicleNameByModel(modelid));
    }
    return 1;
}

CMD:world(playerid, params[]) 
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_FREEROAM) return SendErrorMessage(playerid, "This command is only available in Freeroam.");

    new worldid;
    if(sscanf(params, "i", worldid)) return SendUsageMessage(playerid, "/weapon [weaponid]");

    if(worldid < 256 || worldid > 456) return SendErrorMessage(playerid, "Invalid world ID! (256 - 456)");

    SetPlayerVirtualWorld(playerid, 100 + worldid);
    SendClientMessageEx(playerid, COLOR_CYAN, "You've set your world to %i.", worldid);

    if(IsValidVehicle(PlayerInfo[playerid][FreeroamVehicle])) 
    {
        DestroyVehicle(PlayerInfo[playerid][FreeroamVehicle]);
        PlayerInfo[playerid][FreeroamVehicle] = INVALID_VEHICLE_ID;
    }
    return 1;
}

CMD:damages(playerid, params[])
{
    new targetid, caption_str[64], temp_str[256], primary_str[500], dmgs;
    if(sscanf(params, "u", targetid))
    {
        for(new i; i < MAX_DAMAGES; i++)
        {
            if(DamageInfo[i][dmgExists] == true)
            {
                if(DamageInfo[i][dmgDamaged] == playerid)
                {
                    new weaponName[32];
                    GetWeaponName(DamageInfo[i][dmgWeapon], weaponName, sizeof weaponName);
                    format(temp_str, sizeof temp_str, "%.2f from %s (%s) bodypart: %s\n", DamageInfo[i][dmgAmount], DamageInfo[i][dmgIssuer], weaponName, GetBodyPartName(DamageInfo[i][dmgBody]));
                    strcat(primary_str, temp_str);
                    dmgs++;
                }
            }
        }
        format(caption_str, sizeof caption_str, "%s's damage", PlayerInfo[playerid][Name]);
        if(dmgs)
        {
            ShowPlayerDialog(playerid, 1234, DIALOG_STYLE_LIST, caption_str, primary_str, "Close", "");
        }
        else ShowPlayerDialog(playerid, 1234, DIALOG_STYLE_LIST, caption_str, "No damages were displayed.", "Close", "");
    }
    else
    {
        for(new i; i < MAX_DAMAGES; i++)
        {
            if(DamageInfo[i][dmgExists] == true)
            {
                if(DamageInfo[i][dmgDamaged] == targetid)
                {
                    new weaponName[32];
                    GetWeaponName(DamageInfo[i][dmgWeapon], weaponName, sizeof weaponName);
                    format(temp_str, sizeof temp_str, "%.2f from %s (%s) bodypart: %s\n", DamageInfo[i][dmgAmount], DamageInfo[i][dmgIssuer], weaponName, GetBodyPartName(DamageInfo[i][dmgBody]));
                    strcat(primary_str, temp_str);
                    dmgs++;
                }
            }
        }
        format(caption_str, sizeof caption_str, "%s's damage", PlayerInfo[targetid][Name]);
        if(dmgs)
        {
            ShowPlayerDialog(playerid, 1234, DIALOG_STYLE_LIST, caption_str, primary_str, "Close", "");
        }
        else ShowPlayerDialog(playerid, 1234, DIALOG_STYLE_LIST, caption_str, "No damages were displayed.", "Close", "");
    }
    return 1;
}

cmd:buy(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_TDM && GetPlayerGameMode(playerid) != GAME_MODE_LOBBY) return SendErrorMessage(playerid, "This command is only available in TDM / lobby.");

    new item[32];
    if(sscanf(params, "s[32]", item)) return SendUsageMessage(playerid, "/buy [medkit]");

    if(strcmp(params, "medkit", true) == 0)
    {
        if(PlayerInfo[playerid][Medkit] >= GetMedkitLimit(playerid)) return SendErrorMessage(playerid, "You can only hold %i medkits.", GetMedkitLimit(playerid));

        if(PlayerInfo[playerid][Cash] < 5) return SendErrorMessage(playerid, "You don't have enough money.");

        PlayerInfo[playerid][Cash] -= 5;
        PlayerInfo[playerid][Medkit]++;
        SendClientMessage(playerid, COLOR_GREEN, "You bought a medkit for $5!");
    }
    return 1;
}

cmd:medkit(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_TDM && GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command is only available in TDM.");

    if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE) return SendErrorMessage(playerid, "You must be alive to be able to use this command.");
    if(GetPlayerSpecialAction(playerid) != SPECIAL_ACTION_DUCK) return SendErrorMessage(playerid, "You must be crouched."); 
    if(PlayerInfo[playerid][Reviving] != -1) return SendErrorMessage(playerid, "You're reviving someone.");
    if(!PlayerInfo[playerid][Medkit]) return SendErrorMessage(playerid, "You don't have a medkit.");
    if(PlayerInfo[playerid][MedkitTime]) return 1;

    TogglePlayerControllable(playerid, false);
    PlayerInfo[playerid][Medkit]--;
    PlayerInfo[playerid][MedkitTime] = 9;

    SendClientMessageEx(playerid, COLOR_ACTION, "Using a medkit. Please wait..", PlayerInfo[playerid][Name]);

    ApplyAnimationEx(playerid, "MEDIC","CPR", 4.1, 0, 1, 1, 1, 0, 1);	

    GameTextForPlayer(playerid, "~r~~n~~n~~n~~n~~n~~n~~n~~n~~n~Using Medkit..", 8000, 3);
    return 1;
}

cmd:setarmed(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_TDM && GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command is only available in TDM.");

    if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE) return SendErrorMessage(playerid, "You must be alive to be able to use this command.");

    if(GetPlayerState(playerid) != PLAYER_STATE_PASSENGER) return SendErrorMessage(playerid, "Only passenger can do this!");

    new weapons[13][2], temp_str[128], primary_str[256];

    strcat(primary_str, "Weapon\tAmmo\n");
    strcat(primary_str, "{FF6347}Remove Weapon{ADC3E7}\n");
    for (new i = 0; i <= 12; i++)
    {
        GetPlayerWeaponData(playerid, i, weapons[i][0], weapons[i][1]);
        if(weapons[i][0] != 0 && weapons[i][0] != 24)
        {
            new wepName[32];
            GetWeaponName(weapons[i][0], wepName, sizeof wepName);
            format(temp_str, sizeof temp_str, "%s\t%d\n", wepName, weapons[i][1]);
            strcat(primary_str, temp_str);
        }
    }
    ShowPlayerDialog(playerid, DIALOG_WEAPON_SWITCH, DIALOG_STYLE_TABLIST_HEADERS, "Weapon Switch", primary_str, "Select", "Cancel");
    return 1;
}

CMD:pay(playerid, params[])
{
    new playerb,
        amount;

    if(sscanf(params, "ud", playerb, amount))
        return SendUsageMessage(playerid, "/pay [playerid/name] [amount]");

    if(playerb == INVALID_PLAYER_ID)
        return SendErrorMessage(playerid, "The player you specified isn't connected.");

    if(playerb == playerid)
        return SendErrorMessage(playerid, "The player you specified isn't connected.");

    if(amount < 1)
        return SendErrorMessage(playerid, "The amount can't be less than $1.");  

    if(PlayerInfo[playerid][Cash] < amount)
        return SendErrorMessage(playerid, "You dont have enough money.");

    if(!IsPlayerNearPlayer(playerid, playerb, 5.0))
        return SendErrorMessage(playerid, "You must be near the player.");

    PlayerInfo[playerid][Cash] -= amount;
    PlayerInfo[playerb][Cash] += amount;
    SendClientMessageEx(playerid, COLOR_GREY, "* You have sent %s, $%d.", PlayerInfo[playerb][Name], amount);
    SendClientMessageEx(playerb, COLOR_GREY, "* You have received $%d from %s.", amount, PlayerInfo[playerid][Name]);

    foreach (new i : Player) 
	{
		if (PlayerInfo[i][AdminLevel])
		{
            if(amount >= 100) SendClientMessageEx(i, 0xbf9000FF, "[AdmWarn] %s has given %s, $%d.", PlayerInfo[playerid][Name], PlayerInfo[playerb][Name], amount);
		}
	}
    return 1;
}

cmd:fugitives(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_TDM && GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE)return SendErrorMessage(playerid, "This command is only available in TDM.");
    
    if(PlayerInfo[playerid][Team] != TEAM_LSPD && PlayerInfo[playerid][CC_Team] != TEAM_LSPD) return SendErrorMessage(playerid, "You must be a cop to use this command.");

    SendClientMessageEx(playerid, COLOR_RED, "Active Fugitive:");
    foreach(new i : Player)
    {
        if(GetPlayerWantedLevel(i) >= 3)
        {
            SendClientMessageEx(playerid, COLOR_GREY, "%s (%d)", PlayerInfo[i][Name], i);
        }
    }
    return 1;
}

CMD:duel(playerid, params[])
{
	if(GetPlayerGameMode(playerid) != GAME_MODE_LOBBY) return SendErrorMessage(playerid, "You must be in lobby to use this command.");
	new target, bet;
	if(sscanf(params, "ii", target, bet)) return SendUsageMessage(playerid, "/duel [playerid/name] [bet]");
	if(!IsPlayerConnected(target)) return SendErrorMessage(playerid, "[Duel] The player you specified isn't connected");
	if(GetPlayerGameMode(target) != GAME_MODE_LOBBY) return SendErrorMessage(playerid, "[Duel] The player you specified isn't in lobby.");
	if(PlayerInfo[playerid][Cash] < bet) return SendErrorMessage(playerid, "[Duel] You don't have enough money.");
	if(bet < 0) return SendErrorMessage(playerid, "[Duel] Invalid bet amount.");
	if(target == playerid) return SendErrorMessage(playerid, "[Duel] The player you specified isn't connected.");

	DuelInfo[playerid][DuelEnemy] = target;
	DuelInfo[target][DuelEnemy] = playerid;

	DuelInfo[playerid][DuelBet] = bet;
	DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelBet] = bet;

 	new str[512];
  	format(str, 512, "{FF0000}» {FFFFFF}Chain Saw\n{FF0000}» {FFFFFF}Grenade\n{FF0000}» {FFFFFF}Molotov\n{FF0000}» {FFFFFF}9MM\n{FF0000}» {FFFFFF}Silenced 9MM\n{FF0000}» {FFFFFF}Desert Eagle\n{FF0000}» {FFFFFF}Shotgun\n{FF0000}» {FFFFFF}Sawn Off\n{FF0000}» {FFFFFF}Combat Shotgun\n{FF0000}» {FFFFFF}UZI\n{FF0000}» {FFFFFF}MP5\n{FF0000}» {FFFFFF}AK-47\n{FF0000}» {FFFFFF}M4\n{FF0000}» {FFFFFF}TEC-9\n{FF0000}» {FFFFFF}Rifle\n{FF0000}» {FFFFFF}Sniper Rifle");
	ShowPlayerDialog(playerid, DIALOG_DUEL_WEAPON, DIALOG_STYLE_LIST, "Duel Weapon 1", str, "Select", "Abort");
	return 1;
}

cmd:me(playerid, params[])
{
    new action[128];

    if(PlayerInfo[playerid][Muted]) return SendErrorMessage(playerid, "You're muted!");

    if (sscanf(params, "s[128]", action)) return SendUsageMessage(playerid, "/me [action]");

    if (strlen(action) > 99) 
    {
        SendNearbyMessage(playerid, 30.0, 0xC2A2DAFF, "* %s {C2A2DA}%.99s", ReturnName(playerid), action);
        SendNearbyMessage(playerid, 30.0, 0xC2A2DAFF, "{C2A2DA}...%s", action[99]);
    }
    else SendNearbyMessage(playerid, 30.0, 0xC2A2DAFF, "* %s {C2A2DA}%s", ReturnName(playerid), action);
    return 1;
}

cmd:do(playerid, params[])
{
    new action[128];

    if(PlayerInfo[playerid][Muted]) return SendErrorMessage(playerid, "You're muted!");

    if (sscanf(params, "s[128]", action)) return SendUsageMessage(playerid, "/me [action]");

    if (strlen(action) > 99) 
    {
        SendNearbyMessage(playerid, 30.0, 0xC2A2DAFF, "* {C2A2DA}%.99s", action);
        SendNearbyMessage(playerid, 30.0, 0xC2A2DAFF, "{C2A2DA}...%s (( %s{C2A2DA} ))", action[99], ReturnName(playerid));
    }
    else SendNearbyMessage(playerid, 30.0, 0xC2A2DAFF, "* %s (( %s{C2A2DA} ))", action, ReturnName(playerid));
    return 1;
}

cmd:local(playerid, params[])
{
    new action[128];

    if(PlayerInfo[playerid][Muted]) return SendErrorMessage(playerid, "You're muted!");

    if(sscanf(params, "s[128]", action)) return SendUsageMessage(playerid, "/local [message]");

    SendNearbyMessage(playerid, 15.0, GetPlayerColor(playerid), "%s{FFFFFF} says: %s", ReturnName(playerid), action);
    return 1;
}

cmd:shout(playerid, params[])
{
    new action[128];

    if(PlayerInfo[playerid][Muted]) return SendErrorMessage(playerid, "You're muted!");

    if(sscanf(params, "s[128]", action)) return SendUsageMessage(playerid, "/s(hout) [message]");

    SendNearbyMessage(playerid, 30.0, GetPlayerColor(playerid), "%s{FFFFFF} shouts: %s", ReturnName(playerid), action);
    return 1;
}

cmd:b(playerid, params[])
{
    new action[128];

    if(PlayerInfo[playerid][Muted]) return SendErrorMessage(playerid, "You're muted!");

    if(sscanf(params, "s[128]", action)) return SendUsageMessage(playerid, "/b [message]");

    SendNearbyMessage(playerid, 30.0, 0xAFAFAFFF, "(( {%06x}%s [%d]{AFAFAF}: %s ))", GetPlayerColor(playerid) >>> 8, ReturnName(playerid), playerid, action);
    return 1;
}

cmd:cw(playerid, params[])
{
    new action[128];

    if(PlayerInfo[playerid][Muted]) return SendErrorMessage(playerid, "You're muted!");

    if(sscanf(params, "s[128]", action)) return SendUsageMessage(playerid, "/cw [message]");

    if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER && GetPlayerState(playerid) != PLAYER_STATE_PASSENGER) return SendErrorMessage(playerid, "You must be in car to use this command.");

    foreach(new i : Player)
    {
        if(IsPlayerConnected(i) && GetPlayerVehicleID(i) == GetPlayerVehicleID(playerid)) SendClientMessageEx(i, COLOR_YELLOWEX, "(%s) %s{FFFF00} whispers: %s", GetVehicleNameByModel(GetVehicleModel(GetPlayerVehicleID(playerid))), ReturnName(playerid), action);
    }
    return 1;
}

cmd:megaphone(playerid, params[])
{
    new action[128];

    if(PlayerInfo[playerid][Muted]) return SendErrorMessage(playerid, "You're muted!");

    if(PlayerInfo[playerid][CC_Team] != TEAM_LSPD && PlayerInfo[playerid][Team] != TEAM_LSPD) return SendErrorMessage(playerid, "You must be a cop to use this command.");

    if(sscanf(params, "s[128]", action)) return SendUsageMessage(playerid, "/m(egaphone) [message]");

    SendNearbyMessage(playerid, 50.0, COLOR_YELLOW, "[ %s:o< %s ]", PlayerInfo[playerid][Name], action);
    return 1;
}

cmd:perks(playerid, params[])
{
    ShowPlayerPerks(playerid);
    return 1;
}
