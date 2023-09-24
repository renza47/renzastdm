
/*
                    	    Renza's Team Deathmatch
						San Andreas Multiplayer | 0.3.DL open.mp server
--------------------------------------------------------------------------------
Gamemode version: 1.4.0

*/

#include 	<a_samp>
#include    <a_http>

#undef	  	MAX_PLAYERS
#define	 	MAX_PLAYERS			64

#pragma warning disable 239

#include 	<a_mysql>
#include    <streamer>
#include    <sscanf2>
#include    <samp_bcrypt>
#include    <Pawn.CMD>

#include 	<a_gametext>

#include    <eSelection>
#include    <YSI_Data\y_foreach>
#include    <gangzone>
#include    <YSI_Coding\y_hooks>
#include    "../include/gl_common.inc"
#include    <anticheat>
#include    <callbacks>
#include    <omp>
#include    <actor_robbery>
#include    <gmenu>
#include    <garageblocker>

#define DB_SERVER 0
#define DB_LOCAL 1

#define SERVER_DB DB_SERVER

#if SERVER_DB == DB_SERVER
	#define		MYSQL_HOST 			""
	#define		MYSQL_USER 			""
	#define		MYSQL_PASSWORD 		"*"
	#define		MYSQL_DATABASE 		""
#elseif SERVER_DB == DB_LOCAL
	#define		MYSQL_HOST 			""
	#define		MYSQL_USER 			""
	#define		MYSQL_PASSWORD 		""
	#define		MYSQL_DATABASE 		""
#endif

#define		SECONDS_TO_LOGIN 	300

#define 	DEFAULT_POS_X 		0.0
#define 	DEFAULT_POS_Y 		0.0
#define 	DEFAULT_POS_Z 		0.0
#define 	DEFAULT_POS_A 		0.0

#define     SERVER_VERSION      "1.4.0"

new MySQL: g_SQL;

#define OnPlayerEnterDA OnPlayerEnterDynamicArea
#define OnPlayerExitDA OnPlayerLeaveDynamicArea

#define SendUsageMessage(%0,%1) \
	SendClientMessageEx(%0, 0xff7400FF, "USAGE: "%1) 
	
#define SendErrorMessage(%0,%1) \
	SendClientMessageEx(%0, 0xe06666FF, ""%1)

#define SendErrorPermMessage(%0) \
	SendClientMessageEx(%0, 0xe06666FF, "You don't have the right permission to use this command.")

