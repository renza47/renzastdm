forward OnPlayerBanned(playerid, admin_id, const reason[]);
public OnPlayerBanned(playerid, admin_id, const reason[]) 
{
	SendClientMessageToAllEx(0xe06666FF, "Admin %s has banned %s from the server for: %s", PlayerInfo[admin_id][Name], PlayerInfo[playerid][Name], reason);
	DelayedKick(playerid);
	return 1;
} 

forward OnPlayerOfflineBanned(target[], admin_id, const reason[]);
public OnPlayerOfflineBanned(target[], admin_id, const reason[]) 
{
	SendClientMessageToAllEx(0xe06666FF, "Admin %s has offline banned %s from the server for: %s", PlayerInfo[admin_id][Name], target, reason);
	return 1;
} 

forward OnPlayerUnbanned(admin_id);
public OnPlayerUnbanned(admin_id)
{
	if(cache_num_rows() > 0)
	{
		new ban_id, user_name[MAX_PLAYER_NAME];
		cache_get_value_int(0, "ban_id", ban_id);
		cache_get_value(0, "username", user_name, 24);

		SendClientMessageToAllEx(0xe06666FF, "Admin %s has unbanned %s from the server.", PlayerInfo[admin_id][Name], user_name);

		new query[145];
		mysql_format(g_SQL, query, sizeof query, "DELETE FROM `bans` WHERE `ban_id` = '%d'", ban_id);
		mysql_tquery(g_SQL, query);
	}
	else 
	{
		SendClientMessage(admin_id, 0xe06666FF, "This player isn't banned.");
	}
	return 1;
}

ReturnAdminRank(playerid) 
{
	new admin_rank[32];
	switch (PlayerInfo[playerid][AdminLevel])
	{
		case 0: format(admin_rank, sizeof admin_rank, "Player");
		case 1: format(admin_rank, sizeof admin_rank, "Moderator");
		case 2: format(admin_rank, sizeof admin_rank, "Senior Moderator");
		case 3: format(admin_rank, sizeof admin_rank, "Level 1 Admin");
		case 4: format(admin_rank, sizeof admin_rank, "Level 2 Admin");
		case 5: format(admin_rank, sizeof admin_rank, "Level 3 Admin");
		case 6: format(admin_rank, sizeof admin_rank, "Senior Admin");
		case 7: format(admin_rank, sizeof admin_rank, "Lead Admin");
		case 8: format(admin_rank, sizeof admin_rank, "Manager");
	}
	return admin_rank;
}

stock SendAdminMessageEx(color, level, const str[], {Float,_}:...)
{
	static
	    args,
	    start,
	    end,
	    string[156]
	;
	#emit LOAD.S.pri 8
	#emit STOR.pri args

	if (args > 12)
	{
		#emit ADDR.pri str
		#emit STOR.pri start

	    for (end = start + (args - 12); end > start; end -= 4)
		{
	        #emit LREF.pri end
	        #emit PUSH.pri
		}
		#emit PUSH.S str
		#emit PUSH.C 156
		#emit PUSH.C string
		#emit PUSH.C args
		#emit SYSREQ.C format


		foreach(new i : Player)
		{
			if(PlayerInfo[i][AdminLevel] >= level)
			{
				SendClientMessage(i, color, string);
			}
		}

		#emit LCTRL 5
		#emit SCTRL 4
		#emit RETN
	}
	else
	{
		foreach(new i : Player)
		{
			if(PlayerInfo[i][AdminLevel] >= level)
			{
				SendClientMessage(i, color, str);
			}
		}
	}
	return 1;
}

SendAdminMessage(level, color, const message[])
{
	foreach (new i : Player) 
	{
		if (PlayerInfo[i][AdminLevel] >= level)
		{
			SendClientMessageEx(i, color, message);
		}
	}
	return 1;
}