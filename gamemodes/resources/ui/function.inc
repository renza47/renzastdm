ShowPlayerBox(playerid, text[], time = 0)
{
    KillTimer(g_boxTimer[playerid]);

    PlayerTextDrawSetString(playerid, ui_box[playerid], text);
    PlayerTextDrawShow(playerid, ui_box[playerid]);
    if(time > 0) g_boxTimer[playerid] = SetTimerEx("HidePlayerBox", time, false, "i", playerid);
    return 1;
}

forward HidePlayerBox(playerid); 
public HidePlayerBox(playerid)
{
    if(playerid == INVALID_PLAYER_ID) return 0;

    PlayerTextDrawHide(playerid, ui_box[playerid]);
    return 1;
}

SendClientMessageEx(playerid, color, const str[], {Float,_}:...)
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

		SendClientMessage(playerid, color, string);

		#emit LCTRL 5
		#emit SCTRL 4
		#emit RETN
	}
	return SendClientMessage(playerid, color, str);
}

SendClientMessageToAllEx(color, const str[], {Float,_}:...)
{
	static
	    args,
	    start,
	    end,
	    string[144]
	;
	#emit LOAD.S.pri 8
	#emit STOR.pri args

	if (args > 8)
	{
		#emit ADDR.pri str
		#emit STOR.pri start

	    for (end = start + (args - 8); end > start; end -= 4)
		{
	        #emit LREF.pri end
	        #emit PUSH.pri
		}
		#emit PUSH.S str
		#emit PUSH.C 144
		#emit PUSH.C string

		#emit LOAD.pri args
		#emit ADD.C 4
		#emit PUSH.pri
		#emit SYSREQ.C format

        #emit LCTRL 5
		#emit SCTRL 4

		foreach (new i : Player) {
			SendClientMessage(i, color, string);
		}
		return 1;
	}
	return SendClientMessageToAll(color, str);
}

SendGameMessage(gamemode, color, const str[], {Float,_}:...)
{
	static
	    args,
	    start,
	    end,
	    ster[156]
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
		#emit PUSH.C ster
		#emit PUSH.C args
		#emit SYSREQ.C format


		foreach(new i : Player)
		{
			if(GetPlayerGameMode(i) == gamemode)
			{
				SendClientMessage(i, color, ster);
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
			if(GetPlayerGameMode(i) == gamemode)
			{
				SendClientMessage(i, color, str);
			}
		}
	}
	return 1;
}

forward SendLocalMessage(playerid, Float:radi, string[], color1, color2, color3, color4);
public SendLocalMessage(playerid, Float:radi, string[], color1, color2, color3, color4)
{
	new
		Float:currentPos[3], 
		Float:oldPos[3],
		Float:checkPos[3]
	;
		
	GetPlayerPos(playerid, oldPos[0], oldPos[1], oldPos[2]); 
	foreach (new i : Player)
	{		
		GetPlayerPos(i, currentPos[0], currentPos[1], currentPos[2]); 
		for (new p = 0; p < 3; p++)
		{
			checkPos[p] = (oldPos[p] - currentPos[p]);  
		}
		
		if (GetPlayerVirtualWorld(i) != GetPlayerVirtualWorld(playerid))
			continue;
			
		if (((checkPos[0] < radi/16) && (checkPos[0] > -radi/16)) && ((checkPos[1] < radi/16) && (checkPos[1] > -radi/16)) && ((checkPos[2] < radi/16) && (checkPos[2] > -radi/16)))
		{
			SendClientMessageEx(i, color1, string);
		}
		else if (((checkPos[0] < radi/8) && (checkPos[0] > -radi/8)) && ((checkPos[1] < radi/8) && (checkPos[1] > -radi/8)) && ((checkPos[2] < radi/8) && (checkPos[2] > -radi/8)))
		{
			SendClientMessageEx(i, color2, string);
		}
		else if (((checkPos[0] < radi/4) && (checkPos[0] > -radi/4)) && ((checkPos[1] < radi/4) && (checkPos[1] > -radi/4)) && ((checkPos[2] < radi/4) && (checkPos[2] > -radi/4)))
		{
			SendClientMessageEx(i, color3, string);
		}
		else if (((checkPos[0] < radi/2) && (checkPos[0] > -radi/2)) && ((checkPos[1] < radi/2) && (checkPos[1] > -radi/2)) && ((checkPos[2] < radi/2) && (checkPos[2] > -radi/2)))
		{
			SendClientMessageEx(i, color4, string);
		}	
	}
	return 1;
}

stock ReturnDate()
{
	new sendString[90], MonthStr[40], month, day, year;
	new hour, minute, second;
	
	gettime(hour, minute, second);
	getdate(year, month, day);
	switch(month)
	{
	    case 1:  MonthStr = "January";
	    case 2:  MonthStr = "February";
	    case 3:  MonthStr = "March";
	    case 4:  MonthStr = "April";
	    case 5:  MonthStr = "May";
	    case 6:  MonthStr = "June";
	    case 7:  MonthStr = "July";
	    case 8:  MonthStr = "August";
	    case 9:  MonthStr = "September";
	    case 10: MonthStr = "October";
	    case 11: MonthStr = "November";
	    case 12: MonthStr = "December";
	}
	
	format(sendString, 90, "%s %d, %d %02d:%02d:%02d", MonthStr, day, year, hour, minute, second);
	return sendString;
}

SendNearbyMessage(playerid, Float:radius, color, const str[], {Float,_}:...)
{
	static
	    args,
	    start,
	    end,
	    string[144]
	;
	#emit LOAD.S.pri 8
	#emit STOR.pri args

	if (args > 16)
	{
		#emit ADDR.pri str
		#emit STOR.pri start

	    for (end = start + (args - 16); end > start; end -= 4)
		{
	        #emit LREF.pri end
	        #emit PUSH.pri
		}
		#emit PUSH.S str
		#emit PUSH.C 144
		#emit PUSH.C string

		#emit LOAD.S.pri 8
		#emit CONST.alt 4
		#emit SUB
		#emit PUSH.pri

		#emit SYSREQ.C format
		#emit LCTRL 5
		#emit SCTRL 4

        foreach (new i : Player)
		{
			if (IsPlayerNearPlayer(i, playerid, radius)) {
  				SendClientMessage(i, color, string);
			}
		}
		return 1;
	}
	foreach (new i : Player)
	{
		if (IsPlayerNearPlayer(i, playerid, radius)) {
			SendClientMessage(i, color, str);
		}
	}
	return 1;
}

IsPlayerNearPlayer(playerid, targetid, Float:radius)
{
	static
		Float:fX,
		Float:fY,
		Float:fZ;

	GetPlayerPos(targetid, fX, fY, fZ);

	return (GetPlayerInterior(playerid) == GetPlayerInterior(targetid) && GetPlayerVirtualWorld(playerid) == GetPlayerVirtualWorld(targetid)) && IsPlayerInRangeOfPoint(playerid, radius, fX, fY, fZ);
}

randomEx(min, max)
{
    new randm = random(max-min)+min;
    return randm;
}

GetWeaponNameEx(weaponid)
{
	new weapname[32];
	switch(weaponid)
	{
		case 0: weapname = "N/A";
		case 22: weapname = "9MM";
		case 24: weapname = "Desert Eagle";
		case 25: weapname = "Shotgun";
		case 28: weapname = "UZI";
		case 29: weapname = "MP5";
		case 30: weapname = "AK-47";
		case 31: weapname = "M4";
		case 32: weapname = "TEC-9";
	}
	return weapname;
}