#define PRESSED(%0) \
	(((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))

#define HOLDING(%0) \
	((newkeys & (%0)) == (%0))

#define RELEASED(%0) \
	(((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))
	
new g_MysqlRaceCheck[MAX_PLAYERS];

new bool:WeaponBalancing = false;
new bool:GlobalChat = false;

new Text3D:LeaderboardText;
new LeaderboardActor;

new Text3D:ArenaText;
new Text3D:CopchaseText;
new Text3D:TDMText;

native IsValidVehicle(vehicleid);

enum E_SERVER
{
	Minute,
	Hour,
	Day
};
new ServerInfo[E_SERVER];

enum G_VOTEKICK
{
	VoteID,
	VoteIssuer,
	VoteAmount,
	VoteMaxAmount,
	VoteReason[128],
	VoteTime,
	VoteCooldown
};
new VoteInfo[G_VOTEKICK];

// dialog data
enum
{
	DIALOG_UNUSED,

	DIALOG_LOGIN,
	DIALOG_REGISTER,

	//TDM

	DIALOG_WEAPON,
	DIALOG_WEAPON1,
	DIALOG_WEAPON2,

	DIALOG_SKIN,

	DIALOG_WEAPON_SWITCH,

	// DM arena

	DIALOG_ARENA,

	// Duel zone

	DIALOG_DUEL_WEAPON,
	DIALOG_DUEL_WEAPON2,
	DIALOG_DUEL_MAP,
	DIALOG_DUEL,

	DIALOG_VWEAPON,

	DIALOG_HUD,

	DIALOG_EDIT_BONE,

	DIALOG_LOBBIES,
	DIALOG_PRIVATE_LOBBIES,

	DIALOG_PERKS
};

enum
{
	MENU_UNUSED,

	MENU_VEHICLE_TRUNK
};

enum
{
	MODEL_SELECTION_SKIN,
	MODEL_SELECTION_SAMP_SKIN,
	MODEL_SELECTION_COP_SKIN,
	MODEL_SELECTION_COP_VEHICLE,
	MODEL_SELECTION_FUG_VEHICLE
};

// timers
new PlayerUpdateTimer;
new VehicleUpdateTimer;
new ServerUpdateTimer;
new LeaderboardUpdateTimer;
new CopchaseTimer[3];

enum WeaponDamageInfo
{
	WeaponID,
	Float:WepDamage,
};

new WeaponDamage[][WeaponDamageInfo] =
{
	// WeaponID		WeaponDamage
	{0,				0.0},	//Unarmed
	{1,				15.0},	//Brass Knuckles
	{2,				15.0},	//Golf Club
	{3,				20.0},	//Nite Stick
	{4,				30.0},	//Knife
	{5,				10.0},	//Baseball Bat
	{6,				15.0},	//Shovel
	{7,				10.0},	//Pool Cue
	{8,				35.0},	//Katana
	{9,				0.0},	//Chainsaw
	{10,			1.0},	//Purple Dildo
	{12,			1.0},	//Large White Vibrator
	{11,			1.0},	//Small White Vibrator
	{13,			1.0},	//Silver Vibrator
	{14,			0.0},	//Flowers
	{15,			7.0},	//Cane
	{16,			0.0},	//Grenade
	{17,			0.0},	//Tear Gas
	{18,			10.0},	//Molotov Coctail
	{19,			0.0},	//Invalid Weapon
	{20,			0.0},	//Invalid Weapon
	{11,			0.0},	//Invalid Weapon
	{22,			20.0},	//Colt 9mm
	{23,			0.0},	//Silenced Colt 9mm
	{24,			47.5},	//Desert Eagle
	{25,			25.0},	//Shotgun
	{26,			0.0},	//Sawn-off Shotgun
	{27,			0.0},	//Combat Shotgun
	{28,			17.5},	//Micro SMG
	{29,			20.0},	//MP5
	{30,			27.5},	//AK-47
	{31,			23.5},	//M4
	{32,			17.5},	//Tec9
	{33,			65.0},	//Country Rifle
	{34,			85.0},	//Sniper Rifle
	{35,			0.0},	//Rocket Launcher
	{36,			0.0},	//HS Rocket Launcher
	{37,			0.0},	//Flamethrower
	{38,			0.0},	//Minigun
	{39,			0.0},	//Satchel Charge
	{40,			2.0},	//Satchel Detonator
	{41,			0.0},	//Spraycan
	{42,			0.0},	//Fire Extinguisher
	{43,			0.0},	//Camera
	{44,			0.0},	//Nightvision Goggles
	{45,			0.0},	//Thermal Goggles
	{46,			0.0},	//Thermal Goggles
	{47,			0.0},	//Fake Pistol
	{48,			0.0},	//Invalid Weapon
	{49,			15.0},	//Vehicle
	{50,			0.0},	//Heli-Blades
	{51,			100.0},	//Explosion
	{52,			0.0},	//Invalid Weapon
	{53,			5.0},	//Drowned
	{54,			0.0}	//Splat
};

// resources
#include    "resources/color.inc"

#include    "resources/ui.inc"

//#include    "resources/game_lobbies.inc"

#include    "resources/account.inc"

#include    "resources/attachment.inc"

#include    "resources/property.inc"

#include    "resources/game.inc"

#include    "resources/vehicle.inc"

#include    "resources/pvp.inc"

#include    "resources/command.inc"

#include    "resources/map.inc"

#include    "resources/animations.inc"

main() {}

public OnGameModeInit()
{	
	new InitializingTime = GetTickCount();

    //NOTE: I don't include the custom models since I don't own them, so please delete line 203-370
    // skins
	AddCharModel(292, 20001, "revaldo.dff", "revaldo.txd");
	AddCharModel(292, 20002, "roblox.dff", "roblox.txd");
	AddCharModel(292, 20003, "skinff.dff", "skinff.txd");
	AddCharModel(292, 20004, "pocong.dff", "pocong.txd");

	AddCharModel(102, 20005, "betaballer1.dff", "betaballer1.txd");
	AddCharModel(104, 20006, "betaballer2.dff", "betaballer2.txd");
	AddCharModel(103, 20007, "betaballer3.dff", "betaballer3.txd");
	//gsf
	AddCharModel(106, 20008, "betafam3.dff", "betafam3.txd");
	AddCharModel(105, 20009, "betafam5.dff", "betafam5.txd");
	AddCharModel(107, 20010, "betafam6.dff", "betafam6.txd");
	AddCharModel(106, 20075, "FeeG.dff", "FeeG.txd");
	AddCharModel(106, 20076, "Krizpy.dff", "Krizpy.txd");
	AddCharModel(107, 20077, "renza.dff", "renza.txd");
	//lsv
	AddCharModel(109, 20011, "betaLSV1.dff", "betaLSV1.txd");
	AddCharModel(108, 20012, "betalsv3.dff", "betalsv3.txd");
	AddCharModel(110, 20013, "betaLSV2.dff", "betaLSV2.txd");
	//vla
	AddCharModel(116, 20014, "betavla1.dff", "betavla1.txd");
	AddCharModel(115, 20015, "betavla4.dff", "betavla4.txd");
	AddCharModel(116, 20016, "betavla5.dff", "betavla5.txd");
	//lspd
	AddCharModel(280, 20017, "lspd1.dff", "lspd1.txd");
	AddCharModel(280, 20018, "lspd2.dff", "lspd2.txd");
	AddCharModel(280, 20019, "lssd1.dff", "lssd1.txd");
	AddCharModel(280, 20020, "lssd2.dff", "lssd2.txd");
	AddCharModel(280, 20021, "lssd3.dff", "lssd3.txd");
	AddCharModel(280, 20022, "lssd4.dff", "lssd4.txd");
	AddCharModel(211, 20023, "lssd5.dff", "lssd5.txd");
	AddCharModel(211, 20025, "lssd7.dff", "lssd7.txd");

	// custom skin
	AddCharModel(106, 20026, "avondre2.dff", "avondre2.txd");
	AddCharModel(22, 20027, "w_tshirt.dff", "w_tshirt.txd");
	AddCharModel(22, 20028, "jizzy.dff", "jizzy.txd");
	AddCharModel(116, 20029, "h_w_toptank.dff", "h_w_toptank.txd");
	AddCharModel(22, 20030, "Sharif_Paris.dff", "Sharif_Paris.txd");
	AddCharModel(22, 20031, "MonclerBleu.dff", "MonclerBleu.txd");
	AddCharModel(22, 20032, "jacket.dff", "jacket.txd");
	AddCharModel(22, 20033, "ktsrobb.dff", "ktsrobb.txd");
	AddCharModel(2, 20034, "rivq.dff", "rivq.txd");

	AddCharModel(280, 20035, "vc_cop.dff", "vc_cop.txd");
	AddCharModel(280, 20036, "III_cop.dff", "III_cop.txd");
	AddCharModel(280, 20037, "lcs_cop.dff", "lcs_cop.txd");

	AddCharModel(2, 20038, "III_criminal01.dff", "III_criminal01.txd");
	AddCharModel(2, 20039, "III_criminal02.dff", "III_criminal02.txd");

	AddCharModel(2, 20040, "lcs_criminal01.dff", "lcs_criminal01.txd");
	AddCharModel(2, 20041, "lcs_criminal02.dff", "lcs_criminal02.txd");

	AddCharModel(106, 20042, "lcs_gang01.dff", "lcs_gang01.txd");
	AddCharModel(106, 20043, "lcs_gang02.dff", "lcs_gang02.txd");
	AddCharModel(106, 20044, "lcs_gang03.dff", "lcs_gang03.txd");
	AddCharModel(106, 20045, "lcs_gang04.dff", "lcs_gang04.txd");
	AddCharModel(106, 20046, "lcs_gang05.dff", "lcs_gang05.txd");
	AddCharModel(106, 20047, "lcs_gang06.dff", "lcs_gang06.txd");
	AddCharModel(106, 20048, "lcs_gang07.dff", "lcs_gang07.txd");
	AddCharModel(106, 20049, "lcs_gang08.dff", "lcs_gang08.txd");
	AddCharModel(106, 20050, "lcs_gang09.dff", "lcs_gang09.txd");
	AddCharModel(106, 20051, "lcs_gang10.dff", "lcs_gang10.txd");
	AddCharModel(106, 20052, "lcs_gang11.dff", "lcs_gang11.txd");
	AddCharModel(106, 20053, "lcs_gang12.dff", "lcs_gang12.txd");
	AddCharModel(106, 20054, "lcs_gang13.dff", "lcs_gang13.txd");
	AddCharModel(106, 20055, "lcs_gang14.dff", "lcs_gang14.txd");
	AddCharModel(106, 20056, "lcs_gang15.dff", "lcs_gang15.txd");
	AddCharModel(106, 20057, "lcs_gang16.dff", "lcs_gang16.txd");
	AddCharModel(106, 20058, "lcs_gang17.dff", "lcs_gang17.txd");
	AddCharModel(106, 20059, "lcs_gang18.dff", "lcs_gang18.txd");

	AddCharModel(106, 20060, "III_gang01.dff", "III_gang01.txd");
	AddCharModel(106, 20061, "III_gang02.dff", "III_gang02.txd");
	AddCharModel(106, 20062, "III_gang03.dff", "III_gang03.txd");
	AddCharModel(106, 20064, "III_gang04.dff", "III_gang04.txd");
	AddCharModel(106, 20065, "III_gang05.dff", "III_gang05.txd");
	AddCharModel(106, 20066, "III_gang06.dff", "III_gang06.txd");
	AddCharModel(106, 20067, "III_gang07.dff", "III_gang07.txd");
	AddCharModel(106, 20068, "III_gang08.dff", "III_gang08.txd");
	AddCharModel(106, 20069, "III_gang09.dff", "III_gang09.txd");
	AddCharModel(106, 20070, "III_gang10.dff", "III_gang10.txd");
	AddCharModel(106, 20071, "III_gang11.dff", "III_gang11.txd");
	AddCharModel(106, 20072, "III_gang12.dff", "III_gang12.txd");
	AddCharModel(106, 20073, "III_gang13.dff", "III_gang13.txd");
	AddCharModel(106, 20074, "III_gang14.dff", "III_gang14.txd");

	new veh, actor;
	veh = CreateVehicle(596, 2487.7205,-1728.5927,13.1018,89.7753, 0, 1, 0, 1);
	SetVehicleVirtualWorld(veh, 7000);

	veh = CreateVehicle(420, 2477.2942,-1728.4729,13.1622,90.0351, 6, 6, 0, 1);
	SetVehicleVirtualWorld(veh, 7000);

	veh = CreateVehicle(492,2462.1541,-1661.3224,13.0862,341.6649, 7, 1, 0, 1);
	SetVehicleVirtualWorld(veh, 7000);

	veh = CreateVehicle(412,2463.0034,-1655.9059,13.1403,58.4659, 7, 7, 0, 1);
	SetVehicleVirtualWorld(veh, 7000);

	veh = CreateVehicle(566,2441.6653,-1661.8779,13.1150,296.6581, 56, 56, 0, 1);
	SetVehicleVirtualWorld(veh, 7000);

	veh = CreateVehicle(566,2439.2002,-1656.3134,13.1288,205.7942, 56, 56, 0, 1);
	SetVehicleVirtualWorld(veh, 7000);
	
    actor = CreateDynamicActor(265, 2487.9966,-1730.2977,13.3828,79.3702, 1, 100.0,  7000);
	ApplyDynamicActorAnimation(actor, "SHOP", "SHP_Gun_Aim", 4.1, 0, 1, 1, 1, 0);

	actor = CreateDynamicActor(266, 2487.7769,-1726.8719,13.5469,93.5372, 1, 100.0,  7000);
	ApplyDynamicActorAnimation(actor, "SHOP", "SHP_Gun_Aim", 4.1, 0, 1, 1, 1, 0);

	actor = CreateDynamicActor(0, 2481.0906,-1728.4080,13.3828,91.6529, 1, 100.0,  7000);
	ApplyDynamicActorAnimation(actor, "SHOP", "SHP_Rob_HandsUp", 4.1, 0, 1, 1, 1, 0);


	actor = CreateDynamicActor(20009, 2464.1650,-1660.7711,13.3091,78.3646, 1, 100.0,  7000);
	ApplyDynamicActorAnimation(actor, "PYTHON", "python_crouchfire", 4.1, 0, 1, 1, 1, 0);

	actor = CreateDynamicActor(106, 2463.2004,-1663.7327,13.3047,79.9021, 1, 100.0,  7000);
	ApplyDynamicActorAnimation(actor, "SHOP", "SHP_Gun_Aim", 4.1, 0, 1, 1, 1, 0);

	actor = CreateDynamicActor(20010, 2463.5291,-1654.2349,13.3047,97.4831, 1, 100.0,  7000);
	ApplyDynamicActorAnimation(actor, "PYTHON", "python_crouchreload", 4.1, 0, 1, 1, 1, 0);

	actor = CreateDynamicActor(107, 2465.7249,-1655.7673,13.2934,95.7920, 1, 100.0,  7000);
	ApplyDynamicActorAnimation(actor, "SHOP", "SHP_Gun_Aim", 4.1, 0, 1, 1, 1, 0);

	actor = CreateDynamicActor(20007, 2439.2712,-1661.3621,13.3442,277.7109, 1, 100.0,  7000);
	ApplyDynamicActorAnimation(actor, "SHOP", "SHP_Gun_Aim", 4.1, 0, 1, 1, 1, 0);

	actor = CreateDynamicActor(102, 2438.4275,-1663.7355,13.3526,271.5999, 1, 100.0,  7000);
	ApplyDynamicActorAnimation(actor, "PYTHON", "python_crouchfire", 4.1, 0, 1, 1, 1, 0);

	actor = CreateDynamicActor(104, 2436.3628,-1653.9188,13.5206,268.5824, 1, 100.0,  7000);
	ApplyDynamicActorAnimation(actor, "PYTHON", "python_crouchfire", 4.1, 0, 1, 1, 1, 0);

	actor = CreateDynamicActor(20006, 2438.1584,-1657.6045,13.3553,266.5069, 1, 100.0,  7000);
	ApplyDynamicActorAnimation(actor, "PYTHON", "python_crouchfire", 4.1, 0, 1, 1, 1, 0);


	actor = CreateDynamicActor(28, 2285.5872,-1645.2002,15.1403,133.2250, 1, 100.0,  7000);
	ApplyDynamicActorAnimation(actor, "PARACHUTE", "FALL_skyDive_DIE", 4.1, 0, 1, 1, 1, 0);

	actor = CreateDynamicActor(271, 2284.2849,-1647.4507,15.1012,323.7913, 1, 100.0,  7000);

	actor = CreateDynamicActor(0,2283.4165,-1646.2390,15.1514,300.2290, 1, 100.0,  7000);

  
    actor = CreateDynamicActor(105, 2307.4314,-1716.8354,14.6496,183.5248, 1, 100.0,  7000);
	ApplyDynamicActorAnimation(actor, "BEACH", "ParkSit_M_loop", 4.1, 0, 1, 1, 1, 0);

	actor = CreateDynamicActor(106, 2308.1743,-1719.3240,14.3281,14.8632, 1, 100.0,  7000);
	ApplyDynamicActorAnimation(actor, "PED","IDLE_CHAT", 4.1, 0, 1, 1, 1, 0);

	actor = CreateDynamicActor(107, 2305.1926,-1716.6707,14.6496,272.2776, 1, 100.0,  7000);
	ApplyDynamicActorAnimation(actor, "GANGS","leanIDLE", 4.1, 0, 1, 1, 1, 0);

	new MySQLOpt: option_id = mysql_init_options();

	mysql_set_option(option_id, AUTO_RECONNECT, true); // it automatically reconnects when loosing connection to mysql server

	g_SQL = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DATABASE, option_id); // AUTO_RECONNECT is enabled for this connection handle only
	if (g_SQL == MYSQL_INVALID_HANDLE || mysql_errno(g_SQL) != 0)
	{
		printf("(Error ID: #%d) MySQL connection failed. Server is shutting down.", mysql_errno(g_SQL));
		SendRconCommand("exit"); // close the server if there is no connection
		return 1;
	}

	print("MySQL connection is successful.");

	SetGameModeText("R-TDM "SERVER_VERSION"");
	
	EnableStuntBonusForAll(false);
	ManualVehicleEngineAndLights();
	DisableInteriorEnterExits();
	SetNameTagDrawDistance(20.0);

	SendRconCommand("loadfs maps");
	SendRconCommand("loadfs antisob");

	Robbery_CreateActor(93, -27.9002,-186.8377,1003.5469,2.1694, 10, 100);
	Robbery_CreateActor(155, 374.6786,-117.2747,1001.4922,180.9690, 10, 100);
	Robbery_CreateActor(155, 374.6786,-117.2747,1001.4922,180.9690, 21958, 10, 100);
	Robbery_CreateActor(7, -27.9002,-186.8377,1003.5469,2.1694, 40099, 10, 100);

	Robbery_CreateActor(167, 2393.0833,-1910.7480,13.5545,2.3841, 0, 75, 500);
	Robbery_CreateActor(155, 2116.7766,-1805.1068,13.5516,89.463, 0, 75, 500);

	mysql_tquery(g_SQL, "SELECT * FROM `properties` ORDER BY id", "LoadProperties");

	PlayerUpdateTimer = SetTimer("PlayerUpdate", 1000, true);
	VehicleUpdateTimer = SetTimer("VehicleUpdate", 1000, true);
	ServerUpdateTimer = SetTimer("ServerUpdate", 1000, true);
	LeaderboardUpdateTimer = SetTimer("LeaderboardUpdate", 60 * 1000, true);
	CopchaseTimer[0] = SetTimer("CopchaseUpdate", 1000, true);
	CopchaseTimer[1] = SetTimer("CopchasePlayerCheck", 15 * 1000, true);
	CopchaseTimer[2] = SetTimer("CopchaseStarting", 1000, true);

	ServerInfo[Hour] = 23; // dunno why i added this thing
	ServerInfo[Minute] = 30;
	ServerInfo[Day] = 1; // monday

	VoteInfo[VoteID] = INVALID_PLAYER_ID;
	VoteInfo[VoteAmount] = 0;
	VoteInfo[VoteIssuer] = INVALID_PLAYER_ID;
	format(VoteInfo[VoteReason],126, "NaN");
	VoteInfo[VoteMaxAmount] = 0;
	VoteInfo[VoteTime] = 0;
	VoteInfo[VoteCooldown] = 0;

	LeaderboardActor = CreateDynamicActor(0, 2339.1626,583.9326,106.6129,170.8836);
	LeaderboardUpdate();

	LeaderboardText = CreateDynamic3DTextLabel("Initializing..", COLOR_YELLOW, 2339.1626,583.9326,106.6129,25.0);
    ArenaText = CreateDynamic3DTextLabel("Initializing..", COLOR_YELLOW, 2322.7886,583.4646,106.6475,25.0);
	CopchaseText = CreateDynamic3DTextLabel("Initializing..", COLOR_YELLOW, 2331.0000,583.5938,106.6385,25.0);
	TDMText = CreateDynamic3DTextLabel("Initializing..", COLOR_YELLOW, 2322.8752,568.0782,106.6364,25.0);
	CreateDynamic3DTextLabel("Freeroam (/freeroam)", COLOR_RED, 2331.0293,567.9372,106.6041,25.0); // freeroam

	CreateDynamicActor(230, 2322.7886,583.4646,106.6475,180.4031); // dm
	CreateDynamicActor(106, 2322.8752,568.0782,106.6364,350.4149); // tdm
	CreateDynamicActor(280, 2331.0000,583.5938,106.6385,172.3195); // copchase
	CreateDynamicActor(0, 2331.0293,567.9372,106.6041,353.9522); // freeroam

	//CreateGameLobby("Public Lobby 1", "0.3.DL-R1", 32);
	//CreateGameLobby("Public Lobby 2", "0.3.7", 32);
	//CreateGameLobby("Public Lobby 3", "Cross Version", 32);

	//LANGUAGE_BAHASA = LoadLanguage("bahasa.lang.inc");


    new hostname[64];
    GetServerVarAsString("name", hostname, sizeof(hostname));
    printf("%s", hostname);
	printf("Game version: %s - Max players: %d - Max NPCs: %d", SERVER_VERSION, GetServerVarAsInt("max_players"), GetServerVarAsInt("max_bots"));
	printf("Server took %d ms to start.", GetTickCount() - InitializingTime);
	return 1;
}

public OnGameModeExit()
{
	// save all player data before closing connection
	foreach (new i : Player)
	{
		if (IsPlayerConnected(i))
		{
			// reason is set to 1 for normal 'Quit'
			CallLocalFunction("OnPlayerDisconnect", "ii", i, 1);
		}
	}

	SaveProperties();

	KillTimer(PlayerUpdateTimer);
	KillTimer(VehicleUpdateTimer);
	KillTimer(ServerUpdateTimer);
	KillTimer(LeaderboardUpdateTimer);
	KillTimer(CopchaseTimer[0]);
	KillTimer(CopchaseTimer[1]);
	KillTimer(CopchaseTimer[2]);

	mysql_close(g_SQL);
	return 1;
}

public OnPlayerUpdate(playerid)
{
	if(IsPlayerNPC(playerid)) return 1;

	PlayerInfo[playerid][PauseCheck] = GetTickCount(); 

	if(PlayerInfo[playerid][AdminDuty] == true && GetPlayerState(playerid) != PLAYER_STATE_SPECTATING)
	{
		SetPlayerChatBubble(playerid, "Admin Duty", COLOR_RED, 30.0, 2500); 
		GameTextForPlayer(playerid, "~r~~n~~n~~n~~n~~n~~n~~n~~n~~n~admin duty", 3000, 3);
	}

	if(PlayerInfo[playerid][State] == PLAYER_STATE_WOUNDED)
	{
		if(GetPlayerGameMode(playerid) == GAME_MODE_TDM || GetPlayerGameMode(playerid) == GAME_MODE_COPCHASE)
		{
			new string[128];
			format(string, sizeof(string), "(( Has been hit %d times, /damages %d to check their damages. ))", GetPlayerInjuries(playerid), playerid);
			SetPlayerChatBubble(playerid, string, 0xd05b46FF, 30.0, 2500); 
			
			if(GetPlayerAnimationIndex(playerid) != 1701 && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) ApplyAnimation(playerid, "WUZI", "CS_Dead_Guy", 4.1, 0, 0, 0, 1, 0, 1);
			else if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER || GetPlayerState(playerid) == PLAYER_STATE_PASSENGER) ApplyAnimation(playerid, "ped", "CAR_dead_LHS", 4.1, 0, 0, 0, 1, 0, 1);
		}	
	}
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	if(VoteInfo[VoteID] == playerid)
    {
        SendClientMessageToAllEx(COLOR_RED, "VOTEKICK: Vote ended, %s left the server.", PlayerInfo[VoteInfo[VoteID]][Name]);

        VoteInfo[VoteID] = INVALID_PLAYER_ID;
        VoteInfo[VoteAmount] = 0;
		VoteInfo[VoteIssuer] = INVALID_PLAYER_ID;
        format(VoteInfo[VoteReason], 126, "NaN");
        VoteInfo[VoteMaxAmount] = 0;
        VoteInfo[VoteTime] = 0;
        foreach(new i : Player)
        {
            PlayerInfo[i][Vote] = 0;
        }
		VoteInfo[VoteCooldown] = 30;
    }

	for(new i = 1; i < MAX_REPORTS; i++)
    {
        if(ReportInfo[i][rID] && ReportInfo[i][rIssuer] == playerid)
        {
            ReportInfo[i][rID] = 0;
            break;
        }
    }

	SetPlayerName(playerid, PlayerInfo[playerid][Name]);

	new dReason[3][] =
    {
        "Timeout/Crash",
        "Quit",
        "Kicked/Banned"
    };

	SendGameMessage(GetPlayerGameMode(playerid), COLOR_RED,  "~ {AFAFAF}%s left the server. (%s)", PlayerInfo[playerid][Name], dReason[reason]);
	return 1;
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	if(IsPlayerNPC(playerid)) return 1;

	if(PlayerInfo[playerid][State] == PLAYER_STATE_WOUNDED)
	{
		new Float:pos[3];
		GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
		SetPlayerPos(playerid, pos[0], pos[1], pos[2]);
		ApplyAnimationEx(playerid, "WUZI", "CS_Dead_Guy", 4.1, 0, 0, 0, 1, 0, 1);	
	}
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	if(IsPlayerNPC(playerid)) return 1;

	PlayerTextDrawHide(playerid, speedo1[playerid]);
	PlayerTextDrawHide(playerid, speedo2[playerid]);
	PlayerTextDrawHide(playerid, speedo3[playerid]);
	return 1;
}

