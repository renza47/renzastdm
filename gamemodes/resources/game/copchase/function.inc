StartCopchase()
{
    if(GetLobbyPlayersCount() < 3) return printf("ERROR: Failed to start copchase. (Not enough player)");

    if(CopchaseInfo[CC_Started] == true) return printf("ERROR: Failed to start copchase. (There is an active round already)");

    new members[MAX_PLAYERS], count;

    CopchaseInfo[CC_StartingTime] = 10;
    CopchaseInfo[CC_EnableInterior] = false;
    CopchaseInfo[CC_InteriorDelay] = 15;
    CopchaseInfo[CC_Map] = random(7);
    SetSVarInt("CCMap", CopchaseInfo[CC_Map]);
    CopchaseInfo[CC_WeaponEnabled] = false;
    CopchaseInfo[CC_TraceDelay] = 0;

    LoadCopchaseVehiclesFromFile("vehicles/ls_law.txt");
    LoadCopchaseVehiclesFromFile("vehicles/ls_airport.txt");
    LoadCopchaseVehiclesFromFile("vehicles/ls_gen_inner.txt");
    LoadCopchaseVehiclesFromFile("vehicles/ls_gen_outer.txt");

    foreach(new i : Player)
    {
        if(IsPlayerConnected(i))
        {
            if(PlayerInfo[i][IsLoggedIn] == true && GetPlayerGameMode(i) == GAME_MODE_LOBBY && IsPlayerInCopchaseLobby(i))
            {
                if(IsValidDynamicActor(PlayerInfo[i][Actor])) DestroyDynamicActor(PlayerInfo[i][Actor]);

                SetPlayerGameMode(i, GAME_MODE_COPCHASE);

                SetPlayerVirtualWorld(i, 2);
                SetPlayerInterior(i, 0);

                PlayerInfo[i][SelfRevive] = 0;

                PlayerInfo[i][FixVeh] = 0;
                PlayerInfo[i][FlipVeh] = 0;

                PlayerInfo[i][Mask] = 0;

                PlayerInfo[i][MedkitRevive] = 0;

                PlayerInfo[i][State] = PLAYER_STATE_ALIVE;
                ResetPlayerWeapons(i);
                PlayerInfo[i][DeathTime] = 0;

                ClearAnimations(i);

                PlayerInfo[i][KillStreak] = 0;

                PlayerInfo[i][MedkitTime] = 0;

                ClearPlayerDamages(i);

                SetCameraBehindPlayer(i);

                SetPlayerSkin(i, PlayerInfo[i][Model]);

                SetPlayerSkillLevel(i, WEAPONSKILL_PISTOL, 0);
                SetPlayerSkillLevel(i, WEAPONSKILL_MICRO_UZI, 0);
                SetPlayerSkillLevel(i, WEAPONSKILL_SPAS12_SHOTGUN, 0);

                if(!PlayerHasPerk(i, 2)) SetPlayerHP(i, 100.0);
	            else SetPlayerHP(i, 125.0);
                SetPlayerArmor(i, 50);

                PlayerInfo[i][Team] = -1;
                PlayerInfo[i][DM] = -1;

                PlayerInfo[i][BrokenLeg] = false;

                DisableRemoteVehicleCollisions(i, 1);

                members[count++] = i;
                PlayerTextDrawShow(i, ui_clock[i]);
                PlayerTextDrawShow(i, ui_ProgressInfo[i]);
		        //ShowPlayerProgressBar(i, ui_ProgressBar[i]);

                //if(CopchaseInfo[CC_Map] == 5 || CopchaseInfo[CC_Map] == 6) CreatePlayerVcMinimap(i);

                if(count == 64) break; // 64 players maximum
            }
        }
    }

    listShuffle(members, count);

    new FugitiveCount = floatround(count * 0.4);

    new temp_veh, lights, alarm, doors, bonnet, boot, objective, Float:vPos[3];

    for(new i; i < count; i++)
    {
        new member = members[i];
        if(i < FugitiveCount)
        {
            PlayerInfo[member][CC_Team] = TEAM_FUGITIVE;
            SetPlayerColor(member,COLOR_RED);

            SendClientMessagef(member, COLOR_RED, "> {FFFFFF}You are a fugitive. Escape the cops!");

            TogglePlayerControllable(member, false);

            SetPlayerPos(member, vPos[0], vPos[1], vPos[2] - 1.0);

            if(!IsValidVehicle(temp_veh))
            {
                temp_veh = CreateCopchaseVehicle(TEAM_FUGITIVE, CopchaseInfo[CC_Map], member);
                SetVehicleHealth(temp_veh, 2000.0);
                SetVehicleParamsEx(temp_veh, VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective);
                GetVehiclePos(temp_veh, vPos[0], vPos[1], vPos[2]);

                VehicleInfo[temp_veh][vTeam] = TEAM_FUGITIVE;

                VehicleInfo[temp_veh][vGun][1] = 29;
                VehicleInfo[temp_veh][vGunAmmo][1] = 250;

                VehicleInfo[temp_veh][vGun][3] = 29;
                VehicleInfo[temp_veh][vGunAmmo][3] = 250;

                VehicleInfo[temp_veh][vGun][4] = 31;
                VehicleInfo[temp_veh][vGunAmmo][4] = 250;

                PutPlayerInVehicle(member, temp_veh, 0);
            }

            if(!IsPlayerInAnyVehicle(member))
            {
                if(!IsVehicleSeatTaken(temp_veh, 0))
                {
                    PutPlayerInVehicle(member, temp_veh, 0);
                }
                else if(!IsVehicleSeatTaken(temp_veh, 1))
                {
                    PutPlayerInVehicle(member, temp_veh, 1);
                }
                else if(!IsVehicleSeatTaken(temp_veh, 2))
                {
                    PutPlayerInVehicle(member, temp_veh, 2);
                }
                else if(!IsVehicleSeatTaken(temp_veh, 3))
                {
                    PutPlayerInVehicle(member, temp_veh, 3);
                }
                else
                {
                    temp_veh = CreateCopchaseVehicle(TEAM_FUGITIVE, CopchaseInfo[CC_Map], member);
                    SetVehicleParamsEx(temp_veh, VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective);
                    SetVehicleHealth(temp_veh, 2000.0);

                    VehicleInfo[temp_veh][vTeam] = TEAM_FUGITIVE;

                    VehicleInfo[temp_veh][vGun][1] = 29;
                    VehicleInfo[temp_veh][vGunAmmo][1] = 250;

                    VehicleInfo[temp_veh][vGun][3] = 29;
                    VehicleInfo[temp_veh][vGunAmmo][3] = 250;

                    PutPlayerInVehicle(member, temp_veh, 0);
                }
            }
            TogglePlayerControllable(member, true);
        }
        else
        {
            PlayerInfo[member][CC_Team] = TEAM_LSPD;

            SetPlayerColor(member,COLOR_COP);

            SetPlayerSkin(member, PlayerInfo[member][CopSkin]);

            SendClientMessagef(member, COLOR_COP, "> {FFFFFF}You are a cop. Chase the criminal!");

            temp_veh = CreateCopchaseVehicle(TEAM_LSPD, CopchaseInfo[CC_Map], member);
            SetVehicleVirtualWorld(temp_veh, 2);
            SetVehicleHealth(temp_veh, 2000.0);

            TogglePlayerControllable(member, false);

            GetVehiclePos(temp_veh, vPos[0], vPos[1], vPos[2]);

            SetPlayerPos(member, vPos[0], vPos[1], vPos[2] - 5.0);

            VehicleInfo[temp_veh][vGun][1] = 29;
            VehicleInfo[temp_veh][vGunAmmo][1] = 250;

            VehicleInfo[temp_veh][vGun][3] = 29;
            VehicleInfo[temp_veh][vGunAmmo][3] = 250;

            VehicleInfo[temp_veh][vTeam] = TEAM_LSPD;

            PutPlayerInVehicle(member, temp_veh, 0);
            TogglePlayerControllable(member, true);
        }
    }
    new charge[64];
    switch(random(9)) {
        case 0: charge = "Attempted Murder";
        case 1: charge = "Vehicular Murder";
        case 2: charge = "Burglary";
        case 3: charge = "Hit and Run";
        case 4: charge = "Prisoner Breakout";
        case 5: charge = "Driving with a Suspended License";
        case 6: charge = "Driving without a License";
        case 7: charge = "Driving without Insurance";
        case 8: charge = "Disturbing the Peace";
        case 9: charge = "Possession of a Unlicensed Firearm";
    }

    foreach(new id : Player)
    {
        if(GetPlayerGameMode(id) == GAME_MODE_COPCHASE && PlayerInfo[id][CC_Team] == TEAM_LSPD) SendClientMessageEx(id, COLOR_RADIO, "** [CH: 911 S: 1] A suspect has been seen at %s. [Charged for %s]", ReturnLocation(GetRandomFugitive()), charge);
        if(GetPlayerGameMode(id) == GAME_MODE_COPCHASE && PlayerInfo[id][CC_Team] == TEAM_FUGITIVE) SendClientMessageEx(id, COLOR_RED, "> {FFFFFF}You are charged for %s.", charge);
    }
    
    SetTimer("CopchaseCollision", 15 * 1000, false);
    SendGameMessage(GAME_MODE_COPCHASE, COLOR_RED, "> {FFFFFF}Vehicle collision will be enabled in 15 seconds.", CopchaseInfo[CC_Time]);
    CopchaseInfo[CC_Time] = 420;
    CopchaseInfo[CC_Started] = true;
    printf("Copchase started. Fugitive count: %d", FugitiveCount);
    return 1;
}

