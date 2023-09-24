SendPlayerToFreeroam(playerid)
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
	SetPlayerArmor(playerid, 100.0);

	SetPlayerGameMode(playerid, GAME_MODE_FREEROAM);

    SetPlayerPos(playerid, 2489.1853,-1670.2133,13.3359); //ganton
    SetPlayerVirtualWorld(playerid, 100 + 256);
    SetPlayerInterior(playerid, 0);
	return 1;
}