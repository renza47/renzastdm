forward OnPlayerBanCheck(playerid, race_check);
public OnPlayerBanCheck(playerid, race_check)
{
	/*	race condition check:
		player A connects -> SELECT query is fired -> this query takes very long
		while the query is still processing, player A with playerid 2 disconnects
		player B joins now with playerid 2 -> our laggy SELECT query is finally finished, but for the wrong player
		what do we do against it?
		we create a connection count for each playerid and increase it everytime the playerid connects or disconnects
		we also pass the current value of the connection count to our OnPlayerDataLoaded callback
		then we check if current connection count is the same as connection count we passed to the callback
		if yes, everything is okay, if not, we just kick the player
	*/
	//if (race_check != g_MysqlRaceCheck[playerid]) return Kick(playerid);

	if(cache_num_rows() > 0)
	{
		new ban_id, admin_name[MAX_PLAYER_NAME], ban_date[60], reason[120];
		cache_get_value_int(0, "ban_id", ban_id);
		cache_get_value(0, "admin_name", admin_name, 24);
		cache_get_value(0, "reason", reason, 120);
		cache_get_value(0, "ban_date", ban_date, 60);

        new string[128];
		format(string, sizeof string, "[#%d] %s has been kicked for logging in while banned.", ban_id, PlayerInfo[playerid][Name]);
		SendAdminMessage(1, COLOR_GREY, string);

		SendClientMessageEx(playerid, 0xe06666FF, "[#%d] You were banned by %s for:", ban_id, admin_name);
		SendClientMessageEx(playerid, 0xAFAFAFFF, "\"%s\"", reason);
		SendClientMessageEx(playerid, 0xAFAFAFFF, "Ban date: %s", ban_date);
		SendClientMessageEx(playerid, 0xe06666FF, "[ ! ] You can make a ban appeal in our discord server! (https://discord.gg/gDuREwJtGM)");
		DelayedKick(playerid);
	}
	else 
	{
		new rand = random(2);
		if(rand == 0) PlayAudioStreamForPlayer(playerid, "https://cdn.discordapp.com/attachments/843751512109875223/1096228981012971572/crash_theme.mp3");
		else if(rand == 1) PlayAudioStreamForPlayer(playerid, "https://cdn.discordapp.com/attachments/947695555075797055/1096937852387983541/gs_theme.mp3");
		else if(rand == 2) PlayAudioStreamForPlayer(playerid, "https://cdn.discordapp.com/attachments/947695555075797055/1096937852387983541/gs_theme.mp3");
		new query[103];
		mysql_format(g_SQL, query, sizeof query, "SELECT * FROM `players` WHERE `username` = '%e' LIMIT 1", PlayerInfo[playerid][Name]);
		mysql_tquery(g_SQL, query, "OnPlayerDataLoaded", "dd", playerid, g_MysqlRaceCheck[playerid]);
	}
	return 1;
}

forward OnPlayerIPBanCheck(playerid, race_check);
public OnPlayerIPBanCheck(playerid, race_check)
{
	/*	race condition check:
		player A connects -> SELECT query is fired -> this query takes very long
		while the query is still processing, player A with playerid 2 disconnects
		player B joins now with playerid 2 -> our laggy SELECT query is finally finished, but for the wrong player
		what do we do against it?
		we create a connection count for each playerid and increase it everytime the playerid connects or disconnects
		we also pass the current value of the connection count to our OnPlayerDataLoaded callback
		then we check if current connection count is the same as connection count we passed to the callback
		if yes, everything is okay, if not, we just kick the player
	*/
	
	//if (race_check != g_MysqlRaceCheck[playerid]) return Kick(playerid);

	if(cache_num_rows() > 0)
	{
		new ban_id, admin_name[MAX_PLAYER_NAME], ban_date[60], reason[120];
		cache_get_value_int(0, "ban_id", ban_id);
		cache_get_value(0, "admin_name", admin_name, 24);
		cache_get_value(0, "reason", reason, 120);
		cache_get_value(0, "ban_date", ban_date, 60);

        new string[128];
		format(string, sizeof string, "[#%d] %s has been kicked for logging in while banned.", ban_id, PlayerInfo[playerid][Name]);
		SendAdminMessage(1, COLOR_GREY, string);

		SendClientMessageEx(playerid, 0xe06666FF, "[#%d] You were banned by %s for:", ban_id, admin_name);
		SendClientMessageEx(playerid, 0xAFAFAFFF, "\"%s\"", reason);
		SendClientMessageEx(playerid, 0xAFAFAFFF, "Ban date: %s", ban_date);
		SendClientMessageEx(playerid, 0xe06666FF, "[ ! ] You can make a ban appeal in our discord server! (https://discord.gg/gDuREwJtGM)");
		DelayedKick(playerid);
	}
	else 
	{
		new query[103];
		mysql_format(g_SQL, query, sizeof query, "SELECT * FROM `bans` WHERE `username` = '%e' LIMIT 1", PlayerInfo[playerid][Name]);
		mysql_tquery(g_SQL, query, "OnPlayerBanCheck", "dd", playerid, g_MysqlRaceCheck[playerid]);
	}
	return 1;
}

forward OnPlayerDataLoaded(playerid, race_check);
public OnPlayerDataLoaded(playerid, race_check)
{
	/*	race condition check:
		player A connects -> SELECT query is fired -> this query takes very long
		while the query is still processing, player A with playerid 2 disconnects
		player B joins now with playerid 2 -> our laggy SELECT query is finally finished, but for the wrong player
		what do we do against it?
		we create a connection count for each playerid and increase it everytime the playerid connects or disconnects
		we also pass the current value of the connection count to our OnPlayerDataLoaded callback
		then we check if current connection count is the same as connection count we passed to the callback
		if yes, everything is okay, if not, we just kick the player
	*/
	//if (race_check != g_MysqlRaceCheck[playerid]) return Kick(playerid);

	new string[256];
	if(cache_num_rows() > 0)
	{
		// we store the password and the salt so we can compare the password the player inputs
		// and save the rest so we won't have to execute another query later
		cache_get_value(0, "password", PlayerInfo[playerid][Password], 65);
		cache_get_value(0, "salt", PlayerInfo[playerid][Salt], 17);

		// saves the active cache in the memory and returns an cache-id to access it for later use
		PlayerInfo[playerid][Cache_ID] = cache_save();

		format(string, sizeof string, "{FFFFFF}Welcome {FF6347}%s{FFFFFF}!\n\nThis account is registered in the server.\n\n{FF6347}In order to proceed, please login by entering your password in the field below:", PlayerInfo[playerid][Name]);
		ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", string, "Proceed", "Abort");

		// from now on, the player has 30 seconds to login
		PlayerInfo[playerid][LoginTimer][0] = SetTimerEx("OnLoginTimeout", SECONDS_TO_LOGIN * 1000, false, "d", playerid);
	}
	else
	{
		format(string, sizeof string, "{FFFFFF}Welcome {FF6347}%s{FFFFFF}!\n\nThis account is available!\n\n{FF6347}In order to proceed, enter a password in the field below:", PlayerInfo[playerid][Name]);
		ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Registration", string, "Register", "Abort");
	}
	return 1;
}

forward OnLoginTimeout(playerid);
public OnLoginTimeout(playerid)
{
	if(PlayerInfo[playerid][IsLoggedIn] == true) return 1;
	
	// reset the variable that stores the timerid
	PlayerInfo[playerid][LoginTimer][0] = 0;

	ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Login", "You have been kicked for taking too long to login successfully to your account.", "Okay", "");
	DelayedKick(playerid);
	return 1;
}

forward OnPlayerRegister(playerid);
public OnPlayerRegister(playerid)
{
	TogglePlayerSpectating(playerid, false);
	// retrieves the ID generated for an AUTO_INCREMENT column by the sent query
	PlayerInfo[playerid][ID] = cache_insert_id();

	PlayerInfo[playerid][IsLoggedIn] = true;
	PlayerInfo[playerid][SelectingTeam] = false;

	if(!PlayerInfo[playerid][Tog][2]) PlayerTextDrawShow(playerid, ui_stats[playerid]);

	PlayerInfo[playerid][X_Pos] = DEFAULT_POS_X;
	PlayerInfo[playerid][Y_Pos] = DEFAULT_POS_Y;
	PlayerInfo[playerid][Z_Pos] = DEFAULT_POS_Z;
	PlayerInfo[playerid][A_Pos] = DEFAULT_POS_A;

    foreach(new i : Player)
	{
		if(IsPlayerConnected(i))
		{
			if(!PlayerInfo[i][Tog][0]) SendClientMessageEx(i, COLOR_RED, "~ {AFAFAF} %s(%d) joined the server for the first time!", PlayerInfo[playerid][Name], playerid);
		}
	}

	StopAudioStreamForPlayer(playerid);

	SendClientMessageEx(playerid, -1, "Welcome to the server, {FFFF00}%s.", PlayerInfo[playerid][Name]);

    SetSpawnInfo(playerid, NO_TEAM, 0, PlayerInfo[playerid][X_Pos], PlayerInfo[playerid][Y_Pos], PlayerInfo[playerid][Z_Pos], PlayerInfo[playerid][A_Pos], 0, 0, 0, 0, 0, 0);
	SpawnPlayer(playerid);
	return 1;
}