public OnPlayerText(playerid, text[])
{
	if(!PlayerInfo[playerid][IsLoggedIn]) return 0;

	if(PlayerInfo[playerid][Muted])
	{
		SendErrorMessage(playerid, "You're muted!");
		return 0;
	}

    if(GetPlayerGameMode(playerid) == GAME_MODE_LOBBY)
	{
		SendGameMessage(GetPlayerGameMode(playerid), 0x9999ffff, "~ {%06x}%s(%d){FFFFFF}: %s", GetPlayerColor(playerid) >>> 8, PlayerInfo[playerid][Name], playerid, text);
	}
	else if(GetPlayerGameMode(playerid) == GAME_MODE_DUEL)
	{
		SendClientMessageEx(playerid, 0x9999ffff, "~ {%06x}%s(%d){FFFFFF}: %s", GetPlayerColor(playerid) >>> 8, PlayerInfo[playerid][Name], playerid, text);
		SendClientMessageEx(DuelInfo[playerid][DuelEnemy], 0x9999ffff, "~ {%06x}%s(%d){FFFFFF}: %s", GetPlayerColor(playerid) >>> 8, PlayerInfo[playerid][Name], playerid, text);
	}
	else if(GetPlayerGameMode(playerid) == GAME_MODE_DM)
	{
		foreach(new i : Player)
		{
			if(IsPlayerConnected(i) && PlayerInfo[i][IsLoggedIn] == true)
			{
				if(GetPlayerGameMode(i) == GAME_MODE_DM && PlayerInfo[i][DM] == PlayerInfo[playerid][DM])
				{
					SendClientMessageEx(i, 0x9999ffff, "~ {%06x}%s(%d){FFFFFF}: %s", GetPlayerColor(playerid) >>> 8, PlayerInfo[playerid][Name], playerid, text);
				}
			}
		}
	}
    else if(GetPlayerGameMode(playerid) == GAME_MODE_TDM)
	{
		if(PlayerInfo[playerid][Team] == -1) return 0;

		if(strlen(text) < 128) 
		{
			SendGameMessage(GAME_MODE_TDM, ReturnTeamColorEx(PlayerInfo[playerid][Team]), "%s{FFFFFF}: %s", ReturnName(playerid), text);
			return 0;
		}
	}
	else if(GetPlayerGameMode(playerid) == GAME_MODE_COPCHASE)
	{
		if(strlen(text) < 128) 
		{
			SendNearbyMessage(playerid, 15.0, GetPlayerColor(playerid), "%s{FFFFFF} says: %s", PlayerInfo[playerid][Name], text);
			return 0;
		}
	}
	else if(GetPlayerGameMode(playerid) == GAME_MODE_FREEROAM)
	{
		foreach(new i : Player)
		{
			if(IsPlayerConnected(i) && PlayerInfo[i][IsLoggedIn] == true)
			{
				if(GetPlayerGameMode(i) == GAME_MODE_FREEROAM && GetPlayerVirtualWorld(i) == GetPlayerVirtualWorld(playerid))
				{
					SendClientMessageEx(i, 0x9999ffff, "~ {%06x}%s(%d){FFFFFF}: %s", GetPlayerColor(playerid) >>> 8, PlayerInfo[playerid][Name], playerid, text);
				}
			}
		}
	}
	PlayerInfo[playerid][ChatDelay] = 1;
	return 0;
}

