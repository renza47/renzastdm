AddPlayerDamage(playerid, const issuer[], weaponid, Float:amount, bodypart)
{
    for(new i; i < MAX_DAMAGES; i++)
    {
        if(DamageInfo[i][dmgExists] == false)
        {
            DamageInfo[i][dmgExists] = true;
            DamageInfo[i][dmgWeapon] = weaponid;
            DamageInfo[i][dmgDamaged] = playerid;
            format(DamageInfo[i][dmgIssuer], MAX_PLAYER_NAME, issuer);
            DamageInfo[i][dmgAmount] = amount;
            DamageInfo[i][dmgBody] = bodypart;
            return i;
        }
    }
    return 1;
}

ClearPlayerDamages(playerid)
{
    for(new i; i < MAX_DAMAGES; i++)
    {
        if(DamageInfo[i][dmgExists] == true && DamageInfo[i][dmgDamaged] == playerid)
        {
            DamageInfo[i][dmgExists] = false;
            DamageInfo[i][dmgDamaged] = -1;
            DamageInfo[i][dmgWeapon] = 0;
            format(DamageInfo[i][dmgIssuer], MAX_PLAYER_NAME, "NaN");
            DamageInfo[i][dmgAmount] = 0.0;
            DamageInfo[i][dmgBody] = 0;
        } 
    }
    return 1;
}

GetPlayerInjuries(playerid)
{
    new injuries;
    for(new i; i < MAX_DAMAGES; i++)
    {
        if(DamageInfo[i][dmgExists] == true && DamageInfo[i][dmgDamaged] == playerid)
        {
            injuries++;
        } 
    }
    return injuries;
}

GetBodyPartName(index)
{
    new bodypart[12];
    switch(index) 
    {
        case 3: format(bodypart, sizeof bodypart, "Torso");
        case 4: format(bodypart, sizeof bodypart, "Groin");
        case 5: format(bodypart, sizeof bodypart, "Left Arm");
        case 6: format(bodypart, sizeof bodypart, "Right Arm");
        case 7: format(bodypart, sizeof bodypart, "Left Leg");
        case 8: format(bodypart, sizeof bodypart, "Right Leg");
        case 9: format(bodypart, sizeof bodypart, "Head");
    }
    return bodypart;
}

GetWeaponIDByName(const weapon[])
{
    new wepid;
    if(strcmp(weapon, "Colt 45", true) == 0) wepid = 22;
    else if(strcmp(weapon, "Silenced 9mm", true) == 0) wepid = 23;
    else if(strcmp(weapon, "Desert Eagle", true) == 0) wepid = 24;
    else if(strcmp(weapon, "Shotgun", true) == 0) wepid = 25;
    else if(strcmp(weapon, "UZI", true) == 0) wepid = 28;
    else if(strcmp(weapon, "MP5", true) == 0) wepid = 29;
    else if(strcmp(weapon, "AK47", true) == 0) wepid = 30;
    else if(strcmp(weapon, "M4", true) == 0) wepid = 31;
    else if(strcmp(weapon, "Tec9", true) == 0) wepid = 32;
    else if(strcmp(weapon, "Rifle", true) == 0) wepid = 33;
    else if(strcmp(weapon, "Fist", true) == 0) wepid = 0;
    else if(strcmp(weapon, "Empty", true) == 0) wepid = 0;
    return wepid;
}

HitMarkerForPlayer(playerid, damagedid, weaponid, Float:amount, bool:informer = true)
{
    new string[128], wepname[32];
    GetWeaponName(weaponid, wepname, sizeof wepname);
    KillTimer(HitMarkerTimer[playerid]);

    if(informer == true)
    {
        if(PlayerInfo[playerid][Tog][3] == 0)
        {
            format(string, sizeof string, "~g~+%.1f - %s", amount, wepname);
            PlayerTextDrawSetString(playerid, ui_DamageGiven[playerid], string);
            PlayerTextDrawShow(playerid, ui_DamageGiven[playerid]);
        }
        if(PlayerInfo[damagedid][Tog][3] == 0)
        {
            format(string, sizeof string, "~r~-%.1f - %s", amount, wepname);
            PlayerTextDrawSetString(damagedid, ui_DamageTaken[damagedid], string);
            PlayerTextDrawShow(damagedid, ui_DamageTaken[damagedid]);
        }
    }

    if(PlayerInfo[playerid][HitMarker]) PlayerTextDrawShow(playerid, ui_hitmarker[playerid]);
    HitMarkerTimer[playerid] = SetTimerEx("HideHitMarker", 1000, false, "ii", playerid, damagedid);
    return 1;
}

forward HideHitMarker(playerid, damagedid);
public HideHitMarker(playerid, damagedid)
{
    PlayerTextDrawHide(playerid, ui_DamageGiven[playerid]);
    PlayerTextDrawHide(damagedid, ui_DamageTaken[damagedid]);
    PlayerTextDrawHide(playerid, ui_hitmarker[playerid]);
    return 1;
}