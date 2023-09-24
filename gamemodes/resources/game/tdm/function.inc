SendPlayerToTeamSelection(playerid) 
{
	ResetPlayerWeapons(playerid);

	PlayerInfo[playerid][State] = PLAYER_STATE_ALIVE;
	PlayerInfo[playerid][DeathTime] = 0;

    ClearAnimations(playerid);

    PlayerInfo[playerid][KillStreak] = 0;

    PlayerInfo[playerid][MedkitTime] = 0;

    ClearPlayerDamages(playerid);

	SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, 0);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, 0);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_SPAS12_SHOTGUN, 0);

	if(!PlayerHasPerk(playerid, 2)) SetPlayerHP(playerid, 150.0);
	else SetPlayerHP(playerid, 200.0);

    TogglePlayerControllable(playerid, false);

	SetPlayerWantedLevel(playerid, 0);

    PlayerInfo[playerid][SelectingTeam] = true;

	PlayerInfo[playerid][SelfRevive] = 0;

	PlayerInfo[playerid][FixVeh] = 0;
	PlayerInfo[playerid][FlipVeh] = 0;

	SetPlayerGameMode(playerid, GAME_MODE_TDM);

    SetPVarInt(playerid, "CurrentSkin", 0);
    SetPlayerSkin(playerid, PlayerInfo[playerid][GangSkin][GetPVarInt(playerid, "CurrentSkin") + 1]);

    SetPlayerInterior(playerid,0);
    SetPlayerVirtualWorld(playerid, random(100000) + playerid);
    SetPlayerPos(playerid, gTeamCameras[0][0], gTeamCameras[0][1], gTeamCameras[0][2]);
	SetPlayerCameraPos(playerid, gTeamCameras[0][3], gTeamCameras[0][4], gTeamCameras[0][5]);
	SetPlayerCameraLookAt(playerid, gTeamCameras[0][0], gTeamCameras[0][1], gTeamCameras[0][2]);
    SetPlayerFacingAngle(playerid,gTeamCameras[0][6]);
	ApplyAnimation(playerid, "FREEWEIGHTS", "gym_free_celebrate", 4.0, 0, 0, 0, 0, 0, 1);

    PlayerTextDrawShow(playerid, ui_class_skinName[playerid]);
    PlayerTextDrawShow(playerid, ui_class_arrowLeft[playerid]);
    PlayerTextDrawShow(playerid, ui_class_arrowRight[playerid]);
    PlayerTextDrawShow(playerid, ui_class_select[playerid]);
    PlayerTextDrawShow(playerid, ui_class_name[playerid]);

	PlayerTextDrawHide(playerid, ui_clock[playerid]);
	PlayerTextDrawHide(playerid, ui_serverName[playerid]);
	PlayerTextDrawHide(playerid, ui_serverWeb[playerid]);

	PlayerTextDrawSetString(playerid, ui_class_name[playerid], ReturnTeamTagForText(gTeams[0][0]));
	PlayerTextDrawSetString(playerid, ui_class_skinName[playerid], ReturnTeamName(gTeams[0][0]));

    SelectTextDraw(playerid, 0xAFAFAFFF);
    return 1;
}

ReturnTeamTagForText(team)
{
	new team_tag[32];
	if(team == 1) format(team_tag, sizeof(team_tag), "~g~[GSF]");
    else if(team == 2) format(team_tag, sizeof team_tag,"~g~~h~[SBF]");
    else if(team == 3) format(team_tag, sizeof team_tag,"~g~[TDF]");
    else if(team == 4) format(team_tag, sizeof team_tag,"~p~[FYB]");
    else if(team == 5) format(team_tag, sizeof team_tag,"~p~[RHB]");
    else if(team == 6) format(team_tag, sizeof team_tag,"~p~[KTB]");
    else if(team == 7) format(team_tag, sizeof team_tag,"~p~[TDB]");
	else if(team == 8) format(team_tag, sizeof team_tag,"~y~[LSV]");
	else if(team == 9) format(team_tag, sizeof team_tag,"~b~~h~~h~~h~[VLA]");
	else if(team == 10) format(team_tag, sizeof team_tag,"~b~[LSPD]");
	return team_tag;
}

