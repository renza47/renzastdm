ShowPlayerLobbyDialog(playerid)
{
	new str[1048], sub_str[1048];
	format(sub_str, sizeof sub_str, "#\tLobby\tGame Version\tPlayers\n{FFFFFF}");
	strcat(str, sub_str);
	format(sub_str, sizeof sub_str, "{0D7304}View Private Lobbies\n{FFFFFF}");
	strcat(str, sub_str);
	for(new i; i < MAX_LOBBIES; i++)
	{
		if(LobbyInfo[i][LobbyExists])
		{
			if(LobbyInfo[i][LobbyPlayers] >= LobbyInfo[i][LobbyMaxPlayers])
			{
				format(sub_str, sizeof sub_str, "{FFFFFF}%i\t%s\t%s\t{FF6347}%i/%i{FFFFFF}\n", i, LobbyInfo[i][LobbyName], LobbyInfo[i][LobbyVersion], LobbyInfo[i][LobbyPlayers], LobbyInfo[i][LobbyMaxPlayers]);
	            strcat(str, sub_str);
			}
			else
			{
				format(sub_str, sizeof sub_str, "{FFFFFF}%i\t%s\t%s\t{0D7304}%i/%i{FFFFFF}\n", i, LobbyInfo[i][LobbyName], LobbyInfo[i][LobbyVersion], LobbyInfo[i][LobbyPlayers], LobbyInfo[i][LobbyMaxPlayers]);
	            strcat(str, sub_str);
			}
		}
	}
	ShowPlayerDialog(playerid, DIALOG_LOBBIES, DIALOG_STYLE_TABLIST_HEADERS, "Game Lobbies", str, "Select", "Abort");
	return 1;
}

ShowPlayerPrivateLobbyDialog(playerid)
{
	new str[1048], sub_str[1048];
	format(sub_str, sizeof sub_str, "#\tLobby\tGame Version\tPlayers\n{FFFFFF}");
	strcat(str, sub_str);

	format(sub_str, sizeof sub_str, "{0D7304}Create your own lobby\n{FFFFFF}");
	strcat(str, sub_str);
	for(new i; i < MAX_LOBBIES; i++)
	{
		if(LobbyInfo[i][LobbyExists] && LobbyInfo[i][LobbyCreator] != -1)
		{
			if(LobbyInfo[i][LobbyPlayers] >= LobbyInfo[i][LobbyMaxPlayers])
			{
				format(sub_str, sizeof sub_str, "{FFFFFF}%i\t%s\t%s\t{FF6347}%i/%i{FFFFFF}\n", i, LobbyInfo[i][LobbyName], LobbyInfo[i][LobbyVersion], LobbyInfo[i][LobbyPlayers], LobbyInfo[i][LobbyMaxPlayers]);
	            strcat(str, sub_str);
			}
			else
			{
				format(sub_str, sizeof sub_str, "{FFFFFF}%i\t%s\t%s\t{0D7304}%i/%i{FFFFFF}\n", i, LobbyInfo[i][LobbyName], LobbyInfo[i][LobbyVersion], LobbyInfo[i][LobbyPlayers], LobbyInfo[i][LobbyMaxPlayers]);
	            strcat(str, sub_str);
			}
		}
	}
	ShowPlayerDialog(playerid, DIALOG_PRIVATE_LOBBIES, DIALOG_STYLE_TABLIST_HEADERS, "Private Lobbies", str, "Select", "<<");
	return 1;
}

CreateGameLobby(const name[], const version[], maxplayers)
{
    new id = -1;
    for(new i; i < MAX_LOBBIES; i++)
    {
        if(LobbyInfo[i][LobbyExists] == false)
        {
            id = i;
            break;
        }
    }

    LobbyInfo[id][LobbyExists] = true;
    format(LobbyInfo[id][LobbyName], 64, name);
    format(LobbyInfo[id][LobbyVersion], 64, version);
    LobbyInfo[id][LobbyPlayers] = 0;
    LobbyInfo[id][LobbyMaxPlayers] = maxplayers;
	LobbyInfo[id][LobbyCreator] = -1;
	format(LobbyInfo[id][LobbyPassword], BCRYPT_HASH_LENGTH, " ");
    return id;
}

CreatePrivateLobby(playerid, const name[], const version[], const password[], maxplayers)
{
    new id = -1;
    for(new i; i < MAX_LOBBIES; i++)
    {
        if(LobbyInfo[i][LobbyExists] == false)
        {
            id = i;
            break;
        }
    }

    LobbyInfo[id][LobbyExists] = true;
	LobbyInfo[id][LobbyCreator] = PlayerInfo[playerid][ID];

    format(LobbyInfo[id][LobbyName], 64, name);
    format(LobbyInfo[id][LobbyVersion], 64, version);
    LobbyInfo[id][LobbyPlayers] = 0;
    LobbyInfo[id][LobbyMaxPlayers] = maxplayers;

	if(!isnull(password)) bcrypt_hash(playerid, "OnLobbyPasswordHashed", password, BCRYPT_COST, "i", id);
	else if(isnull(password)) format(LobbyInfo[id][LobbyPassword], BCRYPT_HASH_LENGTH, " ");
    return id;
}

forward OnLobbyPasswordHashed(lobbyid);
public OnLobbyPasswordHashed(lobbyid)
{
	bcrypt_get_hash(LobbyInfo[lobbyid][LobbyPassword], BCRYPT_HASH_LENGTH);
	return 1;
}

DeleteLobby(lobbyid)
{
    if(LobbyInfo[lobbyid][LobbyExists] == false) return -1;

    LobbyInfo[lobbyid][LobbyExists] = false;
    return 1;
}