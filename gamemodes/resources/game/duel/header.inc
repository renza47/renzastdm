enum E_DUEL
{
	DuelEnemy,
	DuelWeapon,
	DuelWeapon2,
	DuelBet,
	DuelMap,
	DuelTick
};
new DuelInfo[MAX_PLAYERS][E_DUEL];
new DuelCountdown[MAX_PLAYERS], DuelTimer[MAX_PLAYERS];