ReturnTeamTagForTextEx(team)
{
	new team_tag[32];
	if(team == 1) format(team_tag, sizeof(team_tag), "~g~GSF");
    else if(team == 2) format(team_tag, sizeof team_tag,"~g~~h~SBF");
    else if(team == 3) format(team_tag, sizeof team_tag,"~g~TDF");
    else if(team == 4) format(team_tag, sizeof team_tag,"~p~FYB");
    else if(team == 5) format(team_tag, sizeof team_tag,"~p~RHB");
    else if(team == 6) format(team_tag, sizeof team_tag,"~p~KTB");
    else if(team == 7) format(team_tag, sizeof team_tag,"~p~TDB");
	else if(team == 8) format(team_tag, sizeof team_tag,"~y~LSV");
	else if(team == 9) format(team_tag, sizeof team_tag,"~b~~h~~h~~h~VLA");
	else if(team == 10) format(team_tag, sizeof team_tag,"~b~LSPD");
	return team_tag;
}

ReturnTeamTag(team)
{
	new team_tag[16];
	if(team == 1) format(team_tag, sizeof(team_tag), "[GSF]");
    else if(team == 2) format(team_tag, sizeof team_tag,"[SBF]");
    else if(team == 3) format(team_tag, sizeof team_tag,"[TDF]");
    else if(team == 4) format(team_tag, sizeof team_tag,"[FYB]");
    else if(team == 5) format(team_tag, sizeof team_tag,"[RHB]");
    else if(team == 6) format(team_tag, sizeof team_tag,"[KTB]");
    else if(team == 7) format(team_tag, sizeof team_tag,"[TDB]");
	else if(team == 8) format(team_tag, sizeof team_tag,"[LSV]");
	else if(team == 9) format(team_tag, sizeof team_tag,"[VLA]");
	else if(team == 10) format(team_tag, sizeof team_tag,"[LSPD]");
	return team_tag;
}

ReturnTeamName(team)
{
	new team_name[64];
	if(team == 1) format(team_name, sizeof(team_name), "Grove Street Families");
    else if(team == 2) format(team_name, sizeof team_name,"Seville Boulevard Families");
    else if(team == 3) format(team_name, sizeof team_name,"Temple Drive Families");
    else if(team == 4) format(team_name, sizeof team_name,"Front Yard Ballas");
    else if(team == 5) format(team_name, sizeof team_name,"Rollin Heights Ballas");
    else if(team == 6) format(team_name, sizeof team_name,"Kilo Tray Ballas");
    else if(team == 7) format(team_name, sizeof team_name,"Temple Drive Ballas");
	else if(team == 8) format(team_name, sizeof team_name,"Los Santos Vagos");
	else if(team == 9) format(team_name, sizeof team_name,"Varrios Los Aztecas");
	else if(team == 10) format(team_name, sizeof team_name,"Los Santos Police Departement");
	return team_name;
}

ReturnTeamColor(team)
{
	new team_color[16];
	if(team == 1) format(team_color, sizeof(team_color), "{6aa84f}");
    else if(team == 2) format(team_color, sizeof team_color,"{8fce00}");
    else if(team == 3) format(team_color, sizeof team_color,"{38761d}");
    else if(team == 4) format(team_color, sizeof team_color,"{6a329f}");
    else if(team == 5) format(team_color, sizeof team_color,"{5a1e92}");
    else if(team == 6) format(team_color, sizeof team_color,"{dc4af3}");
    else if(team == 7) format(team_color, sizeof team_color,"{6a329f}");
	else if(team == 8) format(team_color, sizeof team_color,"{f1c232}");
	else if(team == 9) format(team_color, sizeof team_color,"{6fa8dc}");
	else if(team == 10) format(team_color, sizeof team_color,"{8888FF}");
	return team_color;
}