EndCopchase()
{
    if(!CopchaseInfo[CC_Time] && GetFugitivesCount() && GetCopsCount())
    {
        SendClientMessageToAll(COLOR_RED, "Copchase ended. The criminals escaped.");
    }

    if(GetFugitivesCount() && !GetCopsCount())
    {
        SendClientMessageToAll(COLOR_RED, "Copchase ended. The cops has been eliminated.");
    }
    else if(!GetFugitivesCount() && GetCopsCount())
    {
        SendGameMessage(GAME_MODE_COPCHASE, COLOR_COP, "Copchase ended. The criminals has been eliminated.");
    }

    CopchaseInfo[CC_StartingTime] = 10;

    foreach(new i : Player)
    {
        if(IsPlayerConnected(i))
        {
            if(PlayerInfo[i][IsLoggedIn] == true && GetPlayerGameMode(i) == GAME_MODE_COPCHASE)
            {
                if(IsValidDynamicActor(PlayerInfo[i][Actor])) DestroyDynamicActor(PlayerInfo[i][Actor]);

                RemovePlayerFromCopchase(i);
            }
        }
    }
    for(new vehid; vehid < MAX_VEHICLES; vehid++) 
    {
        if(IsValidVehicle(vehid) && GetVehicleVirtualWorld(vehid) == 2) 
        {
            ResetVehicleWeapon(vehid);
            VehicleInfo[vehid][vTeam] = 0;
            DestroyVehicle(vehid);
        }
    }
    CopchaseInfo[CC_Time] = 0;
    CopchaseInfo[CC_Started] = false;
    CopchaseInfo[CC_EnableInterior] = false;
    CopchaseInfo[CC_InteriorDelay] = 15;
    return 1;
}