public OnPlayerSpawn(playerid)
{
	StopAudioStreamForPlayer(playerid);
	
	if(IsPlayerNPC(playerid)) return 1;

	if(PlayerInfo[playerid][IsLoggedIn] == false) return Kick(playerid);

	if(GetPlayerGameMode(playerid) == GAME_MODE_DUEL)
	{
	    PlayerInfo[DuelInfo[playerid][DuelEnemy]][Cash] += DuelInfo[playerid][DuelBet];
	    PlayerInfo[playerid][Cash] -= DuelInfo[playerid][DuelBet];
	    SendGameMessage(GAME_MODE_LOBBY, COLOR_RED, "DUEL: {FFFFFF} %s defeated %s in a duel. (%s)", ReturnName(DuelInfo[playerid][DuelEnemy]),  ConvertTime(GetTickCount() - DuelInfo[playerid][DuelTick]));
		SendGameMessage(GAME_MODE_DM, COLOR_RED, "DUEL: {FFFFFF} %s defeated %s in a duel. (%s)", ReturnName(DuelInfo[playerid][DuelEnemy]),  ConvertTime(GetTickCount() - DuelInfo[playerid][DuelTick]));
        SendGameMessage(GAME_MODE_DUEL, COLOR_RED, "DUEL: {FFFFFF} %s defeated %s in a duel. (%s)", ReturnName(DuelInfo[playerid][DuelEnemy]),  ConvertTime(GetTickCount() - DuelInfo[playerid][DuelTick]));
       	SendPlayerToLobby(DuelInfo[playerid][DuelEnemy]);
	    DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelEnemy] = INVALID_PLAYER_ID;
	    DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelWeapon] = 0;
	    DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelWeapon2] = 0;
	    DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelBet] = 0;
	    DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelMap] = 0;

        SendPlayerToLobby(playerid);
        DuelInfo[playerid][DuelEnemy] = INVALID_PLAYER_ID;
        DuelInfo[playerid][DuelWeapon] = 0;
        DuelInfo[playerid][DuelWeapon2] = 0;
        DuelInfo[playerid][DuelBet] = 0;
        DuelInfo[playerid][DuelMap] = 0;
	}

	// spawn the player to their last saved position
	//SetPlayerInterior(playerid, PlayerInfo[playerid][Interior]);
	SetPlayerPos(playerid, PlayerInfo[playerid][X_Pos], PlayerInfo[playerid][Y_Pos], PlayerInfo[playerid][Z_Pos]);
	SetPlayerFacingAngle(playerid, PlayerInfo[playerid][A_Pos]);

	SetCameraBehindPlayer(playerid);

	SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, 0);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, 0);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_SPAS12_SHOTGUN, 0);

    if(GetPlayerGameMode(playerid) == GAME_MODE_LOBBY || GetPlayerGameMode(playerid) == GAME_MODE_COPCHASE)
	{
		SendPlayerToLobby(playerid);
	}
	else if(GetPlayerGameMode(playerid) == GAME_MODE_TDM)
	{
		for(new i = 0; i < MAX_ZONES; i++)
		{
			if(ZoneInfo[i][zCapturedBy] != 0)
			{
				GangZoneFlashForPlayer(playerid, i, 0xFF634766);
			}
		}

		if (PlayerInfo[playerid][Team] == -1) 
		{
			SendPlayerToTeamSelection(playerid);
		}
		else
		{
			SetPlayerPosition(playerid);
		}
	}
	else if(GetPlayerGameMode(playerid) == GAME_MODE_DM)
	{
		if(PlayerInfo[playerid][DM] != -1) SendPlayerToDMArena(playerid, PlayerInfo[playerid][DM]);
	}
	else if(GetPlayerGameMode(playerid) == GAME_MODE_FREEROAM)
	{
		SendPlayerToFreeroam(playerid);
	}
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	if(IsPlayerNPC(playerid)) return SpawnPlayer(playerid);

    if(GetPlayerGameMode(playerid) == GAME_MODE_DUEL)
	{
	    PlayerInfo[DuelInfo[playerid][DuelEnemy]][Cash] += DuelInfo[playerid][DuelBet];
	    PlayerInfo[playerid][Cash] -= DuelInfo[playerid][DuelBet];
	    SendGameMessage(GAME_MODE_LOBBY, COLOR_RED, "DUEL: {FFFFFF} %s defeated %s in a duel.", ReturnName(DuelInfo[playerid][DuelEnemy]),  ConvertTime(GetTickCount() - DuelInfo[playerid][DuelTick]));
		SendGameMessage(GAME_MODE_DM, COLOR_RED, "DUEL: {FFFFFF} %s defeated %s in a duel.", ReturnName(DuelInfo[playerid][DuelEnemy]),  ConvertTime(GetTickCount() - DuelInfo[playerid][DuelTick]));
		SendGameMessage(GAME_MODE_DUEL, COLOR_RED, "DUEL: {FFFFFF} %s defeated %s in a duel.", ReturnName(DuelInfo[playerid][DuelEnemy]),  ConvertTime(GetTickCount() - DuelInfo[playerid][DuelTick]));

        SendPlayerToLobby(DuelInfo[playerid][DuelEnemy]);
	    DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelEnemy] = INVALID_PLAYER_ID;
	    DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelWeapon] = 0;
	    DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelWeapon2] = 0;
	    DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelBet] = 0;
	    DuelInfo[DuelInfo[playerid][DuelEnemy]][DuelMap] = 0;

        SendPlayerToLobby(playerid);
        DuelInfo[playerid][DuelEnemy] = INVALID_PLAYER_ID;
        DuelInfo[playerid][DuelWeapon] = 0;
        DuelInfo[playerid][DuelWeapon2] = 0;
        DuelInfo[playerid][DuelBet] = 0;
        DuelInfo[playerid][DuelMap] = 0;
	}

	ClearPlayerDamages(playerid);

	ResetPlayerWeapons(playerid);
	PlayerInfo[playerid][State] = PLAYER_STATE_ALIVE;
	PlayerInfo[playerid][DeathTime] = 0;
	PlayerInfo[playerid][KillStreak] = 0;
	SetPlayerHP(playerid, 100.0);
	SpawnPlayer(playerid);
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch (dialogid)
	{
		case DIALOG_UNUSED: return 1; // Useful for dialogs that contain only information and we do nothing depending on whether they responded or not

		default: return 0; // dialog ID was not found, search in other scripts
	}
	return 1;
}

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	new weaponid = GetPlayerWeapon(playerid);
	if(GetPlayerGameMode(playerid) == GAME_MODE_TDM || GetPlayerGameMode(playerid) == GAME_MODE_COPCHASE)
	{
		if(PlayerInfo[playerid][CbugCheck] && !IsPlayerInAnyVehicle(playerid) && (weaponid == 24 || weaponid == 25 || weaponid == 33 || weaponid == 34))
		{
			if(PRESSED(KEY_CROUCH))
			{
				if(GetPlayerSpecialAction(playerid) != SPECIAL_ACTION_DUCK)
				{
					ClearAnimations(playerid);
					SetPlayerArmedWeapon(playerid, 0);
					GameTextForPlayer(playerid, "~r~NO CBUG!", 2000, 6);

					PlayerInfo[playerid][CbugCheck] = 0;
				}
			}
		}
	}
	if(GetPlayerGameMode(playerid) == GAME_MODE_DM)
	{
		if(PRESSED(KEY_HANDBRAKE) || PRESSED(KEY_FIRE) || PRESSED(KEY_ACTION))
		{
			if(weaponid == 24 || weaponid == 25 || weaponid == 33 || weaponid == 34) PlayerInfo[playerid][CbugCheck] = 2;
		}
		if(PlayerInfo[playerid][CbugCheck] && !IsPlayerInAnyVehicle(playerid))
		{
			if(PRESSED(KEY_CROUCH))
			{
				if(PlayerInfo[playerid][State] == PLAYER_STATE_ALIVE) ClearAnimations(playerid);
				SetPlayerArmedWeapon(playerid, 0);
				GameTextForPlayer(playerid, "~r~NO CBUG!", 2000, 6);

				PlayerInfo[playerid][CbugCheck] = 0;
			}
		}
	}
	if(GetPlayerGameMode(playerid) == GAME_MODE_COPCHASE)
	{
		if(PRESSED(KEY_YES))
		{
			if(PlayerInfo[playerid][ObjectLoad])
			{
				PlayerInfo[playerid][ObjectLoad] = 0;
                TogglePlayerControllable(playerid, 1);
			}
		}
	}
	if(PRESSED(KEY_NO))
	{
		if(PlayerInfo[playerid][State] == PLAYER_STATE_WOUNDED) return 1;

		new vehicleid = GetPlayerVehicleID(playerid), engine, lights, alarm, doors, bonnet, boot, objective;
		if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return 1;
		
		GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

		if (engine == VEHICLE_PARAMS_UNSET || engine == VEHICLE_PARAMS_OFF)
		{
			SendNearbyMessage(playerid, 20.0, 0xC2A2DAFF, "** %s {C2A2DA}starts the vehicle engine.", ReturnName(playerid));
			SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective);
		}
	}
	if(PRESSED(KEY_SUBMISSION))
	{
		if(PlayerInfo[playerid][State] == PLAYER_STATE_WOUNDED) return 1;

		new vehicleid = GetPlayerVehicleID(playerid), engine, lights, alarm, doors, bonnet, boot, objective;
		if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return 1;
		
		GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

		if(VehicleInfo[vehicleid][vTeam] != 0 && VehicleInfo[vehicleid][vTeam] != PlayerInfo[playerid][CC_Team]) return SendErrorMessage(playerid, "You don't have the key for the vehicle.");

		if (lights == VEHICLE_PARAMS_UNSET || lights == VEHICLE_PARAMS_OFF)
		{
			GameTextForPlayer(playerid, "~g~LIGHTS ON", 3000, 3);
			SetVehicleParamsEx(vehicleid, engine, VEHICLE_PARAMS_ON, alarm, doors, bonnet, boot, objective);
		}
		else 
		{
			GameTextForPlayer(playerid, "~r~LIGHTS OFF", 3000, 3);
			SetVehicleParamsEx(vehicleid, engine, VEHICLE_PARAMS_OFF, alarm, doors, bonnet, boot, objective);
		}
	}
	return 1;
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && weaponid == 24 || weaponid == 25 || weaponid == 33)
	{
		PlayerInfo[playerid][CbugCheck] = 2;
	}
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if(IsPlayerNPC(playerid)) return 1;

	if(newstate == PLAYER_STATE_DRIVER)
	{
		if(GetPlayerWeapon(playerid) != 0 && GetPlayerWeapon(playerid) == 29 || GetPlayerWeapon(playerid) == 28 || GetPlayerWeapon(playerid) == 32 || GetPlayerWeapon(playerid) == 26)
		{
			SetPlayerArmedWeapon(playerid, 0);
		}

		if(VehicleInfo[GetPlayerVehicleID(playerid)][vTeam] != 0 && VehicleInfo[GetPlayerVehicleID(playerid)][vTeam] == TEAM_LSPD)
		{
			if(GetPlayerGameMode(playerid) == GAME_MODE_COPCHASE)
			{
				if(PlayerInfo[playerid][CC_Team] == TEAM_FUGITIVE)
				{
					if(GetPlayerWantedLevel(playerid) < 3)
					{
						SetPlayerWantedLevel(playerid, 3);
						GameTextForPlayer(playerid, "~r~~n~~n~~n~~n~~n~~n~~n~~n~~n~Wanted level increased!", 1500, 3);

						foreach(new i : Player)
						{
							if(GetPlayerGameMode(i) == GAME_MODE_COPCHASE && PlayerInfo[i][CC_Team] == TEAM_LSPD)
							{
								SendClientMessageEx(i, COLOR_RADIO, "** [CH: 911 S: 1] Stolen vehicle at %s.",  ReturnLocation(playerid));
							}
						}
					}
				}
			}
		}
	}
	if(newstate == PLAYER_STATE_PASSENGER)
	{
		if(GetPlayerWeapon(playerid) != 0 &&  GetPlayerWeapon(playerid) == 24)
		{
			SetPlayerArmedWeapon(playerid, 0);
		}
	}

	/*if(oldstate == PLAYER_STATE_ONFOOT && newstate == PLAYER_STATE_DRIVER)
	{
		if(!PlayerInfo[playerid][Tog][3])
		{
			PlayerTextDrawShow(playerid, speedo1[playerid]);
			PlayerTextDrawShow(playerid, speedo2[playerid]);
			PlayerTextDrawShow(playerid, speedo3[playerid]);
		}
	}*/
	return 1;
}