ReturnTeamColorEx(team)
{
	new team_colorEx;
	if(team == 1) team_colorEx = 0x6aa84fFF;
    else if(team == 2) team_colorEx = 0x8fce00FF;
    else if(team == 3) team_colorEx = 0x38761dFF;
    else if(team == 4) team_colorEx = 0x6a329fFF;
    else if(team == 5) team_colorEx = 0x5a1e92FF;
    else if(team == 6) team_colorEx = 0xdc4af3FF;
    else if(team == 7) team_colorEx = 0x6a329fFF;
	else if(team == 8) team_colorEx = 0xf1c232FF;
	else if(team == 9) team_colorEx = 0x6fa8dcFF;
	else if(team == 10) team_colorEx = COLOR_COP;
	return team_colorEx;
}

ReturnTeamZoneColor(team)
{
	new team_colorEx;
	if(team == 1) team_colorEx = 0x00FF0066;
    else if(team == 2) team_colorEx = 0x7FFF0066;
    else if(team == 3) team_colorEx = 0x00FF0066;
    else if(team == 4) team_colorEx = 0xFF00FF66;
    else if(team == 5) team_colorEx = 0xFF00FF66;
    else if(team == 6) team_colorEx = 0xFF00FF66;
    else if(team == 7) team_colorEx = 0xFF00FF66;
	else if(team == 8) team_colorEx = 0xFFFF0066;
	else if(team == 9) team_colorEx = 0x00FFFF66;
	return team_colorEx;
}

