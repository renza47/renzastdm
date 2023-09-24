#define MAX_REPORTS 1000

enum E_PLAYERS
{
	ID,
	Name[MAX_PLAYER_NAME],
	Password[BCRYPT_HASH_LENGTH], 
    Salt[17],
	Kills,
	Deaths,
	Float: X_Pos,
	Float: Y_Pos,
	Float: Z_Pos,
	Float: A_Pos,
	Interior,

	Cache: Cache_ID,
	bool: IsLoggedIn,
	LoginAttempts,
	LoginTimer[2],

	Team,
	bool:SelectingTeam,

	Skin,

	AdminLevel,
	bool:AdminDuty,

	State,

	DamageFix,
	DeathTime,
	LastDamage,

	Score,
	Cash,

	Reviving,
	RevivingTime,

	Robbing,
	RobTime,

	Weapon[2], // 0 = heavy weapon 1 = shotgun 2 = pisol

	HackWarning[20],

	KillStreak,

	Spectating,

	Medkit,
	MedkitTime,

	GangSkin[11], // index 0 engga dipake ya ges ya

    RecentlySpawned,

	bool:BrokenLeg,

	GoldenGunAmmo,

	Float:Health,
	Float:Armour,

	ChatDelay,

	CbugCheck,

	AdminJail,

	GameMode,

	Model,

	DM,

	HitSound,
	HitMarker,

	Float:SessionKills,
	Float:SessionDeaths,

	// Header

	DuelID,
	bool:DuelStarted,
	DuelMap,
	DuelWeapon1,
	DuelWeapon2,
	Float:DuelHealth,
	Float:DuelArmour,

	// Signature

	i_ExpireTimer,
	i_NameColor,
	i_BackgroundColor,
	i_AvatarColor,
	i_MotoColor,
	PlayerText:i_Textdraw[10],
	bool:b_Toggled,
	s_BackgroundSprite[25],
	s_AvatarSprite[25],
	s_Moto[50],

    Text3D:Nametag,

	PauseCheck,

	CC_Team,

	NoCopchase,

	CopSkin,
	CopCar,

	FugCar,

	Property,

	EnterExitDelay,

	ObjectLoad,

	AntiCheatPause,

	bool:Taser,
	Tased,
	CC_Weapon[12],
	CC_WeaponAmmo[12],

	CuffTime,
	CuffID,

	DragTime,
	DragID,

	Actor,

	Tog[4],

    Traced,

	Vote,

	Muted,
	MuteTime,

	FreeroamVehicle,

	Lobby,

	bool: Hidden,

	StaffActivity[3],
	PlayingHours[3],

	Perk[3],

	SelfRevive,

	FixVeh,
	FlipVeh,

	Mask,

	MedkitRevive,

	Donator,
	DonatorPoint,
	DonatorExpire,
	DonatorTag[32]
};
new PlayerInfo[MAX_PLAYERS][E_PLAYERS];

enum E_REPORTS
{
	rID,
	rIssuer,
	rReason[128],
	rDate[90]
};
new ReportInfo[MAX_REPORTS][E_REPORTS];

new HitMarkerTimer[MAX_PLAYERS];

#define PLAYER_STATE_ALIVE 0
#define PLAYER_STATE_WOUNDED 1

#define GAME_MODE_LOBBY            0
#define GAME_MODE_TDM              1
#define GAME_MODE_DM               2
#define GAME_MODE_DUEL             3
#define GAME_MODE_COPCHASE         4
#define GAME_MODE_FREEROAM         5

new familiesSkin[] = {
	105, 106, 107, 269, 270, 271, 293, 4, 20075, 20076, 20077, 20008, 20009, 20010, 18, 19, 22
};

new ballasSkin[] = {
	102, 103, 104, 20005, 20006, 20007, 18, 19, 21, 22, 28
};

new vagosSkin[] = {
	108, 109, 110, 20011, 20012, 20013, 20029
};

new varriosSkin[] = {
	114, 115, 116, 292, 20014, 20015, 20016, 20029
};

new lspdSkin[] = {
	280, 281, 265, 266, 267, 285, 311, 20017, 20018, 306, 309, 20019, 20020, 20021, 20022, 20023, 20025, 20035, 20036, 20037
};

new gSAMPSkins[] = {
	20008, 20009, 20010, 20026, 20027, 20028, 20030, 20031, 20032, 20033, 
	20060, 20061, 20062, 20064, 20065, 20066, 20067, 20068, 20069, 20070, 20071, 20072,
	20073,  20074, 20042, 20043, 20044, 20045, 20046, 20047, 20048, 20049, 20050, 20051, 20052, 20053, 20054,
	20055, 20056, 20057, 20058, 20059, 20038, 20039,
	1, 2, 3, 4, 5, 6, 7, 8, 14, 15, 16, 17, 18, 19, 
	20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 32, 33, 34, 35, 36, 37, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 57, 58, 59, 60,
	61, 62, 66, 68, 72, 73, 78, 79, 80, 81, 82, 83, 84, 94, 95, 96, 97, 98, 99, 100, 101, 102,
	103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
	121, 122, 123, 124, 125, 126, 127, 128, 132, 133, 134, 135, 136, 137, 142, 143, 144, 146,
	147, 153, 154, 155, 156, 158, 159, 160, 161, 162, 167, 168, 170, 171, 173, 174, 175, 176,
	177, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 200, 202, 203, 204, 206,
	208, 209, 210, 212, 213, 217, 220, 221, 222, 223, 228, 229, 230, 234, 235, 236, 239, 240,
	241, 242, 247, 248, 249, 250, 253, 254, 255, 258, 259, 260, 261, 262, 268, 272, 273, 289,
	290, 291, 292, 293, 294, 295, 296, 297, 299, 10, 11, 12, 13, 31, 38, 39, 40, 41, 53, 54,
	55, 56, 63, 64, 65, 69, 75, 76, 77, 85, 88, 89, 90, 91, 92, 93, 129, 130, 131, 138, 140, 141,
    145, 148, 150, 151, 152, 157, 169, 178, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 201, 
	205, 207, 211, 214, 215, 216, 219, 224, 225,226, 231, 232, 233, 237, 238, 243, 244, 245, 246, 251, 
	256, 257, 263, 298
};