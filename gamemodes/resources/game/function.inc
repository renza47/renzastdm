SendPlayerToLobby(playerid)
{
    for (new i; i < 17; i++)
    {
        WeaponInfo[playerid][i][Hidden] = true;
    }
	for(new i; i<6; i++)
	{
		SendDeathMessageToPlayer(playerid, 02, 202, 202);
	}

	RemovePlayerAttachedObject(playerid, 5);
	RemovePlayerAttachedObject(playerid, 6);
	RemovePlayerAttachedObject(playerid, 7);

	if(IsValidVehicle(PlayerInfo[playerid][FreeroamVehicle])) 
    {
        DestroyVehicle(PlayerInfo[playerid][FreeroamVehicle]);
        PlayerInfo[playerid][FreeroamVehicle] = INVALID_VEHICLE_ID;
    }
	
    WeaponTick[playerid] = 0;
    EditingWeapon[playerid] = 0;

	TogglePlayerControllable(playerid, true);

	PlayerTextDrawHide(playerid, speedo1[playerid]);
	PlayerTextDrawHide(playerid, speedo2[playerid]);
	PlayerTextDrawHide(playerid, speedo3[playerid]);

	PlayerTextDrawHide(playerid, ui_DamageGiven[playerid]);
    PlayerTextDrawHide(playerid, ui_DamageTaken[playerid]);

	SetPlayerWantedLevel(playerid, 0);

	ResetPlayerWeapons(playerid);
	
	SetPlayerName(playerid, PlayerInfo[playerid][Name]);
	SetPlayerRandomColor(playerid);

	PlayerTextDrawHide(playerid, ui_clock[playerid]);
	if(!PlayerInfo[playerid][Tog][1]) {
		PlayerTextDrawShow(playerid, ui_serverName[playerid]);
		PlayerTextDrawShow(playerid, ui_serverWeb[playerid]);
	}

	PlayerTextDrawHide(playerid, ui_ProgressInfo[playerid]);
	//HidePlayerProgressBar(playerid, ui_ProgressBar[playerid]);

    SetPlayerGameMode(playerid, GAME_MODE_LOBBY);

	ClearAnimations(playerid);

	SetPlayerInterior(playerid,18);
    SetPlayerVirtualWorld(playerid, 11);

    PlayerInfo[playerid][Property] = -1;
	PlayerInfo[playerid][Traced] = 0;

    SetPlayerPos(playerid,2307.6270,576.1976,106.5366 + 1.0);

	FreezePlayer(playerid, 1000);

    SetPlayerFacingAngle(playerid, 269.0);

	SetPlayerSkin(playerid, PlayerInfo[playerid][Model]);

	SetPlayerHP(playerid, 100.0);
	return 1;
}

SetPlayerRandomColor(playerid)
{
	new rand = random(sizeof PlayerColors);

	SetPlayerColor(playerid, PlayerColors[rand]);
	return 1;
}

ReturnWeaponNameEx(weaponid)
{
	new weaponstr[45];
	switch(weaponid)
	{
	    case 0: weaponstr = "Empty";
	    case 18: weaponstr = "Molotov Cocktail";
        case 44: weaponstr = "Night Vision Goggles";
        case 45: weaponstr = "Thermal Goggles";
        default: GetWeaponName(weaponid, weaponstr, sizeof(weaponstr));
	}
	return weaponstr;
}

ConvertTime(time)
{
	new str[16];
    new minutes=time/60000;
    new ms=time-((minutes)*60000);
    new seconds=(ms)/1000;
    ms-=seconds*1000;
	format(str, sizeof(str), "%02d:%02d.%03d", minutes, seconds, ms);
	return str;
}