SetPlayerPosition(playerid)
{
	TogglePlayerControllable(playerid, true);
	SetPlayerWantedLevel(playerid, 0);
	PlayerInfo[playerid][BrokenLeg] = false;
	PlayerInfo[playerid][GoldenGunAmmo] = 0;
	SetPlayerArmor(playerid, 0);
	SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
	SetPlayerSkin(playerid, PlayerInfo[playerid][GangSkin][PlayerInfo[playerid][Team]]);
	PlayerInfo[playerid][RecentlySpawned] = gettime();
	PlayerInfo[playerid][SelfRevive] = 0;

	PlayerInfo[playerid][FixVeh] = 0;
	PlayerInfo[playerid][FlipVeh] = 0;

	PlayerInfo[playerid][Mask] = 0;
	PlayerInfo[playerid][MedkitRevive] = 0;
	if(PlayerHasPerk(playerid, 3)) GivePlayerWeapon(playerid, 33, 4);
	switch (PlayerInfo[playerid][Team])
	{
		case TEAM_GSF:
		{
			new randSpawn = random(sizeof(gRandomSpawns_GSF));
			SetPlayerPos(playerid,
			gRandomSpawns_GSF[randSpawn][0],
			gRandomSpawns_GSF[randSpawn][1],
			gRandomSpawns_GSF[randSpawn][2]);
			SetPlayerFacingAngle(playerid,gRandomSpawns_GSF[randSpawn][3]);

			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][0], 9999);
			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][1], 9999);
		}
		case TEAM_SBF:
		{
			new randSpawn = random(sizeof(gRandomSpawns_SBF));
			SetPlayerPos(playerid,
			gRandomSpawns_SBF[randSpawn][0],
			gRandomSpawns_SBF[randSpawn][1],
			gRandomSpawns_SBF[randSpawn][2]);
			SetPlayerFacingAngle(playerid,gRandomSpawns_SBF[randSpawn][3]);

			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][0], 9999);
			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][1], 9999);
		}
		case TEAM_TDF:
		{
			new randSpawn = random(sizeof(gRandomSpawns_TDF));
			SetPlayerPos(playerid,
			gRandomSpawns_TDF[randSpawn][0],
			gRandomSpawns_TDF[randSpawn][1],
			gRandomSpawns_TDF[randSpawn][2]);
			SetPlayerFacingAngle(playerid,gRandomSpawns_TDF[randSpawn][3]);

			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][0], 9999);
			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][1], 9999);
		}
		case TEAM_FYB:
		{
			new randSpawn = random(sizeof(gRandomSpawns_FYB));
			SetPlayerPos(playerid,
			gRandomSpawns_FYB[randSpawn][0],
			gRandomSpawns_FYB[randSpawn][1],
			gRandomSpawns_FYB[randSpawn][2]);
			SetPlayerFacingAngle(playerid,gRandomSpawns_FYB[randSpawn][3]);

			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][0], 9999);
			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][1], 9999);
		}
		case TEAM_RHB:
		{
			new randSpawn = random(sizeof(gRandomSpawns_RHB));
			SetPlayerPos(playerid,
			gRandomSpawns_RHB[randSpawn][0],
			gRandomSpawns_RHB[randSpawn][1],
			gRandomSpawns_RHB[randSpawn][2]);
			SetPlayerFacingAngle(playerid,gRandomSpawns_RHB[randSpawn][3]);

			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][0], 9999);
			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][1], 9999);
		}
		case TEAM_KTB:
		{
			new randSpawn = random(sizeof(gRandomSpawns_KTB));
			SetPlayerPos(playerid,
			gRandomSpawns_KTB[randSpawn][0],
			gRandomSpawns_KTB[randSpawn][1],
			gRandomSpawns_KTB[randSpawn][2]);
			SetPlayerFacingAngle(playerid,gRandomSpawns_KTB[randSpawn][3]);

			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][0], 9999);
			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][1], 9999);
		}
		case TEAM_TDB:
		{
			new randSpawn = random(sizeof(gRandomSpawns_TDB));
			SetPlayerPos(playerid,
			gRandomSpawns_TDB[randSpawn][0],
			gRandomSpawns_TDB[randSpawn][1],
			gRandomSpawns_TDB[randSpawn][2]);
			SetPlayerFacingAngle(playerid,gRandomSpawns_TDB[randSpawn][3]);

			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][0], 9999);
			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][1], 9999);
		}
		case TEAM_LSV:
		{
			new randSpawn = random(sizeof(gRandomSpawns_LSV));
			SetPlayerPos(playerid,
			gRandomSpawns_LSV[randSpawn][0],
			gRandomSpawns_LSV[randSpawn][1],
			gRandomSpawns_LSV[randSpawn][2]);
			SetPlayerFacingAngle(playerid,gRandomSpawns_LSV[randSpawn][3]);

			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][0], 9999);
			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][1], 9999);
		}
		case TEAM_VLA:
		{
			new randSpawn = random(sizeof(gRandomSpawns_VLA));
			SetPlayerPos(playerid,
			gRandomSpawns_VLA[randSpawn][0],
			gRandomSpawns_VLA[randSpawn][1],
			gRandomSpawns_VLA[randSpawn][2]);
			SetPlayerFacingAngle(playerid,gRandomSpawns_VLA[randSpawn][3]);

			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][0], 9999);
			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][1], 9999);
		}
		case TEAM_LSPD:
		{
			new randSpawn = random(sizeof(gRandomSpawns_LSPD));
			SetPlayerPos(playerid,
			gRandomSpawns_LSPD[randSpawn][0],
			gRandomSpawns_LSPD[randSpawn][1],
			gRandomSpawns_LSPD[randSpawn][2]);
			SetPlayerFacingAngle(playerid,gRandomSpawns_LSPD[randSpawn][3]);

			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][0], 9999);
			GivePlayerWeapon(playerid, PlayerInfo[playerid][Weapon][1], 9999);

			SetPlayerArmor(playerid, 75.0);
		}
	}

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
	return 1;
}