RemovePlayerFromCopchase(i)
{
    PlayerInfo[i][AntiCheatPause] = 3;
    PlayerInfo[i][State] = PLAYER_STATE_ALIVE;
    ResetPlayerWeapons(i);
    PlayerInfo[i][DeathTime] = 0;

    ClearAnimations(i);

    SetPlayerInterior(i, 0);
    SetPlayerVirtualWorld(i, 0);

    PlayerInfo[i][KillStreak] = 0;

    PlayerInfo[i][MedkitTime] = 0;

    ClearPlayerDamages(i);

    SetCameraBehindPlayer(i);

    PlayerInfo[i][Tased] = 0;
    PlayerInfo[i][Taser] = false;

    SetPlayerSkin(i, PlayerInfo[i][Model]);

    SetPlayerHP(i, 100);
    PlayerInfo[i][KillStreak] = 0;

    PlayerInfo[i][Team] = -1;
    PlayerInfo[i][DM] = -1;

    PlayerInfo[i][CC_Team] = TEAM_INVALID;
    PlayerInfo[i][DM] = -1;

    PlayerInfo[i][MedkitTime] = 0;

    PlayerInfo[i][BrokenLeg] = false;

    PlayerInfo[i][CuffTime] = 0;
    PlayerInfo[i][CuffID] = INVALID_PLAYER_ID;

    DisableRemoteVehicleCollisions(i, 0);

    TogglePlayerControllable(i, true);

    SendPlayerToLobby(i);
    return 1;
}

GetLobbyPlayersCount()
{
    new result;
    foreach(new i : Player)
    {
        if(IsPlayerConnected(i) && PlayerInfo[i][IsLoggedIn] == true && GetPlayerGameMode(i) == GAME_MODE_LOBBY && IsPlayerInCopchaseLobby(i)) result++;
    }
    return result;
}