public OnPlayerCommandReceived(playerid, cmd[], params[], flags)
{
	if(PlayerInfo[playerid][ChatDelay]) 
	{
		SendErrorMessage(playerid, "Please avoid spamming!");
		return 0;
	}

	if(!PlayerInfo[playerid][IsLoggedIn]) return 0;

	if(GetPlayerGameMode(playerid) == GAME_MODE_TDM && PlayerInfo[playerid][Team] == -1) return 0;

	return 1;
}

public OnPlayerCommandPerformed(playerid, cmd[], params[], result, flags)
{
	printf("[CMD] %s used \"/%s %s\"", ReturnName(playerid), cmd, params);
	if (result == -1)
	{
		SendClientMessage(playerid, 0xe06666FF, "Oops! That command doesn't exist.");
		return 0;
	}
	PlayerInfo[playerid][ChatDelay] = 1;
	return 1;
}

public OnPlayerCheat(playerid, const code)
{
	new string[256];
	if(code == 3 || code == 4 || code == 5 || code == 6 || code == 7 || code == 9 || code == 10 || code == 11 || code == 13) return 0;

    if(!PlayerInfo[playerid][AntiCheatPause])
	{
		if(code == 1 && PlayerInfo[playerid][Team] == -1) return 0;

		PlayerInfo[playerid][HackWarning][code]++;
		if(code != 7) format(string, sizeof string, "[AntiCheat] %s possibly using %s.", PlayerInfo[playerid][Name], ReturnCheatName(code));
		SendAdminMessage(1, 0xbf9000FF, string);

		if(code == 8)
		{
			SetPlayerAmmo(playerid, GetPlayerWeapon(playerid), 0);
		}
		else if(code == 7)
		{
			if(PlayerInfo[playerid][IsLoggedIn] == true && GetTickCount() < (PlayerInfo[playerid][PauseCheck]+2000))
			{
				SendClientMessage(playerid, COLOR_RED, "[ ! ] High ping detected! Please fix your ping or else you will get kicked.");
				if(PlayerInfo[playerid][HackWarning][code] >= 10)
				{
					format(string, sizeof string, "[AntiCheat] %s has been kicked for %s.", PlayerInfo[playerid][Name], ReturnCheatName(code));
					SendAdminMessage(1, 0xbf9000FF, string);
					SendClientMessageEx(playerid, COLOR_RED, "[AntiCheat] You've been kicked from the server for \"%s\".", ReturnCheatName(code));
					DelayedKick(playerid);
				}
			}
		}
		else if(code == 14)
		{
			SendClientMessage(playerid, COLOR_RED, "[ ! ] Fast walk or modified IFP detected. Please turn it off IMMEDIATELY!");
			if(PlayerInfo[playerid][HackWarning][code] >= 4)
			{
				format(string, sizeof string, "[AntiCheat] %s has been kicked for possibly using %s.", PlayerInfo[playerid][Name], ReturnCheatName(code));
				SendAdminMessage(1, 0xbf9000FF, string);
				SendClientMessageEx(playerid, COLOR_RED, "[AntiCheat] You've been kicked from the server for \"%s\".", ReturnCheatName(code));
				DelayedKick(playerid);
			}
		}
		else
		{
			if(code != 1) 
			{
				if(PlayerInfo[playerid][HackWarning][code] >= 3)
				{
					format(string, sizeof string, "[AntiCheat] %s has been kicked for possibly using %s.", PlayerInfo[playerid][Name], ReturnCheatName(code));
					SendAdminMessage(1, 0xbf9000FF, string);
					SendClientMessageEx(playerid, COLOR_RED, "[AntiCheat] You've been kicked from the server for \"%s\".", ReturnCheatName(code));
					DelayedKick(playerid);
				}
			}
		}
	}
    return 1;
}
//-----------------------------------------------------