forward CreateGangZones();
public CreateGangZones()
{
    GangZoneCreate(0.0, 0.0, 0.0, 0.0, 0x00000000); // dunno lol
	ZONE_GSF =  GangZoneCreate(2211.6793823242188, -1834.7031555175781, 2546.6793823242188, -1624.7031555175781, 0x00FF0066);
	ZoneInfo[ZONE_GSF][zOwner] = TEAM_GSF;
	ZONE_SBF =  GangZoneCreate(2566.6842041015625, -2040.693359375, 2816.6842041015625, -1898.693359375, 0x7FFF0066);
	ZoneInfo[ZONE_SBF][zOwner] = TEAM_SBF;
	ZONE_TDF =  GangZoneCreate(952.6557006835938, -1136.665267944336, 1112.6557006835938, -996.6652679443359, 0x00FF0066);
	ZoneInfo[ZONE_TDF][zOwner] = TEAM_TDF;
	ZONE_FYB =  GangZoneCreate(2001.6795043945312, -1834.6983642578125, 2212.6795043945312, -1624.6983642578125, 0xFF00FF66);
	ZoneInfo[ZONE_FYB][zOwner] = TEAM_FYB;
	ZONE_RHB =  GangZoneCreate(2080.6535034179688, -1265.6982727050781, 2285.6535034179688, -1114.6982727050781, 0xFF00FF66);
	ZoneInfo[ZONE_RHB][zOwner] = TEAM_RHB;
	ZONE_LSV =  GangZoneCreate(2420.6792602539062, -1456.6934509277344, 2556.6792602539062, -1270.6934509277344, 0xFFFF0066);
	ZoneInfo[ZONE_LSV][zOwner] = TEAM_LSV;
	ZONE_KTB =  GangZoneCreate(1868.658203125, -1265.6959228515625, 2081.658203125, -1114.6959228515625, 0xFF00FF66);
	ZoneInfo[ZONE_KTB][zOwner] = TEAM_KTB;
	ZONE_TDB =  GangZoneCreate(1163.6273193359375, -964.6426467895508, 1378.6273193359375, -829.6426467895508, 0xFF00FF66);
	ZoneInfo[ZONE_TDB][zOwner] = TEAM_TDB;
	ZONE_VLA =  GangZoneCreate(1822.6387329101562, -2085.6529235839844, 1952.6387329101562, -1958.6529235839844, 0x00FFFF66);
	ZoneInfo[ZONE_VLA][zOwner] = TEAM_VLA;

	Capturable[1] =  GangZoneCreate(1791.6793823242188, -1834.6982727050781, 2002.6793823242188, -1624.6982727050781, 0xFF00FF66); 
	ZoneInfo[Capturable[1]][zOwner] = TEAM_FYB;
	Capturable[2] =  GangZoneCreate(2408.6842041015625, -2040.6889038085938, 2567.6842041015625, -1898.6889038085938, 0x7FFF0066);
	ZoneInfo[Capturable[2]][zOwner] = TEAM_SBF;
	Capturable[3] =  GangZoneCreate(1111.65576171875, -1136.6652221679688, 1336.65576171875, -996.6652221679688, 0x00FF0066);
	ZoneInfo[Capturable[3]][zOwner] = TEAM_TDF;
	Capturable[4] =  GangZoneCreate(1791.6793823242188, -1625.6982421875, 2002.6793823242188, -1414.6982421875, 0xFF00FF66);
	ZoneInfo[Capturable[4]][zOwner] = TEAM_FYB;
	Capturable[5] =  GangZoneCreate(2001.6792907714844, -1625.6983032226562, 2212.6792907714844, -1414.6983032226562, 0xFF00FF66);
	ZoneInfo[Capturable[5]][zOwner] = TEAM_FYB;
	Capturable[6] =  GangZoneCreate(2211.6793212890625, -1624.6982421875, 2421.6793212890625, -1414.6982421875, 0xFF00FF66);
	ZoneInfo[Capturable[6]][zOwner] = TEAM_FYB;
	Capturable[7] =  GangZoneCreate(2420.6792602539062, -1624.6983032226562, 2545.6792602539062, -1455.6983032226562, 0xFF00FF66);
	ZoneInfo[Capturable[7]][zOwner] = TEAM_RHB;
	Capturable[8] =  GangZoneCreate(1791.6793670654297, -1415.6981964111328, 2002.6793670654297, -1264.6981964111328, 0xFF00FF66);
	ZoneInfo[Capturable[8]][zOwner] = TEAM_KTB;
	Capturable[9] =  GangZoneCreate(2001.6792907714844, -1415.6983032226562, 2212.6792907714844, -1264.6983032226562, 0xFF00FF66);
	ZoneInfo[Capturable[9]][zOwner] = TEAM_RHB;
	Capturable[10] =  GangZoneCreate(2211.6792602539062, -1415.6983032226562, 2421.6792602539062, -1264.6983032226562, 0xFF00FF66);
	ZoneInfo[Capturable[10]][zOwner] = TEAM_RHB;
	Capturable[11] =  GangZoneCreate(2284.677001953125, -1265.6981811523438, 2421.677001953125, -1114.6981811523438, 0xFF00FF66);
	ZoneInfo[Capturable[11]][zOwner] = TEAM_RHB;
	Capturable[12] =  GangZoneCreate(2421.679718017578, -1271.690185546875, 2555.679718017578, -1085.690185546875, 0xFFFF0066);
	ZoneInfo[Capturable[12]][zOwner] = TEAM_LSV;
	Capturable[13] =  GangZoneCreate(2554.6697998046875, -1271.68408203125, 2690.6697998046875, -1085.68408203125, 0xFFFF0066);
	ZoneInfo[Capturable[13]][zOwner] = TEAM_LSV;
	Capturable[14] =  GangZoneCreate(2689.6697998046875, -1271.6746520996094, 2824.6697998046875, -1085.6746520996094, 0xFFFF0066);
	ZoneInfo[Capturable[14]][zOwner] = TEAM_LSV;
	Capturable[15] =  GangZoneCreate(2555.674560546875, -1456.68408203125, 2690.674560546875, -1270.68408203125, 0xFFFF0066);
	ZoneInfo[Capturable[15]][zOwner] = TEAM_LSV;
	Capturable[16] =  GangZoneCreate(2689.6697998046875, -1456.6793518066406, 2824.6697998046875, -1270.6793518066406, 0xFFFF0066);
	ZoneInfo[Capturable[16]][zOwner] = TEAM_LSV;
	Capturable[17] =  GangZoneCreate(2545.6792602539062, -1640.6934509277344, 2824.6792602539062, -1455.6934509277344, 0xFFFF0066);
	ZoneInfo[Capturable[17]][zOwner] = TEAM_LSV;
	Capturable[18] =  GangZoneCreate(2545.6792602539062, -1833.6934204101562, 2824.6792602539062, -1640.6934204101562, 0xFFFF0066);
	ZoneInfo[Capturable[18]][zOwner] = TEAM_LSV;
	Capturable[19] =  GangZoneCreate(1691.6322631835938, -2085.65087890625, 1824.6322631835938, -1958.65087890625, 0x00FFFF66);
	ZoneInfo[Capturable[19]][zOwner] = TEAM_VLA;
	Capturable[20] =  GangZoneCreate(1691.626708984375, -2180.649383544922, 1824.626708984375, -2084.649383544922, 0x00FFFF66);
	ZoneInfo[Capturable[20]][zOwner] = TEAM_VLA;
	Capturable[21] =  GangZoneCreate(1822.6304931640625, -2180.649383544922, 1952.6304931640625, -2084.649383544922, 0x00FFFF66);
	ZoneInfo[Capturable[21]][zOwner] = TEAM_VLA;
	Capturable[22] =  GangZoneCreate(1691.626708984375, -1959.6496887207031, 1952.626708984375, -1862.6496887207031, 0x00FFFF66);
	ZoneInfo[Capturable[22]][zOwner] = TEAM_VLA;
	return 1;
}