GetFugitivesCount()
{
    new result;
    foreach(new i : Player)
    {
        if(IsPlayerConnected(i) && GetPlayerGameMode(i) == GAME_MODE_COPCHASE && PlayerInfo[i][CC_Team] == TEAM_FUGITIVE && PlayerInfo[i][State] == PLAYER_STATE_ALIVE) result++;
    }
    return result;
}

GetCopsCount()
{
    new result;
    foreach(new i : Player)
    {
        if(IsPlayerConnected(i) && GetPlayerGameMode(i) == GAME_MODE_COPCHASE && PlayerInfo[i][CC_Team] == TEAM_LSPD && PlayerInfo[i][State] == PLAYER_STATE_ALIVE) result++;
    }
    return result;
}

GetRandomFugitive()
{
    foreach(new i : Player)
    {
        if(IsPlayerConnected(i) && GetPlayerGameMode(i) == GAME_MODE_COPCHASE && PlayerInfo[i][CC_Team] == TEAM_FUGITIVE && PlayerInfo[i][State] == PLAYER_STATE_ALIVE) return i;
    }
    return INVALID_PLAYER_ID;
}

stock listShuffle(items[], currentIndex = sizeof(items)){
    new randomIndex, temp;
    while(0 != currentIndex){
        randomIndex = random(currentIndex);
        currentIndex--;
        temp = items[currentIndex];
        items[currentIndex] = items[randomIndex];
        items[randomIndex] = temp;
    }
}

