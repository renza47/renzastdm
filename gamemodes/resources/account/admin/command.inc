alias:a("admin", "achat")
alias:spectate("spec", "awp")
alias:offlineban("oban", "banaccount")
alias:freeze("froze")
alias:unfreeze("unfroze", "thaw")
alias:adminhelp("ah", "ahelp", "moderatorhelp", "mhelp")

alias:acceptreport("ar")
alias:denyreport("dr")

alias:staff("activities", "staffmembers", "adminlist")

cmd:adminhelp(playerid, params[])
{
    if(!PlayerInfo[playerid][AdminLevel]) return SendErrorPermMessage(playerid);

    if(PlayerInfo[playerid][AdminLevel])
    {
        SendClientMessage(playerid, 0xbf9000FF, "__________________Staff Commands__________________");
        SendClientMessage(playerid, 0xbf9000FF, "Moderator (1): /staff /a(dmin) /kick /(un)mute /reports /a(ccept)r(eport) /d(eny)r(eport)");
        if(PlayerInfo[playerid][AdminLevel] >= 2) SendClientMessage(playerid, 0xbf9000FF, "Senior Moderator (2): /aduty /goto /gethere /setskin /spectate /specoff /(un)ban /respawncars /noglobal");
        if(PlayerInfo[playerid][AdminLevel] >= 2) SendClientMessage(playerid, 0xbf9000FF, "Senior Moderator (2): /freeze /unfreeze /forcelobby /checkip /jetpack /hideme");
        if(PlayerInfo[playerid][AdminLevel] >= 3) SendClientMessage(playerid, 0xbf9000FF, "Admin: /adminhelp /(un)ban /offlineban");
		//if(PlayerInfo[playerid][AdminLevel] >= 7) SendClientMessage(playerid, 0xbf9000FF, "Lead Admin: /changename");
        if(PlayerInfo[playerid][AdminLevel] >= 8) SendClientMessage(playerid, 0xbf9000FF, "Management/Lead (8): /kickall /makeadmin /resethours");
    }
    return 1;
}

cmd:admins(playerid, params[]) 
{
	new total_admins = 0;
	SendClientMessageEx(playerid, COLOR_RED, "---------------------------------");
	foreach (new i : Player) 
	{
		if (PlayerInfo[i][AdminLevel] && PlayerInfo[i][Hidden] == false)
		{
			total_admins++;
			SendClientMessageEx(playerid, COLOR_GREY, "%s %s", ReturnAdminRank(i), PlayerInfo[i][Name]);
		}
	}
	SendClientMessageEx(playerid, COLOR_RED, "---------------------------------");
	SendClientMessageEx(playerid, COLOR_GREY, "There are %d admin(s) online.", total_admins);
	return 1;
}

cmd:staff(playerid, params[])
{
	if (!PlayerInfo[playerid][AdminLevel]) return SendErrorPermMessage(playerid);

	SendClientMessageEx(playerid, COLOR_RED, "Fetching data, please wait...");

	UpdatePlayerData(playerid, 0);

	new query[103];
	mysql_format(g_SQL, query, sizeof query, "SELECT * FROM `players` WHERE `admin_level` != 0");
	mysql_tquery(g_SQL, query, "ShowStaffActivities", "i", playerid);
	return 1;
}

cmd:aduty(playerid, params[])
{
	new string[256];

	if (PlayerInfo[playerid][AdminLevel] < 2) return SendErrorPermMessage(playerid);

	if (PlayerInfo[playerid][AdminDuty] == false) 
	{
		foreach (new i : Player) 
		{
			if (PlayerInfo[i][AdminLevel])
			{
				SendClientMessageEx(i, 0xbf9000FF, "[AdmWarn] %s is now on duty as a %s!", PlayerInfo[playerid][Name], ReturnAdminRank(playerid));
			}
		}

		format(string, sizeof string, "[AdmWarn] %s is now on duty as a %s!", PlayerInfo[playerid][Name], ReturnAdminRank(playerid));
		SetPlayerColor(playerid, 0x8B0000FF);

		PlayerInfo[playerid][AdminDuty] = true;
	}
	else if (PlayerInfo[playerid][AdminDuty] == true) 
	{
		foreach (new i : Player) 
		{
			if (PlayerInfo[i][AdminLevel])
			{
				SendClientMessageEx(i, 0xbf9000FF, "[AdmWarn] %s is no longer on duty!", PlayerInfo[playerid][Name]);
			}
		}
		format(string, sizeof string, "[AdmWarn] %s is no longer on duty!", PlayerInfo[playerid][Name]);

		if(GetPlayerGameMode(playerid) == GAME_MODE_TDM)
		{
			SetPlayerColor(playerid, ReturnTeamColorEx(PlayerInfo[playerid][Team]));
		}
		else SendPlayerToLobby(playerid);

		PlayerInfo[playerid][AdminDuty] = false;
	}
	return 1;
}

