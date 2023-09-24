#include <YSI_Coding\y_hooks>

hook OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	new Float:hp, Float:ar;
    GetPlayerHP(damagedid, hp);
    GetPlayerArmor(damagedid, ar);
    if(GetPlayerGameMode(playerid) == GAME_MODE_TDM)
    {
        if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE)
        {
            amount = 0;
            SetPlayerArmor(damagedid, ar);
            return SetPlayerHP(damagedid, hp);
        }

        if(PlayerInfo[playerid][Team] == PlayerInfo[damagedid][Team])
        {
            amount = 0;
            GameTextForPlayer(playerid, "~r~NO TEAM KILL", 2000, 6);
            SetPlayerArmor(damagedid, ar);
            return SetPlayerHP(damagedid, hp);
        }

        if(gettime() - PlayerInfo[damagedid][RecentlySpawned] < 4)
        {
            GameTextForPlayer(playerid, "~r~NO SPAWN KILL", 2000, 6);
            amount = 0;
            SetPlayerArmor(damagedid, ar);
            return SetPlayerHP(damagedid, hp);
        }

        if(PlayerInfo[playerid][Team] == TEAM_LSPD && !GetPlayerWantedLevel(damagedid))
        {
            GameTextForPlayer(playerid, "~r~THAT PLAYER ISN'T FUGITIVE", 2000, 6);
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

        if(PlayerInfo[damagedid][Team] == TEAM_LSPD)
        {
            if(PlayerInfo[damagedid][Team] != PlayerInfo[playerid][Team])
            {
                if(!GetPlayerWantedLevel(playerid))
                {
                    SetPlayerWantedLevel(playerid, 3);
                    GameTextForPlayer(playerid, "~r~~n~~n~~n~~n~~n~~n~~n~~n~~n~YOU ARE A FUGITIVE!", 1500, 3);
                }
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
        
            HitMarkerForPlayer(playerid, damagedid, weaponid, amount);

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
            PlayerInfo[damagedid][State] = PLAYER_STATE_ALIVE;
            ResetPlayerWeapons(damagedid);
            PlayerInfo[damagedid][DeathTime] = 0;
            PlayerInfo[damagedid][RecentlySpawned] = gettime();
            if(!PlayerHasPerk(damagedid, 2)) SetPlayerHP(damagedid, 150.0);
	        else SetPlayerHP(damagedid, 175.0);

            SetPlayerChatBubble(damagedid, "(( Respawned ))", COLOR_GREY, 30.0, 500); 

            ClearAnimations(damagedid);

            SetPlayerInterior(damagedid, 0);
            SetPlayerVirtualWorld(damagedid, 0);

            PlayerInfo[damagedid][KillStreak] = 0;

            PlayerInfo[damagedid][MedkitTime] = 0;

            ClearPlayerDamages(damagedid);

            SetPlayerPos(damagedid, PlayerInfo[damagedid][X_Pos], PlayerInfo[damagedid][Y_Pos], PlayerInfo[damagedid][Z_Pos]);
            SetPlayerFacingAngle(damagedid, PlayerInfo[damagedid][A_Pos]);

            SetCameraBehindPlayer(damagedid);

            SetPlayerSkin(damagedid, PlayerInfo[damagedid][Skin]);

            SetPlayerSkillLevel(damagedid, WEAPONSKILL_PISTOL, 0);
            SetPlayerSkillLevel(damagedid, WEAPONSKILL_MICRO_UZI, 0);
            SetPlayerSkillLevel(damagedid, WEAPONSKILL_SPAS12_SHOTGUN, 0);

            if (PlayerInfo[damagedid][Team] == -1) 
            {
                SendPlayerToTeamSelection(damagedid);
            }
            else
            {
                SetPlayerPosition(damagedid);
            }
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
                PlayerInfo[damagedid][DeathTime] = 10;
                SendClientMessageEx(playerid, COLOR_WHITE, "(( You have killed {0D7304}%s{FFFFFF}, weapon used - {0D7304}%s{FFFFFF} ))", ReturnName(damagedid), ReturnWeaponNameEx(weaponid));
                SendClientMessage(damagedid, COLOR_RED, "(( You were brutally wounded. Use /damages to check your injuries, /acceptdeath to respawn. ))");
                foreach(new i : Player)
                {
                    if(GetPlayerGameMode(i) == GetPlayerGameMode(playerid)) SendDeathMessageToPlayer(i, playerid, damagedid, weaponid);
                }
                UpdatePlayerDeaths(damagedid);
                UpdatePlayerKills(playerid);	
                new zoneid = GetPlayerGangZone(playerid);
                if(PlayerInfo[playerid][Team] != TEAM_LSPD && ZoneInfo[zoneid][zOwner] == PlayerInfo[damagedid][Team])
                {
                    ZoneInfo[zoneid][zDeaths]++;
                    if(GetPlayersInZone(zoneid, PlayerInfo[playerid][Team]) >= 1 && ZoneInfo[zoneid][zDeaths] >= 3 && !ZoneInfo[zoneid][zCapturedBy])
                    {       
                        new randTime = randomEx(85, 125);
                        ZoneInfo[zoneid][zCapturedBy] = PlayerInfo[playerid][Team];
                        ZoneInfo[zoneid][zCaptureTime] = randTime;
                        GangZoneFlashForAll(zoneid, 0xFF634766);
                        SendClientMessageToAllEx(ReturnTeamColorEx(PlayerInfo[playerid][Team]), "** %s begin to capturing %s's territory!", ReturnTeamName(PlayerInfo[playerid][Team]), ReturnTeamName(ZoneInfo[zoneid][zOwner]));
                        GameTextForPlayer(playerid, "~w~You provoked a gang war.", 2500, 13);
                        foreach(new i : Player)
                        {
                            if(PlayerInfo[i][Team] == ZoneInfo[zoneid][zOwner]) ShowPlayerBox(i, "Our hood is under attack!", 3000);
                        }
                    }
                }
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
                SetPlayerHP(damagedid, 30);
                PlayerInfo[playerid][Score] += 1;
                PlayerInfo[playerid][Cash] += rand;
                PlayerInfo[playerid][KillStreak]++;
                new Float:arm;
                GetPlayerArmor(playerid, arm);
                new weapons[2];
                if(PlayerHasPerk(playerid, 3))
                {
                    GetPlayerWeaponData(playerid, 6, weapons[0], weapons[1]);
                    SetPlayerAmmo(playerid, weapons[0], (weapons[1] + (random(1)+1)));
                }
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
                switch(PlayerInfo[playerid][KillStreak])
                {
                    case 2: GameTextForPlayer(playerid,"~r~Double Kill!",3000,6);
                    case 3: GameTextForPlayer(playerid,"~r~Triple Kill!",3000,6);
                    case 4: GameTextForPlayer(playerid,"~r~Unstoppable!",3000,6);
                    case 5: 
                    {
                        GameTextForPlayer(playerid,"~r~Multi Kill!",3000,6);
                        SendGameMessage(GetPlayerGameMode(playerid), COLOR_RED, "SPREE: %s{FFFFFF} is on 5 killing spree!", PlayerInfo[playerid][Name]);
                        SendClientMessageEx(playerid, COLOR_RED, "SPREE:{FFFFFF} You've been given bonus armour (25.0) for your spree.");
                        PlayerInfo[playerid][Armour] += 25.0;
                        PlayerInfo[playerid][Score] += 2;
                        PlayerInfo[playerid][Cash] += rand;
                    }
                    case 10: 
                    {
                        GameTextForPlayer(playerid,"~r~Monster Kill!",3000,6);
                        SendGameMessage(GetPlayerGameMode(playerid), COLOR_RED, "SPREE: %s{FFFFFF} is on 10 killing spree!", PlayerInfo[playerid][Name]);
                        SendClientMessageEx(playerid, COLOR_RED, "SPREE:{FFFFFF} You've been given bonus armour (50.0) for your spree.");
                        PlayerInfo[playerid][Armour] += 50.0;
                        PlayerInfo[playerid][Score] += 5;
                        PlayerInfo[playerid][Cash] += rand + 10;
                    }
                    case 20: 
                    {
                        GameTextForPlayer(playerid,"~r~Immortal!",3000,6);
                        SendGameMessage(GetPlayerGameMode(playerid), COLOR_RED, "SPREE: %s{FFFFFF} is on 20 killing spree!", PlayerInfo[playerid][Name]);
                        SendClientMessageEx(playerid, COLOR_RED, "SPREE:{FFFFFF} You've been given bonus armour (50.0) for your killing spree.");
                        PlayerInfo[playerid][Armour] += 50.0;
                        PlayerInfo[playerid][Score] += 10;
                        PlayerInfo[playerid][Cash] += rand + 20;
                    }
                }
                return 1;
            }
        }
    }
	return 1;
}

