forward Duel_Countdown(playerid);
public Duel_Countdown(playerid)
{
	switch(DuelCountdown[playerid])
	{
	    case 0:
	    {
	    	GameTextForPlayer(playerid, "~r~START!", 2000, 5),PlayerPlaySound(playerid,1057,0.0,0.0,0.0);
            KillTimer(DuelTimer[playerid]);
            TogglePlayerControllable(playerid, true);
            DuelInfo[playerid][DuelTick] = GetTickCount();
            DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelTick] = GetTickCount();
	    }
	    case 1: GameTextForPlayer(playerid, "~r~1", 1000, 5),PlayerPlaySound(playerid,1056,0.0,0.0,0.0);
	    case 2: GameTextForPlayer(playerid, "~r~2", 1000, 5),PlayerPlaySound(playerid,1056,0.0,0.0,0.0);
	    case 3: GameTextForPlayer(playerid, "~r~3", 1000, 5),PlayerPlaySound(playerid,1056,0.0,0.0,0.0);
	    case 4: GameTextForPlayer(playerid, "~r~4", 1000, 5),PlayerPlaySound(playerid,1056,0.0,0.0,0.0);
	    case 5: GameTextForPlayer(playerid, "~r~~5", 1000, 5),PlayerPlaySound(playerid,1056,0.0,0.0,0.0);
	    case 6: GameTextForPlayer(playerid, "~r~GET READY", 1000, 5);
	}
	DuelCountdown[playerid]--;
	return 1;
}

ReturnMapName(mapid)
{
	new mapstr[56];
	switch(mapid)
	{
	    case 0: mapstr = "LVPD";
	    case 1: mapstr = "Warehouse";
      	case 2: mapstr = "RC Battlefield";
	}
	return mapstr;
}