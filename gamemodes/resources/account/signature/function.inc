ShowPlayerStats(playerid, targetid)
{
    if(IsPlayerConnected(targetid))
    {
        SendClientMessageEx(playerid, COLOR_WHITE, "%s's statistics", PlayerInfo[targetid][Name]);
		SendClientMessage(playerid, COLOR_ORANGE, "==========================================================");
        SendClientMessageEx(playerid, COLOR_GREY, "| Account | Admin:[%d] Score:[%d] Kills/Deaths:[%d/%d] K/D Ratio:[%.1f]", PlayerInfo[targetid][AdminLevel], PlayerInfo[targetid][Score], PlayerInfo[targetid][Kills], PlayerInfo[targetid][Deaths], float(PlayerInfo[targetid][Kills]) / float(PlayerInfo[targetid][Deaths]));
		SendClientMessageEx(playerid, COLOR_GREY, "| Account | Rank:[%s] Rank Point:[%d/%d]", g_ScoreBasedRanks[CalculateRankFromScore(GetPlayerScore(targetid))][s_RankName], PlayerInfo[targetid][Score], g_ScoreBasedRanks[CalculateRankFromScore(GetPlayerScore(targetid)) + 1][i_Score]);
		SendClientMessageEx(playerid, COLOR_GREY, "| Account | Playing Hours:[%i hours, %i minutes, %i seconds]", PlayerInfo[targetid][PlayingHours][0], PlayerInfo[targetid][PlayingHours][1], PlayerInfo[targetid][PlayingHours][2]);
		if(PlayerInfo[targetid][Donator] && PlayerInfo[targetid][DonatorExpire] != 0) SendClientMessageEx(playerid, COLOR_GREY, "| Donator | Rank:[%s{AFAFAF}] Point:[%i] Expires in:[%i days]", ReturnDonorRank(targetid), PlayerInfo[targetid][DonatorPoint], ((PlayerInfo[targetid][DonatorExpire] - gettime()) / 84600));
		if(PlayerInfo[targetid][Donator] && PlayerInfo[targetid][DonatorExpire] == 0) SendClientMessageEx(playerid, COLOR_GREY, "| Donator | Rank:[%s{AFAFAF}] Point:[%i]", ReturnDonorRank(targetid), PlayerInfo[targetid][DonatorPoint]);
        SendClientMessageEx(playerid, COLOR_GREY, "| Session Stats | Kills/Deaths:[%.0f/%.0f] K/D Ratio:[%.1f]", PlayerInfo[targetid][SessionKills], PlayerInfo[targetid][SessionDeaths], (PlayerInfo[targetid][SessionKills] / PlayerInfo[targetid][SessionDeaths]));
		SendClientMessage(playerid, COLOR_ORANGE, "==========================================================");
    }
    return 1;
}

CalculateRankFromScore(score)
{
	if(score <= g_ScoreBasedRanks[0][i_Score])
	{
	    return 0;
	}
	else if(score >= g_ScoreBasedRanks[(sizeof(g_ScoreBasedRanks) - 1)][i_Score])
	{
	    return (sizeof(g_ScoreBasedRanks) - 1);
	}
	else
	{
		for(new i = 0, j = (sizeof(g_ScoreBasedRanks) - 1); i <= j; i++)
		{
			if(score < g_ScoreBasedRanks[i][i_Score])
			{
			    return (i - 1);
			}
		}
		return 0;
	}
}

//------------------------------------------------