cmd:a(playerid, params[]) 
{
	if (!PlayerInfo[playerid][AdminLevel]) return SendErrorPermMessage(playerid);

	if(isnull(params)) return SendClientMessage(playerid, 0xf6b26bFF, "USAGE: /a(dmin) [admin chat]");

	foreach (new i : Player) 
	{
		if (PlayerInfo[i][AdminLevel])
		{
			SendClientMessageEx(i, COLOR_YELLOW, "** %s %s: %s", ReturnAdminRank(playerid), PlayerInfo[playerid][Name], params);
		}
	}
	return 1;
}

cmd:reports(playerid, params[])
{
    new str[256];

	if (!PlayerInfo[playerid][AdminLevel]) return SendClientMessage(playerid, 0xe06666FF, "You don't have the right permission to use this command.");

    SendClientMessage(playerid, COLOR_RED, "----------------- Active Reports -----------------");

    for(new i = 1; i < MAX_REPORTS; i++)
    {
        if(ReportInfo[i][rID])
        {
            format(str, sizeof str, "[#%i %s] %s (%i): \"%s\"", i, ReportInfo[i][rDate], ReturnName(ReportInfo[i][rIssuer], false), ReportInfo[i][rIssuer], ReportInfo[i][rReason]);
			SendClientMessage(playerid, COLOR_GREY, str);
        }
    }
    return 1;
}

cmd:acceptreport(playerid, params[])
{
	new idx;
	if (!PlayerInfo[playerid][AdminLevel]) return SendClientMessage(playerid, 0xe06666FF, "You don't have the right permission to use this command.");

	if (sscanf(params, "d", idx)) return SendClientMessage(playerid, 0xf6b26bFF, "USAGE: /acceptreport [reportid]");

	if (!ReportInfo[idx][rID]) return SendErrorMessage(playerid, "Invalid report ID.");

	SendClientMessageEx(ReportInfo[idx][rIssuer], 0xe06666FF, "Admin %s has accepted your report. (\"%s\")", PlayerInfo[playerid][Name], ReportInfo[idx][rReason]);
	SendClientMessageEx(playerid, 0xe06666FF, "You accepted %s's report. (\"%s\")", PlayerInfo[ReportInfo[idx][rIssuer]][Name], ReportInfo[idx][rReason]);

	SendAdminMessageEx(0xbf9000FF, 1, "[AdmWarn] %s has accepted %s's report.", PlayerInfo[playerid][Name], PlayerInfo[ReportInfo[idx][rIssuer]][Name], ReportInfo[idx][rReason]);

	ReportInfo[idx][rID] = 0;
	return 1;
}

cmd:denyreport(playerid, params[])
{
	new idx;
	if (!PlayerInfo[playerid][AdminLevel]) return SendClientMessage(playerid, 0xe06666FF, "You don't have the right permission to use this command.");

	if (sscanf(params, "d", idx)) return SendClientMessage(playerid, 0xf6b26bFF, "USAGE: /denyreport [reportid]");

	if (!ReportInfo[idx][rID]) return SendErrorMessage(playerid, "Invalid report ID.");

	SendClientMessageEx(ReportInfo[idx][rIssuer], 0xe06666FF, "Admin %s has denied your report. (%s)", PlayerInfo[playerid][Name], ReportInfo[idx][rReason]);
	SendClientMessageEx(playerid, 0xe06666FF, "You denied %s's report. (%s)", PlayerInfo[ReportInfo[idx][rIssuer]][Name], ReportInfo[idx][rReason]);

	SendAdminMessageEx(0xbf9000FF, 1, "[AdmWarn] %s has denied %s's report.", PlayerInfo[playerid][Name], PlayerInfo[ReportInfo[idx][rIssuer]][Name], ReportInfo[idx][rReason]);

	ReportInfo[idx][rID] = 0;
	return 1;
}

