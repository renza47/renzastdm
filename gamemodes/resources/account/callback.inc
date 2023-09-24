#include <YSI_Coding\y_hooks>

hook OnPlayerConnect(playerid) 
{
	if(IsPlayerNPC(playerid)) return SpawnPlayer(playerid);

    g_MysqlRaceCheck[playerid]++;

	// reset player data
	ResetPlayerData(playerid);

	GetPlayerName(playerid, PlayerInfo[playerid][Name], MAX_PLAYER_NAME);
    return 1;
}

public OnPlayerFinishedDownloading(playerid, virtualworld)
{
	if(IsPlayerNPC(playerid)) return SpawnPlayer(playerid);

	if(PlayerInfo[playerid][IsLoggedIn] == false)
	{
		new ipaddress[60];
		GetPlayerIp(playerid, ipaddress, sizeof ipaddress);
		
		TogglePlayerSpectating(playerid, true);

		SetPlayerVirtualWorld(playerid, 7000);

		SetPlayerTime(playerid, 23, 30);

		InterpolateCameraPos(playerid,2476.3250,-1738.1549,15.5469, 2476.3250,-1738.1549,15.5469,500,CAMERA_CUT);
		InterpolateCameraLookAt(playerid, 2485.7910,-1725.9292,13.5469, 2485.7910,-1725.9292,13.5469,500,CAMERA_CUT);
		SetPlayerVirtualWorld(playerid, 7000);

		SetPlayerPos( playerid, 6287.1406,-183.3881,28.0);
		SetPlayerFacingAngle( playerid, 139.1213 );

		new query[103];
		mysql_format(g_SQL, query, sizeof query, "SELECT * FROM `bans` WHERE `ip` = '%e' LIMIT 1", ipaddress);
		mysql_tquery(g_SQL, query, "OnPlayerIPBanCheck", "dd", playerid, g_MysqlRaceCheck[playerid]);

		PlayerInfo[playerid][LoginTimer][1] = SetTimerEx("OnLoginCameraChange", 20000, false, "dd", playerid, 1);
	}
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	if(PlayerInfo[playerid][IsLoggedIn] == false)
	{
		InterpolateCameraPos(playerid,2476.3250,-1738.1549,15.5469, 2476.3250,-1738.1549,15.5469,500,CAMERA_CUT);
		InterpolateCameraLookAt(playerid, 2485.7910,-1725.9292,13.5469, 2485.7910,-1725.9292,13.5469,500,CAMERA_CUT);
	}
	return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	if(IsPlayerNPC(playerid)) return 1;

    g_MysqlRaceCheck[playerid]++;

	UpdatePlayerData(playerid, reason);

	// if the player was kicked (either wrong password or taking too long) during the login part, remove the data from the memory
	if (cache_is_valid(PlayerInfo[playerid][Cache_ID]))
	{
		cache_delete(PlayerInfo[playerid][Cache_ID]);
		PlayerInfo[playerid][Cache_ID] = MYSQL_INVALID_CACHE;
	}

	// if the player was kicked before the time expires (30 seconds), kill the timer
	if (PlayerInfo[playerid][LoginTimer][0])
	{
		KillTimer(PlayerInfo[playerid][LoginTimer][0]);
		PlayerInfo[playerid][LoginTimer][0] = 0;
	}

	if (PlayerInfo[playerid][LoginTimer][1])
	{
		KillTimer(PlayerInfo[playerid][LoginTimer][1]);
		PlayerInfo[playerid][LoginTimer][1] = 0;
	}

	// sets "IsLoggedIn" to false when the player disconnects, it prevents from saving the player data twice when "gmx" is used
	PlayerInfo[playerid][IsLoggedIn] = false;

	if(IsValidDynamicActor(PlayerInfo[playerid][Actor])) DestroyDynamicActor(PlayerInfo[playerid][Actor]);

	if(IsValidDynamic3DTextLabel(PlayerInfo[playerid][Nametag])) 
    {
        DestroyDynamic3DTextLabel(PlayerInfo[playerid][Nametag]);
        PlayerInfo[playerid][Nametag] = Text3D:INVALID_3DTEXT_ID;
    }
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch (dialogid)
	{
		case DIALOG_UNUSED: return 1; // Useful for dialogs that contain only information and we do nothing depending on whether they responded or not

		case DIALOG_LOGIN:
		{
			if (!response) return Kick(playerid);

            bcrypt_verify(playerid, "OnPasswordInsert", inputtext, PlayerInfo[playerid][Password]);
		}
		case DIALOG_REGISTER:
		{
			if (!response) return Kick(playerid);

			if (strlen(inputtext) <= 5) 
            {
                SendClientMessage(playerid, 0xFF0000FF, "Your password must be longer than 5 characters!");
                new string[256];
                format(string, sizeof string, "{FFFFFF}Welcome {FF6347}%s{FFFFFF}!\n\nThis account is available!\n\n{FF6347}In order to proceed, enter a password in the field below:", PlayerInfo[playerid][Name]);
                return ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Registration", string, "Register", "Abort");
            }
			bcrypt_hash(playerid, "OnPasswordHashed", inputtext, BCRYPT_COST, "i", playerid);
		}
		case DIALOG_WEAPON:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0: ShowPlayerDialog(playerid, DIALOG_WEAPON1, DIALOG_STYLE_TABLIST_HEADERS, "Weapon slot 1", "Weapon\tPrice\nM4\t$100\nAK-47\t$75\nMP5\t$50\nShotgun\t$35\nTEC-9\tFREE\nMAC-10\tFREE", "Select", "Back");
					case 1: ShowPlayerDialog(playerid, DIALOG_WEAPON2, DIALOG_STYLE_TABLIST_HEADERS, "Weapon slot 2", "Weapon\tPrice\nDesert Eagle\tFREE\n9MM\tFREE", "Select", "Back");
				}
			}
		}
		case DIALOG_WEAPON1:
		{
			if(!response)
			{
				new string[256];
				format(string, sizeof string, "Slot\tCurrent Weapon\nWeapon slot 1\t%s\nWeapon slot 2\t%s", GetWeaponNameEx(PlayerInfo[playerid][Weapon][0]), GetWeaponNameEx(PlayerInfo[playerid][Weapon][1]));
				return ShowPlayerDialog(playerid, DIALOG_WEAPON, DIALOG_STYLE_TABLIST_HEADERS, "Weapon Loadout", string, "Edit", "Close");
			}

			if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE) return SendErrorMessage(playerid, "You aren't alive!");

			switch(listitem)
			{
				case 0: //m4
				{
					if(WeaponBalancing == true) return SendErrorMessage(playerid, "You can't use this weapon while weapon balancing is on.");
					if(PlayerInfo[playerid][Cash] < 100) return SendErrorMessage(playerid, "You don't have enough money.");

					PlayerInfo[playerid][Cash] -= 100;
					PlayerInfo[playerid][Weapon][0] = 31;
					ResetPlayerWeapons(playerid);
				}
				case 1: //ak47
				{
					if(WeaponBalancing == true) return SendErrorMessage(playerid, "You can't use this weapon while weapon balancing is on.");
					if(PlayerInfo[playerid][Cash] < 75) return SendErrorMessage(playerid, "You don't have enough money.");

					PlayerInfo[playerid][Cash] -= 75;
					PlayerInfo[playerid][Weapon][0] = 30;
					ResetPlayerWeapons(playerid);
				}
				case 2: //mp5
				{
					if(WeaponBalancing == true) return SendErrorMessage(playerid, "You can't use this weapon while weapon balancing is on.");
					if(PlayerInfo[playerid][Cash] < 50) return SendErrorMessage(playerid, "You don't have enough money.");

					PlayerInfo[playerid][Cash] -= 50;
					PlayerInfo[playerid][Weapon][0] = 29;
					ResetPlayerWeapons(playerid);
				}
				case 3: //shotgun
				{
					if(PlayerInfo[playerid][Cash] < 35) return SendErrorMessage(playerid, "You don't have enough money.");

					PlayerInfo[playerid][Cash] -= 35;
					PlayerInfo[playerid][Weapon][0] = 25;
					ResetPlayerWeapons(playerid);
				}
				case 4: //tec9
				{
					PlayerInfo[playerid][Weapon][0] = 32;
					ResetPlayerWeapons(playerid);
				}
				case 5: //uzi
				{
					PlayerInfo[playerid][Weapon][0] = 28;
					ResetPlayerWeapons(playerid);
				}
			}
			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][0], 1000);
			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][1], 1000);
			SetPlayerArmedWeapon(playerid, PlayerInfo[playerid][Weapon][0]);
			SendClientMessageEx(playerid, COLOR_GREEN, "[Weapon slot 1] You will now spawn with %s!", GetWeaponNameEx(PlayerInfo[playerid][Weapon][0]));
		}
		case DIALOG_WEAPON2:
		{
			if(!response)
			{
				new string[256];
				format(string, sizeof string, "Slot\tCurrent Weapon\nWeapon slot 1\t%s\nWeapon slot 2\t%s", GetWeaponNameEx(PlayerInfo[playerid][Weapon][0]), GetWeaponNameEx(PlayerInfo[playerid][Weapon][1]));
				return ShowPlayerDialog(playerid, DIALOG_WEAPON, DIALOG_STYLE_TABLIST_HEADERS, "Weapon Loadout", string, "Edit", "Close");
			}

			if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE) return SendErrorMessage(playerid, "You aren't alive!");

			switch(listitem)
			{
				case 0: //desert eagle
				{
					PlayerInfo[playerid][Weapon][1] = 24;
					ResetPlayerWeapons(playerid);
				}
				case 1: //colt
				{
					PlayerInfo[playerid][Weapon][1] = 22;
					ResetPlayerWeapons(playerid);
				}
			}
			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][0], 1000);
			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][1], 1000);
			SetPlayerArmedWeapon(playerid, PlayerInfo[playerid][Weapon][0]);
			SendClientMessageEx(playerid, COLOR_GREEN, "[Weapon slot 2] You will now spawn with %s!", GetWeaponNameEx(PlayerInfo[playerid][Weapon][1]));
		}
		case DIALOG_SKIN:
		{
			if(response)
			{
				if(listitem == 0)
				{
					switch(PlayerInfo[playerid][Team])
					{
						case 1..3: ShowModelSelectionMenu(playerid, "Team Skin", MODEL_SELECTION_SKIN, familiesSkin, sizeof(familiesSkin), -16.0, 0.0, -55.0);
						case 4..7: ShowModelSelectionMenu(playerid, "Team Skin", MODEL_SELECTION_SKIN, ballasSkin, sizeof(ballasSkin), -16.0, 0.0, -55.0);
						case 8: ShowModelSelectionMenu(playerid, "Team Skin", MODEL_SELECTION_SKIN, vagosSkin, sizeof(vagosSkin), -16.0, 0.0, -55.0);
						case 9: ShowModelSelectionMenu(playerid, "Team Skin", MODEL_SELECTION_SKIN, varriosSkin, sizeof(varriosSkin), -16.0, 0.0, -55.0);
						case 10: ShowModelSelectionMenu(playerid, "Team Skin", MODEL_SELECTION_SKIN, lspdSkin, sizeof(lspdSkin), -16.0, 0.0, -55.0);
					}
				}
				//case 1: ShowModelSelectionMenu(playerid, "Custom skin", MODEL_SELECTION_SKIN, gsf_skin, sizeof(gsf_skin), -16.0, 0.0, -55.0);
			}
		}
		case DIALOG_WEAPON_SWITCH:
		{
			if(response)
			{
				if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE) return SendErrorMessage(playerid, "You must be alive to be able to use this command.");

                if(GetPlayerState(playerid) != PLAYER_STATE_PASSENGER) return SendErrorMessage(playerid, "Only passenger can do this!");

                if(strcmp(inputtext, "Remove Weapon", true) == 0)
				{
					SetPlayerArmedWeapon(playerid, 0);
					ApplyAnimationEx(playerid, "CARRY", "crry_prtial", 1.0, 0, 0, 0, 0, 0);
				}
				else
				{
					new armedWeapon = GetWeaponIDByName(inputtext);
				    SetPlayerArmedWeapon(playerid, armedWeapon);
				}
			}
		}
		case DIALOG_HUD:
		{
			if(response)
			{
				if(listitem == 0)
				{
					switch(PlayerInfo[playerid][Tog][1])
					{
						case 0:
						{
							PlayerInfo[playerid][Tog][1] = 1;
							SendClientMessage(playerid, COLOR_GREY, "Server name togged off.");
							PlayerTextDrawHide(playerid, ui_serverName[playerid]);
		                    PlayerTextDrawHide(playerid, ui_serverWeb[playerid]);
						}
						case 1:
						{
							PlayerInfo[playerid][Tog][1] = 0;
							SendClientMessage(playerid, COLOR_GREY, "Server name togged on.");
							PlayerTextDrawShow(playerid, ui_serverName[playerid]);
		                    PlayerTextDrawShow(playerid, ui_serverWeb[playerid]);
						}
					}
				} 
				else if(listitem == 1)
				{
					switch(PlayerInfo[playerid][Tog][2])
					{
						case 0:
						{
							PlayerInfo[playerid][Tog][2] = 1;
							SendClientMessage(playerid, COLOR_GREY, "HUD togged off.");
							PlayerTextDrawHide(playerid, ui_stats[playerid]);
						}
						case 1:
						{
							PlayerInfo[playerid][Tog][2] = 0;
							SendClientMessage(playerid, COLOR_GREY, "HUD togged on.");
							PlayerTextDrawShow(playerid, ui_stats[playerid]);
						}
					}
				}
			}
		}
		case DIALOG_PERKS:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0:
					{
						SendClientMessage(playerid, COLOR_RED, "Successfully unequipped all perks.");
						for(new i; i < 3; i++)
						{
							PlayerInfo[playerid][Perk][i] = 0;
						}
					}
					case 1:
					{
						if(PlayerHasPerk(playerid, listitem))
						{
							PlayerInfo[playerid][Perk][PlayerPerkSlot(playerid, listitem)] = 0;
							SendClientMessage(playerid, COLOR_RED, "Perk unequipped.");
							return 1;
						}

						if(PlayerInfo[playerid][Cash] < GetPerkPrice(playerid)) return SendErrorMessage(playerid, "You dont have enough money.");

						

						if(GetPerkFreeSLot(playerid) == -1) return SendErrorMessage(playerid, "You don't have any slot left");

						SendClientMessageEx(playerid, COLOR_GREEN, "You've bought \"Self revive\" for $%i.", GetPerkPrice(playerid));
						PlayerInfo[playerid][Cash] -= GetPerkPrice(playerid);
						PlayerInfo[playerid][Perk][GetPerkFreeSLot(playerid)] = listitem;
					}
					case 2:
					{
						if(PlayerHasPerk(playerid, listitem))
						{
							PlayerInfo[playerid][Perk][PlayerPerkSlot(playerid, listitem)] = 0;
							SendClientMessage(playerid, COLOR_RED, "Perk unequipped.");
							return 1;
						}

						if(PlayerInfo[playerid][Cash] < GetPerkPrice(playerid)) return SendErrorMessage(playerid, "You dont have enough money.");

						

						if(GetPerkFreeSLot(playerid) == -1) return SendErrorMessage(playerid, "You don't have any slot left");

						SendClientMessageEx(playerid, COLOR_GREEN, "You've bought \"More HP\" for $%i.", GetPerkPrice(playerid));
						SendClientMessage(playerid, COLOR_GREY, "This perk will affect after a respawn.");
						PlayerInfo[playerid][Cash] -= GetPerkPrice(playerid);
						PlayerInfo[playerid][Perk][GetPerkFreeSLot(playerid)] = listitem;
					}
					case 3:
					{
						if(PlayerHasPerk(playerid, listitem))
						{
							PlayerInfo[playerid][Perk][PlayerPerkSlot(playerid, listitem)] = 0;
							SendClientMessage(playerid, COLOR_RED, "Perk unequipped.");
							return 1;
						}

						if(PlayerInfo[playerid][Cash] < GetPerkPrice(playerid)) return SendErrorMessage(playerid, "You dont have enough money.");

						

						if(GetPerkFreeSLot(playerid) == -1) return SendErrorMessage(playerid, "You don't have any slot left");

                        SendClientMessageEx(playerid, COLOR_GREEN, "You've bought \"Rifle (Random ammo every kill)\" for $%i.", GetPerkPrice(playerid));
						SendClientMessage(playerid, COLOR_GREY, "This perk will affect after a respawn.");
						PlayerInfo[playerid][Cash] -= GetPerkPrice(playerid);
						PlayerInfo[playerid][Perk][GetPerkFreeSLot(playerid)] = listitem;
					}
					case 4:
					{
						if(PlayerHasPerk(playerid, listitem))
						{
							PlayerInfo[playerid][Perk][PlayerPerkSlot(playerid, listitem)] = 0;
							SendClientMessage(playerid, COLOR_RED, "Perk unequipped.");
							return 1;
						}

						if(PlayerInfo[playerid][Cash] < GetPerkPrice(playerid)) return SendErrorMessage(playerid, "You dont have enough money.");

						

						if(GetPerkFreeSLot(playerid) == -1) return SendErrorMessage(playerid, "You don't have any slot left");

                        SendClientMessageEx(playerid, COLOR_GREEN, "You've bought \"Instant revive\" for $%i.", GetPerkPrice(playerid));
						PlayerInfo[playerid][Cash] -= GetPerkPrice(playerid);
						PlayerInfo[playerid][Perk][GetPerkFreeSLot(playerid)] = listitem;
					}
					case 5:
					{
						if(PlayerHasPerk(playerid, listitem))
						{
							PlayerInfo[playerid][Perk][PlayerPerkSlot(playerid, listitem)] = 0;
							SendClientMessage(playerid, COLOR_RED, "Perk unequipped.");
							return 1;
						}

						if(PlayerInfo[playerid][Cash] < GetPerkPrice(playerid)) return SendErrorMessage(playerid, "You dont have enough money.");

						

						if(GetPerkFreeSLot(playerid) == -1) return SendErrorMessage(playerid, "You don't have any slot left");

                        SendClientMessageEx(playerid, COLOR_GREEN, "You've bought \"Mask\" for $%i.", GetPerkPrice(playerid));
						PlayerInfo[playerid][Cash] -= GetPerkPrice(playerid);
						PlayerInfo[playerid][Perk][GetPerkFreeSLot(playerid)] = listitem;
					}
					case 6:
					{
						if(PlayerHasPerk(playerid, listitem))
						{
							PlayerInfo[playerid][Perk][PlayerPerkSlot(playerid, listitem)] = 0;
							SendClientMessage(playerid, COLOR_RED, "Perk unequipped.");
							return 1;
						}

						if(PlayerInfo[playerid][Cash] < GetPerkPrice(playerid)) return SendErrorMessage(playerid, "You dont have enough money.");

						

						if(GetPerkFreeSLot(playerid) == -1) return SendErrorMessage(playerid, "You don't have any slot left");

                        SendClientMessageEx(playerid, COLOR_GREEN, "You've bought \"Vehicle flip & fix\" for $%i.", GetPerkPrice(playerid));
						PlayerInfo[playerid][Cash] -= GetPerkPrice(playerid);
						PlayerInfo[playerid][Perk][GetPerkFreeSLot(playerid)] = listitem;
					}
					case 7:
					{
						if(PlayerHasPerk(playerid, listitem))
						{
							PlayerInfo[playerid][Perk][PlayerPerkSlot(playerid, listitem)] = 0;
							SendClientMessage(playerid, COLOR_RED, "Perk unequipped.");
							return 1;
						}

						if(PlayerInfo[playerid][Cash] < GetPerkPrice(playerid)) return SendErrorMessage(playerid, "You dont have enough money.");

						

						if(GetPerkFreeSLot(playerid) == -1) return SendErrorMessage(playerid, "You don't have any slot left");

                        SendClientMessageEx(playerid, COLOR_GREEN, "You've bought \"+15.0 armour every kill\" for $%i.", GetPerkPrice(playerid));
						PlayerInfo[playerid][Cash] -= GetPerkPrice(playerid);
						PlayerInfo[playerid][Perk][GetPerkFreeSLot(playerid)] = listitem;
					}
					case 8:
					{
						if(PlayerHasPerk(playerid, listitem))
						{
							PlayerInfo[playerid][Perk][PlayerPerkSlot(playerid, listitem)] = 0;
							SendClientMessage(playerid, COLOR_RED, "Perk unequipped.");
							return 1;
						}
						
						if(PlayerInfo[playerid][Cash] < GetPerkPrice(playerid)) return SendErrorMessage(playerid, "You dont have enough money.");

						

						if(GetPerkFreeSLot(playerid) == -1) return SendErrorMessage(playerid, "You don't have any slot left");

                        SendClientMessageEx(playerid, COLOR_GREEN, "You've bought \"Regeneration\" for $%i.", GetPerkPrice(playerid));
						PlayerInfo[playerid][Cash] -= GetPerkPrice(playerid);
						PlayerInfo[playerid][Perk][GetPerkFreeSLot(playerid)] = listitem;
					}
				}
				ShowPlayerPerks(playerid);
			}
		}
		/*case DIALOG_LOBBIES:
		{
			if(response)
			{
				if(strcmp(inputtext, "View Private Lobbies", true) == 0) 
				{
					ShowPlayerPrivateLobbyDialog(playerid);
				}
				else
				{
					new id = strval(inputtext);
					if(LobbyInfo[id][LobbyPlayers] >= LobbyInfo[id][LobbyMaxPlayers])
					{
						SendErrorMessage(playerid, "%s is full, please try again later!", LobbyInfo[id][LobbyName]);
						ShowPlayerLobbyDialog(playerid);
						return 1;
					}

					if(strcmp(LobbyInfo[id][LobbyVersion], "Cross Version", true) == 0)
					{
						LobbyInfo[id][LobbyPlayers]++;
						PlayerInfo[playerid][Lobby] = id;

						SetSpawnInfo(playerid, NO_TEAM, 0, PlayerInfo[playerid][X_Pos], PlayerInfo[playerid][Y_Pos], PlayerInfo[playerid][Z_Pos], PlayerInfo[playerid][A_Pos], 0, 0, 0, 0, 0, 0);
						SpawnPlayer(playerid);
						return 1;
					}

					new version[128];

					GetPlayerVersion(playerid, version, sizeof version);

					if(strcmp(version, LobbyInfo[id][LobbyVersion], true) == 0) 
					{
						LobbyInfo[id][LobbyPlayers]++;
						PlayerInfo[playerid][Lobby] = id;

						SetSpawnInfo(playerid, NO_TEAM, 0, PlayerInfo[playerid][X_Pos], PlayerInfo[playerid][Y_Pos], PlayerInfo[playerid][Z_Pos], PlayerInfo[playerid][A_Pos], 0, 0, 0, 0, 0, 0);
						SpawnPlayer(playerid);
					}
					else 
					{
						SendErrorMessage(playerid, "Invalid game version!");
						ShowPlayerLobbyDialog(playerid);
					}
				}
			}
			else Kick(playerid);
		}
		case DIALOG_PRIVATE_LOBBIES:
		{
			if(response)
			{
				if(strcmp(inputtext, "Create your own lobby", true) == 0) 
				{
					if(PlayerInfo[playerid][Score] < 750) return SendErrorMessage(playerid, "You must have atleast 750 score to create a private lobby.");
					
					//InitializeLobbyCreator(playerid);
				}
				else
				{
					new id = strval(inputtext);
					if(LobbyInfo[id][LobbyPlayers] >= LobbyInfo[id][LobbyMaxPlayers])
					{
						SendErrorMessage(playerid, "%s is full, please try again later!", LobbyInfo[id][LobbyName]);
						ShowPlayerLobbyDialog(playerid);
						return 1;
					}

					if(strcmp(LobbyInfo[id][LobbyVersion], "Cross Version", true) == 0)
					{
						LobbyInfo[id][LobbyPlayers]++;
						PlayerInfo[playerid][Lobby] = id;

						SetSpawnInfo(playerid, NO_TEAM, 0, PlayerInfo[playerid][X_Pos], PlayerInfo[playerid][Y_Pos], PlayerInfo[playerid][Z_Pos], PlayerInfo[playerid][A_Pos], 0, 0, 0, 0, 0, 0);
						SpawnPlayer(playerid);
						return 1;
					}

					new version[128];

					GetPlayerVersion(playerid, version, sizeof version);

					if(strcmp(version, LobbyInfo[id][LobbyVersion], true) == 0) 
					{
						LobbyInfo[id][LobbyPlayers]++;
						PlayerInfo[playerid][Lobby] = id;

						SetSpawnInfo(playerid, NO_TEAM, 0, PlayerInfo[playerid][X_Pos], PlayerInfo[playerid][Y_Pos], PlayerInfo[playerid][Z_Pos], PlayerInfo[playerid][A_Pos], 0, 0, 0, 0, 0, 0);
						SpawnPlayer(playerid);
					}
					else 
					{
						SendErrorMessage(playerid, "Invalid game version!");
						ShowPlayerLobbyDialog(playerid);
					}
				}
			}
			else ShowPlayerLobbyDialog(playerid);
		}*/

		default: return 0; // dialog ID was not found, search in other scripts
	}
    return 1;
}