forward OnPasswordInsert(playerid, bool:success);
public OnPasswordInsert(playerid, bool:success)
{
	new string[256];
    if (success)
    {
		TogglePlayerSpectating(playerid, false);
        //correct password, spawn the player
        //ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Login", "You have been successfully logged in.", "Okay", "");

        // sets the specified cache as the active cache so we can retrieve the rest player data
        cache_set_active(PlayerInfo[playerid][Cache_ID]);

        AssignPlayerData(playerid);

        // remove the active cache from memory and unsets the active cache as well
        cache_delete(PlayerInfo[playerid][Cache_ID]);
        PlayerInfo[playerid][Cache_ID] = MYSQL_INVALID_CACHE;

        KillTimer(PlayerInfo[playerid][LoginTimer][0]);
        PlayerInfo[playerid][LoginTimer][0] = 0;
		KillTimer(PlayerInfo[playerid][LoginTimer][1]);
        PlayerInfo[playerid][LoginTimer][1] = 0;
        PlayerInfo[playerid][IsLoggedIn] = true;
		PlayerInfo[playerid][SelectingTeam] = false;

		SetPlayerGameMode(playerid, GAME_MODE_LOBBY);

		SetPlayerColor(playerid, -1);

		foreach(new i : Player)
		{
			if(IsPlayerConnected(i))
			{
				if(!PlayerInfo[i][Tog][0]) SendClientMessageEx(i, COLOR_RED, "~ {AFAFAF} %s(%d) joined the server.", PlayerInfo[playerid][Name], playerid);
			}
		}

		if(PlayerInfo[playerid][AdminLevel]) 
		{
			SendClientMessageEx(playerid, -1, "Welcome to the server, {FFFF00}%s.", PlayerInfo[playerid][Name]);
		    SendClientMessageEx(playerid, -1, "You logged in as a %s (%d).", ReturnAdminRank(playerid), PlayerInfo[playerid][AdminLevel]);
		}
		else
		{
			SendClientMessageEx(playerid, -1, "Welcome to the server, {FFFF00}%s..", PlayerInfo[playerid][Name]);
		}

		if(!PlayerInfo[playerid][Tog][2]) PlayerTextDrawShow(playerid, ui_stats[playerid]);

        SetSpawnInfo(playerid, NO_TEAM, 0, PlayerInfo[playerid][X_Pos], PlayerInfo[playerid][Y_Pos], PlayerInfo[playerid][Z_Pos], PlayerInfo[playerid][A_Pos], 0, 0, 0, 0, 0, 0);
		SpawnPlayer(playerid);

		StopAudioStreamForPlayer(playerid);
    }
    else
    {
        PlayerInfo[playerid][LoginAttempts]++;

        if (PlayerInfo[playerid][LoginAttempts] >= 3)
        {
            SendClientMessage(playerid, 0xFF0000FF, "You have been kicked from the server for:");
            SendClientMessage(playerid, 0xFF0000FF, "Failed to login. (3 attempts reached)");
            ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, "Login", "You have mistyped your password too often (3 times).", "Okay", "");
            DelayedKick(playerid);
        }
        else 
        {
            SendClientMessage(playerid, 0xFF0000FF, "Wrong password!");
            format(string, sizeof string, "You still have %d attempts to login.", 3 - PlayerInfo[playerid][LoginAttempts]);
            SendClientMessage(playerid, 0xFF0000FF, string);
            format(string, sizeof string, "{FFFFFF}Welcome {FF6347}%s{FFFFFF}!\n\nThis account is registered in the server.\n\n{FF6347}In order to proceed, please login by entering your password in the field below:", PlayerInfo[playerid][Name]);
            ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", string, "Login", "Abort");
        }
    }
    return 1;
}

forward OnPasswordHashed(playerid);
public OnPasswordHashed(playerid)
{
    new query[221];

	bcrypt_get_hash(PlayerInfo[playerid][Password], BCRYPT_HASH_LENGTH);

    mysql_format(g_SQL, query, sizeof query, "INSERT INTO `players` (`username`, `password`, `salt`) VALUES ('%e', '%e', '%e')", PlayerInfo[playerid][Name], PlayerInfo[playerid][Password], PlayerInfo[playerid][Salt]);
	mysql_tquery(g_SQL, query, "OnPlayerRegister", "d", playerid);
    return 1;
}