stock CreateCopchaseVehicle(team, mapid, owner)
{
    new temp_veh, rand = random(2);
    switch(mapid)
    {
        case 0:
        {
            if(team == TEAM_LSPD)
            {
                temp_veh = CreateVehicle(PlayerInfo[owner][CopCar], CopchaseSpawn1[rand][0], CopchaseSpawn1[rand][1], CopchaseSpawn1[rand][2], CopchaseSpawn1[rand][3], 0, 1, -1, 1);
            }
            else if(team == TEAM_FUGITIVE)
            {
                temp_veh = CreateVehicle(PlayerInfo[owner][FugCar], CopchaseSpawn1[4][0], CopchaseSpawn1[4][1], CopchaseSpawn1[4][2], CopchaseSpawn1[3][3], random(128), random(128), -1, 0);
            }
        }
        case 1:
        {
            if(team == TEAM_LSPD)
            {
                temp_veh = CreateVehicle(PlayerInfo[owner][CopCar], CopchaseSpawn2[rand][0], CopchaseSpawn2[rand][1], CopchaseSpawn2[rand][2], CopchaseSpawn2[rand][3], 0, 1, -1, 1);
            }
            else if(team == TEAM_FUGITIVE)
            {
                temp_veh = CreateVehicle(PlayerInfo[owner][FugCar], CopchaseSpawn2[4][0], CopchaseSpawn2[4][1], CopchaseSpawn2[4][2], CopchaseSpawn2[3][3], random(128), random(128), -1, 0);
            }
        }
        case 2:
        {
            if(team == TEAM_LSPD)
            {
                temp_veh = CreateVehicle(PlayerInfo[owner][CopCar], CopchaseSpawn3[rand][0], CopchaseSpawn3[rand][1], CopchaseSpawn3[rand][2], CopchaseSpawn3[rand][3], 0, 1, -1, 1);
            }
            else if(team == TEAM_FUGITIVE)
            {
                temp_veh = CreateVehicle(PlayerInfo[owner][FugCar], CopchaseSpawn3[4][0], CopchaseSpawn3[4][1], CopchaseSpawn3[4][2], CopchaseSpawn3[3][3], random(128), random(128), -1, 0);
            }
        }
        case 3:
        {
            if(team == TEAM_LSPD)
            {
                temp_veh = CreateVehicle(PlayerInfo[owner][CopCar], CopchaseSpawn4[rand][0], CopchaseSpawn4[rand][1], CopchaseSpawn4[rand][2], CopchaseSpawn4[rand][3], 0, 1, -1, 1);
            }
            else if(team == TEAM_FUGITIVE)
            {
                temp_veh = CreateVehicle(PlayerInfo[owner][FugCar], CopchaseSpawn4[4][0], CopchaseSpawn4[4][1], CopchaseSpawn4[4][2], CopchaseSpawn4[3][3], random(128), random(128), -1, 0);
            }
        }
        case 4:
        {
            if(team == TEAM_LSPD)
            {
                temp_veh = CreateVehicle(PlayerInfo[owner][CopCar], CopchaseSpawn5[rand][0], CopchaseSpawn5[rand][1], CopchaseSpawn5[rand][2], CopchaseSpawn5[rand][3], 0, 1, -1, 1);
            }
            else if(team == TEAM_FUGITIVE)
            {
                temp_veh = CreateVehicle(PlayerInfo[owner][FugCar], CopchaseSpawn5[4][0], CopchaseSpawn5[4][1], CopchaseSpawn5[4][2], CopchaseSpawn5[4][3], random(128), random(128), -1, 0);
            }
        }
        case 5:
        {
            if(team == TEAM_LSPD)
            {
                temp_veh = CreateVehicle(PlayerInfo[owner][CopCar], CopchaseSpawn6[rand][0], CopchaseSpawn6[rand][1], CopchaseSpawn6[rand][2], CopchaseSpawn6[rand][3], 16, 1, -1, 1);
            }
            else if(team == TEAM_FUGITIVE)
            {
                temp_veh = CreateVehicle(PlayerInfo[owner][FugCar], CopchaseSpawn6[3][0], CopchaseSpawn6[3][1], CopchaseSpawn6[3][2], CopchaseSpawn6[3][3], random(128), random(128), -1, 0);
            }
        }
        case 6:
        {
            if(team == TEAM_LSPD)
            {
                temp_veh = CreateVehicle(PlayerInfo[owner][CopCar], CopchaseSpawn7[rand][0], CopchaseSpawn7[rand][1], CopchaseSpawn7[rand][2], CopchaseSpawn7[rand][3], 0, 1, -1, 1);
            }
            else if(team == TEAM_FUGITIVE)
            {
                temp_veh = CreateVehicle(PlayerInfo[owner][FugCar], CopchaseSpawn7[3][0], CopchaseSpawn7[3][1], CopchaseSpawn7[3][2], CopchaseSpawn7[3][3], random(128), random(128), -1, 0);
            }
        }
        case 7:
        {
            if(team == TEAM_LSPD)
            {
                temp_veh = CreateVehicle(PlayerInfo[owner][CopCar], CopchaseSpawn8[rand][0], CopchaseSpawn8[rand][1], CopchaseSpawn8[rand][2], CopchaseSpawn8[rand][3], 0, 1, -1, 1);
            }
            else if(team == TEAM_FUGITIVE)
            {
                temp_veh = CreateVehicle(PlayerInfo[owner][FugCar], CopchaseSpawn8[3][0], CopchaseSpawn8[3][1], CopchaseSpawn8[3][2], CopchaseSpawn8[3][3], random(128), random(128), -1, 0);
            }
        }
    }
    SetVehicleVirtualWorld(temp_veh, 2);
    return temp_veh;
}

UpdatePlayerMinimap(playerid)
{
    new Float:Pos[3];
    GetPlayerPos(playerid, Pos[0], Pos[1], Pos[2]);

    foreach(new i : Player)
    {
        if(IsPlayerConnected(i))
        {
            if(GetPlayerGameMode(i) == GAME_MODE_COPCHASE)
            {
                if(PlayerInfo[i][CC_Team] == TEAM_LSPD)
                {
                    DisablePlayerCheckpoint(i);
                    SetPlayerCheckpoint(i, Pos[0], Pos[1], Pos[2], 3.0);
                }
            }
        }
    }
    return 1;
}

HideCopsCheckpoint()
{
    foreach(new i : Player)
    {
        if(IsPlayerConnected(i))
        {
            if(GetPlayerGameMode(i) == GAME_MODE_COPCHASE)
            {
                if(PlayerInfo[i][CC_Team] == TEAM_LSPD)
                {
                    DisablePlayerCheckpoint(i);
                }
            }
        }
    }
    return 1;
}

IsPlayerInCopchaseLobby(playerid)
{
    if(GetPlayerGameMode(playerid) == GAME_MODE_LOBBY && IsPlayerInRangeOfPoint(playerid, 200.0, 246.40,	110.84, 1003.22) && GetPlayerInterior(playerid) == 10 && GetPlayerVirtualWorld(playerid) == 11) return 1;
    return 0;
}