cmd:kick(playerid, params[]) 
{
	new targetid, reason[120];
	if (!PlayerInfo[playerid][AdminLevel]) return SendClientMessage(playerid, 0xe06666FF, "You don't have the right permission to use this command.");

	if (sscanf(params, "us[120]", targetid, reason)) return SendClientMessage(playerid, 0xf6b26bFF, "USAGE: /kick [playerid/name] [reason]");

	if (!IsPlayerConnected(targetid) || targetid == INVALID_PLAYER_ID) return SendClientMessage(playerid, 0xe06666FF, "That player isn't connected.");

    if (PlayerInfo[targetid][AdminLevel] >= PlayerInfo[playerid][AdminLevel]) return SendClientMessage(playerid, 0xe06666FF, "You can't kick that player.");

    SendClientMessageToAllEx(0xe06666FF, "Admin %s has kicked %s for: %s", PlayerInfo[playerid][Name], PlayerInfo[targetid][Name], reason);
	DelayedKick(targetid, 500, true, reason);
	return 1;
}

cmd:mute(playerid, params[])
{
	new targetid, reason[120], time;
	if (!PlayerInfo[playerid][AdminLevel]) return SendClientMessage(playerid, 0xe06666FF, "You don't have the right permission to use this command.");

	if (sscanf(params, "uis[120]", targetid, time, reason)) return SendClientMessage(playerid, 0xf6b26bFF, "USAGE: /mute [playerid/name] [time in seconds (0 for permanent)] [reason]");

	if (!IsPlayerConnected(targetid) || targetid == INVALID_PLAYER_ID) return SendClientMessage(playerid, 0xe06666FF, "That player isn't connected.");

	if (PlayerInfo[targetid][AdminLevel] >= PlayerInfo[playerid][AdminLevel]) return SendClientMessage(playerid, 0xe06666FF, "You can't mute that player.");

	if (PlayerInfo[targetid][Muted]) return SendErrorMessage(playerid, "That player is already muted.");

    PlayerInfo[targetid][MuteTime] = 0;
	PlayerInfo[targetid][Muted] = 1;
	if(time != 0)
	{
		PlayerInfo[targetid][MuteTime] = time;
		SendClientMessageToAllEx(0xe06666FF, "Admin %s has muted %s (%i seconds) for: %s", PlayerInfo[playerid][Name], PlayerInfo[targetid][Name], time, reason);
	}
	else 
	{
		SendClientMessageToAllEx(0xe06666FF, "Admin %s has muted %s for: %s", PlayerInfo[playerid][Name], PlayerInfo[targetid][Name], reason);
	}
	return 1;
}

cmd:unmute(playerid, params[])
{
	new targetid;
	if (!PlayerInfo[playerid][AdminLevel]) return SendClientMessage(playerid, 0xe06666FF, "You don't have the right permission to use this command.");

	if (sscanf(params, "u", targetid)) return SendClientMessage(playerid, 0xf6b26bFF, "USAGE: /mute [playerid/name]");

	if (!IsPlayerConnected(targetid) || targetid == INVALID_PLAYER_ID) return SendClientMessage(playerid, 0xe06666FF, "That player isn't connected.");

	if (!PlayerInfo[targetid][Muted]) return SendErrorMessage(playerid, "That player isn't muted.");

	PlayerInfo[targetid][Muted] = 0;
	SendClientMessageToAllEx(0xe06666FF, "Admin %s has been unmuted by %s.", PlayerInfo[targetid][Name], PlayerInfo[playerid][Name]);
	return 1;
}

cmd:ban(playerid, params[])
{
	new targetid, reason[120], ipban;
	if (PlayerInfo[playerid][AdminLevel] < 2) return SendClientMessage(playerid, 0xe06666FF, "You don't have the right permission to use this command.");

	if (sscanf(params, "uds[120]", targetid, ipban, reason)) return SendClientMessage(playerid, 0xf6b26bFF, "USAGE: /ban [playerid/name] [IP ban (0 = false/1 = true)] [reason]");

	if (!IsPlayerConnected(targetid) || targetid == INVALID_PLAYER_ID) return SendClientMessage(playerid, 0xe06666FF, "That player isn't connected.");

	if (PlayerInfo[targetid][AdminLevel] >= PlayerInfo[playerid][AdminLevel]) return SendClientMessage(playerid, 0xe06666FF, "You can't ban that player.");

    new query[221], ipaddress[60];
	GetPlayerIp(targetid, ipaddress, sizeof ipaddress);
	if(ipban == 1)
	{
		mysql_format(g_SQL, query, sizeof query, "INSERT INTO `bans` (`user_id`, `username`, `admin_id`, `admin_name`, `reason`, `ban_date`, `ip`) VALUES ('%d', '%e', '%d', '%e', '%e', NOW(), '%e')", PlayerInfo[targetid][ID], PlayerInfo[targetid][Name], PlayerInfo[playerid][ID], PlayerInfo[playerid][Name], reason, ipaddress);
	    mysql_tquery(g_SQL, query, "OnPlayerBanned", "dds", targetid, playerid, reason);
	}
	else
	{
		mysql_format(g_SQL, query, sizeof query, "INSERT INTO `bans` (`user_id`, `username`, `admin_id`, `admin_name`, `reason`, `ban_date`) VALUES ('%d', '%e', '%d', '%e', '%e', NOW())", PlayerInfo[targetid][ID], PlayerInfo[targetid][Name], PlayerInfo[playerid][ID], PlayerInfo[playerid][Name], reason);
	    mysql_tquery(g_SQL, query, "OnPlayerBanned", "dds", targetid, playerid, reason);
	}
	return 1;
}