forward ShowGangZonesForPlayer(playerid);
public ShowGangZonesForPlayer(playerid) 
{
	GangZoneShowForPlayer(playerid, ZONE_GSF, ReturnTeamZoneColor(ZoneInfo[ZONE_GSF][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, ZONE_SBF, ReturnTeamZoneColor(ZoneInfo[ZONE_SBF][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[2], ReturnTeamZoneColor(ZoneInfo[Capturable[2]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, ZONE_TDF, ReturnTeamZoneColor(ZoneInfo[ZONE_TDF][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[3], ReturnTeamZoneColor(ZoneInfo[Capturable[3]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, ZONE_FYB, ReturnTeamZoneColor(ZoneInfo[ZONE_FYB][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[1], ReturnTeamZoneColor(ZoneInfo[Capturable[1]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[4], ReturnTeamZoneColor(ZoneInfo[Capturable[4]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[5], ReturnTeamZoneColor(ZoneInfo[Capturable[5]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[6], ReturnTeamZoneColor(ZoneInfo[Capturable[6]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[7], ReturnTeamZoneColor(ZoneInfo[Capturable[7]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[8], ReturnTeamZoneColor(ZoneInfo[Capturable[8]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[9], ReturnTeamZoneColor(ZoneInfo[Capturable[9]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[10], ReturnTeamZoneColor(ZoneInfo[Capturable[10]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, ZONE_RHB, ReturnTeamZoneColor(ZoneInfo[ZONE_RHB][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[11], ReturnTeamZoneColor(ZoneInfo[Capturable[11]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, ZONE_LSV, ReturnTeamZoneColor(ZoneInfo[ZONE_LSV][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[12], ReturnTeamZoneColor(ZoneInfo[Capturable[12]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[13], ReturnTeamZoneColor(ZoneInfo[Capturable[13]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[14], ReturnTeamZoneColor(ZoneInfo[Capturable[14]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[15], ReturnTeamZoneColor(ZoneInfo[Capturable[15]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[16], ReturnTeamZoneColor(ZoneInfo[Capturable[16]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[17], ReturnTeamZoneColor(ZoneInfo[Capturable[17]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[18], ReturnTeamZoneColor(ZoneInfo[Capturable[18]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, ZONE_KTB, ReturnTeamZoneColor(ZoneInfo[ZONE_KTB][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, ZONE_TDB, ReturnTeamZoneColor(ZoneInfo[ZONE_TDB][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, ZONE_VLA, ReturnTeamZoneColor(ZoneInfo[ZONE_VLA][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[19], ReturnTeamZoneColor(ZoneInfo[Capturable[19]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[20], ReturnTeamZoneColor(ZoneInfo[Capturable[20]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[21], ReturnTeamZoneColor(ZoneInfo[Capturable[21]][zOwner]), 0x00000000);
	GangZoneShowForPlayer(playerid, Capturable[22], ReturnTeamZoneColor(ZoneInfo[Capturable[22]][zOwner]), 0x00000000);

    for(new i = 0; i < MAX_ZONES; i++)
	{
		if(ZoneInfo[i][zCapturedBy] != 0)
		{
			GangZoneFlashForPlayer(playerid, i, 0xFF634766);
		}
    }
	return 1;
}

GetPlayersInZone(zoneid, teamid)
{
    new count;
    foreach(new i : Player)
    {
        if(IsPlayerConnected(i) && PlayerInfo[i][Team] == teamid && GetPlayerGangZone(i) == zoneid)
        {
            count++;
        }
    }
    return count;
}

GetTDMPlayersCount()
{
    new result;
    foreach(new i : Player)
    {
        if(IsPlayerConnected(i) && PlayerInfo[i][IsLoggedIn] == true && GetPlayerGameMode(i) == GAME_MODE_TDM) result++;
    }
    return result;
}

GetTeamMembers(teamid)
{
    new result;
    foreach(new i : Player)
    {
        if(IsPlayerConnected(i) && PlayerInfo[i][IsLoggedIn] == true && GetPlayerGameMode(i) == GAME_MODE_TDM && PlayerInfo[i][Team] == teamid) result++;
    }
    return result;
}

/*ReturnLocation(playerid)
{
	new 
		playerLocation[MAX_ZONE_NAME]
	;
	
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
}*/