forward PlayerUpdate();
public PlayerUpdate()
{
	new textdrawupdate[256], str[256];
	foreach (new i : Player)
	{
		if(IsPlayerConnected(i) && !IsPlayerNPC(i))
		{
			if(PlayerInfo[i][IsLoggedIn] == true)
			{
				PlayerInfo[i][PlayingHours][2]++; // Seconds
				if(PlayerInfo[i][PlayingHours][2] >= 60)
				{
					PlayerInfo[i][PlayingHours][2] = 0;
					PlayerInfo[i][PlayingHours][1]++; // Minutes
					if(PlayerInfo[i][PlayingHours][1] >= 60)
					{
						PlayerInfo[i][PlayingHours][1] = 0;
						PlayerInfo[i][PlayingHours][0]++; // Hours
					}
				}
				if(PlayerInfo[i][AdminLevel])
				{
					PlayerInfo[i][StaffActivity][2]++; // Seconds
					if(PlayerInfo[i][StaffActivity][2] >= 60)
					{
						PlayerInfo[i][StaffActivity][2] = 0;
						PlayerInfo[i][StaffActivity][1]++; // Minutes
						if(PlayerInfo[i][StaffActivity][1] >= 60)
						{
							PlayerInfo[i][StaffActivity][1] = 0;
							PlayerInfo[i][StaffActivity][0]++; // Hours
						}
					}
				}
				if(PlayerInfo[i][Donator])
				{
					if(PlayerInfo[i][DonatorExpire] != 0 && gettime() > PlayerInfo[i][DonatorExpire])
					{
						PlayerInfo[i][Donator] = 0;
						PlayerInfo[i][DonatorExpire] = 0;
						SendClientMessage(i, COLOR_RED, "-------------------------------------------------------");
						SendClientMessage(i, COLOR_WHITE, "Your donator rank has been expired.");
						SendClientMessage(i, COLOR_WHITE, "Thank you for buying!");
						SendClientMessage(i, COLOR_RED, "-------------------------------------------------------");
					}
				}
				if(PlayerInfo[i][Muted])
				{
					if(PlayerInfo[i][MuteTime])
					{
						PlayerInfo[i][MuteTime]--;
						if(PlayerInfo[i][MuteTime] <= 1)
						{
							PlayerInfo[i][MuteTime] = 0;
							PlayerInfo[i][Muted] = 0;
							SendClientMessage(i, COLOR_WHITE, "You have served your mute time. Be good!");
						}
					}
				}
				if(PlayerInfo[i][Mask])
				{
					PlayerInfo[i][Mask]--;
					if(GetPlayerGameMode(i) == GAME_MODE_TDM || GetPlayerGameMode(i) == GAME_MODE_COPCHASE)
					{
						foreach(new pid : Player)
						{
							if(GetPlayerGameMode(pid) == GAME_MODE_TDM || GetPlayerGameMode(pid) == GAME_MODE_COPCHASE)
							{
								SetPlayerMarkerForPlayer(pid, i, (GetPlayerColor(i) & 0xFFFFFF00));
							}
						}
					}
					else
					{
						foreach(new pid : Player)
						{
							if(GetPlayerGameMode(pid) != GAME_MODE_TDM && GetPlayerGameMode(pid) != GAME_MODE_COPCHASE)
							{
								SetPlayerMarkerForPlayer(pid, i, GetPlayerColor(i));
							}
						}
					}

					if(!PlayerInfo[i][Mask])
					{
						foreach(new pid : Player)
						{
							if(GetPlayerGameMode(pid) != GAME_MODE_TDM && GetPlayerGameMode(pid) != GAME_MODE_COPCHASE)
							{
								SetPlayerMarkerForPlayer(pid, i, GetPlayerColor(i));
							}
						}
					}
				}
				if(!PlayerInfo[i][Tog][2])
				{
					format(textdrawupdate, sizeof textdrawupdate, "~W~FPS ~R~%d ~W~PING ~R~%d ms ~W~P/L ~R~%.1f", GetPlayerFPS(i), GetPlayerPing(i), NetStats_PacketLossPercent(i));
					PlayerTextDrawSetString(i, ui_stats[i], textdrawupdate);
				}

				if(GetPlayerState(i) == PLAYER_STATE_SPECTATING && PlayerInfo[i][AdminLevel] && PlayerInfo[i][Spectating] != INVALID_PLAYER_ID)
				{
					new target = PlayerInfo[i][Spectating];
					format(str, sizeof str, "~n~~n~~n~~n~~n~~n~~g~%s (%i)~n~~r~FPS: %i~n~PING: %i~n~PL: %.2f~n~SPEED: %.2f", PlayerInfo[target][Name], target, GetPlayerFPS(target), GetPlayerPing(target), NetStats_PacketLossPercent(target), GetPlayerSpeed(target));
					GameTextForPlayer(i, str, 3000, 3);
				}

				/*if(IsPlayerInAnyVehicle(i) && GetPlayerState(i) == PLAYER_STATE_DRIVER)
				{
					if(!PlayerInfo[i][Tog][3])
				    {
						format(textdrawupdate, sizeof textdrawupdate, "%d MPH", GetVehicleSpeed(GetPlayerVehicleID(i)));
						PlayerTextDrawSetString(i, speedo3[i], textdrawupdate);

						switch(GetVehicleSpeed(GetPlayerVehicleID(i)))
						{
							case 0..10: format(textdrawupdate, sizeof textdrawupdate, "mdl-5000:speedo_1");
							case 11..30: format(textdrawupdate, sizeof textdrawupdate, "mdl-5000:speedo_2");
							case 31..60: format(textdrawupdate, sizeof textdrawupdate, "mdl-5000:speedo_3");
							case 61..90: format(textdrawupdate, sizeof textdrawupdate, "mdl-5000:speedo_4");
							case 91..120: format(textdrawupdate, sizeof textdrawupdate, "mdl-5000:speedo_5");
							case 121..150: format(textdrawupdate, sizeof textdrawupdate, "mdl-5000:speedo_6");
							case 151..180: format(textdrawupdate, sizeof textdrawupdate, "mdl-5000:speedo_7");
							case 181..210: format(textdrawupdate, sizeof textdrawupdate, "mdl-5000:speedo_8");
							case 212..1000: format(textdrawupdate, sizeof textdrawupdate, "mdl-5000:speedo_9");
						}

						PlayerTextDrawSetString(i, speedo2[i], textdrawupdate);
					}
				}*/

                UpdateNameTag(i);

				if(PlayerInfo[i][ChatDelay] == 1)
				{
					PlayerInfo[i][ChatDelay] = 0;
				}
				if(PlayerInfo[i][CbugCheck])
				{
					PlayerInfo[i][CbugCheck]--;
				}
				if(GetPlayerGameMode(i) == GAME_MODE_LOBBY)
				{
					PlayerInfo[i][Health] = 100.0;
					PlayerInfo[i][Armour] = 0.0;

					SetPlayerHealth(i, PlayerInfo[i][Health]);
					if(IsPlayerInCopchaseLobby(i))
					{
						if(CopchaseInfo[CC_Started] == false && CopchaseInfo[CC_Starting] == false && GetLobbyPlayersCount() < 3)
						{
							format(str, sizeof str, "~w~WAITING FOR PLAYERS~n~%d more players", 3 - GetLobbyPlayersCount());
							GameTextForPlayer(i, str, 2500, 3);
						}
					}
				}
				
				if(GetPlayerGameMode(i) == GAME_MODE_TDM)
				{
					/*if(PlayerInfo[i][Score] < 50)
					{
						newbies++;
					}
					else
					{
						pros++;
					}
					if(newbies > pros) WeaponBalancing = true;
					else WeaponBalancing = false;*/

					if(WeaponBalancing == true)
					{
						if(PlayerInfo[i][Weapon][0] == 31 || PlayerInfo[i][Weapon][0] == 30 || PlayerInfo[i][Weapon][0] == 29)
						{
							printf("weapon balancing");
							SendClientMessage(i, COLOR_RED, "[ ! ] Weapon balancing is on. Your weapon has been replaced with a shotgun and $50.");
							PlayerInfo[i][Weapon][0] = 25;
							PlayerInfo[i][Cash] += 50;
						}
					}
					if (PlayerInfo[i][SelectingTeam] == false)
					{
						new string[128];
						format(string, sizeof(string), "~w~%s%d:%s%d", (ServerInfo[Hour] < 10) ? ("0") : (""), ServerInfo[Hour], (ServerInfo[Minute] < 10) ? ("0") : (""), ServerInfo[Minute]);
						PlayerTextDrawSetString(i, ui_clock[i], string);

						SetPlayerTime(i, ServerInfo[Hour], ServerInfo[Minute]);
					}
					else
					{
						SelectTextDraw(i, 0xAFAFAFFF);
					}

					if(PlayerInfo[i][State] == PLAYER_STATE_ALIVE)
					{
						SetPlayerHealth(i, PlayerInfo[i][Health]);
					}
					else if(PlayerInfo[i][State] == PLAYER_STATE_WOUNDED)
					{
						SetPlayerHealth(i, 30);
					}

					if(PlayerInfo[i][Team] != -1)
					{
						if(PlayerHasPerk(i, 8) && PlayerInfo[i][State] == PLAYER_STATE_ALIVE)
						{
							new Float:HP;
							GetPlayerHP(i, HP);
							if(PlayerHasPerk(i, 2) && HP < 175.0)
							{
								SetPlayerHP(i, HP += 1.0);
							}
							else if(!PlayerHasPerk(i, 2) && HP < 150.0)
							{
								SetPlayerHP(i, HP += 1.0);
							}
						}
					}
				}
				if(GetPlayerGameMode(i) == GAME_MODE_DM)
				{
					if(PlayerInfo[i][State] == PLAYER_STATE_ALIVE)
					{
						SetPlayerHealth(i, PlayerInfo[i][Health]);
						if(PlayerInfo[i][Armour]) SetPlayerArmour(i, PlayerInfo[i][Armour]);
					}
					else if(PlayerInfo[i][State] == PLAYER_STATE_WOUNDED)
					{
						SetPlayerHealth(i, 30);
					}
				}
				if(GetPlayerGameMode(i) == GAME_MODE_COPCHASE)
				{
					new string[128];
					format(string, sizeof(string), "~w~%s%d:%s%d", (ServerInfo[Hour] < 10) ? ("0") : (""), ServerInfo[Hour], (ServerInfo[Minute] < 10) ? ("0") : (""), ServerInfo[Minute]);
					PlayerTextDrawSetString(i, ui_clock[i], string);

					SetPlayerTime(i, ServerInfo[Hour], ServerInfo[Minute]);
					for(new prop; prop < MAX_PROPERTIES; prop++)
					{
						if(IsPlayerInDynamicCP(i, PropertyInfo[prop][PropCheckpoint]) && PropertyInfo[prop][PropID] != 0)
						{
							new satring[256];
							format(satring, sizeof(satring), "~p~%s~n~~w~Type~b~ /enter~w~ to go inside", PropertyInfo[prop][PropName]);
							GameTextForPlayer(i, satring, 3000, 3);
						}
					}
					if(PlayerInfo[i][Traced])
					{
						PlayerInfo[i][Traced]--;
						UpdatePlayerMinimap(i);
						if(PlayerInfo[i][Traced] <= 1)
						{
							PlayerInfo[i][Traced] = 0;
							HideCopsCheckpoint();
						}
					}
					if(PlayerInfo[i][Tased])
					{
						PlayerInfo[i][Tased]--;
						if(PlayerInfo[i][Tased] <= 1)
						{
							if(!IsPlayerControllable(i)) TogglePlayerControllable(i, true);
							PlayerInfo[i][Tased] = 0;
							ClearAnimations(i);
						}
					}
					if(PlayerInfo[i][CuffTime])
					{
						PlayerInfo[i][CuffTime]--;
						if(PlayerInfo[i][CuffTime] <= 1)
						{
							new target = PlayerInfo[i][CuffID];

							PlayerInfo[i][CuffTime] = 0;

							if(PlayerInfo[i][CC_Team] != TEAM_LSPD) return 1;

							if(PlayerInfo[i][State] == PLAYER_STATE_WOUNDED) return 1;

							if(GetPlayerState(i) != PLAYER_STATE_ONFOOT) return 1;

							if(CopchaseInfo[CC_WeaponEnabled] == false) return 1;

							if(!IsPlayerConnected(target)) return 1;

							if(PlayerInfo[target][CC_Team] == TEAM_LSPD) return 1;

							if(!IsPlayerNearPlayer(i, target, 3.0)) return 1;

							if(GetPlayerWantedLevel(target) >= 3) return 1;

							if(GetPlayerState(target) != PLAYER_STATE_ONFOOT) return 1;

							SendGameMessage(GAME_MODE_COPCHASE, COLOR_COP, "Criminal %s has been hand cuffed and detained by %s.", ReturnName(target), ReturnName(i));

                            RemovePlayerFromCopchase(target);

							PlayerInfo[i][Score] += 2;

							PlayerInfo[i][CuffID] = INVALID_PLAYER_ID;
						}
					}
					if(PlayerInfo[i][DragTime])
					{
						PlayerInfo[i][DragTime]--;
						if(PlayerInfo[i][DragTime] <= 1)
						{
							new target = PlayerInfo[i][DragID];

							PlayerInfo[i][DragTime] = 0;

							if(PlayerInfo[i][CC_Team] != TEAM_LSPD) return 1;

							if(PlayerInfo[i][State] == PLAYER_STATE_WOUNDED) return 1;

							if(GetPlayerState(i) != PLAYER_STATE_ONFOOT) return 1;

							if(CopchaseInfo[CC_WeaponEnabled] == false) return 1;

							if(!IsPlayerConnected(target)) return 1;

							if(PlayerInfo[target][CC_Team] == TEAM_LSPD) return 1;

							if(!IsPlayerNearPlayer(i, target, 3.0)) return 1;

							if(GetPlayerWantedLevel(target) >= 3) return 1;

							if(GetPlayerState(target) == PLAYER_STATE_ONFOOT) return 1;

                            if(IsPlayerInAnyVehicle(target)) RemovePlayerFromVehicle(target);
							
                            SetPlayerChatBubble(target, "(( Dragged out ))", COLOR_GREY, 30.0, 500); 

							PlayerInfo[i][DragID] = INVALID_PLAYER_ID;
						}
					}
				}

				if(GetPlayerSurfingVehicleID(i) != INVALID_VEHICLE_ID)
				{
					new Float:PosX, Float:PosY, Float:PosZ;
					GameTextForPlayer(i, "~r~Do not car surfing!", 2000, 6);
					GetPlayerPos(i, PosX, PosY, PosZ);
					SetPlayerPos(i, PosX, PosY, PosZ + 1.5);
				}
				if(PlayerInfo[i][DamageFix])
				{
					PlayerInfo[i][DamageFix]--;
					if(PlayerInfo[i][DamageFix] <= 1)
					{
						PlayerInfo[i][DamageFix] = 0;
						if(GetPlayerGameMode(i) == GAME_MODE_DM && PlayerInfo[i][State] == PLAYER_STATE_WOUNDED) SpawnPlayer(i);
						else if(GetPlayerGameMode(i) == GAME_MODE_FREEROAM && PlayerInfo[i][State] == PLAYER_STATE_WOUNDED) SpawnPlayer(i);
					}
				}
				if(PlayerInfo[i][ObjectLoad])
				{
					PlayerInfo[i][ObjectLoad]--;
					if(PlayerInfo[i][ObjectLoad] <= 1)
					{
						PlayerInfo[i][ObjectLoad] = 0;
						if(!IsPlayerControllable(i)) TogglePlayerControllable(i, 1);
					}
				}
				if(PlayerInfo[i][AntiCheatPause])
				{
					PlayerInfo[i][AntiCheatPause]--;
					if(PlayerInfo[i][AntiCheatPause] <= 1)
					{
						PlayerInfo[i][AntiCheatPause] = 0;
					}
				}

				if(PlayerInfo[i][DeathTime])
				{
					PlayerInfo[i][DeathTime]--;
					if(PlayerInfo[i][DeathTime] <= 1)
					{
						PlayerInfo[i][DeathTime] = 0;
					}
				}

				if(PlayerInfo[i][Reviving] != -1)
				{
					if(PlayerInfo[i][RevivingTime])
					{
						PlayerInfo[i][RevivingTime]--;
						if(PlayerInfo[i][RevivingTime] <= 1)
						{
							new target = PlayerInfo[i][Reviving];
							PlayerInfo[i][RevivingTime] = 0;

							if(PlayerInfo[i][State] != PLAYER_STATE_WOUNDED) TogglePlayerControllable(i, true);

							if(!IsPlayerConnected(target)) 
							{
								PlayerInfo[i][Reviving] = -1;
								return SendErrorMessage(i, "The player you revived isn't connected.");
							}
							if(!PlayerInfo[target][IsLoggedIn]) 
							{
								PlayerInfo[i][Reviving] = -1;
							    return SendErrorMessage(i, "The player you revived isn't logged in.");
							}
							if(PlayerInfo[i][Team] != PlayerInfo[target][Team]) 
							{
								PlayerInfo[i][Reviving] = -1;
							    return SendErrorMessage(i, "That player isn't in your team.");
							}
							if(PlayerInfo[i][State] != PLAYER_STATE_ALIVE) 
							{
								PlayerInfo[i][Reviving] = -1;
							    return SendErrorMessage(i, "You must be alive to be able to revive.");
							}
							if(!IsPlayerNearPlayer(i, target, 5.0)) 
							{
								PlayerInfo[i][Reviving] = -1;
							    return SendErrorMessage(i, "You aren't near that player."); 
							}
							if(PlayerInfo[target][State] != PLAYER_STATE_WOUNDED) 
							{
								PlayerInfo[i][Reviving] = -1;
							    return SendErrorMessage(i, "That player is alive.");
							}

                            if(IsPlayerInAnyVehicle(target)) RemovePlayerFromVehicle(target);

							PlayerInfo[target][State] = PLAYER_STATE_ALIVE;
							ClearAnimations(target);
							ClearAnimations(i);
							TogglePlayerControllable(target, true);

							SetPlayerChatBubble(target, "(( Revived ))", COLOR_GREY, 30.0, 500); 

							TogglePlayerControllable(i, true);
							SendClientMessage(target, COLOR_RED, "You are revived!");
							SendClientMessage(i, COLOR_ACTION, "Successfully revived."); 
							PlayerInfo[i][Reviving] = -1;
						}
					}

				}
				if(PlayerInfo[i][MedkitTime])
				{
					PlayerInfo[i][MedkitTime]--;
					if(PlayerInfo[i][MedkitTime] <= 1)
					{
						PlayerInfo[i][MedkitTime] = 0;

						if(PlayerInfo[i][State] != PLAYER_STATE_ALIVE) return SendErrorMessage(i, "You must be alive to be able to use a medkit.");

                        TogglePlayerControllable(i, true);
						ClearAnimations(i);
						SetPlayerHP(i, 100);
						SendClientMessage(i, COLOR_GREEN, "Medkit used! +100 HP.");
						PlayerInfo[i][BrokenLeg] = false;
					}
				}

				if(GetPlayerMoney(i) != PlayerInfo[i][Cash]) SetPlayerMoney(i, PlayerInfo[i][Cash]);

				if(GetPlayerScore(i) != PlayerInfo[i][Score]) SetPlayerScore(i, PlayerInfo[i][Score]);
			}

			if(GetPlayerState(i) == PLAYER_STATE_DRIVER)
			{
				if(GetPlayerWeapon(i) != 0 && GetPlayerWeapon(i) == 29 || GetPlayerWeapon(i) == 28 || GetPlayerWeapon(i) == 32 || GetPlayerWeapon(i) == 26)
				{
					SetPlayerArmedWeapon(i, 0);
				}
			}

			if(GetPlayerState(i) == PLAYER_STATE_PASSENGER)
			{
				if(GetPlayerWeapon(i) != 0 && GetPlayerWeapon(i) == 24)
				{
					SetPlayerArmedWeapon(i, 0);
				}
			}

			if(!IsMeleeWeapon(GetPlayerWeapon(i)) && GetPlayerAmmo(i) !=  sandac_GetPlayerAmmo(i)) SetPlayerAmmo(i, GetPlayerWeapon(i), sandac_GetPlayerAmmo(i));

			/*if(GetPlayerSkillLevel(i, WEAPONSKILL_PISTOL) != 0 || GetPlayerSkillLevel(i, WEAPONSKILL_MICRO_UZI) != 0 || GetPlayerSkillLevel(i, WEAPONSKILL_SPAS12_SHOTGUN) != 0)
			{
				SetPlayerSkillLevel(i, WEAPONSKILL_PISTOL, 0);
				SetPlayerSkillLevel(i, WEAPONSKILL_MICRO_UZI, 0);
				SetPlayerSkillLevel(i, WEAPONSKILL_SPAS12_SHOTGUN, 0);
			}*/
		}
	}
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);
	if(GetVehicleVirtualWorld(vehicleid) == 2) 
	{
		ResetVehicleWeapon(vehicleid);
		DestroyVehicle(vehicleid);
	}
	else SetVehicleToRespawn(vehicleid);
	return 1;
}

