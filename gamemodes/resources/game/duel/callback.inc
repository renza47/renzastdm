#include <YSI_Coding\y_hooks>

hook OnPlayerConnect(playerid)
{
	DuelInfo[playerid][DuelEnemy] = INVALID_PLAYER_ID;
	DuelInfo[playerid][DuelWeapon] = 0;
	DuelInfo[playerid][DuelWeapon2] = 0;
	DuelInfo[playerid][DuelBet] = 0;
	DuelInfo[playerid][DuelMap] = 0;
	return 1;
}

hook OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
    new Float:hp, Float:ar;
    GetPlayerHP(damagedid, hp);
    GetPlayerArmor(damagedid, ar);
    if(GetPlayerGameMode(playerid) == GAME_MODE_DUEL)
    {
        if(gettime() - PlayerInfo[damagedid][RecentlySpawned] < 4)
        {
            GameTextForPlayer(playerid, "~r~NO SPAWN KILL", 2000, 6);
            amount = 0;
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

		UpdateNameTag(damagedid, true);

        if(bodypart == 9)
        {
            amount = amount * 1.5;
            if(PlayerInfo[damagedid][State] == PLAYER_STATE_ALIVE) GameTextForPlayer(playerid,"~r~Headshot!",2000,6);
        }

        if(PlayerInfo[playerid][HitSound]) PlayerPlaySound(playerid, 17802, 0.0, 0.0, 0.0);

        HitMarkerForPlayer(playerid, damagedid, weaponid, amount);

        if(ar > 1)
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

        if(hp - amount <= 6)
        {
            if(PlayerInfo[damagedid][State] == PLAYER_STATE_ALIVE)
            {
                UpdatePlayerDeaths(damagedid);
                UpdatePlayerKills(DuelInfo[damagedid][DuelEnemy]);	

                new
                    s_Name[MAX_PLAYER_NAME],
                    s_Str[144]
                ;
                
                //signature for player (the one who died)
                GetPlayerName(playerid, s_Name, MAX_PLAYER_NAME);
                format(s_Str, sizeof(s_Str), "You got killed by ~r~%s(%i)", s_Name, playerid);

                PlayerTextDrawSetString(damagedid, PlayerInfo[damagedid][i_Textdraw][0], s_Str);
                PlayerTextDrawShow(damagedid, PlayerInfo[damagedid][i_Textdraw][0]);

                GetPlayerName(damagedid, s_Name, MAX_PLAYER_NAME);
                format(s_Str, sizeof(s_Str), "You killed ~r~%s(%i)", s_Name, damagedid);

                PlayerTextDrawSetString(playerid, PlayerInfo[playerid][i_Textdraw][0], s_Str);
                PlayerTextDrawShow(playerid, PlayerInfo[playerid][i_Textdraw][0]);

                PlayerInfo[damagedid][State] = PLAYER_STATE_WOUNDED;

                PlayerInfo[DuelInfo[damagedid][DuelEnemy]][Cash] += DuelInfo[damagedid][DuelBet];
                PlayerInfo[damagedid][Cash] -= DuelInfo[damagedid][DuelBet];

                SendGameMessage(GAME_MODE_LOBBY, COLOR_RED, "DUEL: {FFFFFF} %s defeated %s in a duel. (%s)", ReturnName(DuelInfo[damagedid][DuelEnemy]), ReturnName(damagedid),  ConvertTime(GetTickCount() - DuelInfo[damagedid][DuelTick]));
                SendGameMessage(GAME_MODE_DM, COLOR_RED, "DUEL: {FFFFFF} %s defeated %s in a duel. (%s)", ReturnName(DuelInfo[damagedid][DuelEnemy]), ReturnName(damagedid),  ConvertTime(GetTickCount() - DuelInfo[damagedid][DuelTick]));
                SendGameMessage(GAME_MODE_DUEL, COLOR_RED, "DUEL: {FFFFFF} %s defeated %s in a duel. (%s)", ReturnName(DuelInfo[damagedid][DuelEnemy]), ReturnName(damagedid),  ConvertTime(GetTickCount() - DuelInfo[damagedid][DuelTick]));

                SendPlayerToLobby(DuelInfo[damagedid][DuelEnemy]);
                DuelInfo[DuelInfo[damagedid][DuelEnemy]][DuelEnemy] = INVALID_PLAYER_ID;
                DuelInfo[DuelInfo[damagedid][DuelEnemy]][DuelWeapon] = 0;
                DuelInfo[DuelInfo[damagedid][DuelEnemy]][DuelWeapon2] = 0;
                DuelInfo[DuelInfo[damagedid][DuelEnemy]][DuelBet] = 0;
                DuelInfo[DuelInfo[damagedid][DuelEnemy]][DuelMap] = 0;

                SendPlayerToLobby(damagedid);
                DuelInfo[damagedid][DuelEnemy] = INVALID_PLAYER_ID;
                DuelInfo[damagedid][DuelWeapon] = 0;
                DuelInfo[damagedid][DuelWeapon2] = 0;
                DuelInfo[damagedid][DuelBet] = 0;
                DuelInfo[damagedid][DuelMap] = 0;
            }
        }
    }
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        case DIALOG_DUEL_WEAPON:
		{
			if(response)
			{
				new weaponid;
				switch(listitem)
				{
					case 0: weaponid = 9;
					case 1: weaponid = 16;
					case 2: weaponid = 18;
					case 3: weaponid = 22;
					case 4: weaponid = 23;
					case 5: weaponid = 24;
					case 6: weaponid = 25;
					case 7: weaponid = 26;
					case 8: weaponid = 27;
					case 9: weaponid = 28;
					case 10: weaponid = 29;
					case 11: weaponid = 30;
					case 12: weaponid = 31;
					case 13: weaponid = 32;
					case 14: weaponid = 33;
					case 15: weaponid = 34;
					default: weaponid = 24;
				}
				DuelInfo[playerid][DuelWeapon] = weaponid;
				DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelWeapon] = weaponid;

				new str[512];
				format(str, 512, "{FF0000}» {FFFFFF}Chain Saw\n{FF0000}» {FFFFFF}Grenade\n{FF0000}» {FFFFFF}Molotov\n{FF0000}» {FFFFFF}9MM\n{FF0000}» {FFFFFF}Silenced 9MM\n{FF0000}» {FFFFFF}Desert Eagle\n{FF0000}» {FFFFFF}Shotgun\n{FF0000}» {FFFFFF}Sawn Off\n{FF0000}» {FFFFFF}Combat Shotgun\n{FF0000}» {FFFFFF}UZI\n{FF0000}» {FFFFFF}MP5\n{FF0000}» {FFFFFF}AK-47\n{FF0000}» {FFFFFF}M4\n{FF0000}» {FFFFFF}TEC-9\n{FF0000}» {FFFFFF}Rifle\n{FF0000}» {FFFFFF}Sniper Rifle");
				ShowPlayerDialog(playerid, DIALOG_DUEL_WEAPON2, DIALOG_STYLE_LIST, "Duel Weapon 2", str, "Select", "Abort");
			}
		}
		case DIALOG_DUEL_WEAPON2:
		{
			if(response)
			{
				new weaponid;
				switch(listitem)
				{
					case 0: weaponid = 9;
					case 1: weaponid = 16;
					case 2: weaponid = 18;
					case 3: weaponid = 22;
					case 4: weaponid = 23;
					case 5: weaponid = 24;
					case 6: weaponid = 25;
					case 7: weaponid = 26;
					case 8: weaponid = 27;
					case 9: weaponid = 28;
					case 10: weaponid = 29;
					case 11: weaponid = 30;
					case 12: weaponid = 31;
					case 13: weaponid = 32;
					case 14: weaponid = 33;
					case 15: weaponid = 34;
					default: weaponid = 24;
				}
				DuelInfo[playerid][DuelWeapon2] = weaponid;
				DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelWeapon2] = weaponid;

				ShowPlayerDialog(playerid, DIALOG_DUEL_MAP, DIALOG_STYLE_LIST, "Duel Map", "{FF0000}» {FFFFFF}LVPD\n\
																														{FF0000}» {FFFFFF}Warehouse\n\
																														{FF0000}» {FFFFFF}RC Battlefield", "Select", "Abort");
			}
		}
		case DIALOG_DUEL_MAP:
		{
			if(response)
			{
				new mapid;
				if(listitem == 0) mapid = 0;
				if(listitem == 1) mapid = 1;
				if(listitem == 2) mapid = 2;

				DuelInfo[playerid][DuelMap] = mapid;
				DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelMap] = mapid;

				if(!IsPlayerConnected(DuelInfo[playerid][DuelEnemy])) return SendErrorMessage(playerid, "[Duel] The player you specified isn't connected.");
				if(GetPlayerGameMode(DuelInfo[playerid][DuelEnemy]) != GAME_MODE_LOBBY) return SendErrorMessage(playerid, "[Duel] The player you specified isn't in lobby.");
				if(PlayerInfo[playerid][Cash] < DuelInfo[playerid][DuelBet]) return SendErrorMessage(playerid,, 0x99CC00FF, "[Duel] You don't have enough money.");
				if(DuelInfo[playerid][DuelBet] < 0) return SendErrorMessage(playerid, "[Duel] Invalid bet amount.");

				new strr[256];
				SendClientMessageEx(playerid, COLOR_RED, "DUEL: {FFFFFF} You've sent a duel request to %s(%i).", ReturnName(DuelInfo[playerid][DuelEnemy]), DuelInfo[playerid][DuelEnemy]);
				SendClientMessageEx(playerid, COLOR_RED, "DUEL: {FFFFFF} Weapons: %s & %s.",ReturnWeaponNameEx(DuelInfo[playerid][DuelWeapon]), ReturnWeaponNameEx(DuelInfo[playerid][DuelWeapon2]));
				SendClientMessageEx(playerid, COLOR_RED, "DUEL: {FFFFFF} Bet: $%i. Map: %s", DuelInfo[playerid][DuelBet], ReturnMapName(DuelInfo[playerid][DuelMap]));
				format(strr, sizeof(strr), "{FF0000}» {FFFFFF}Opponent: %s(%i)\n\n\
												{FF0000}» {FFFFFF}Bet: $%i\n\n\
												{FF0000}» {FFFFFF}Weapon 1: %s\n\
												{FF0000}» {FFFFFF}Weapon 2: %s\n\n\
												{FF0000}» {FFFFFF}Map: %s", ReturnName(playerid), playerid, DuelInfo[playerid][DuelBet], ReturnWeaponNameEx(DuelInfo[playerid][DuelWeapon]), ReturnWeaponNameEx(DuelInfo[playerid][DuelWeapon2]) , ReturnMapName(DuelInfo[playerid][DuelMap]));
				ShowPlayerDialog(DuelInfo[playerid][DuelEnemy], DIALOG_DUEL, DIALOG_STYLE_MSGBOX, "Duel", strr, "Accept", "Abort");
			}
		}
		case DIALOG_DUEL:
		{
			if(!response)
			{
				new string[256];
				format(string, sizeof(string), "DUEL: %s refused the duel request.", ReturnName(DuelInfo[playerid][DuelEnemy]));
				SendClientMessage(DuelInfo[playerid][DuelEnemy], COLOR_RED, string);

				//SendPlayerToLobby(DuelInfo[playerid][DuelEnemy]);
				DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelEnemy] = INVALID_PLAYER_ID;
				DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelWeapon] = 0;
				DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelWeapon2] = 0;
				DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelBet] = 0;

				//SendPlayerToLobby(playerid);
				DuelInfo[playerid][DuelEnemy] = INVALID_PLAYER_ID;
				DuelInfo[playerid][DuelWeapon] = 0;
				DuelInfo[playerid][DuelWeapon2] = 0;
				DuelInfo[playerid][DuelBet] = 0;
			}
			else if(response)
			{
				if(!IsPlayerConnected(DuelInfo[playerid][DuelEnemy])) return SendErrorMessage(playerid, "[Duel] The player you specified isn't connected.");
				if(GetPlayerGameMode(DuelInfo[playerid][DuelEnemy]) != GAME_MODE_LOBBY) return SendErrorMessage(playerid, "[Duel] The player you specified isn't in lobby.");
				if(PlayerInfo[playerid][Cash] < DuelInfo[playerid][DuelBet]) return SendErrorMessage(playerid, "[Duel] You don't have enough money.");
				if(DuelInfo[playerid][DuelBet] < 0) return SendErrorMessage(playerid, "[Duel] Invalid duel amount.");

				SendGameMessage(GAME_MODE_LOBBY, COLOR_RED, "DUEL: {FFFFFF}Duel between %s vs %s has been started!", ReturnName(DuelInfo[playerid][DuelEnemy]), ReturnName(playerid));
		        SendGameMessage(GAME_MODE_DM, COLOR_RED, "DUEL: {FFFFFF}Duel between %s vs %s has been started!", ReturnName(DuelInfo[playerid][DuelEnemy]), ReturnName(playerid));

				ResetPlayerWeapons(playerid);
				SetPlayerGameMode(playerid, GAME_MODE_DUEL);
				GivePlayerWeapon(playerid, DuelInfo[playerid][DuelWeapon], 5000);
				GivePlayerWeapon(playerid, DuelInfo[playerid][DuelWeapon2], 5000);
				SetPlayerHP(playerid, 100.0);
				SetPlayerArmor(playerid, 100.0);

				ResetPlayerWeapons(DuelInfo[playerid][DuelEnemy]);
				SetPlayerGameMode(DuelInfo[playerid][DuelEnemy], GAME_MODE_DUEL);
				GivePlayerWeapon(DuelInfo[playerid][DuelEnemy], DuelInfo[playerid][DuelWeapon], 5000);
				GivePlayerWeapon(DuelInfo[playerid][DuelEnemy], DuelInfo[playerid][DuelWeapon2], 5000);
				SetPlayerHP(DuelInfo[playerid][DuelEnemy], 100.0);
				SetPlayerArmor(DuelInfo[playerid][DuelEnemy], 100.0);

				switch(DuelInfo[playerid][DuelMap])
				{
					case 0:
					{
						SetPlayerInterior(playerid, 3);
						SetPlayerPos(playerid, 289.3659,169.0151,1007.1719);
						SetPlayerFacingAngle(playerid,2.0);
						SetPlayerVirtualWorld(playerid, playerid+100);
						SetCameraBehindPlayer(playerid);

                        SetPlayerInterior(DuelInfo[playerid][DuelEnemy], 3);
						SetPlayerPos(DuelInfo[playerid][DuelEnemy], 217.0308,168.7015,1003.0234);
						SetPlayerFacingAngle(DuelInfo[playerid][DuelEnemy],90.0);
						SetPlayerVirtualWorld(DuelInfo[playerid][DuelEnemy], playerid+100);
						SetCameraBehindPlayer(DuelInfo[playerid][DuelEnemy]);
					}
					case 1:
					{
						SetPlayerInterior(playerid, 18);
						SetPlayerPos(playerid, 1306.6357,-0.0754,1001.0282);

						SetPlayerVirtualWorld(playerid, playerid+100);
						SetCameraBehindPlayer(playerid);

						SetPlayerInterior(DuelInfo[playerid][DuelEnemy], 18);
						SetPlayerPos(DuelInfo[playerid][DuelEnemy],1251.5892,-33.8911,1001.0344);

						SetPlayerVirtualWorld(DuelInfo[playerid][DuelEnemy], playerid+100);
						SetCameraBehindPlayer(DuelInfo[playerid][DuelEnemy]);
					}
					case 2:
					{
						SetPlayerPos(playerid, -1131.9055, 1057.8958, 1346.4146);
						SetPlayerInterior(playerid, 10);
						SetPlayerVirtualWorld(playerid, playerid+100);
						SetCameraBehindPlayer(playerid);

						SetPlayerPos(DuelInfo[playerid][DuelEnemy], -974.6671, 1060.8036, 1345.6719);
						SetPlayerInterior( DuelInfo[playerid][DuelEnemy], 10);
						SetPlayerVirtualWorld( DuelInfo[playerid][DuelEnemy], playerid+100);
						SetCameraBehindPlayer(DuelInfo[playerid][DuelEnemy]);
					}
				}
				TogglePlayerControllable(playerid, false);
				TogglePlayerControllable(DuelInfo[playerid][DuelEnemy], false);

				DuelCountdown[playerid] = 6;
				KillTimer(DuelTimer[playerid]);
				DuelTimer[playerid] = SetTimerEx("Duel_Countdown", 1000, true, "i", playerid);

				DuelCountdown[DuelInfo[playerid][DuelEnemy]] = 6;
				KillTimer(DuelTimer[DuelInfo[playerid][DuelEnemy]]);
				DuelTimer[DuelInfo[playerid][DuelEnemy]] = SetTimerEx("Duel_Countdown", 1000, true, "i", DuelInfo[playerid][DuelEnemy]);
				return 1;
			}
		}
    }
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	if(GetPlayerGameMode(playerid) == GAME_MODE_DUEL)
	{
	    PlayerInfo[DuelInfo[playerid][DuelEnemy]][Cash] += DuelInfo[playerid][DuelBet];
	    PlayerInfo[playerid][Cash] -= DuelInfo[playerid][DuelBet];
	    SendGameMessage(GAME_MODE_LOBBY, COLOR_RED, "DUEL: {FFFFFF} %s defeated %s in a duel. (%s)", ReturnName(DuelInfo[playerid][DuelEnemy]),  ConvertTime(GetTickCount() - DuelInfo[playerid][DuelTick]));
		SendGameMessage(GAME_MODE_DM, COLOR_RED, "DUEL: {FFFFFF} %s defeated %s in a duel. (%s)", ReturnName(DuelInfo[playerid][DuelEnemy]),  ConvertTime(GetTickCount() - DuelInfo[playerid][DuelTick]));
        SendGameMessage(GAME_MODE_DUEL, COLOR_RED, "DUEL: {FFFFFF} %s defeated %s in a duel. (%s)", ReturnName(DuelInfo[playerid][DuelEnemy]),  ConvertTime(GetTickCount() - DuelInfo[playerid][DuelTick]));
	    
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
	KillTimer(DuelTimer[playerid]);
	return 1;
}