public OnModelSelectionResponse(playerid, extraid, index, modelid, response)
{
	if ((response) && (extraid == MODEL_SELECTION_SKIN))
	{
		SendClientMessageEx(playerid, COLOR_GREY, "You've updated your %s skin to %d. Respawn to see the changes.",ReturnTeamName(PlayerInfo[playerid][Team]), modelid);
	    PlayerInfo[playerid][GangSkin][PlayerInfo[playerid][Team]] = modelid;
	}    
	else if ((response) && (extraid == MODEL_SELECTION_SAMP_SKIN))
	{
		PlayerInfo[playerid][Model] = modelid;
		SetPlayerSkin(playerid, modelid);
		SendClientMessageEx(playerid, COLOR_GREY, "You've changed your lobby/fugitive skin to %d.", modelid);
	} 
	else if ((response) && (extraid == MODEL_SELECTION_COP_SKIN))
	{
		PlayerInfo[playerid][CopSkin] = modelid;
		SetPlayerSkin(playerid, modelid);
		SendClientMessageEx(playerid, COLOR_GREY, "You've changed your cop skin to %d.", modelid);
	} 
	else if ((response) && (extraid == MODEL_SELECTION_COP_VEHICLE))
	{
		PlayerInfo[playerid][CopCar] = modelid;
		SendClientMessageEx(playerid, COLOR_GREY, "You've changed your cop car to %s.", GetVehicleNameByModel(modelid));
	} 
	else if ((response) && (extraid == MODEL_SELECTION_FUG_VEHICLE))
	{
		PlayerInfo[playerid][FugCar] = modelid;
		SendClientMessageEx(playerid, COLOR_GREY, "You've changed your fugitive car to %s.", GetVehicleNameByModel(modelid));
	} 
	return 1;
}

