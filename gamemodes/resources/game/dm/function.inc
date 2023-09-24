SendPlayerToDMArena(playerid, dmid)
{
	SetPlayerSkin(playerid, PlayerInfo[playerid][Model]);
	ResetPlayerWeapons(playerid);
	PlayerInfo[playerid][State] = PLAYER_STATE_ALIVE;
	PlayerInfo[playerid][DeathTime] = 0;
	PlayerInfo[playerid][DamageFix] = 0;

    ClearAnimations(playerid);

    PlayerInfo[playerid][KillStreak] = 0;

    PlayerInfo[playerid][MedkitTime] = 0;

    ClearPlayerDamages(playerid);

	SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, 0);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, 0);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_SPAS12_SHOTGUN, 0);

	SetPlayerHP(playerid, 100.0);
	SetPlayerArmor(playerid, 50.0);

	PlayerInfo[playerid][DM] = dmid;
	SetPlayerGameMode(playerid, GAME_MODE_DM);

	if(dmid == GAME_LVPD)
	{
		SetPlayerInterior(playerid,3);
        SetPlayerVirtualWorld(playerid, 18);

		new randSpawn = random(sizeof(gRandomSpawns_LVPD));
		SetPlayerPos(playerid,
		gRandomSpawns_LVPD[randSpawn][0],
		gRandomSpawns_LVPD[randSpawn][1],
		gRandomSpawns_LVPD[randSpawn][2]);
		SetPlayerFacingAngle(playerid,gRandomSpawns_LVPD[randSpawn][3]);

		GivePlayerWeapon(playerid, 24, 9999);
		GivePlayerWeapon(playerid, 25, 9999);
	}
	else if(dmid == GAME_WAREHOUSE)
	{
		SetPlayerInterior(playerid,1);
        SetPlayerVirtualWorld(playerid, 14);

		new randSpawn = random(sizeof(gRandomSpawns_Warehouse));
		SetPlayerPos(playerid,
		gRandomSpawns_Warehouse[randSpawn][0],
		gRandomSpawns_Warehouse[randSpawn][1],
		gRandomSpawns_Warehouse[randSpawn][2]);
		SetPlayerFacingAngle(playerid,gRandomSpawns_Warehouse[randSpawn][3]);

		GivePlayerWeapon(playerid, 24, 9999);
		GivePlayerWeapon(playerid, 25, 9999);
	}
	else if(dmid == GAME_WAREHOUSE2)
	{
		SetPlayerInterior(playerid,0);
        SetPlayerVirtualWorld(playerid, 15);

		new randSpawn = random(sizeof(gRandomSpawns_Warehouse2));
		SetPlayerPos(playerid,
		gRandomSpawns_Warehouse2[randSpawn][0],
		gRandomSpawns_Warehouse2[randSpawn][1],
		gRandomSpawns_Warehouse2[randSpawn][2]);
		SetPlayerFacingAngle(playerid,gRandomSpawns_Warehouse2[randSpawn][3]);

		GivePlayerWeapon(playerid, 24, 9999);
		GivePlayerWeapon(playerid, 25, 9999);
	}
	else if(dmid == GAME_RCBG)
	{
		SetPlayerInterior(playerid,10);
        SetPlayerVirtualWorld(playerid, 19);

		new randSpawn = random(sizeof(gRandomSpawns_Battleground));
		SetPlayerPos(playerid,
		gRandomSpawns_Battleground[randSpawn][0],
		gRandomSpawns_Battleground[randSpawn][1],
		gRandomSpawns_Battleground[randSpawn][2]);
		SetPlayerFacingAngle(playerid,gRandomSpawns_Battleground[randSpawn][3]);

		GivePlayerWeapon(playerid, 24, 9999);
		GivePlayerWeapon(playerid, 25, 9999);
		GivePlayerWeapon(playerid, 34, 9999);
	}
	else if(dmid == GAME_GHOST_TOWN)
	{
		SetPlayerInterior(playerid,0);
        SetPlayerVirtualWorld(playerid, 20);

		new randSpawn = random(sizeof(gRandomSpawns_GhostTown));
		SetPlayerPos(playerid,
		gRandomSpawns_GhostTown[randSpawn][0],
		gRandomSpawns_GhostTown[randSpawn][1],
		gRandomSpawns_GhostTown[randSpawn][2]);
		SetPlayerFacingAngle(playerid,gRandomSpawns_GhostTown[randSpawn][3]);

		GivePlayerWeapon(playerid, 24, 9999);
	}
	return 1;
}