forward ResetPlayerData(playerid);
public ResetPlayerData(playerid)  
{
	PlayerInfo[playerid][ID] = -1;

	PlayerInfo[playerid][Kills] = 0;
	PlayerInfo[playerid][Deaths] = 0;

	//SetPlayerLanguage(playerid, LANGUAGE_ENGLISH);

	PlayerInfo[playerid][X_Pos] = DEFAULT_POS_X;
	PlayerInfo[playerid][Y_Pos] = DEFAULT_POS_Y;
	PlayerInfo[playerid][Z_Pos] = DEFAULT_POS_Z;
	PlayerInfo[playerid][A_Pos] = DEFAULT_POS_A;
	PlayerInfo[playerid][Interior] = 0;

	PlayerInfo[playerid][Team] = -1;
	PlayerInfo[playerid][SelectingTeam] = false;

	PlayerInfo[playerid][Skin] = 299;

	PlayerInfo[playerid][AdminLevel] = 0;
	PlayerInfo[playerid][AdminDuty] = false;

	PlayerInfo[playerid][State] = PLAYER_STATE_ALIVE;

	PlayerInfo[playerid][DamageFix] = 0;
	PlayerInfo[playerid][DeathTime] = 0;
	PlayerInfo[playerid][LastDamage] = 0;

	PlayerInfo[playerid][Score] = 0;
	PlayerInfo[playerid][Cash] = 0;

	PlayerInfo[playerid][Reviving] = -1;
	PlayerInfo[playerid][RevivingTime] = 0;

	PlayerInfo[playerid][Weapon][0] = 32;
	PlayerInfo[playerid][Weapon][1] = 0;

	for(new i = 1; i < 20; i++) PlayerInfo[playerid][HackWarning][i] = 0;

	PlayerInfo[playerid][KillStreak] = 0;

	PlayerInfo[playerid][Spectating] = INVALID_PLAYER_ID;

	PlayerInfo[playerid][Medkit] = 0;
	PlayerInfo[playerid][MedkitTime] = 0;

	PlayerInfo[playerid][GangSkin][1] = 106;
	PlayerInfo[playerid][GangSkin][2] = 106;
	PlayerInfo[playerid][GangSkin][3] = 106;
	PlayerInfo[playerid][GangSkin][4] = 102;
	PlayerInfo[playerid][GangSkin][5] = 102;
	PlayerInfo[playerid][GangSkin][6] = 102;
	PlayerInfo[playerid][GangSkin][7] = 102;
	PlayerInfo[playerid][GangSkin][8] = 108;
	PlayerInfo[playerid][GangSkin][9] = 114;
	PlayerInfo[playerid][GangSkin][10] = 280;

	PlayerInfo[playerid][RecentlySpawned] = 0;

	PlayerInfo[playerid][BrokenLeg] = false;

	PlayerInfo[playerid][GoldenGunAmmo] = 0;

	PlayerInfo[playerid][Health] = 100.0;
	PlayerInfo[playerid][Armour] = 0.0;

	PlayerInfo[playerid][ChatDelay] = 0;
	PlayerInfo[playerid][CbugCheck] = 0;

	PlayerInfo[playerid][AdminJail] = 0;

	PlayerInfo[playerid][GameMode] = GAME_MODE_LOBBY;

	PlayerInfo[playerid][Model] = 264;

	PlayerInfo[playerid][DM] = -1;

	PlayerInfo[playerid][HitSound] = 1;
	PlayerInfo[playerid][HitMarker] = 1;

	PlayerInfo[playerid][SessionKills] = 0.0;
	PlayerInfo[playerid][SessionDeaths] = 0.0;

	PlayerInfo[playerid][i_ExpireTimer] = -1;
    PlayerInfo[playerid][i_NameColor] = -1;
    PlayerInfo[playerid][i_BackgroundColor] = -1;
    PlayerInfo[playerid][i_AvatarColor] = -1;
    PlayerInfo[playerid][i_MotoColor] = -1;
    PlayerInfo[playerid][b_Toggled] = true;

	PlayerInfo[playerid][CC_Team] = 0;

	PlayerInfo[playerid][NoCopchase] = 0;

	PlayerInfo[playerid][CopSkin] = 280;
	PlayerInfo[playerid][CopCar] = 596;

	PlayerInfo[playerid][FugCar] = 566;

	PlayerInfo[playerid][Property] = -1;

	PlayerInfo[playerid][EnterExitDelay] = 100;

	PlayerInfo[playerid][ObjectLoad] = 0;

	PlayerInfo[playerid][AntiCheatPause] = 0;

	PlayerInfo[playerid][Taser] = false;

	PlayerInfo[playerid][Tased] = 0;

	PlayerInfo[playerid][CuffTime] = 0;
	PlayerInfo[playerid][CuffID] = INVALID_PLAYER_ID;

	PlayerInfo[playerid][DragTime] = 0;
	PlayerInfo[playerid][DragID] = INVALID_PLAYER_ID;

	PlayerInfo[playerid][Traced] = 0;

	PlayerInfo[playerid][Vote] = 0;

	PlayerInfo[playerid][Muted] = 0;
	PlayerInfo[playerid][MuteTime] = 0;

	PlayerInfo[playerid][FreeroamVehicle] = INVALID_VEHICLE_ID;

	PlayerInfo[playerid][Hidden] = false;
	
	PlayerInfo[playerid][SelfRevive] = 0;

	PlayerInfo[playerid][FixVeh] = 0;
	PlayerInfo[playerid][FlipVeh] = 0;

	PlayerInfo[playerid][Mask] = 0;
	PlayerInfo[playerid][MedkitRevive] = 0;

	PlayerInfo[playerid][Donator] = 0;
	PlayerInfo[playerid][DonatorPoint] = 0;
	PlayerInfo[playerid][DonatorExpire] = 0;

	format(PlayerInfo[playerid][DonatorTag], 32, "");

	for(new i; i < 3; i++)
	{
		PlayerInfo[playerid][Perk][i] = 0;
	}

	for(new i = 0; i < 3; i++) PlayerInfo[playerid][StaffActivity][i] = 0;
	for(new i = 0; i < 3; i++) PlayerInfo[playerid][PlayingHours][i] = 0;

	if(IsValidDynamicActor(PlayerInfo[playerid][Actor])) DestroyDynamicActor(PlayerInfo[playerid][Actor]);

	for(new i; i < 12; i++)
	{
		PlayerInfo[playerid][CC_Weapon][i] = 0;
		PlayerInfo[playerid][CC_WeaponAmmo][i] = 0;
	}

	for(new i; i < 4; i++)
	{
		PlayerInfo[playerid][Tog][i] = 0;
	}

    format(PlayerInfo[playerid][s_BackgroundSprite], 25, "loadsc4:loadsc4");
    format(PlayerInfo[playerid][s_AvatarSprite], 25, "LD_TATT:9gun2");
    format(PlayerInfo[playerid][s_Moto], 50, "Hey there!");

	KillTimer(HitMarkerTimer[playerid]);

	if(IsValidDynamic3DTextLabel(PlayerInfo[playerid][Nametag])) 
    {
        DestroyDynamic3DTextLabel(PlayerInfo[playerid][Nametag]);
        PlayerInfo[playerid][Nametag] = Text3D:INVALID_3DTEXT_ID;
    }
	
	ClearPlayerDamages(playerid);

	SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, 0);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, 0);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_SPAS12_SHOTGUN, 0);

	SetPlayerWantedLevel(playerid, 0);

	SetPVarInt(playerid, "CurrentZone", 0);

	ui_class_skinName[playerid] = CreatePlayerTextDraw(playerid, 321.000000, 383.000000, "Grove Street Families");
    PlayerTextDrawFont(playerid, ui_class_skinName[playerid], 2);
    PlayerTextDrawLetterSize(playerid, ui_class_skinName[playerid], 0.208333, 1.000000);
    PlayerTextDrawTextSize(playerid, ui_class_skinName[playerid], 400.000000, 129.000000);
    PlayerTextDrawSetOutline(playerid, ui_class_skinName[playerid], 1);
    PlayerTextDrawSetShadow(playerid, ui_class_skinName[playerid], 0);
    PlayerTextDrawAlignment(playerid, ui_class_skinName[playerid], 2);
    PlayerTextDrawColor(playerid, ui_class_skinName[playerid], -1);
    PlayerTextDrawBackgroundColor(playerid, ui_class_skinName[playerid], 255);
    PlayerTextDrawBoxColor(playerid, ui_class_skinName[playerid], 30);
    PlayerTextDrawUseBox(playerid, ui_class_skinName[playerid], 0);
    PlayerTextDrawSetProportional(playerid, ui_class_skinName[playerid], 1);
    PlayerTextDrawSetSelectable(playerid, ui_class_skinName[playerid], 0);

    ui_class_arrowLeft[playerid] = CreatePlayerTextDraw(playerid, 241.000000, 378.000000, "ld_beat:left");
    PlayerTextDrawFont(playerid, ui_class_arrowLeft[playerid], 4);
    PlayerTextDrawLetterSize(playerid, ui_class_arrowLeft[playerid], 0.600000, 2.000000);
    PlayerTextDrawTextSize(playerid, ui_class_arrowLeft[playerid], 22.000000, 22.000000);
    PlayerTextDrawSetOutline(playerid, ui_class_arrowLeft[playerid], 1);
    PlayerTextDrawSetShadow(playerid, ui_class_arrowLeft[playerid], 0);
    PlayerTextDrawAlignment(playerid, ui_class_arrowLeft[playerid], 1);
    PlayerTextDrawColor(playerid, ui_class_arrowLeft[playerid], -1);
    PlayerTextDrawBackgroundColor(playerid, ui_class_arrowLeft[playerid], 255);
    PlayerTextDrawBoxColor(playerid, ui_class_arrowLeft[playerid], 50);
    PlayerTextDrawUseBox(playerid, ui_class_arrowLeft[playerid], 1);
    PlayerTextDrawSetProportional(playerid, ui_class_arrowLeft[playerid], 1);
    PlayerTextDrawSetSelectable(playerid, ui_class_arrowLeft[playerid], 1);

    ui_class_arrowRight[playerid] = CreatePlayerTextDraw(playerid, 380.000000, 378.000000, "ld_beat:right");
    PlayerTextDrawFont(playerid, ui_class_arrowRight[playerid], 4);
    PlayerTextDrawLetterSize(playerid, ui_class_arrowRight[playerid], 0.600000, 2.000000);
    PlayerTextDrawTextSize(playerid, ui_class_arrowRight[playerid], 22.000000, 22.000000);
    PlayerTextDrawSetOutline(playerid, ui_class_arrowRight[playerid], 1);
    PlayerTextDrawSetShadow(playerid, ui_class_arrowRight[playerid], 0);
    PlayerTextDrawAlignment(playerid, ui_class_arrowRight[playerid], 1);
    PlayerTextDrawColor(playerid, ui_class_arrowRight[playerid], -1);
    PlayerTextDrawBackgroundColor(playerid, ui_class_arrowRight[playerid], 255);
    PlayerTextDrawBoxColor(playerid, ui_class_arrowRight[playerid], 50);
    PlayerTextDrawUseBox(playerid, ui_class_arrowRight[playerid], 1);
    PlayerTextDrawSetProportional(playerid, ui_class_arrowRight[playerid], 1);
    PlayerTextDrawSetSelectable(playerid, ui_class_arrowRight[playerid], 1);

    ui_class_select[playerid] = CreatePlayerTextDraw(playerid, 321.000000, 400.000000, "<SPAWN>");
    PlayerTextDrawFont(playerid, ui_class_select[playerid], 2);
    PlayerTextDrawLetterSize(playerid, ui_class_select[playerid], 0.291666, 1.450000);
    PlayerTextDrawTextSize(playerid, ui_class_select[playerid], 16.500000, 54.000000);
    PlayerTextDrawSetOutline(playerid, ui_class_select[playerid], 1);
    PlayerTextDrawSetShadow(playerid, ui_class_select[playerid], 0);
    PlayerTextDrawAlignment(playerid, ui_class_select[playerid], 2);
    PlayerTextDrawColor(playerid, ui_class_select[playerid], -1);
    PlayerTextDrawBackgroundColor(playerid, ui_class_select[playerid], 255);
    PlayerTextDrawBoxColor(playerid, ui_class_select[playerid], 200);
    PlayerTextDrawUseBox(playerid, ui_class_select[playerid], 0);
    PlayerTextDrawSetProportional(playerid, ui_class_select[playerid], 1);
    PlayerTextDrawSetSelectable(playerid, ui_class_select[playerid], 1);

    ui_class_name[playerid] = CreatePlayerTextDraw(playerid, 325.000000, 362.000000, "~g~[GSF]");
    PlayerTextDrawFont(playerid, ui_class_name[playerid], 3);
    PlayerTextDrawLetterSize(playerid, ui_class_name[playerid], 0.479166, 1.649999);
    PlayerTextDrawTextSize(playerid, ui_class_name[playerid], 400.000000, 129.000000);
    PlayerTextDrawSetOutline(playerid, ui_class_name[playerid], 1);
    PlayerTextDrawSetShadow(playerid, ui_class_name[playerid], 0);
    PlayerTextDrawAlignment(playerid, ui_class_name[playerid], 2);
    PlayerTextDrawColor(playerid, ui_class_name[playerid], -1);
    PlayerTextDrawBackgroundColor(playerid, ui_class_name[playerid], 255);
    PlayerTextDrawBoxColor(playerid, ui_class_name[playerid], 30);
    PlayerTextDrawUseBox(playerid, ui_class_name[playerid], 0);
    PlayerTextDrawSetProportional(playerid, ui_class_name[playerid], 1);
    PlayerTextDrawSetSelectable(playerid, ui_class_name[playerid], 0);

	ui_clock[playerid] = CreatePlayerTextDraw(playerid, 577.000000, 22.000000, "12:00");
	PlayerTextDrawFont(playerid, ui_clock[playerid], 3);
	PlayerTextDrawLetterSize(playerid, ui_clock[playerid], 0.458332, 1.749999);
	PlayerTextDrawTextSize(playerid, ui_clock[playerid], 400.000000, 17.000000);
	PlayerTextDrawSetOutline(playerid, ui_clock[playerid], 2);
	PlayerTextDrawSetShadow(playerid, ui_clock[playerid], 0);
	PlayerTextDrawAlignment(playerid, ui_clock[playerid], 2);
	PlayerTextDrawColor(playerid, ui_clock[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, ui_clock[playerid], 255);
	PlayerTextDrawBoxColor(playerid, ui_clock[playerid], 50);
	PlayerTextDrawUseBox(playerid, ui_clock[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ui_clock[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, ui_clock[playerid], 0);

	ui_dayNotifier[playerid] = CreatePlayerTextDraw(playerid, 84.000000, 425.000000, "Sunday");
	PlayerTextDrawFont(playerid, ui_dayNotifier[playerid], 0);
	PlayerTextDrawLetterSize(playerid, ui_dayNotifier[playerid], 0.416666, 1.450000);
	PlayerTextDrawTextSize(playerid, ui_dayNotifier[playerid], 400.000000, 129.000000);
	PlayerTextDrawSetOutline(playerid, ui_dayNotifier[playerid], 1);
	PlayerTextDrawSetShadow(playerid, ui_dayNotifier[playerid], 0);
	PlayerTextDrawAlignment(playerid, ui_dayNotifier[playerid], 2);
	PlayerTextDrawColor(playerid, ui_dayNotifier[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, ui_dayNotifier[playerid], 255);
	PlayerTextDrawBoxColor(playerid, ui_dayNotifier[playerid], 30);
	PlayerTextDrawUseBox(playerid, ui_dayNotifier[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ui_dayNotifier[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, ui_dayNotifier[playerid], 0);

	ui_serverName[playerid] = CreatePlayerTextDraw(playerid, 574.000000, 418.000000, "~r~R~w~enza's ~b~T~W~eam ~r~D~w~eathmatch");
	PlayerTextDrawFont(playerid, ui_serverName[playerid], 0);
	PlayerTextDrawLetterSize(playerid, ui_serverName[playerid], 0.358332, 1.549999);
	PlayerTextDrawTextSize(playerid, ui_serverName[playerid], 400.000000, 129.000000);
	PlayerTextDrawSetOutline(playerid, ui_serverName[playerid], 1);
	PlayerTextDrawSetShadow(playerid, ui_serverName[playerid], 0);
	PlayerTextDrawAlignment(playerid, ui_serverName[playerid], 2);
	PlayerTextDrawColor(playerid, ui_serverName[playerid], -293409025);
	PlayerTextDrawBackgroundColor(playerid, ui_serverName[playerid], 120);
	PlayerTextDrawBoxColor(playerid, ui_serverName[playerid], 30);
	PlayerTextDrawUseBox(playerid, ui_serverName[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ui_serverName[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, ui_serverName[playerid], 0);

	ui_serverWeb[playerid] = CreatePlayerTextDraw(playerid, 577.000000, 433.000000, "~r~www~w~.r-tdm.~b~com");
	PlayerTextDrawFont(playerid, ui_serverWeb[playerid], 1);
	PlayerTextDrawLetterSize(playerid, ui_serverWeb[playerid], 0.245833, 1.150000);
	PlayerTextDrawTextSize(playerid, ui_serverWeb[playerid], 400.000000, 129.000000);
	PlayerTextDrawSetOutline(playerid, ui_serverWeb[playerid], 1);
	PlayerTextDrawSetShadow(playerid, ui_serverWeb[playerid], 0);
	PlayerTextDrawAlignment(playerid, ui_serverWeb[playerid], 2);
	PlayerTextDrawColor(playerid, ui_serverWeb[playerid], -293409025);
	PlayerTextDrawBackgroundColor(playerid, ui_serverWeb[playerid], 120);
	PlayerTextDrawBoxColor(playerid, ui_serverWeb[playerid], 30);
	PlayerTextDrawUseBox(playerid, ui_serverWeb[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ui_serverWeb[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, ui_serverWeb[playerid], 0);

	ui_stats[playerid] = CreatePlayerTextDraw(playerid, 636.000000, 4.000000, "~W~FPS ~R~50 ~W~PING ~R~210 ms ~W~P/L ~R~15.0");
	PlayerTextDrawFont(playerid, ui_stats[playerid], 1);
	PlayerTextDrawLetterSize(playerid, ui_stats[playerid], 0.170829, 0.749997);
	PlayerTextDrawTextSize(playerid, ui_stats[playerid], 400.000000, 129.000000);
	PlayerTextDrawSetOutline(playerid, ui_stats[playerid], 1);
	PlayerTextDrawSetShadow(playerid, ui_stats[playerid], 0);
	PlayerTextDrawAlignment(playerid, ui_stats[playerid], 3);
	PlayerTextDrawColor(playerid, ui_stats[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, ui_stats[playerid], 255);
	PlayerTextDrawBoxColor(playerid, ui_stats[playerid], 30);
	PlayerTextDrawUseBox(playerid, ui_stats[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ui_stats[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, ui_stats[playerid], 0);

	ui_hitmarker[playerid] = CreatePlayerTextDraw(playerid, 340.000000, 173.000000, "~w~X");
	PlayerTextDrawFont(playerid, ui_hitmarker[playerid], 2);
	PlayerTextDrawLetterSize(playerid, ui_hitmarker[playerid], 0.266665, 1.250000);
	PlayerTextDrawTextSize(playerid, ui_hitmarker[playerid], 400.000000, 129.000000);
	PlayerTextDrawSetOutline(playerid, ui_hitmarker[playerid], 1);
	PlayerTextDrawSetShadow(playerid, ui_hitmarker[playerid], 0);
	PlayerTextDrawAlignment(playerid, ui_hitmarker[playerid], 2);
	PlayerTextDrawColor(playerid, ui_hitmarker[playerid], -1962934017);
	PlayerTextDrawBackgroundColor(playerid, ui_hitmarker[playerid], 255);
	PlayerTextDrawBoxColor(playerid, ui_hitmarker[playerid], 30);
	PlayerTextDrawUseBox(playerid, ui_hitmarker[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ui_hitmarker[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, ui_hitmarker[playerid], 0);

	ui_DamageGiven[playerid] = CreatePlayerTextDraw(playerid, 469.000000, 355.000000, "~r~-50 - Desert Eagle");
	PlayerTextDrawFont(playerid, ui_DamageGiven[playerid], 2);
	PlayerTextDrawLetterSize(playerid, ui_DamageGiven[playerid], 0.191666, 0.950000);
	PlayerTextDrawTextSize(playerid, ui_DamageGiven[playerid], 400.000000, 129.000000);
	PlayerTextDrawSetOutline(playerid, ui_DamageGiven[playerid], 1);
	PlayerTextDrawSetShadow(playerid, ui_DamageGiven[playerid], 0);
	PlayerTextDrawAlignment(playerid, ui_DamageGiven[playerid], 2);
	PlayerTextDrawColor(playerid, ui_DamageGiven[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, ui_DamageGiven[playerid], 255);
	PlayerTextDrawBoxColor(playerid, ui_DamageGiven[playerid], 30);
	PlayerTextDrawUseBox(playerid, ui_DamageGiven[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ui_DamageGiven[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, ui_DamageGiven[playerid], 0);

	ui_DamageTaken[playerid] = CreatePlayerTextDraw(playerid, 217.000000, 355.000000, "~g~+75 - Desert Eagle");
	PlayerTextDrawFont(playerid, ui_DamageTaken[playerid], 2);
	PlayerTextDrawLetterSize(playerid, ui_DamageTaken[playerid], 0.191666, 0.950000);
	PlayerTextDrawTextSize(playerid, ui_DamageTaken[playerid], 400.000000, 129.000000);
	PlayerTextDrawSetOutline(playerid, ui_DamageTaken[playerid], 1);
	PlayerTextDrawSetShadow(playerid, ui_DamageTaken[playerid], 0);
	PlayerTextDrawAlignment(playerid, ui_DamageTaken[playerid], 2);
	PlayerTextDrawColor(playerid, ui_DamageTaken[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, ui_DamageTaken[playerid], 255);
	PlayerTextDrawBoxColor(playerid, ui_DamageTaken[playerid], 30);
	PlayerTextDrawUseBox(playerid, ui_DamageTaken[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ui_DamageTaken[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, ui_DamageTaken[playerid], 0);

	ui_box[playerid] = CreatePlayerTextDraw(playerid, 16.000000, 137.000000, "NaN");
    PlayerTextDrawFont(playerid, ui_box[playerid], 1);
    PlayerTextDrawLetterSize(playerid, ui_box[playerid], 0.374998, 1.599998);
    PlayerTextDrawTextSize(playerid, ui_box[playerid], 153.500000, 121.500000);
    PlayerTextDrawSetOutline(playerid, ui_box[playerid], 0);
    PlayerTextDrawSetShadow(playerid, ui_box[playerid], 0);
    PlayerTextDrawAlignment(playerid, ui_box[playerid], 1);
    PlayerTextDrawColor(playerid, ui_box[playerid], -1061109505);
    PlayerTextDrawBackgroundColor(playerid, ui_box[playerid], 255);
    PlayerTextDrawBoxColor(playerid, ui_box[playerid], 200);
    PlayerTextDrawUseBox(playerid, ui_box[playerid], 1);
    PlayerTextDrawSetProportional(playerid, ui_box[playerid], 1);
    PlayerTextDrawSetSelectable(playerid, ui_box[playerid], 0);

	ui_ProgressInfo[playerid] = CreatePlayerTextDraw(playerid, 550.000000, 130.000000, "TIME      05:00");
	PlayerTextDrawFont(playerid, ui_ProgressInfo[playerid], 2);
	PlayerTextDrawLetterSize(playerid, ui_ProgressInfo[playerid], 0.266665, 1.350000);
	PlayerTextDrawTextSize(playerid, ui_ProgressInfo[playerid], 400.000000, 129.000000);
	PlayerTextDrawSetOutline(playerid, ui_ProgressInfo[playerid], 1);
	PlayerTextDrawSetShadow(playerid, ui_ProgressInfo[playerid], 0);
	PlayerTextDrawAlignment(playerid, ui_ProgressInfo[playerid], 2);
	PlayerTextDrawColor(playerid, ui_ProgressInfo[playerid], -1378294017);
	PlayerTextDrawBackgroundColor(playerid, ui_ProgressInfo[playerid], 255);
	PlayerTextDrawBoxColor(playerid, ui_ProgressInfo[playerid], 30);
	PlayerTextDrawUseBox(playerid, ui_ProgressInfo[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ui_ProgressInfo[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, ui_ProgressInfo[playerid], 0);

	speedo1[playerid] = CreatePlayerTextDraw(playerid, 521.000000, 336.000000, "mdl-5000:speedo_main");
	PlayerTextDrawFont(playerid, speedo1[playerid], 4);
	PlayerTextDrawLetterSize(playerid, speedo1[playerid], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, speedo1[playerid], 100.000000, 100.000000);
	PlayerTextDrawSetOutline(playerid, speedo1[playerid], 1);
	PlayerTextDrawSetShadow(playerid, speedo1[playerid], 0);
	PlayerTextDrawAlignment(playerid, speedo1[playerid], 1);
	PlayerTextDrawColor(playerid, speedo1[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, speedo1[playerid], 255);
	PlayerTextDrawBoxColor(playerid, speedo1[playerid], 50);
	PlayerTextDrawUseBox(playerid, speedo1[playerid], 1);
	PlayerTextDrawSetProportional(playerid, speedo1[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, speedo1[playerid], 0);

	speedo2[playerid] = CreatePlayerTextDraw(playerid, 521.000000, 336.000000, "mdl-5000:speedo_1");
	PlayerTextDrawFont(playerid, speedo2[playerid], 4);
	PlayerTextDrawLetterSize(playerid, speedo2[playerid], 0.600000, 2.000000);
	PlayerTextDrawTextSize(playerid, speedo2[playerid], 100.000000, 100.000000);
	PlayerTextDrawSetOutline(playerid, speedo2[playerid], 1);
	PlayerTextDrawSetShadow(playerid, speedo2[playerid], 0);
	PlayerTextDrawAlignment(playerid, speedo2[playerid], 1);
	PlayerTextDrawColor(playerid, speedo2[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, speedo2[playerid], 255);
	PlayerTextDrawBoxColor(playerid, speedo2[playerid], 50);
	PlayerTextDrawUseBox(playerid, speedo2[playerid], 1);
	PlayerTextDrawSetProportional(playerid, speedo2[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, speedo2[playerid], 0);

	speedo3[playerid] = CreatePlayerTextDraw(playerid, 571.000000, 416.000000, "280 MPH");
	PlayerTextDrawFont(playerid, speedo3[playerid], 2);
	PlayerTextDrawLetterSize(playerid, speedo3[playerid], 0.195833, 1.200000);
	PlayerTextDrawTextSize(playerid, speedo3[playerid], 400.000000, 129.000000);
	PlayerTextDrawSetOutline(playerid, speedo3[playerid], 1);
	PlayerTextDrawSetShadow(playerid, speedo3[playerid], 0);
	PlayerTextDrawAlignment(playerid, speedo3[playerid], 2);
	PlayerTextDrawColor(playerid, speedo3[playerid], -1);
	PlayerTextDrawBackgroundColor(playerid, speedo3[playerid], 255);
	PlayerTextDrawBoxColor(playerid, speedo3[playerid], 30);
	PlayerTextDrawUseBox(playerid, speedo3[playerid], 0);
	PlayerTextDrawSetProportional(playerid, speedo3[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, speedo3[playerid], 0);
	return 1;
}

AssignPlayerData(playerid)
{
	new string[128];
	cache_get_value_int(0, "id", PlayerInfo[playerid][ID]);

	cache_get_value_int(0, "kills", PlayerInfo[playerid][Kills]);
	cache_get_value_int(0, "deaths", PlayerInfo[playerid][Deaths]);

	cache_get_value_float(0, "x", PlayerInfo[playerid][X_Pos]);
	cache_get_value_float(0, "y", PlayerInfo[playerid][Y_Pos]);
	cache_get_value_float(0, "z", PlayerInfo[playerid][Z_Pos]);
	cache_get_value_float(0, "angle", PlayerInfo[playerid][A_Pos]);
	cache_get_value_int(0, "interior", PlayerInfo[playerid][Interior]);

	cache_get_value_int(0, "skin", PlayerInfo[playerid][Skin]);

	cache_get_value_int(0, "admin_level", PlayerInfo[playerid][AdminLevel]);

	cache_get_value_int(0, "score", PlayerInfo[playerid][Score]);
	cache_get_value_int(0, "cash", PlayerInfo[playerid][Cash]);

	cache_get_value_int(0, "weapon1", PlayerInfo[playerid][Weapon][0]);
	cache_get_value_int(0, "weapon2", PlayerInfo[playerid][Weapon][1]);

	cache_get_value_int(0, "medkit", PlayerInfo[playerid][Medkit]);

	for(new id = 1; id <= 10; id++)
	{
		format(string, sizeof string, "skin_%d", id);
		cache_get_value_name_int(0, string, PlayerInfo[playerid][GangSkin][id]);
	}

	cache_get_value_int(0, "model", PlayerInfo[playerid][Model]);

	cache_get_value_int(0, "hitmarker", PlayerInfo[playerid][HitMarker]);
	cache_get_value_int(0, "hitsound", PlayerInfo[playerid][HitSound]);

	cache_get_value_int(0, "signature_bgcolor",  PlayerInfo[playerid][i_BackgroundColor]);
	cache_get_value_int(0, "signature_avcolor", PlayerInfo[playerid][i_AvatarColor]);
	cache_get_value_int(0, "signature_motocolor", PlayerInfo[playerid][i_MotoColor]);

	cache_get_value_name(0, "signature_bg", PlayerInfo[playerid][s_BackgroundSprite]);
	cache_get_value_name(0, "signature_av", PlayerInfo[playerid][s_AvatarSprite]);
	cache_get_value_name(0, "signature_moto", PlayerInfo[playerid][s_Moto]);

	cache_get_value_int(0, "cop_skin", PlayerInfo[playerid][CopSkin]);
	cache_get_value_int(0, "cop_car", PlayerInfo[playerid][CopCar]);

	cache_get_value_int(0, "fug_car", PlayerInfo[playerid][FugCar]);

	cache_get_value_int(0, "muted", PlayerInfo[playerid][Muted]);
	cache_get_value_int(0, "muteTime", PlayerInfo[playerid][MuteTime]);

	for(new id = 1; id <= 3; id++)
	{
		format(string, sizeof string, "activity_%d", id);
		cache_get_value_name_int(0, string, PlayerInfo[playerid][StaffActivity][id -1]);
	}

	for(new id = 1; id <= 3; id++)
	{
		format(string, sizeof string, "hours_%d", id);
		cache_get_value_name_int(0, string, PlayerInfo[playerid][PlayingHours][id -1]);
	}

	for(new id = 1; id <= 3; id++)
	{
		format(string, sizeof string, "perk_%d", id);
		cache_get_value_name_int(0, string, PlayerInfo[playerid][Perk][id -1]);
	}
	
	cache_get_value_int(0, "donator", PlayerInfo[playerid][Donator]);
	cache_get_value_int(0, "donator_point", PlayerInfo[playerid][DonatorPoint]);
	cache_get_value_int(0, "donator_expire", PlayerInfo[playerid][DonatorExpire]);

	cache_get_value_int(0, "donator_tag", PlayerInfo[playerid][DonatorTag]);
	return 1;
}

UpdatePlayerData(playerid, reason)
{
	if (PlayerInfo[playerid][IsLoggedIn] == false) return 0;

	// if the client crashed, it's not possible to get the player's position in OnPlayerDisconnect callback
	// so we will use the last saved position (in case of a player who registered and crashed/kicked, the position will be the default spawn point)
	if (reason == 1)
	{
		GetPlayerPos(playerid, PlayerInfo[playerid][X_Pos], PlayerInfo[playerid][Y_Pos], PlayerInfo[playerid][Z_Pos]);
		GetPlayerFacingAngle(playerid, PlayerInfo[playerid][A_Pos]);
	}

	new query[512];
	mysql_format(g_SQL, query, sizeof query, "UPDATE `players` SET `x` = %f, `y` = %f, `z` = %f, `angle` = %f, `interior` = %d WHERE `id` = %d LIMIT 1", PlayerInfo[playerid][X_Pos], PlayerInfo[playerid][Y_Pos], PlayerInfo[playerid][Z_Pos], PlayerInfo[playerid][A_Pos], GetPlayerInterior(playerid), PlayerInfo[playerid][ID]);
	mysql_tquery(g_SQL, query);

	mysql_format(g_SQL, query, sizeof query, "UPDATE `players` SET `skin` = %d, `admin_level` = %d, `score` = %d, `cash` = %d  WHERE `id` = %d LIMIT 1", PlayerInfo[playerid][Skin], PlayerInfo[playerid][AdminLevel], PlayerInfo[playerid][Score], PlayerInfo[playerid][Cash], PlayerInfo[playerid][ID]);
	mysql_tquery(g_SQL, query);

	mysql_format(g_SQL, query, sizeof query, "UPDATE `players` SET `weapon1` = %d, `weapon2` = %d, `medkit` = %d  WHERE `id` = %d LIMIT 1", PlayerInfo[playerid][Weapon][0], PlayerInfo[playerid][Weapon][1], PlayerInfo[playerid][Medkit], PlayerInfo[playerid][ID]);
	mysql_tquery(g_SQL, query);

    for(new id = 1; id <= 10; id++)
	{
		mysql_format(g_SQL, query, sizeof query, "UPDATE `players` SET `skin_%d` = %d  WHERE `id` = %d LIMIT 1", id, PlayerInfo[playerid][GangSkin][id], PlayerInfo[playerid][ID]);
		mysql_tquery(g_SQL, query);
	}

	mysql_format(g_SQL, query, sizeof query, "UPDATE `players` SET `model` = %d, `hitsound` = %d, `hitmarker` = %d WHERE `id` = %d LIMIT 1", PlayerInfo[playerid][Model],PlayerInfo[playerid][HitSound],PlayerInfo[playerid][HitMarker], PlayerInfo[playerid][ID]);
	mysql_tquery(g_SQL, query);

	mysql_format(g_SQL, query, sizeof query, "UPDATE `players` SET `signature_bg` = '%e', `signature_av` = '%e', `signature_moto` = '%e' WHERE `id` = %d LIMIT 1", PlayerInfo[playerid][s_BackgroundSprite], PlayerInfo[playerid][s_AvatarSprite], PlayerInfo[playerid][s_Moto], PlayerInfo[playerid][ID]);
	mysql_tquery(g_SQL, query);

	mysql_format(g_SQL, query, sizeof query, "UPDATE `players` SET `signature_bgcolor` = %d, `signature_avcolor` = %d, `signature_motocolor` = %d WHERE `id` = %d LIMIT 1", PlayerInfo[playerid][i_BackgroundColor], PlayerInfo[playerid][i_AvatarColor], PlayerInfo[playerid][i_MotoColor], PlayerInfo[playerid][ID]);
	mysql_tquery(g_SQL, query);

	mysql_format(g_SQL, query, sizeof query, "UPDATE `players` SET `cop_skin` = %d, `cop_car` = %d, `fug_car` = %d, `muted` = %d, `muteTime` = %d WHERE `id` = %d LIMIT 1", PlayerInfo[playerid][CopSkin], PlayerInfo[playerid][CopCar], PlayerInfo[playerid][FugCar], PlayerInfo[playerid][Muted], PlayerInfo[playerid][MuteTime], PlayerInfo[playerid][ID]);
	mysql_tquery(g_SQL, query);

	for(new id = 1; id <= 3; id++)
	{
		mysql_format(g_SQL, query, sizeof query, "UPDATE `players` SET `activity_%d` = %d  WHERE `id` = %d LIMIT 1", id, PlayerInfo[playerid][StaffActivity][id - 1], PlayerInfo[playerid][ID]);
		mysql_tquery(g_SQL, query);
	}

	for(new id = 1; id <= 3; id++)
	{
		mysql_format(g_SQL, query, sizeof query, "UPDATE `players` SET `hours_%d` = %d  WHERE `id` = %d LIMIT 1", id, PlayerInfo[playerid][PlayingHours][id - 1], PlayerInfo[playerid][ID]);
		mysql_tquery(g_SQL, query);
	}

	for(new id = 1; id <= 3; id++)
	{
		mysql_format(g_SQL, query, sizeof query, "UPDATE `players` SET `perk_%d` = %d  WHERE `id` = %d LIMIT 1", id, PlayerInfo[playerid][Perk][id - 1], PlayerInfo[playerid][ID]);
		mysql_tquery(g_SQL, query);
	}

	mysql_format(g_SQL, query, sizeof query, "UPDATE `players` SET `donator_tag` = '%e', `donator` = %d, `donator_point` = %d, `donator_expire` = %i WHERE `id` = %d LIMIT 1", PlayerInfo[playerid][DonatorTag], PlayerInfo[playerid][Donator], PlayerInfo[playerid][DonatorPoint], PlayerInfo[playerid][DonatorExpire], PlayerInfo[playerid][ID]);
	mysql_tquery(g_SQL, query);
	return 1;
}

UpdatePlayerDeaths(playerid)
{
	if (PlayerInfo[playerid][IsLoggedIn] == false) return 0;

	PlayerInfo[playerid][Deaths]++;
	PlayerInfo[playerid][SessionDeaths]++;

	new query[70];
	mysql_format(g_SQL, query, sizeof query, "UPDATE `players` SET `deaths` = %d WHERE `id` = %d LIMIT 1", PlayerInfo[playerid][Deaths], PlayerInfo[playerid][ID]);
	mysql_tquery(g_SQL, query);
	return 1;
}

UpdatePlayerKills(killerid)
{
	// we must check before if the killer wasn't valid (connected) player to avoid run time error 4
	if (killerid == INVALID_PLAYER_ID) return 0;
	if (PlayerInfo[killerid][IsLoggedIn] == false) return 0;

	PlayerInfo[killerid][Kills]++;
	PlayerInfo[killerid][SessionKills]++;

	new query[70];
	mysql_format(g_SQL, query, sizeof query, "UPDATE `players` SET `kills` = %d WHERE `id` = %d LIMIT 1", PlayerInfo[killerid][Kills], PlayerInfo[killerid][ID]);
	mysql_tquery(g_SQL, query);
	return 1;
}

PlayerHasPerk(playerid, perk)
{
	for(new i; i < 3; i++)
	{
		if(PlayerInfo[playerid][Perk][i] == perk) return 1;
	}
	return 0;
}

PlayerPerkSlot(playerid, perk)
{
	for(new i; i < 3; i++)
	{
		if(PlayerInfo[playerid][Perk][i] == perk) return i;
	}
	return -1;
}

GetPerkFreeSLot(playerid)
{
	for(new i; i < 3; i++)
	{
		if(PlayerInfo[playerid][Donator])
		{
			if(PlayerInfo[playerid][Perk][i] == 0) return i;
		}
		else if(PlayerInfo[playerid][Donator] == 0)
		{
			if(PlayerInfo[playerid][Perk][i] == 0 && i <= 1) return i;
		}
	}
	return -1;
}

GetPerkPrice(playerid)
{
	new count = 0, output;
	for(new i; i < 3; i++)
	{
		if(PlayerInfo[playerid][Perk][i] != 0)
		{
			count++;
		}
	}

    if(PlayerInfo[playerid][Donator] == 0)
	{
		if(count == 0) output = 5000;
		else if(count == 1) output = 10000;
		else if(count == 2) output = 10000;
		else if(count == 3) output = 10000;
	}
	else if(PlayerInfo[playerid][Donator] == 1)
	{
		if(count == 0) output = 5000;
		else if(count == 1) output = 10000;
		else if(count == 2) output = 10000;
		else if(count == 3) output = 10000;
	}
	else if(PlayerInfo[playerid][Donator] == 2)
	{
		if(count == 0) output = 2500;
		else if(count == 1) output = 5000;
		else if(count == 2) output = 7500;
		else if(count == 3) output = 7500;
	}
	return output;
}

GetMedkitLimit(playerid)
{
	new output;
    if(PlayerInfo[playerid][Donator] == 0) output = 3;
	else if(PlayerInfo[playerid][Donator] == 1) output = 5;
	else if(PlayerInfo[playerid][Donator] == 2) output = 10;
	return output;
}

ShowPlayerPerks(playerid)
{
	new temp_str[1096], primary_str[1096];
	strcat(primary_str, "Perks\tPrice\n");
	format(temp_str, sizeof temp_str, "{ff6347}Unequip perks\n");
	strcat(primary_str, temp_str);
	format(temp_str, sizeof temp_str, "{FFFFFF}Selfrevive (50 percent chance)%s.\t{008000}$%i{FFFFFF}\n", PlayerHasPerk(playerid, 1) ? " {008000}(EQUIPPED){FFFFFF}" : "", GetPerkPrice(playerid), GetPerkPrice(playerid));
	strcat(primary_str, temp_str);
	format(temp_str, sizeof temp_str, "{FFFFFF}50 percent more HP%s.\t{008000}$%i{FFFFFF}\n", PlayerHasPerk(playerid, 2) ? " {008000}(EQUIPPED){FFFFFF}" : "", GetPerkPrice(playerid));
	strcat(primary_str, temp_str);
	format(temp_str, sizeof temp_str, "{FFFFFF}Rifle (Random ammo every kill)%s.\t{008000}$%i{FFFFFF}\n", PlayerHasPerk(playerid, 3) ? " {008000}(EQUIPPED){FFFFFF}" : "", GetPerkPrice(playerid));
	strcat(primary_str, temp_str);
	format(temp_str, sizeof temp_str, "{FFFFFF}Instant revive%s.\t{008000}$%i{FFFFFF}\n", PlayerHasPerk(playerid, 4) ? " {008000}(EQUIPPED){FFFFFF}" : "", GetPerkPrice(playerid));
	strcat(primary_str, temp_str);
	format(temp_str, sizeof temp_str, "{FFFFFF}Ability to dissapear from radar for 60 seconds (/mask)%s.\t{008000}$%i{FFFFFF}\n", PlayerHasPerk(playerid, 5) ? " {008000}(EQUIPPED){FFFFFF}" : "", GetPerkPrice(playerid));
	strcat(primary_str, temp_str);
	format(temp_str, sizeof temp_str, "{FFFFFF}Fix or flip vehicle (/fix & /flip)%s.\t{008000}$%i{FFFFFF}\n", PlayerHasPerk(playerid, 6) ? " {008000}(EQUIPPED){FFFFFF}" : "", GetPerkPrice(playerid));
	strcat(primary_str, temp_str);
	format(temp_str, sizeof temp_str, "{FFFFFF}+15.0 armor every kill (75.0 max)%s.\t{008000}$%i{FFFFFF}\n", PlayerHasPerk(playerid, 7) ? " {008000}(EQUIPPED){FFFFFF}" : "", GetPerkPrice(playerid));
	strcat(primary_str, temp_str);
	format(temp_str, sizeof temp_str, "{FFFFFF}Regeneration (gain 1.0 HP every second)%s.\t{008000}$%i{FFFFFF}\n", PlayerHasPerk(playerid, 8) ? " {008000}(EQUIPPED){FFFFFF}" : "", GetPerkPrice(playerid));
	strcat(primary_str, temp_str);
	ShowPlayerDialog(playerid, DIALOG_PERKS, DIALOG_STYLE_TABLIST_HEADERS, "Perks", primary_str, "Buy", "Close");
}

GetHighestScore(teamid)
{
	new highest_score = 0, playerid = INVALID_PLAYER_ID;
	foreach(new i : Player)
    {
        if(IsPlayerConnected(i) && PlayerInfo[i][IsLoggedIn] == true && GetPlayerGameMode(i) == GAME_MODE_TDM && PlayerInfo[i][Team] == teamid)
		{
			if(PlayerInfo[i][Score] > highest_score) 
			{
				highest_score = PlayerInfo[i][Score];
				playerid = i;
			}
		}
    }
	return playerid;
}

ReturnDonorRank(playerid) 
{
	new output[32];
	switch (PlayerInfo[playerid][Donator])
	{
		case 0: format(output, sizeof output, "{AFAFAF}None{FFFFFF}");
		case 1: format(output, sizeof output, "{a52a2a}Bronze Donator{FFFFFF}");
		case 2: format(output, sizeof output, "{ffd700}Gold Donator{FFFFFF}");
	}
	return output;
}

ReturnName(playerid, bool:gangtag = true) 
{
	new result[64];
	if(playerid != -1)
	{

		format(result, sizeof result, PlayerInfo[playerid][Name]);

		if (gangtag == true && GetPlayerGameMode(playerid) == GAME_MODE_TDM)
		{
			if(GetHighestScore(PlayerInfo[playerid][Team]) != playerid)
			{
				format(result, sizeof result, "%s%s%s", ReturnTeamColor(PlayerInfo[playerid][Team]), ReturnTeamTag(PlayerInfo[playerid][Team]), PlayerInfo[playerid][Name]);
			}
			else
			{
				if(PlayerInfo[playerid][Team] != 10)
				{
					format(result, sizeof result, "%s%s%s[OG]", ReturnTeamColor(PlayerInfo[playerid][Team]), ReturnTeamTag(PlayerInfo[playerid][Team]), PlayerInfo[playerid][Name]);
				}
				else
				{
					format(result, sizeof result, "%s%s%s", ReturnTeamColor(PlayerInfo[playerid][Team]), ReturnTeamTag(PlayerInfo[playerid][Team]), PlayerInfo[playerid][Name]);
				}
			}
		}
	}
	else format(result, sizeof result, "NaN");
	return result;
}

SetPlayerMoney(playerid, amount) 
{
	ResetPlayerMoney(playerid);
	GivePlayerMoney(playerid,amount);
	return 1;
}

Float:GetPlayerHP(playerid, &Float:health = 0.0)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		health = 0.0;

		return 0.0;
	}

	health = PlayerInfo[playerid][Health];

	return health;
}

SetPlayerHP(playerid, Float:health)
{
	if (playerid == INVALID_PLAYER_ID) 
	{
		return 0;
	}

	if (health <= 1.0) 
	{
		PlayerInfo[playerid][Health] = 0.0;
		SetPlayerHealth(playerid, 30);
	} 
	else 
	{
		PlayerInfo[playerid][Health] = health;
		SetPlayerHealth(playerid, PlayerInfo[playerid][Health]);
	}

	return 1;
}

Float:GetPlayerArmor(playerid, &Float:armour = 0.0)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		armour = 0.0;

		return 0.0;
	}

	armour = PlayerInfo[playerid][Armour];

	return armour;
}

SetPlayerArmor(playerid, Float:armour)
{
	if (playerid == INVALID_PLAYER_ID) 
	{
		return 0;
	}

	if (armour <= 0.0) 
	{
		PlayerInfo[playerid][Armour] = 0.0;
		SetPlayerArmour(playerid, PlayerInfo[playerid][Armour]);
	} 
	else 
	{

		if (armour > 100.0) 
		{
			armour = 100.0;
		}
		PlayerInfo[playerid][Armour] = armour;
		SetPlayerArmour(playerid, PlayerInfo[playerid][Armour]);
	}

	return 1;
}

GetPlayerGameMode(playerid) 
{
	new gmode = PlayerInfo[playerid][GameMode];
	return gmode;
}

SetPlayerGameMode(playerid, gamemode) 
{
	PlayerInfo[playerid][GameMode] = gamemode;
	return 1;
}

stock GetHealthDots(playerid)
{
    new dots[64];
    new Float:HP;
 
    GetPlayerHP(playerid, HP);

	if(PlayerInfo[playerid][State] == PLAYER_STATE_WOUNDED) HP = 0;
 
    if(HP >= 100)
        dots = "----------";
    else if(HP >= 90 && HP < 100)
        dots = "---------{660000}-";
    else if(HP >= 80 && HP < 90)
        dots = "--------{660000}--";
    else if(HP >= 70 && HP < 80)
        dots = "-------{660000}---";
    else if(HP >= 60 && HP < 70)
        dots = "------{660000}----";
    else if(HP >= 50 && HP < 60)
        dots = "-----{660000}-----";
    else if(HP >= 40 && HP < 50)
        dots = "----{660000}------";
    else if(HP >= 30 && HP < 40)
        dots = "---{660000}-------";
    else if(HP >= 20 && HP < 30)
        dots = "--{660000}--------";
    else if(HP >= 10 && HP < 20)
        dots = "--{660000}--------";
    else if(HP >= 0 && HP < 10)
        dots = "{660000}----------";
 
    return dots;
}
stock GetArmorDots(playerid)
{
    new dots[64];
    new Float:AR;
 
    GetPlayerArmor(playerid, AR);
 
    if(AR >= 100)
        dots = "----------";
    else if(AR >= 90 && AR < 100)
        dots = "---------{666666}-";
    else if(AR >= 80 && AR < 90)
        dots = "--------{666666}--";
    else if(AR >= 70 && AR < 80)
        dots = "-------{666666}---";
    else if(AR >= 60 && AR < 70)
        dots = "------{666666}----";
    else if(AR >= 50 && AR < 60)
        dots = "-----{666666}-----";
    else if(AR >= 40 && AR < 50)
        dots = "----{666666}------";
    else if(AR >= 30 && AR < 40)
        dots = "---{666666}-------";
    else if(AR >= 20 && AR < 30)
        dots = "--{666666}--------";
    else if(AR >= 10 && AR < 20)
        dots = "--{666666}--------";
    else if(AR >= 0 && AR < 10)
        dots = "{666666}----------";
 
    return dots;
}

stock GetPlayerCount()
{
	new output;
	foreach(new i : Player)
	{
		if(PlayerInfo[i][IsLoggedIn]) output++;
	}
	return output;
}

UpdateNameTag(playerid, bool:hit = false)
{
	new nametag[128], playername[64], Float:armour;
	if(!IsValidDynamic3DTextLabel(PlayerInfo[playerid][Nametag]) && GetPlayerState(playerid) != PLAYER_STATE_SPECTATING)
	{
		PlayerInfo[playerid][Nametag] = CreateDynamic3DTextLabel("Loading nametag...", 0xFFFFFFFF, 0.0, 0.0, 0.1, 15.0, .attachedplayer = playerid, .testlos = 1);
	}
	if(hit == true)
	{
		if(GetPlayerWantedLevel(playerid) >= 3 && GetPlayerGameMode(playerid) == GAME_MODE_COPCHASE)
		{
			GetPlayerArmor(playerid, armour);
			GetPlayerName(playerid, playername, sizeof(playername));
			if(armour > 1.0)
			{
				format(nametag, sizeof(nametag), "{FF6347}(( Player has shot! ))\n{9F0909}%s (%i)\n{FFFFFF}%s\n{FF0000}%s", playername, playerid, GetArmorDots(playerid), GetHealthDots(playerid));
			}
			else
			{
				format(nametag, sizeof(nametag), "{FF6347}(( Player has shot! ))\n{9F0909}%s (%i)\n{FF0000}%s", playername, playerid, GetHealthDots(playerid));
			}
			UpdateDynamic3DTextLabelText(PlayerInfo[playerid][Nametag], 0x9F0909FF, nametag);
		}
		else
		{
			GetPlayerArmor(playerid, armour);
			GetPlayerName(playerid, playername, sizeof(playername));
			if(armour > 1.0)
			{
				format(nametag, sizeof(nametag), "%s (%i)\n{FFFFFF}%s\n{FF0000}%s", ReturnName(playerid), playerid, GetArmorDots(playerid), GetHealthDots(playerid));
			}
			else
			{
				format(nametag, sizeof(nametag), "%s (%i)\n{FF0000}%s", ReturnName(playerid), playerid, GetHealthDots(playerid));
			}
			UpdateDynamic3DTextLabelText(PlayerInfo[playerid][Nametag], 0x9F0909FF, nametag);
		}
	}
	else 
	{
		if(GetPlayerWantedLevel(playerid) >= 3 && GetPlayerGameMode(playerid) == GAME_MODE_COPCHASE)
		{
			if(GetTickCount() > (PlayerInfo[playerid][PauseCheck]+2000))
			{
				GetPlayerArmor(playerid, armour);
				GetPlayerName(playerid, playername, sizeof(playername));
				if(armour > 1.0)
				{
					format(nametag, sizeof(nametag), "{FF6347}(( Player has shot! ))\n[Paused]\n{%06x}%s (%i)\n{FFFFFF}%s\n{FF0000}%s", GetPlayerColor(playerid) >>> 8, playername, playerid, GetArmorDots(playerid), GetHealthDots(playerid));
				}
				else
				{
					format(nametag, sizeof(nametag), "{FF6347}(( Player has shot! ))\n[Paused]\n{%06x}%s (%i)\n{FF0000}%s", GetPlayerColor(playerid) >>> 8, playername, playerid, GetHealthDots(playerid));
				}
				UpdateDynamic3DTextLabelText(PlayerInfo[playerid][Nametag], COLOR_RED, nametag);
			}
			else
			{
				GetPlayerArmor(playerid, armour);
				GetPlayerName(playerid, playername, sizeof(playername));
				if(armour > 1.0)
				{
					format(nametag, sizeof(nametag), "{FF6347}(( Player has shot! ))\n{%06x}%s (%i)\n{FFFFFF}%s\n{FF0000}%s", GetPlayerColor(playerid) >>> 8, playername, playerid, GetArmorDots(playerid), GetHealthDots(playerid));
				}
				else
				{
					format(nametag, sizeof(nametag), "{FF6347}(( Player has shot! ))\n{%06x}%s (%i)\n{FF0000}%s", GetPlayerColor(playerid) >>> 8, playername, playerid, GetHealthDots(playerid));
				}
				UpdateDynamic3DTextLabelText(PlayerInfo[playerid][Nametag], 0xFFFFFFFF, nametag);
			}
		}
		else
		{
			if(GetTickCount() > (PlayerInfo[playerid][PauseCheck]+2000))
			{
				GetPlayerArmor(playerid, armour);
				GetPlayerName(playerid, playername, sizeof(playername));
				if(armour > 1.0)
				{
					format(nametag, sizeof(nametag), "[Paused]\n{%06x}%s (%i)\n{FFFFFF}%s\n{FF0000}%s", GetPlayerColor(playerid) >>> 8, ReturnName(playerid), playerid, GetArmorDots(playerid), GetHealthDots(playerid));
				}
				else
				{
					format(nametag, sizeof(nametag), "[Paused]\n{%06x}%s (%i)\n{FF0000}%s", GetPlayerColor(playerid) >>> 8, ReturnName(playerid), playerid, GetHealthDots(playerid));
				}
				UpdateDynamic3DTextLabelText(PlayerInfo[playerid][Nametag], COLOR_RED, nametag);
			}
			else
			{
				GetPlayerArmor(playerid, armour);
				GetPlayerName(playerid, playername, sizeof(playername));
				if(armour > 1.0)
				{
					format(nametag, sizeof(nametag), "{%06x}%s (%i)\n{FFFFFF}%s\n{FF0000}%s", GetPlayerColor(playerid) >>> 8, ReturnName(playerid), playerid, GetArmorDots(playerid), GetHealthDots(playerid));
				}
				else
				{
					format(nametag, sizeof(nametag), "{%06x}%s (%i)\n{FF0000}%s", GetPlayerColor(playerid) >>> 8, ReturnName(playerid), playerid, GetHealthDots(playerid));
				}
				UpdateDynamic3DTextLabelText(PlayerInfo[playerid][Nametag], 0xFFFFFFFF, nametag);
			}
		}
	}
}

ReturnLocation(playerid)
{
	new 
		playerLocation[MAX_ZONE_NAME];
	
	/*if(GetPlayerGameMode(playerid) == GAME_MODE_COPCHASE) 
	{
		if(GetSVarInt("CCMap") == 5) format(playerLocation, MAX_ZONE_NAME, gVCZones[pVcMinimapCurrentArea[playerid]][VCZONE_NAME]);
		else GetPlayer2DZone(playerid, playerLocation, MAX_ZONE_NAME);
		return playerLocation;
	}*/

	GetPlayer2DZone(playerid, playerLocation, MAX_ZONE_NAME);
	return playerLocation;
}

GetPlayer2DZone(playerid, zone[], len) //Credits to Cueball, Betamaster, Mabako, and Simon (for finetuning).
{
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
	for(new i = 0; i != sizeof(gSAZones); i++ )
	{
        if(x >= gSAZones[i][SAZONE_AREA][0] && x <= gSAZones[i][SAZONE_AREA][3] && y >= gSAZones[i][SAZONE_AREA][1] && y <= gSAZones[i][SAZONE_AREA][4])
        {
            return format(zone, len, gSAZones[i][SAZONE_NAME], 0);
        }
	}
	return 0;
}