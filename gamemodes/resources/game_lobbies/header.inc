#define MAX_LOBBIES (20)

enum G_LOBBIES
{
	bool:LobbyExists,
	LobbyCreator,
	
	LobbyName[64],
	LobbyVersion[64],
	LobbyPlayers,
	LobbyMaxPlayers,

	LobbyPassword[BCRYPT_HASH_LENGTH]
};
new LobbyInfo[MAX_LOBBIES][G_LOBBIES];