forward VehicleUpdate();
public VehicleUpdate()
{
	foreach ( new i : Vehicle)
	{
		if(IsValidVehicle(i))
		{
			new Float:vHP;
			GetVehicleHealth(i, vHP);

			if(vHP <= 300.0)
			{
				new engine, lights, alarm, doors, bonnet, boot, objective;
				GetVehicleParamsEx(i, engine, lights, alarm, doors, bonnet, boot, objective);
				SetVehicleParamsEx(i, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);
				if(GetVehicleVirtualWorld(i) == 2) 
				{
					SetVehicleHealth(i, 301.0);
				}
				else
				{
					SetVehicleHealth(i, 301.0);
				}
			}
		}
	}
	return 1;
}

forward ServerUpdate();
public ServerUpdate()
{
	new day, hour, minute, second, query[128];
	getdate(day, day, day);
	gettime(hour, minute, second);

	if(day == 7 && hour == 11 && minute == 59 && second >= 59)
	{
		foreach(new i : Player)
		{
			if(IsPlayerConnected(i) && PlayerInfo[i][AdminLevel])
			{
				for(new id = 1; id <= 3; id++)
				{
					PlayerInfo[i][StaffActivity][id - 1] = 0;
				}
				SendClientMessageEx(i, 0xbf9000FF, "[AdmActivity] Staff hours has been reset!");
			}
		}
		for(new idd = 1; idd <= 3; idd++)
		{
			mysql_format(g_SQL, query, sizeof query, "UPDATE `players` SET `activity_%d` = 0  WHERE `admin_level` != 0", idd);
			mysql_tquery(g_SQL, query);
		}
	}

	ServerInfo[Minute]++;
	if (ServerInfo[Minute] > 59)
	{
		ServerInfo[Minute] = 0;
		ServerInfo[Hour]++;
		if (ServerInfo[Hour] > 23)
		{
			ServerInfo[Hour] = 0;
			ServerInfo[Day]++;
			if (ServerInfo[Day] > 7)
			{
				ServerInfo[Day] = 1;
			}
		}
	}
	if(VoteInfo[VoteCooldown])
	{
		VoteInfo[VoteCooldown]--;
		if(VoteInfo[VoteCooldown] <= 1)
		{
			VoteInfo[VoteCooldown] = 0;
		}
	}
	if(VoteInfo[VoteID] != INVALID_PLAYER_ID)
    {
		VoteInfo[VoteTime]--;
		if(VoteInfo[VoteTime] < 1)
        {
			SendClientMessageToAllEx(COLOR_RED, "VOTEKICK: Vote ended.");

			VoteInfo[VoteID] = INVALID_PLAYER_ID;
			VoteInfo[VoteAmount] = 0;
			VoteInfo[VoteIssuer] = INVALID_PLAYER_ID;
			format(VoteInfo[VoteReason], 126, "NaN");
			VoteInfo[VoteMaxAmount] = 0;
			VoteInfo[VoteTime] = 0;
			foreach(new i : Player)
			{
				PlayerInfo[i][Vote] = 0;
			}
			VoteInfo[VoteCooldown] = 30;
		}
    }
	for(new i = 0; i < MAX_ZONES; i++)
	{
		if(ZoneInfo[i][zCapturedBy])
		{
			if(GetPlayersInZone(i, ZoneInfo[i][zCapturedBy]) >= 1)
			{
				if(ZoneInfo[i][zCaptureTime])
				{
					ZoneInfo[i][zCaptureTime]--;
					GangZoneFlashForAll(i, 0xFF634766);
				}
				else
				{
					foreach(new playerid : Player) 
					{
						if(IsPlayerConnected(playerid))
						{
							if(PlayerInfo[playerid][Team] == ZoneInfo[i][zCapturedBy])
							{
								SendClientMessageEx(playerid, COLOR_GREEN, "Your team has captured a territory. You've been given bonus score and $50!");
								PlayerInfo[playerid][Score] += 25;
								PlayerInfo[playerid][Cash] += 200;
							}
						}
					}
					GangZoneStopFlashForAll(i);
					ZoneInfo[i][zOwner] = ZoneInfo[i][zCapturedBy];
					ZoneInfo[i][zCaptureTime] = 0;
					ZoneInfo[i][zCapturedBy] = 0;
					ZoneInfo[i][zDeaths] = 0;
					GangZoneShowForAll(i, ReturnTeamZoneColor(ZoneInfo[i][zOwner]), 0x00000000);
				}
			}
			else
			{
				GangZoneStopFlashForAll(i);
				ZoneInfo[i][zCaptureTime] = 0;
				ZoneInfo[i][zCapturedBy] = 0;
				ZoneInfo[i][zDeaths] = 0;
			}
		}
	}
	for(new game; game < MAX_GAMES; game++)
	{
		new gameCount = 0;
		foreach(new playerid : Player)
		{
			if(PlayerInfo[playerid][DM] == game) gameCount++;
			
			GameInfo[game][GamePlayers] = gameCount;
		}
	}
	
	new temp_str[1096], primary_str[1096];
	strcat(primary_str, "DM Arenas (/dm)\n{FFFFFF}");
	format(temp_str, sizeof temp_str, "LVPD - %d/10 players\n", GameInfo[0][GamePlayers]);
	strcat(primary_str, temp_str);
	format(temp_str, sizeof temp_str, "Warehouse - %d/10 players\n", GameInfo[1][GamePlayers]);
	strcat(primary_str, temp_str);
	format(temp_str, sizeof temp_str, "Warehouse 2 - %d/10 players\n", GameInfo[2][GamePlayers]);
	strcat(primary_str, temp_str);
	format(temp_str, sizeof temp_str, "RC Battleground - %d/10 players\n", GameInfo[3][GamePlayers]);
	strcat(primary_str, temp_str);
	format(temp_str, sizeof temp_str, "Ghost Town (headshot only) - %d/10 players\n", GameInfo[4][GamePlayers]);
	strcat(primary_str, temp_str);

	UpdateDynamic3DTextLabelText(ArenaText, COLOR_RED, primary_str);

    format(primary_str, sizeof primary_str, "Copchase (/copchase)\n{FFFFFF}");
    if(CopchaseInfo[CC_Started] == false)
	{
		format(temp_str, sizeof temp_str, "%d/32 players waiting\n", GetLobbyPlayersCount());
	    strcat(primary_str, temp_str);
	}
	else 
	{
		format(temp_str, sizeof temp_str, "Game Started\n%d players playing\n", GetLobbyPlayersCount());
	    strcat(primary_str, temp_str);
	}

	UpdateDynamic3DTextLabelText(CopchaseText, COLOR_RED, primary_str);

	format(primary_str, sizeof primary_str, "Team Deathmatch (/tdm)\n{FFFFFF}");
	format(temp_str, sizeof temp_str, "%d player(s) playing\n", GetTDMPlayersCount());
	strcat(primary_str, temp_str);

	UpdateDynamic3DTextLabelText(TDMText, COLOR_RED, primary_str);
	return 1;
}

