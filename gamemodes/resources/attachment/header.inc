enum E_WEAPONS
{
    Float:Position[6],
    Bone,
    Hidden
}
new WeaponInfo[MAX_PLAYERS][17][E_WEAPONS], WeaponTick[MAX_PLAYERS], EditingWeapon[MAX_PLAYERS];