ShowPlayerSignature(playerid, ofplayerid, expiretime = -1)
{
	PlayerTextDrawShow(playerid, PlayerInfo[playerid][i_Textdraw][1]);
	
	PlayerTextDrawSetString(playerid, PlayerInfo[playerid][i_Textdraw][2], PlayerInfo[ofplayerid][s_BackgroundSprite]);
	PlayerTextDrawColor(playerid, PlayerInfo[playerid][i_Textdraw][2], PlayerInfo[ofplayerid][i_BackgroundColor]);
	PlayerTextDrawShow(playerid, PlayerInfo[playerid][i_Textdraw][2]);

	PlayerTextDrawShow(playerid, PlayerInfo[playerid][i_Textdraw][3]);
	
	new
	    s_Name[MAX_PLAYER_NAME]
 	;
 	GetPlayerName(ofplayerid, s_Name, MAX_PLAYER_NAME);
	PlayerTextDrawSetString(playerid, PlayerInfo[playerid][i_Textdraw][4], s_Name);
	PlayerTextDrawColor(playerid, PlayerInfo[playerid][i_Textdraw][4], PlayerInfo[ofplayerid][i_NameColor]);
	PlayerTextDrawShow(playerid, PlayerInfo[playerid][i_Textdraw][4]);

	PlayerTextDrawShow(playerid, PlayerInfo[playerid][i_Textdraw][5]);
	
	PlayerTextDrawSetString(playerid, PlayerInfo[playerid][i_Textdraw][6], PlayerInfo[ofplayerid][s_Moto]);
	PlayerTextDrawColor(playerid, PlayerInfo[playerid][i_Textdraw][6], PlayerInfo[ofplayerid][i_MotoColor]);
	PlayerTextDrawShow(playerid, PlayerInfo[playerid][i_Textdraw][6]);

	new
		i_Rank = CalculateRankFromScore(GetPlayerScore(ofplayerid)),
		s_Str[100]
	;
	format(s_Str, sizeof(s_Str), "~w~~h~~h~%s ~w~~h~(~w~~h~~h~Rank Point: %i~w~~h~)", g_ScoreBasedRanks[i_Rank][s_RankName], GetPlayerScore(ofplayerid));
	PlayerTextDrawSetString(playerid, PlayerInfo[playerid][i_Textdraw][7], s_Str);
	PlayerTextDrawColor(playerid, PlayerInfo[playerid][i_Textdraw][7], COLOR_SIGNATURE_RANK);
	PlayerTextDrawShow(playerid, PlayerInfo[playerid][i_Textdraw][7]);
	
	PlayerTextDrawSetString(playerid, PlayerInfo[playerid][i_Textdraw][8], PlayerInfo[ofplayerid][s_AvatarSprite]);
	PlayerTextDrawColor(playerid, PlayerInfo[playerid][i_Textdraw][8], PlayerInfo[ofplayerid][i_AvatarColor]);
	PlayerTextDrawShow(playerid, PlayerInfo[playerid][i_Textdraw][8]);

	if(PlayerInfo[playerid][i_ExpireTimer] != -1)
	{
	    KillTimer(PlayerInfo[playerid][i_ExpireTimer]);
	    PlayerInfo[playerid][i_ExpireTimer] = -1;
	}
	if(expiretime != -1)
 	{
	 	PlayerInfo[playerid][i_ExpireTimer] = SetTimerEx("OnPlayerSignatureExpire", expiretime, false, "i", playerid);
	}
	
	return true;
}

//------------------------------------------------

HidePlayerSignature(playerid)
{
	return OnPlayerSignatureExpire(playerid);
}

//------------------------------------------------

forward OnPlayerSignatureExpire(playerid);
public OnPlayerSignatureExpire(playerid)
{
	for(new i; i < 10; i++)
	{
		PlayerTextDrawHide(playerid, PlayerInfo[playerid][i_Textdraw][i]);
	}
	
	return 1;
}

//------------------------------------------------

RGB(red, green, blue, alpha)
{
	return (red * 16777216) + (green * 65536) + (blue * 256) + alpha;
}

//------------------------------------------------

HexToInt(const string[])
{
  	if(! string[0])
	{
		return 0;
  	}

  	new cur = 1;
  	new res = 0;
  	for(new i = strlen(string); i > 0; i--)
  	{
    	if(string[i-1] < 58)
		{
			res = res + cur * (string[i-1] - 48);
		}
		else
		{
			res = res + cur * (string[i-1] - 65 + 10);
    	}
		cur = cur * 16;
	}
	return res;
}