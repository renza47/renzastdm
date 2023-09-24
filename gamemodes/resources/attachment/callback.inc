#include <YSI_Coding\y_hooks>

hook OnPlayerUpdate(playerid)
{
    if (NetStats_GetConnectedTime(playerid) - WeaponTick[playerid] >= 250)
    {
        new weaponid, ammo, objectslot, count, index;

        for (new i = 2; i <= 7; i++) //Loop only through the slots that may contain the wearable weapons
        {
            GetPlayerWeaponData(playerid, i, weaponid, ammo);
            index = weaponid - 22;

            if (weaponid && ammo && !WeaponInfo[playerid][index][Hidden] && IsWeaponWearable(weaponid) && EditingWeapon[playerid] != weaponid)
            {
                objectslot = GetWeaponObjectSlot(weaponid);

                if (GetPlayerWeapon(playerid) != weaponid)
                    SetPlayerAttachedObject(playerid, objectslot, GetWeaponModel(weaponid), WeaponInfo[playerid][index][Bone], WeaponInfo[playerid][index][Position][0], WeaponInfo[playerid][index][Position][1], WeaponInfo[playerid][index][Position][2], WeaponInfo[playerid][index][Position][3], WeaponInfo[playerid][index][Position][4], WeaponInfo[playerid][index][Position][5], 1.0, 1.0, 1.0);

                else if (IsPlayerAttachedObjectSlotUsed(playerid, objectslot)) RemovePlayerAttachedObject(playerid, objectslot);
            }
        }
        for (new i; i >= 5; i++) if (IsPlayerAttachedObjectSlotUsed(playerid, i))
        {
            count = 0;

            for (new j = 22; j <= 34; j++) if (_PlayerHasWeapon(playerid, j) && GetWeaponObjectSlot(j) == i)
                count++;

            if (!count) RemovePlayerAttachedObject(playerid, i);
        }
        WeaponTick[playerid] = NetStats_GetConnectedTime(playerid);
    }
    return 1;
}

hook OnPlayerConnect(playerid)
{
    for (new i; i < 17; i++)
    {
        WeaponInfo[playerid][i][Position][0] = -0.116;
        WeaponInfo[playerid][i][Position][1] = -0.164;
        WeaponInfo[playerid][i][Position][2] = 0.088;
        WeaponInfo[playerid][i][Position][3] = 0.0;
        WeaponInfo[playerid][i][Position][4] = 44.50;
        WeaponInfo[playerid][i][Position][5] = 0.0;
        WeaponInfo[playerid][i][Bone] = 0;
        WeaponInfo[playerid][i][Hidden] = false;
    }
    WeaponTick[playerid] = 0;
    EditingWeapon[playerid] = 0;
	return 1;
}

hook OnPlayerDisconnect(playerid)
{
    for (new i; i < 17; i++)
    {
        WeaponInfo[playerid][i][Position][0] = -0.116;
        WeaponInfo[playerid][i][Position][1] = -0.164;
        WeaponInfo[playerid][i][Position][2] = 0.088;
        WeaponInfo[playerid][i][Position][3] = 0.0;
        WeaponInfo[playerid][i][Position][4] = 44.50;
        WeaponInfo[playerid][i][Position][5] = 0.0;
        WeaponInfo[playerid][i][Bone] = 0;
        WeaponInfo[playerid][i][Hidden] = false;
    }
    WeaponTick[playerid] = 0;
    EditingWeapon[playerid] = 0;
	return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        case DIALOG_EDIT_BONE:
        {
            if (response)
            {
                new weaponid = EditingWeapon[playerid], weaponname[18], string[68];

                GetWeaponName(weaponid, weaponname, sizeof(weaponname));
                WeaponInfo[playerid][weaponid - 22][Bone] = listitem + 1;
                WeaponInfo[playerid][weaponid - 22][Position][0] = -0.116;
                WeaponInfo[playerid][weaponid - 22][Position][1] = 0.175;
                WeaponInfo[playerid][weaponid - 22][Position][2] = 0.088;
                WeaponInfo[playerid][weaponid - 22][Position][3] = 0.0;
                WeaponInfo[playerid][weaponid - 22][Position][4] = 45.0;
                WeaponInfo[playerid][weaponid - 22][Position][5] = 0.0;

                format(string, sizeof(string), "You have successfully changed the bone of your %s.", weaponname);
                SendClientMessage(playerid, COLOR_GREEN, string);
            }
            EditingWeapon[playerid] = 0;
        }
    }
    return 1;
}

hook OnPlayerEditAttachedObject(playerid, response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ)
{
    new weaponid = EditingWeapon[playerid];
    if (response)
    {
        if (weaponid)
        {
            new enum_index = weaponid - 22, weaponname[18];
            GetWeaponName(weaponid, weaponname, sizeof(weaponname));
            WeaponInfo[playerid][enum_index][Position][0] = fOffsetX;
            WeaponInfo[playerid][enum_index][Position][1] = fOffsetY;
            WeaponInfo[playerid][enum_index][Position][2] = fOffsetZ;
            WeaponInfo[playerid][enum_index][Position][3] = fRotX;
            WeaponInfo[playerid][enum_index][Position][4] = fRotY;
            WeaponInfo[playerid][enum_index][Position][5] = fRotZ;
            EditingWeapon[playerid] = 0;
            SetPlayerAttachedObject(playerid, GetWeaponObjectSlot(weaponid), GetWeaponModel(weaponid), WeaponInfo[playerid][enum_index][Bone], fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, 1.0, 1.0, 1.0);
            SendClientMessageEx(playerid, COLOR_GREEN, "You've successfully set the position of your %s.", weaponname);
        }
    }
    return 0;
}