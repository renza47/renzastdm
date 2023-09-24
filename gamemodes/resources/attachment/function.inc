GetWeaponObjectSlot(weaponid)
{
    new objectslot;

    switch (weaponid)
    {
        case 22..24: objectslot = 5;
        case 25..27: objectslot = 6;
        case 28, 29, 32: objectslot = 7;
        case 30, 31: objectslot = 6;
        case 33, 34: objectslot = 6;
    }
    return objectslot;
}

GetWeaponModel(weaponid) //Will only return the model of wearable weapons (22-38)
{
    new model;

    switch(weaponid)
    {
        case 22..29: model = 324 + weaponid;
        case 30: model = 355;
        case 31: model = 356;
        case 32: model = 372;
        case 33: model = 357;
        case 34: model = 358;
    }
    return model;
}

_PlayerHasWeapon(playerid, weaponid)
{
    new weapon, ammo;

    for (new i; i < 13; i++)
    {
        GetPlayerWeaponData(playerid, i, weapon, ammo);
        if (weapon == weaponid && ammo) return 1;
    }
    return 0;
}

IsWeaponWearable(weaponid)
    return (weaponid >= 22 && weaponid <= 34);