cmd:offlineban(playerid, params[])
{
	new targetid[24], reason[120], query[221];
	if (PlayerInfo[playerid][AdminLevel] < 3) return SendClientMessage(playerid, 0xe06666FF, "You don't have the right permission to use this command.");

	if (sscanf(params, "s[24]s[120]", targetid, reason)) return SendClientMessage(playerid, 0xf6b26bFF, "USAGE: /ban [playerid/name] [reason]");

	mysql_format(g_SQL, query, sizeof query, "INSERT INTO `bans` (`user_id`, `username`, `admin_id`, `admin_name`, `reason`, `ban_date`) VALUES ('-1', '%e', '%d', '%e', '%e', NOW())", targetid, PlayerInfo[playerid][ID], PlayerInfo[playerid][Name], reason);
	mysql_tquery(g_SQL, query, "OnPlayerOfflineBanned", "sds", targetid, playerid, reason);
	return 1;
}

cmd:unban(playerid, params[])
{
	new target[MAX_PLAYER_NAME];
	if (PlayerInfo[playerid][AdminLevel] < 3) return SendClientMessage(playerid, 0xe06666FF, "You don't have the right permission to use this command.");

	if (sscanf(params, "s[24]", target)) return SendClientMessage(playerid, 0xf6b26bFF, "USAGE: /unban [name]");

    new query[221];
	mysql_format(g_SQL, query, sizeof query, "SELECT * FROM `bans` WHERE `username` = '%e' LIMIT 1", target);
	mysql_tquery(g_SQL, query, "OnPlayerUnbanned", "d", playerid);
	return 1;
}

cmd:forcelobby(playerid, params[])
{
	if(PlayerInfo[playerid][AdminLevel] < 2)
		return SendErrorPermMessage(playerid);
		
	new playerb;
	
	if (sscanf(params, "u", playerb)) 
		return SendUsageMessage(playerid, "/forcelobby [playerid/name]"); 
	
	if(!IsPlayerConnected(playerb))
		return SendErrorMessage(playerid, "The player you specified isn't connected."); 
	
	if(PlayerInfo[playerb][IsLoggedIn] == false)
		return SendErrorMessage(playerid, "The player you specified isn't logged in.");

	SendPlayerToLobby(playerb);
		
	foreach (new i : Player) 
	{
		if (PlayerInfo[i][AdminLevel])
		{
			SendClientMessageEx(i, 0xbf9000FF, "[AdmWarn] %s has send %s to lobby.", PlayerInfo[playerid][Name], PlayerInfo[playerb][Name]);
		}
	}
	SendClientMessage(playerb, -1, "You have been sent to lobby.");
	return 1;
}

cmd:forcenocopchase(playerid, params[])
{
	if(PlayerInfo[playerid][AdminLevel] < 2)
		return SendErrorPermMessage(playerid);
		
	new playerb;
	
	if (sscanf(params, "u", playerb)) 
		return SendUsageMessage(playerid, "/forcenocopchase [playerid/name]"); 
	
	if(!IsPlayerConnected(playerb))
		return SendErrorMessage(playerid, "The player you specified isn't connected."); 
	
	if(PlayerInfo[playerb][IsLoggedIn] == false)
		return SendErrorMessage(playerid, "The player you specified isn't logged in.");

	PlayerInfo[playerb][NoCopchase] = 1;
		
	foreach (new i : Player) 
	{
		if (PlayerInfo[i][AdminLevel])
		{
			SendClientMessageEx(i, 0xbf9000FF, "[AdmWarn] %s has marked %s as afk.", PlayerInfo[playerid][Name], PlayerInfo[playerb][Name]);
		}
	}
	SendClientMessage(playerb, -1, "Autojoining copchase has been disabled since you were AFK.");
	return 1;
}

