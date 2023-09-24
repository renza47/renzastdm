#include <YSI_Coding\y_hooks>

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch (dialogid)
	{
		case DIALOG_ARENA:
		{
			if(response)
			{
				if((GameInfo[listitem][GamePlayers] + 1) >= 10) return SendErrorMessage(playerid, "Max arena player reached. Returning to lobby.");

                SendGameMessage(GAME_MODE_LOBBY, COLOR_RED, "[ # ]{AFAFAF} %s has joined DM arena (/dm %d)", ReturnName(playerid), listitem + 1);
	            SendGameMessage(GAME_MODE_DM, COLOR_RED, "[ # ]{AFAFAF} %s has joined DM arena (/dm %d)", ReturnName(playerid), listitem + 1);
				SendPlayerToDMArena(playerid, listitem);
			}
		}
	}
	return 1;
}

hook OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	new Float:hp, Float:ar;
    GetPlayerHP(damagedid, hp);
    GetPlayerArmor(damagedid, ar);
	if(GetPlayerGameMode(playerid) == GAME_MODE_DM)
    {
        if(gettime() - PlayerInfo[damagedid][RecentlySpawned] < 4)
        {
            GameTextForPlayer(playerid, "~r~NO SPAWN KILL", 2000, 6);
            amount = 0;
            SetPlayerArmor(damagedid, ar);
            return SetPlayerHP(damagedid, hp);
        }

        if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE)
        {
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

        if(PlayerInfo[playerid][DM] == GAME_GHOST_TOWN && PlayerInfo[damagedid][DM] == GAME_GHOST_TOWN) amount = 0.0;

        UpdateNameTag(damagedid, true);

        if(bodypart == 9)
        {
            amount = amount * 1.5;
            if(PlayerInfo[damagedid][State] == PLAYER_STATE_ALIVE) GameTextForPlayer(playerid,"~r~Headshot!",2000,6);

            if(PlayerInfo[playerid][DM] == GAME_GHOST_TOWN && PlayerInfo[damagedid][DM] == GAME_GHOST_TOWN)
            {
                HitMarkerForPlayer(playerid, damagedid, weaponid, amount);
                if(PlayerInfo[playerid][HitSound]) PlayerPlaySound(playerid, 17802, 0.0, 0.0, 0.0);

                if(PlayerInfo[damagedid][State] == PLAYER_STATE_ALIVE)
                {
                    new rand = randomEx(5, 10);
                    if(bodypart != 9) ApplyAnimation(damagedid, "PED","KO_shot_front", 4.1, 0, 0, 0, 1, 0, 1);
                    else ApplyAnimation(damagedid, "PED", "KO_shot_face", 4.1, 0, 0, 0, 1, 0, 1);

                    new
                        s_Name[MAX_PLAYER_NAME],
                        s_Str[144]
                    ;
                    
                    //signature for player (the one who died)
                    GetPlayerName(playerid, s_Name, MAX_PLAYER_NAME);
                    format(s_Str, sizeof(s_Str), "You got killed by ~r~%s(%i)", s_Name, playerid);

                    PlayerTextDrawSetString(damagedid, PlayerInfo[damagedid][i_Textdraw][0], s_Str);
                    PlayerTextDrawShow(damagedid, PlayerInfo[damagedid][i_Textdraw][0]);

                    ShowPlayerSignature(damagedid, playerid, 3000);

                    GetPlayerName(damagedid, s_Name, MAX_PLAYER_NAME);
                    format(s_Str, sizeof(s_Str), "You killed ~r~%s(%i)", s_Name, damagedid);

                    PlayerTextDrawSetString(playerid, PlayerInfo[playerid][i_Textdraw][0], s_Str);
                    PlayerTextDrawShow(playerid, PlayerInfo[playerid][i_Textdraw][0]);

                    if(PlayerInfo[playerid][i_ExpireTimer] != -1)
                    {
                        KillTimer(PlayerInfo[playerid][i_ExpireTimer]);
                        PlayerInfo[playerid][i_ExpireTimer] = -1;
                    }

                    PlayerInfo[playerid][i_ExpireTimer] = SetTimerEx("OnPlayerSignatureExpire", 3000, false, "i", playerid);

                    if(PlayerInfo[damagedid][KillStreak] >= 3) SendGameMessage(GetPlayerGameMode(playerid), COLOR_RED, "SPREE: %s{FFFFFF} has ended %s's %d kill spree.", PlayerInfo[playerid][Name], PlayerInfo[damagedid][Name], PlayerInfo[damagedid][KillStreak]);

                    PlayerInfo[damagedid][State] = PLAYER_STATE_WOUNDED;
                    PlayerInfo[damagedid][DamageFix] = 4;

                    PlayerInfo[damagedid][KillStreak] = 0;

                    PlayerInfo[damagedid][MedkitTime] = 0;

                    if((PlayerInfo[playerid][Health] + 25.0) <= 100) PlayerInfo[playerid][Health] += 25.0;
                    else if((PlayerInfo[playerid][Health] + 25.0) > 100) SetPlayerHP(playerid, 100.0);

                    ClearPlayerDamages(damagedid);

                    SetPlayerSkillLevel(damagedid, WEAPONSKILL_PISTOL, 0);
                    SetPlayerSkillLevel(damagedid, WEAPONSKILL_MICRO_UZI, 0);
                    SetPlayerSkillLevel(damagedid, WEAPONSKILL_SPAS12_SHOTGUN, 0);

                    foreach(new i : Player)
                    {
                        if(GetPlayerGameMode(i) == GetPlayerGameMode(playerid)) SendDeathMessageToPlayer(i, playerid, damagedid, weaponid);
                    }
                    UpdatePlayerDeaths(damagedid);
                    UpdatePlayerKills(playerid);	
                    SetPlayerHP(damagedid, 30);
                    PlayerInfo[playerid][Score] += 1;
                    PlayerInfo[playerid][Cash] += rand;
                    PlayerInfo[playerid][KillStreak]++;
                    switch(PlayerInfo[playerid][KillStreak])
                    {
                        case 2: GameTextForPlayer(playerid,"~r~Double Kill!",3000,6);
                        case 3: GameTextForPlayer(playerid,"~r~Triple Kill!",3000,6);
                        case 4: GameTextForPlayer(playerid,"~r~Unstoppable!",3000,6);
                        case 5: 
                        {
                            GameTextForPlayer(playerid,"~r~Multi Kill!",3000,6);
                            SendGameMessage(GetPlayerGameMode(playerid), COLOR_RED, "SPREE: %s{FFFFFF} is on 5 killing spree!", PlayerInfo[playerid][Name]);
                            SendClientMessageEx(playerid, COLOR_RED, "SPREE:{FFFFFF} You've been given bonus armour (25.0) for your killing spree.");
                            PlayerInfo[playerid][Armour] += 25.0;
                            PlayerInfo[playerid][Score] += 2;
                            PlayerInfo[playerid][Cash] += rand + 5;
                        }
                        case 10: 
                        {
                            GameTextForPlayer(playerid,"~r~Monster Kill!",3000,6);
                            SendGameMessage(GetPlayerGameMode(playerid), COLOR_RED, "SPREE: %s{FFFFFF} is on 10 killing spree!", PlayerInfo[playerid][Name]);
                            SendClientMessageEx(playerid, COLOR_RED, "SPREE:{FFFFFF} You've been given bonus armour (50.0) for your killing spree.");
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

        if(PlayerInfo[playerid][HitSound]) PlayerPlaySound(playerid, 17802, 0.0, 0.0, 0.0);

        HitMarkerForPlayer(playerid, damagedid, weaponid, amount);

        new stringx[128];
        format(stringx, sizeof(stringx), "%.2f from {%06x}%s", amount, GetPlayerColor(playerid) >>> 8, ReturnName(playerid));
        SetPlayerChatBubble(damagedid, stringx, COLOR_RED, 30.0, 1500); 

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
                new rand = randomEx(5, 10);
                if(bodypart != 9) ApplyAnimation(damagedid, "PED","KO_shot_front", 4.1, 0, 0, 0, 1, 0, 1);
                else ApplyAnimation(damagedid, "PED", "KO_shot_face", 4.1, 0, 0, 0, 1, 0, 1);

                new
                    s_Name[MAX_PLAYER_NAME],
                    s_Str[144]
                ;
                
                //signature for player (the one who died)
                GetPlayerName(playerid, s_Name, MAX_PLAYER_NAME);
                format(s_Str, sizeof(s_Str), "You got killed by ~r~%s(%i)", s_Name, playerid);

                PlayerTextDrawSetString(damagedid, PlayerInfo[damagedid][i_Textdraw][0], s_Str);
                PlayerTextDrawShow(damagedid, PlayerInfo[damagedid][i_Textdraw][0]);

                ShowPlayerSignature(damagedid, playerid, 3000);

                GetPlayerName(damagedid, s_Name, MAX_PLAYER_NAME);
                format(s_Str, sizeof(s_Str), "You killed ~r~%s(%i)", s_Name, damagedid);

                PlayerTextDrawSetString(playerid, PlayerInfo[playerid][i_Textdraw][0], s_Str);
                PlayerTextDrawShow(playerid, PlayerInfo[playerid][i_Textdraw][0]);

                if(PlayerInfo[playerid][i_ExpireTimer] != -1)
                {
                    KillTimer(PlayerInfo[playerid][i_ExpireTimer]);
                    PlayerInfo[playerid][i_ExpireTimer] = -1;
                }

                PlayerInfo[playerid][i_ExpireTimer] = SetTimerEx("OnPlayerSignatureExpire", 3000, false, "i", playerid);

                if(PlayerInfo[damagedid][KillStreak] >= 3) SendGameMessage(GetPlayerGameMode(playerid), COLOR_RED, "SPREE: %s{FFFFFF} has ended %s's %d kill spree.", PlayerInfo[playerid][Name], PlayerInfo[damagedid][Name], PlayerInfo[damagedid][KillStreak]);

                PlayerInfo[damagedid][State] = PLAYER_STATE_WOUNDED;
                PlayerInfo[damagedid][DamageFix] = 4;

                PlayerInfo[damagedid][KillStreak] = 0;

                PlayerInfo[damagedid][MedkitTime] = 0;

                if((PlayerInfo[playerid][Health] + 25.0) <= 100) PlayerInfo[playerid][Health] += 25.0;
                else if((PlayerInfo[playerid][Health] + 25.0) > 100) SetPlayerHP(playerid, 100.0);

                ClearPlayerDamages(damagedid);

                SetPlayerSkillLevel(damagedid, WEAPONSKILL_PISTOL, 0);
                SetPlayerSkillLevel(damagedid, WEAPONSKILL_MICRO_UZI, 0);
                SetPlayerSkillLevel(damagedid, WEAPONSKILL_SPAS12_SHOTGUN, 0);

                foreach(new i : Player)
                {
                    if(GetPlayerGameMode(i) == GetPlayerGameMode(playerid)) SendDeathMessageToPlayer(i, playerid, damagedid, weaponid);
                }
                UpdatePlayerDeaths(damagedid);
                UpdatePlayerKills(playerid);	
                SetPlayerHP(damagedid, 30);
                PlayerInfo[playerid][Score] += 1;
                PlayerInfo[playerid][Cash] += rand;
                PlayerInfo[playerid][KillStreak]++;
                switch(PlayerInfo[playerid][KillStreak])
                {
                    case 2: GameTextForPlayer(playerid,"~r~Double Kill!",3000,6);
                    case 3: GameTextForPlayer(playerid,"~r~Triple Kill!",3000,6);
                    case 4: GameTextForPlayer(playerid,"~r~Unstoppable!",3000,6);
                    case 5: 
                    {
                        GameTextForPlayer(playerid,"~r~Multi Kill!",3000,6);
                        SendGameMessage(GetPlayerGameMode(playerid), COLOR_RED, "SPREE: %s{FFFFFF} is on 5 killing spree!", PlayerInfo[playerid][Name]);
                        SendClientMessageEx(playerid, COLOR_RED, "SPREE:{FFFFFF} You've been given bonus armour (25.0) for your killing spree.");
                        PlayerInfo[playerid][Armour] += 25.0;
                        PlayerInfo[playerid][Score] += 2;
                        PlayerInfo[playerid][Cash] += rand + 5;
                    }
                    case 10: 
                    {
                        GameTextForPlayer(playerid,"~r~Monster Kill!",3000,6);
                        SendGameMessage(GetPlayerGameMode(playerid), COLOR_RED, "SPREE: %s{FFFFFF} is on 10 killing spree!", PlayerInfo[playerid][Name]);
                        SendClientMessageEx(playerid, COLOR_RED, "SPREE:{FFFFFF} You've been given bonus armour (50.0) for your killing spree.");
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