hook OnPlayerClickPlayerTD(playerid, PlayerText:playertextid)
{
    if (playertextid == ui_class_arrowLeft[playerid])
	{
		PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
		new currentSkin = GetPVarInt(playerid, "CurrentSkin");
		if(currentSkin <= 0)
		{
			SetPVarInt(playerid, "CurrentSkin", 9);
			SetPlayerSkin(playerid, PlayerInfo[playerid][GangSkin][10]);
			PlayerTextDrawSetString(playerid, ui_class_name[playerid], ReturnTeamTagForText(gTeams[9][0]));
			PlayerTextDrawSetString(playerid, ui_class_skinName[playerid], "Los Santos Police Department");
            SetPlayerPos(playerid, gTeamCameras[9][0], gTeamCameras[9][1], gTeamCameras[9][2]);
            SetPlayerFacingAngle(playerid,gTeamCameras[9][6]);
            SetPlayerCameraPos(playerid, gTeamCameras[9][3], gTeamCameras[9][4], gTeamCameras[9][5]);
            SetPlayerCameraLookAt(playerid, gTeamCameras[9][0], gTeamCameras[9][1], gTeamCameras[9][2]);
            ApplyAnimation(playerid, "FREEWEIGHTS", "gym_free_celebrate", 4.0, 0, 0, 0, 0, 0, 1);
			return 1;
		}

        SetPlayerPos(playerid, gTeamCameras[currentSkin - 1][0], gTeamCameras[currentSkin - 1][1], gTeamCameras[currentSkin - 1][2]);
        SetPlayerFacingAngle(playerid,gTeamCameras[currentSkin - 1][6]);
        SetPlayerCameraPos(playerid, gTeamCameras[currentSkin - 1][3], gTeamCameras[currentSkin - 1][4], gTeamCameras[currentSkin - 1][5]);
        SetPlayerCameraLookAt(playerid, gTeamCameras[currentSkin - 1][0], gTeamCameras[currentSkin - 1][1], gTeamCameras[currentSkin - 1][2]);
        ApplyAnimation(playerid, "FREEWEIGHTS", "gym_free_celebrate", 4.0, 0, 0, 0, 0, 0, 1);

        SetPlayerSkin(playerid, PlayerInfo[playerid][GangSkin][(currentSkin + 1) - 1]);
		PlayerTextDrawSetString(playerid, ui_class_name[playerid], ReturnTeamTagForText(gTeams[currentSkin - 1][0]));
		PlayerTextDrawSetString(playerid, ui_class_skinName[playerid], ReturnTeamName(gTeams[currentSkin - 1][0]));
		SetPVarInt(playerid, "CurrentSkin", currentSkin - 1);
	}
	else if (playertextid == ui_class_arrowRight[playerid])
	{
		PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
		new currentSkin = GetPVarInt(playerid, "CurrentSkin");
		if(currentSkin >= 9)
		{
			SetPVarInt(playerid, "CurrentSkin", 0);
			SetPlayerSkin(playerid, PlayerInfo[playerid][GangSkin][1]);
			PlayerTextDrawSetString(playerid, ui_class_name[playerid], ReturnTeamTagForText(gTeams[0][0]));
			PlayerTextDrawSetString(playerid, ui_class_skinName[playerid], "Grove Street Families");

            SetPlayerPos(playerid, gTeamCameras[0][0], gTeamCameras[0][1], gTeamCameras[0][2]);
            SetPlayerFacingAngle(playerid,gTeamCameras[0][6]);
            SetPlayerCameraPos(playerid, gTeamCameras[0][3], gTeamCameras[0][4], gTeamCameras[0][5]);
            SetPlayerCameraLookAt(playerid, gTeamCameras[0][0], gTeamCameras[0][1], gTeamCameras[0][2]);
            ApplyAnimation(playerid, "FREEWEIGHTS", "gym_free_celebrate", 4.0, 0, 0, 0, 0, 0, 1);
			return 1;
		}

        SetPlayerPos(playerid, gTeamCameras[currentSkin + 1][0], gTeamCameras[currentSkin + 1][1], gTeamCameras[currentSkin + 1][2]);
        SetPlayerFacingAngle(playerid,gTeamCameras[currentSkin + 1][6]);
        SetPlayerCameraPos(playerid, gTeamCameras[currentSkin + 1][3], gTeamCameras[currentSkin + 1][4], gTeamCameras[currentSkin + 1][5]);
        SetPlayerCameraLookAt(playerid, gTeamCameras[currentSkin + 1][0], gTeamCameras[currentSkin + 1][1], gTeamCameras[currentSkin + 1][2]);
        ApplyAnimation(playerid, "FREEWEIGHTS", "gym_free_celebrate", 4.0, 0, 0, 0, 0, 0, 1);

        SetPlayerSkin(playerid, PlayerInfo[playerid][GangSkin][(currentSkin + 1) + 1]);
		PlayerTextDrawSetString(playerid, ui_class_name[playerid], ReturnTeamTagForText(gTeams[currentSkin + 1][0]));
		PlayerTextDrawSetString(playerid, ui_class_skinName[playerid], ReturnTeamName(gTeams[currentSkin + 1][0]));
		SetPVarInt(playerid, "CurrentSkin", currentSkin + 1);
	}
	else if (playertextid == ui_class_select[playerid]) 
	{
		new selectedTeam = GetPVarInt(playerid, "CurrentSkin");
		PlayerInfo[playerid][Team] = gTeams[selectedTeam][0];

		CancelSelectTextDraw(playerid);

		SendClientMessageEx(playerid, -1, "You have spawned as %s%s", ReturnTeamColor(gTeams[selectedTeam][0]), ReturnTeamTag(gTeams[selectedTeam][0]));

        PlayerTextDrawHide(playerid, ui_class_skinName[playerid]);
		PlayerTextDrawHide(playerid, ui_class_arrowLeft[playerid]);
		PlayerTextDrawHide(playerid, ui_class_arrowRight[playerid]);
		PlayerTextDrawHide(playerid, ui_class_select[playerid]);
		PlayerTextDrawHide(playerid, ui_class_name[playerid]);

		PlayerTextDrawShow(playerid, ui_clock[playerid]);
		if(!PlayerInfo[playerid][Tog][1]) {
            PlayerTextDrawShow(playerid, ui_serverName[playerid]);
		    PlayerTextDrawShow(playerid, ui_serverWeb[playerid]);
        }

		TogglePlayerControllable(playerid, true);
		SetPlayerInterior(playerid, 0);
		SetPlayerVirtualWorld(playerid, 0);
		SetCameraBehindPlayer(playerid);

		PlayerInfo[playerid][SelectingTeam] = false;

		PlayerInfo[playerid][Skin] = GetPlayerSkin(playerid);

        new nametag[MAX_PLAYER_NAME];
		if(GetHighestScore(PlayerInfo[playerid][Team]) != playerid)
		{
			format(nametag, sizeof nametag, "%s%s", ReturnTeamTag(PlayerInfo[playerid][Team]), PlayerInfo[playerid][Name]);
            SetPlayerName(playerid, nametag);
		}
		else
		{
            if(PlayerInfo[playerid][Team] != TEAM_LSPD)
			{
                format(nametag, sizeof nametag, "%s%s[OG]", ReturnTeamTag(PlayerInfo[playerid][Team]), PlayerInfo[playerid][Name]);
                SetPlayerName(playerid, nametag);
            }
            else
            {
                format(nametag, sizeof nametag, "%s%s", ReturnTeamTag(PlayerInfo[playerid][Team]), PlayerInfo[playerid][Name]);
                SetPlayerName(playerid, nametag);
            }
		}
        ClearAnimations(playerid);
		SetPlayerColor(playerid, ReturnTeamColorEx(PlayerInfo[playerid][Team]));
		SetPlayerPosition(playerid);
		ShowGangZonesForPlayer(playerid);
	}
    return 1;
}

hook OnGameModeInit()
{
    CreateGangZones();
    return 1;
}

public OnPlayerEnterGangZone(playerid, zone)
{
	if(GetPlayerVirtualWorld(playerid) == 0)
	{
		if(ZoneInfo[zone][zOwner] != 0)
		{
			if(GetPVarInt(playerid, "CurrentZone") != ZoneInfo[zone][zOwner])
			{
				/*new string[128];
				format(string, sizeof string, "~n~~n~~n~~n~~n~~n~~n~~n~~n~%s's~w~ territory", ReturnTeamTagForTextEx(ZoneInfo[zone][zOwner]));
				GameTextForPlayer(playerid, string, 3000, 3);*/
				SetPVarInt(playerid, "CurrentZone", ZoneInfo[zone][zOwner]);
			}
		}
	}
	return 1;
}