cmd:freeze(playerid, params[])
{
	if(PlayerInfo[playerid][AdminLevel] < 2)
		return SendErrorPermMessage(playerid);
		
	new playerb;
	
	if (sscanf(params, "u", playerb)) 
		return SendUsageMessage(playerid, "/freeze [playerid/name]"); 
	
	if(!IsPlayerConnected(playerb))
		return SendErrorMessage(playerid, "The player you specified isn't connected."); 
	
	if(PlayerInfo[playerb][IsLoggedIn] == false)
		return SendErrorMessage(playerid, "The player you specified isn't logged in.");

	TogglePlayerControllable(playerb, 0);
		
	foreach (new i : Player) 
	{
		if (PlayerInfo[i][AdminLevel])
		{
			SendClientMessageEx(i, 0xbf9000FF, "[AdmWarn] %s frozed  %s!", PlayerInfo[playerid][Name], PlayerInfo[playerb][Name]);
		}
	}
	SendClientMessage(playerb, -1, "You have been frozed.");
	return 1;
}

cmd:unfreeze(playerid, params[])
{
	if(PlayerInfo[playerid][AdminLevel] < 2)
		return SendErrorPermMessage(playerid);
		
	new playerb;
	
	if (sscanf(params, "u", playerb)) 
		return SendUsageMessage(playerid, "/unfreeze [playerid/name]"); 
	
	if(!IsPlayerConnected(playerb))
		return SendErrorMessage(playerid, "The player you specified isn't connected."); 
	
	if(PlayerInfo[playerb][IsLoggedIn] == false)
		return SendErrorMessage(playerid, "The player you specified isn't logged in.");

	TogglePlayerControllable(playerb, 1);
		
	foreach (new i : Player) 
	{
		if (PlayerInfo[i][AdminLevel])
		{
			SendClientMessageEx(i, 0xbf9000FF, "[AdmWarn] %s unfrozed  %s!", PlayerInfo[playerid][Name], PlayerInfo[playerb][Name]);
		}
	}
	SendClientMessage(playerb, -1, "You have been thawed.");
	return 1;
}

cmd:goto(playerid, params[])
{
	if(PlayerInfo[playerid][AdminLevel] < 2)
		return SendErrorPermMessage(playerid);
		
	new playerb, Float:X, Float:Y, Float:Z;
	
	if (sscanf(params, "u", playerb)) 
		return SendUsageMessage(playerid, "/goto [playerid/name]"); 
	
	if(!IsPlayerConnected(playerb))
		return SendErrorMessage(playerid, "The player you specified isn't connected."); 
	
	if(PlayerInfo[playerb][IsLoggedIn] == false)
		return SendErrorMessage(playerid, "The player you specified isn't logged in.");

	if (PlayerInfo[playerid][AdminDuty] == false) return SendErrorMessage(playerid, "You must be on admin duty to use this command.");
		
	GetPlayerPos(playerb, X, Y, Z);
	//Using the player variable to avoid making other variables; 
	
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		SetVehiclePos(GetPlayerVehicleID(playerid), X, Y - 1, Z);
	
	else
		SetPlayerPos(playerid, X, Y - 1, Z);
		
	SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(playerb));
	
	if(GetPlayerInterior(playerb) != 0)
		SetPlayerInterior(playerid, GetPlayerInterior(playerb)); 
		
	SendClientMessage(playerid, -1, "You have been teleported.");	
	SendClientMessage(playerb, -1, "You have been teleported.");
	return 1;
}

cmd:gethere(playerid, params[])
{
	if(PlayerInfo[playerid][AdminLevel] < 2)
		return SendErrorPermMessage(playerid);
		
	new playerb, Float:X, Float:Y, Float:Z;
	
	if (sscanf(params, "u", playerb)) 
		return SendUsageMessage(playerid, "/goto [playerid/name]"); 
	
	if(!IsPlayerConnected(playerb))
		return SendErrorMessage(playerid, "The player you specified isn't connected."); 

	if(PlayerInfo[playerb][IsLoggedIn] == false)
		return SendErrorMessage(playerid, "The player you specified isn't logged in.");

	if (PlayerInfo[playerid][AdminDuty] == false) return SendErrorMessage(playerid, "You must be on admin duty to use this command.");
		
	GetPlayerPos(playerid, X, Y, Z);
	//Using the player variable to avoid making other variables; 
	
	if(GetPlayerState(playerb) == PLAYER_STATE_DRIVER)
		SetVehiclePos(GetPlayerVehicleID(playerb), X, Y - 1, Z);
		
	else
		SetPlayerPos(playerb, X, Y - 1, Z);
		
	SetPlayerVirtualWorld(playerb, GetPlayerVirtualWorld(playerid));
	
	if(GetPlayerInterior(playerid) != 0)
		SetPlayerInterior(playerb, GetPlayerInterior(playerid)); 
		
	SendClientMessage(playerb, -1, "You have been teleported.");
	SendClientMessageEx(playerid, -1, "You teleported %s to your position.", ReturnName(playerb));
	
	return 1;
}