forward OnLoginCameraChange(playerid, type);
public OnLoginCameraChange(playerid, type)
{
	if(PlayerInfo[playerid][IsLoggedIn] == true) return 1;

	PlayerInfo[playerid][LoginTimer][1] = 0;

    switch(type)
	{
		case 1:
		{
			InterpolateCameraPos(playerid,2476.3250,-1738.1549,15.5469, 2478.9895,-1654.9093,16.3220,25000,CAMERA_MOVE);
		    InterpolateCameraLookAt(playerid, 2485.7910,-1725.9292,13.5469, 2450.8357,-1659.1113,13.5469,25000,CAMERA_MOVE);

			PlayerInfo[playerid][LoginTimer][1] = SetTimerEx("OnLoginCameraChange", 45000, false, "dd", playerid, 2);
		}
		case 2:
		{
			InterpolateCameraPos(playerid,2478.9895,-1654.9093,16.3220, 2285.9353,-1654.9027,14.9256, 25000,CAMERA_MOVE);
		    InterpolateCameraLookAt(playerid, 2450.8357,-1659.1113,13.5469, 2277.4260,-1642.6184,15.2629, 25000,CAMERA_MOVE);

			PlayerInfo[playerid][LoginTimer][1] = SetTimerEx("OnLoginCameraChange", 45000, false, "dd", playerid, 3);
		}
		case 3:
		{
			InterpolateCameraPos(playerid,2285.9353,-1654.9027,14.9256, 2319.6384,-1730.5212,13.3828, 25000,CAMERA_MOVE);
		    InterpolateCameraLookAt(playerid, 2277.4260,-1642.6184,15.2629, 2313.6042,-1716.0900,14.6496, 25000,CAMERA_MOVE);

			PlayerInfo[playerid][LoginTimer][1] = SetTimerEx("OnLoginCameraChange", 45000, false, "dd", playerid, 4);
		}
		case 4:
		{
			InterpolateCameraPos(playerid, 2319.6384,-1730.5212,13.3828, 2476.3250,-1738.1549,15.5469, 25000,CAMERA_MOVE);
		    InterpolateCameraLookAt(playerid, 2313.6042,-1716.0900,14.6496, 2485.7910,-1725.9292,13.5469, 25000,CAMERA_MOVE);

			PlayerInfo[playerid][LoginTimer][1] = SetTimerEx("OnLoginCameraChange", 45000, false, "dd", playerid, 1);
		}
	}
	return 1;
}