forward LeaderboardUpdate();
public LeaderboardUpdate()
{
	new query[103];
	mysql_format(g_SQL, query, sizeof query, "SELECT username, kills, skin FROM `players` ORDER BY kills DESC LIMIT 10");
	mysql_tquery(g_SQL, query, "OnLeaderboardUpdate");
	return 1;
}

forward OnLeaderboardUpdate();
public OnLeaderboardUpdate()
{
	if(cache_num_rows() > 0)
	{
		new temp_str[1096], primary_str[1096], kill, username[MAX_PLAYER_NAME], skin;
		strcat(primary_str, "-Top 10 kills-\n{FFFFFF}");
		for(new rows; rows < 10; rows++)
		{
			cache_get_value_int(rows, "kills", kill);
			cache_get_value_name(rows, "username", username);
			format(temp_str, sizeof temp_str, "%s - %d kills\n", username, kill);
			strcat(primary_str, temp_str);
		}
		cache_get_value_int(0, "skin", skin);
		SetActorSkin(LeaderboardActor, skin);
		UpdateDynamic3DTextLabelText(LeaderboardText, COLOR_RED, primary_str);
	}
	return 1;
}

forward _KickPlayerDelayed(playerid);
public _KickPlayerDelayed(playerid)
{
	PlayerTextDrawDestroy(playerid, td_kickBox[playerid]);
	PlayerTextDrawDestroy(playerid, td_kickBox2[playerid]);
	PlayerTextDrawDestroy(playerid, td_kickBox3[playerid]);
	PlayerTextDrawDestroy(playerid, td_kickReason[playerid]);
	Kick(playerid);
	return 1;
}

forward _FreezePlayer(playerid);
public _FreezePlayer(playerid)
{
	if(PlayerInfo[playerid][State] == PLAYER_STATE_ALIVE) TogglePlayerControllable(playerid, true);
	return 1;
}

//-----------------------------------------------------
DelayedKick(playerid, time = 500, bool:hud = false, const reason[] = " ")
{
	if(hud == true)
	{
		new string[256];
		strcat(string, "Reason: ");
		strcat(string, reason);

		for(new i; i < 25; i++) SendClientMessage(playerid, -1, " ");

		td_kickBox[playerid] = CreatePlayerTextDraw(playerid, 303.000000, -31.000000, "_");
		PlayerTextDrawFont(playerid, td_kickBox[playerid], 1);
		PlayerTextDrawLetterSize(playerid, td_kickBox[playerid], 0.600000, 79.100006);
		PlayerTextDrawTextSize(playerid, td_kickBox[playerid], 298.500000, 701.000000);
		PlayerTextDrawSetOutline(playerid, td_kickBox[playerid], 1);
		PlayerTextDrawSetShadow(playerid, td_kickBox[playerid], 0);
		PlayerTextDrawAlignment(playerid, td_kickBox[playerid], 2);
		PlayerTextDrawColor(playerid, td_kickBox[playerid], -1);
		PlayerTextDrawBackgroundColor(playerid, td_kickBox[playerid], 255);
		PlayerTextDrawBoxColor(playerid, td_kickBox[playerid], 208);
		PlayerTextDrawUseBox(playerid, td_kickBox[playerid], 1);
		PlayerTextDrawSetProportional(playerid, td_kickBox[playerid], 1);
		PlayerTextDrawSetSelectable(playerid, td_kickBox[playerid], 0);

		td_kickBox2[playerid] = CreatePlayerTextDraw(playerid, 411.000000, 159.000000, "~Y~YOU'VE BEEN KICKED");
		PlayerTextDrawFont(playerid, td_kickBox2[playerid], 3);
		PlayerTextDrawLetterSize(playerid, td_kickBox2[playerid], 0.524999, 2.499999);
		PlayerTextDrawTextSize(playerid, td_kickBox2[playerid], 400.000000, 129.000000);
		PlayerTextDrawSetOutline(playerid, td_kickBox2[playerid], 1);
		PlayerTextDrawSetShadow(playerid, td_kickBox2[playerid], 0);
		PlayerTextDrawAlignment(playerid, td_kickBox2[playerid], 3);
		PlayerTextDrawColor(playerid, td_kickBox2[playerid], -2686721);
		PlayerTextDrawBackgroundColor(playerid, td_kickBox2[playerid], 255);
		PlayerTextDrawBoxColor(playerid, td_kickBox2[playerid], 30);
		PlayerTextDrawUseBox(playerid, td_kickBox2[playerid], 0);
		PlayerTextDrawSetProportional(playerid, td_kickBox2[playerid], 1);
		PlayerTextDrawSetSelectable(playerid, td_kickBox2[playerid], 0);

		td_kickBox3[playerid] = CreatePlayerTextDraw(playerid, 384.000000, 181.000000, "-");
		PlayerTextDrawFont(playerid, td_kickBox3[playerid], 3);
		PlayerTextDrawLetterSize(playerid, td_kickBox3[playerid], 8.954160, 1.049999);
		PlayerTextDrawTextSize(playerid, td_kickBox3[playerid], 400.000000, 129.000000);
		PlayerTextDrawSetOutline(playerid, td_kickBox3[playerid], 0);
		PlayerTextDrawSetShadow(playerid, td_kickBox3[playerid], 0);
		PlayerTextDrawAlignment(playerid, td_kickBox3[playerid], 3);
		PlayerTextDrawColor(playerid, td_kickBox3[playerid], -741092353);
		PlayerTextDrawBackgroundColor(playerid, td_kickBox3[playerid], 255);
		PlayerTextDrawBoxColor(playerid, td_kickBox3[playerid], 30);
		PlayerTextDrawUseBox(playerid, td_kickBox3[playerid], 0);
		PlayerTextDrawSetProportional(playerid, td_kickBox3[playerid], 1);
		PlayerTextDrawSetSelectable(playerid, td_kickBox3[playerid], 0);

		td_kickReason[playerid] = CreatePlayerTextDraw(playerid, 321.000000, 192.000000, string);
		PlayerTextDrawFont(playerid, td_kickReason[playerid], 1);
		PlayerTextDrawLetterSize(playerid, td_kickReason[playerid], 0.233333, 1.100000);
		PlayerTextDrawTextSize(playerid, td_kickReason[playerid], 400.000000, 129.000000);
		PlayerTextDrawSetOutline(playerid, td_kickReason[playerid], 0);
		PlayerTextDrawSetShadow(playerid, td_kickReason[playerid], 0);
		PlayerTextDrawAlignment(playerid, td_kickReason[playerid], 2);
		PlayerTextDrawColor(playerid, td_kickReason[playerid], -741092353);
		PlayerTextDrawBackgroundColor(playerid, td_kickReason[playerid], 255);
		PlayerTextDrawBoxColor(playerid, td_kickReason[playerid], 30);
		PlayerTextDrawUseBox(playerid, td_kickReason[playerid], 0);
		PlayerTextDrawSetProportional(playerid, td_kickReason[playerid], 1);
		PlayerTextDrawSetSelectable(playerid, td_kickReason[playerid], 0);

		PlayerTextDrawShow(playerid, td_kickBox[playerid]);
		PlayerTextDrawShow(playerid, td_kickBox2[playerid]);
		PlayerTextDrawShow(playerid, td_kickBox3[playerid]);
		PlayerTextDrawShow(playerid, td_kickReason[playerid]);

		PlayerTextDrawHide(playerid, ui_stats[playerid]);
		PlayerTextDrawHide(playerid, ui_serverName[playerid]);

		TogglePlayerSpectating(playerid, true);
		TogglePlayerControllable(playerid, false);

        SetPlayerInterior(playerid, 0);
		InterpolateCameraPos(playerid, 2183.5325,-1752.2958,20.9256, 1821.3784,-1752.8165,29.0417,100000,CAMERA_MOVE);
		InterpolateCameraLookAt(playerid, 1821.3784,-1752.8165,29.0417, 1821.3784,-1752.8165,29.0417,100000,CAMERA_CUT);
		SetPlayerVirtualWorld(playerid, 7000);
		SetPlayerPos( playerid, 2183.5325,-1752.2958,-1);
		SetPlayerFacingAngle( playerid, 139.1213 );
	}

	SetTimerEx("_KickPlayerDelayed", time, false, "d", playerid);
	return 1;
}

FreezePlayer(playerid, time = 500)
{
    TogglePlayerControllable(playerid, false);
	SetTimerEx("_FreezePlayer", time, false, "d", playerid);
	return 1;
}

/*ReturnDay() 
{
	new result[32];
	if(ServerInfo[Day] == 1) format(result, sizeof(result), "Monday");
    else if(ServerInfo[Day] == 2) format(result, sizeof(result),"Tuesday");
    else if(ServerInfo[Day] == 3) format(result, sizeof(result),"Wednesday");
    else if(ServerInfo[Day] == 4) format(result, sizeof(result),"Thursday");
    else if(ServerInfo[Day] == 5) format(result, sizeof(result),"Friday");
    else if(ServerInfo[Day] == 6) format(result, sizeof(result),"Saturday");
    else if(ServerInfo[Day] == 7) format(result, sizeof(result),"Sunday");
	return result; 
}*/

ApplyAnimationEx(playerid, animlib[], animname[], Float:fDelta, loop, lockx, locky, freeze, time, forcesync = 0)
{
	for(new i =0; i <4; i++)
    	ApplyAnimation(playerid, animlib, animname, Float:fDelta, loop, lockx, locky, freeze, time, forcesync);	
	return 1;
}