CMD:setskin(playerid, params[])
{
	if (PlayerInfo[playerid][AdminLevel] < 2) return SendErrorPermMessage(playerid);

	new targetid, skinid;

	if(sscanf(params, "ud", targetid, skinid)) return SendUsageMessage(playerid, "/setskin [playerid/name] [skin id]");

	if(!IsPlayerConnected(targetid)) return SendErrorMessage(playerid, "The player you specified isn't connected.");
	//if(PlayerInfo[targetid][Team] == -1) return SendErrorMessage(playerid, "The player you specified isn't logged in.");
	if(PlayerInfo[targetid][IsLoggedIn] == false) return SendErrorMessage(playerid, "The player you specified isn't logged in.");

	SetPlayerSkin(targetid, skinid);

	if(GetPlayerGameMode(targetid) != GAME_MODE_TDM) PlayerInfo[targetid][Model] = skinid;

	foreach (new i : Player) 
	{
		if (PlayerInfo[i][AdminLevel])
		{
			SendClientMessageEx(i, 0xbf9000FF, "[AdmWarn] %s has set %s's skin to %d!", PlayerInfo[playerid][Name], PlayerInfo[targetid][Name], skinid);
		}
	}
	return 1;
}

CMD:spectate(playerid, params[])
{
	if(PlayerInfo[playerid][AdminLevel] < 2)
		return SendErrorPermMessage(playerid);
		
	new playerb;
	
	if (sscanf(params, "u", playerb))
		return SendUsageMessage(playerid, "/spectate [playerid/name]");
		
	if(!IsPlayerConnected(playerb))
		return SendErrorMessage(playerid, "The player you specified isn't connected."); 
		
	if(PlayerInfo[playerb][Spectating] != INVALID_PLAYER_ID)
		return SendErrorMessage(playerid, "That player is spectating another player."); 
		
	if(playerb == playerid) 
		return SendErrorMessage(playerid, "You can't spectate yourself.");

	//if (PlayerInfo[playerid][AdminDuty] == false) return SendErrorMessage(playerid, "You must be on admin duty to use this command.");
	
	SetPlayerInterior(playerid, GetPlayerInterior(playerb));
	SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(playerb));
	
	TogglePlayerSpectating(playerid, true); 
	PlayerSpectatePlayer(playerid, playerb);
		
	PlayerInfo[playerid][Spectating] = playerb; 
	SendClientMessageEx(playerid, COLOR_GREY, "You're now spectating %s.", PlayerInfo[playerb][Name]);
	SendClientMessage(playerid, COLOR_GREY, "/specoff to stop spectating.");

	if(IsValidDynamic3DTextLabel(PlayerInfo[playerid][Nametag])) 
    {
        DestroyDynamic3DTextLabel(PlayerInfo[playerid][Nametag]);
        PlayerInfo[playerid][Nametag] = Text3D:INVALID_3DTEXT_ID;
    }

	foreach (new i : Player) 
	{
		if (PlayerInfo[i][AdminLevel])
		{
			SendClientMessageEx(i, 0xbf9000FF, "[AdmWarn] %s starts spectating  %s!", PlayerInfo[playerid][Name], PlayerInfo[playerb][Name]);
		}
	}
	return 1;
}

CMD:specoff(playerid, params[])
{
	if(PlayerInfo[playerid][AdminLevel] < 2)
		return SendErrorPermMessage(playerid);
		
	if (GetPlayerState(playerid) != PLAYER_STATE_SPECTATING)
		return SendErrorMessage(playerid, "You aren't spectating anyone."); 
		
	SendClientMessageEx(playerid, COLOR_GREY, "You stopped spectating %s.", PlayerInfo[PlayerInfo[playerid][Spectating]][Name]);
	
	TogglePlayerSpectating(playerid, false); 
	ResetPlayerWeapons(playerid);
	PlayerInfo[playerid][Spectating] = INVALID_PLAYER_ID; 
	GameTextForPlayer(playerid, "_", 500, 3);
	if(GetPlayerGameMode(playerid) == GAME_MODE_TDM)
	{
		if (PlayerInfo[playerid][Team] == -1) 
		{
			SendPlayerToTeamSelection(playerid);
		}
		else
		{
			SetPlayerPosition(playerid);
		}
	}
	else SendPlayerToLobby(playerid);
	return 1;
}

CMD:respawncars(playerid, params[])
{
	new string[256];
	if(PlayerInfo[playerid][AdminLevel] < 2) return SendErrorPermMessage(playerid);

	for(new i; i < MAX_VEHICLES; i++)
	{
		if(IsValidVehicle(i) && GetVehicleVirtualWorld(i) == 0 && !IsVehicleOccupied(i))
		{
			SetVehicleToRespawn(i);
		}
	}
	format(string, sizeof string, "[AdmWarn] All vehicle has been respawned by %s.", PlayerInfo[playerid][Name]);
	SendAdminMessage(1, 0xbf9000FF, string);
	return 1;
}

cmd:noglobal(playerid, params[])
{
	if(PlayerInfo[playerid][AdminLevel] < 2) return SendErrorPermMessage(playerid);

	if(GlobalChat == true)
	{
		GlobalChat = false;
		SendClientMessageToAllEx(0xe06666FF, "* %s has disabled the global chat.", PlayerInfo[playerid][Name]);
	}
	else 
	{
		GlobalChat = true;
		SendClientMessageToAllEx(COLOR_GREEN, "* %s has enabled the global chat!", PlayerInfo[playerid][Name]);
	}
	return 1;
}

cmd:jetpack(playerid, params[])
{
	if(PlayerInfo[playerid][AdminLevel] < 2) return SendErrorPermMessage(playerid);

	SetPlayerSpecialAction(playerid, 2);
	return 1;
}

cmd:checkip(playerid, params[])
{
	if(PlayerInfo[playerid][AdminLevel] < 2)
		return SendErrorPermMessage(playerid);
		
	new playerb, string[128];
	
	if (sscanf(params, "u", playerb)) 
		return SendUsageMessage(playerid, "/checkip [playerid/name]"); 
	
	if(!IsPlayerConnected(playerb))
		return SendErrorMessage(playerid, "The player you specified isn't connected."); 
	
	if(PlayerInfo[playerb][IsLoggedIn] == false)
		return SendErrorMessage(playerid, "The player you specified isn't logged in.");

	new ipaddress[60];
	GetPlayerIp(playerb, ipaddress, sizeof ipaddress);

	SendClientMessageEx(playerb, COLOR_RED, "[IP Check] %s's IP address is %s", PlayerInfo[playerb][Name], ipaddress);
	format(string, sizeof string, "www.shroomery.org/ythan/proxycheck.php?ip=%s", ipaddress);
	HTTP(playerid, HTTP_GET, string, "", "VPNCheck");
	return 1;
}

cmd:hideme(playerid, params[])
{
	if(PlayerInfo[playerid][AdminLevel] < 2) return SendErrorPermMessage(playerid);

	switch(PlayerInfo[playerid][Hidden]) 
	{
		case false:  
		{
			PlayerInfo[playerid][Hidden] = true;
			foreach (new i : Player) 
			{
				if (PlayerInfo[i][AdminLevel])
				{
					SendClientMessageEx(i, 0xbf9000FF, "[AdmWarn] %s has set their self to HIDDEN and will not be showed on /admins!", PlayerInfo[playerid][Name]);
				}
			}
		}
		case true:  
		{
			PlayerInfo[playerid][Hidden] = false;
			foreach (new i : Player) 
			{
				if (PlayerInfo[i][AdminLevel])
				{
					SendClientMessageEx(i, 0xbf9000FF, "[AdmWarn] %s has set their self to VISIBLE and will be visible on /admins!", PlayerInfo[playerid][Name]);
				}
			}
		}
	}
	return 1;
}


/*cmd:changename(playerid, params[]) 
{
	new targetid, name[120];
	if (!PlayerInfo[playerid][AdminLevel]) return SendClientMessage(playerid, 0xe06666FF, "You don't have the right permission to use this command.");

	if (sscanf(params, "us[120]", targetid, name)) return SendClientMessage(playerid, 0xf6b26bFF, "USAGE: /changename [playerid/name] [name]");

	if (!IsPlayerConnected(targetid) || targetid == INVALID_PLAYER_ID) return SendClientMessage(playerid, 0xe06666FF, "That player isn't connected.");

    if (PlayerInfo[targetid][AdminLevel] >= PlayerInfo[playerid][AdminLevel]) return SendClientMessage(playerid, 0xe06666FF, "You can't do that.");

    foreach (new i : Player) 
	{
		if (PlayerInfo[i][AdminLevel])
		{
			SendClientMessageEx(i, 0xbf9000FF, "[AdmWarn] %s has changed %s's name to %s!", PlayerInfo[playerid][Name], PlayerInfo[targetid][Name], name);
		}
	}
	DelayedKick(targetid, 500, true, reason);
	return 1;
}*/

CMD:kickall(playerid, params[]) 
{
	if(PlayerInfo[playerid][AdminLevel] < 8) return SendClientMessage(playerid, 0xe06666FF, "Oops! That command doesn't exist.");

	if(isnull(params))
	{
		SendClientMessage(playerid, COLOR_RED, "Are you sure you want to do this? (This will kick all online players)");
		return SendClientMessage(playerid, -1, "Type \'/kickall confirm\' to proceed.");
	}

	foreach(new i : Player)
	{
		if(IsPlayerConnected(i) && i != playerid)
		{
			TogglePlayerControllable(i, false);
			DelayedKick(i);
		}
	}
	SendClientMessageToAllEx(0xe06666FF, "Admin %s has kicked all player from the server.", PlayerInfo[playerid][Name]);
	return 1;
}

CMD:resethours(playerid, params[]) 
{
	new query[128];
	if(PlayerInfo[playerid][AdminLevel] < 8) return SendClientMessage(playerid, 0xe06666FF, "Oops! That command doesn't exist.");

	if(isnull(params))
	{
		SendClientMessage(playerid, COLOR_RED, "Are you sure you want to do this? (This will reset all staff hours)");
		return SendClientMessage(playerid, -1, "Type \'/resethours confirm\' to proceed.");
	}

    
	foreach(new i : Player)
	{
		if(IsPlayerConnected(i) && PlayerInfo[i][AdminLevel])
		{
			for(new id = 1; id <= 3; id++)
			{
				PlayerInfo[i][StaffActivity][id - 1] = 0;
			}
		}
	}
	for(new idd = 1; idd <= 3; idd++)
	{
		mysql_format(g_SQL, query, sizeof query, "UPDATE `players` SET `activity_%d` = 0  WHERE `admin_level` != 0", idd);
	    mysql_tquery(g_SQL, query);
	}
	foreach (new i : Player) 
	{
		if (PlayerInfo[i][AdminLevel])
		{
			SendClientMessageEx(i, 0xbf9000FF, "[AdmWarn] %s has reset all staff hours.", PlayerInfo[playerid][Name]);
		}
	}
	return 1;
}

CMD:makeadmin(playerid, params[]) 
{
	new targetid, level;
	if(PlayerInfo[playerid][AdminLevel] < 8) return SendClientMessage(playerid, 0xe06666FF, "Oops! That command doesn't exist.");

	if(sscanf(params, "ud", targetid, level)) return SendUsageMessage(playerid, "/makeadmin [playerid/name] [level]");

	if(!IsPlayerConnected(targetid)) return 0;

	PlayerInfo[targetid][AdminLevel] = level;
	return 1;
}

CMD:makeproperty(playerid, params[]) 
{
	new name[64], Float:pointE[3], Float:point[3], interior;
	if(PlayerInfo[playerid][AdminLevel] < 8) return SendClientMessage(playerid, 0xe06666FF, "Oops! That command doesn't exist.");

	if(sscanf(params, "fffis[64]", point[0], point[1], point[2], interior, name)) return SendUsageMessage(playerid, "/makeproperty [interior X] [interior Y] [interior Z] [interiorid] [name]");

    GetPlayerPos(playerid, pointE[0], pointE[1], pointE[2]);
	CreateProperty(name, pointE[0], pointE[1], pointE[2], point[0], point[1], point[2], interior);
	return 1;
}

CMD:makedonator(playerid, params[]) 
{
	new targetid, level, expire;
	if(PlayerInfo[playerid][AdminLevel] < 8) return SendClientMessage(playerid, 0xe06666FF, "Oops! That command doesn't exist.");

	if(sscanf(params, "udd", targetid, level, expire)) return SendUsageMessage(playerid, "/makedonator [playerid/name] [level] [day (0 for permanent rank)]");

	if(!IsPlayerConnected(targetid)) return 0;

	PlayerInfo[targetid][Donator] = level;
	if(expire != 0) PlayerInfo[targetid][DonatorExpire] = (gettime() + (expire * 84600));
	else PlayerInfo[targetid][DonatorExpire] = 0;

	if(expire != 0) SendClientMessageEx(playerid, COLOR_WHITE, "You've set {FFFF00}%s's {ffffff}donator rank to {FFFF00}%s {ffffff}for {FFFF00}%i days{ffffff}.", ReturnName(targetid), ReturnDonorRank(targetid), expire);
	else SendClientMessageEx(playerid, COLOR_WHITE, "You've set {FFFF00}%s's {ffffff}donator rank to {FFFF00}%s{ffffff}.", ReturnName(targetid), ReturnDonorRank(targetid));

	UpdatePlayerData(targetid, 0);
	return 1;
}