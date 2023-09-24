cmd:attachment(playerid, params[])
{
    new weaponid = GetPlayerWeapon(playerid), option[32];

    if (!weaponid)
        return SendErrorMessage(playerid, "You aren't holding a weapon.");

    if (!IsWeaponWearable(weaponid))
        return SendErrorMessage(playerid, "This weapon cannot be edited.");

    if (sscanf(params, "s[32]", option)) return SendUsageMessage(playerid, "/attachment [bone/hide/position]");

    if (!strcmp(params, "position", true))
    {
        if(IsWeaponWearable(weaponid))
        {
            if (EditingWeapon[playerid])
                return SendErrorMessage(playerid, "You are already editing a weapon.");

            if (WeaponInfo[playerid][weaponid - 22][Hidden])
                return SendErrorMessage(playerid, "You cannot adjust a hidden weapon.");

            new index = weaponid - 22;

            SetPlayerArmedWeapon(playerid, 0);

            SetPlayerAttachedObject(playerid, GetWeaponObjectSlot(weaponid), GetWeaponModel(weaponid), WeaponInfo[playerid][index][Bone], WeaponInfo[playerid][index][Position][0], WeaponInfo[playerid][index][Position][1], WeaponInfo[playerid][index][Position][2], WeaponInfo[playerid][index][Position][3], WeaponInfo[playerid][index][Position][4], WeaponInfo[playerid][index][Position][5], 1.0, 1.0, 1.0);
            EditAttachedObject(playerid, GetWeaponObjectSlot(weaponid));

            EditingWeapon[playerid] = weaponid;
        }
    }
    else if (!strcmp(params, "bone", true))
    {
        if(IsWeaponWearable(weaponid))
        {
            ShowPlayerDialog(playerid, DIALOG_EDIT_BONE, DIALOG_STYLE_LIST, "Weapon Attachment", "Spine\nHead\nLeft upper arm\nRight upper arm\nLeft hand\nRight hand\nLeft thigh\nRight thigh\nLeft foot\nRight foot\nRight calf\nLeft calf\nLeft forearm\nRight forearm\nLeft shoulder\nRight shoulder\nNeck\nJaw", "Choose", "Cancel");
            EditingWeapon[playerid] = weaponid;
        }
    }
    else if (!strcmp(params, "hide", true))
    {
        if (EditingWeapon[playerid])
            return SendErrorMessage(playerid, "You cannot hide a weapon while you're' editing it.");

        new index = weaponid - 22, weaponname[18], string[48];

        GetWeaponName(weaponid, weaponname, sizeof(weaponname));

        if (WeaponInfo[playerid][index][Hidden])
        {
            format(string, sizeof(string), "You have set your %s to show.", weaponname);
            WeaponInfo[playerid][index][Hidden] = false;
        }
        else
        {
            if (IsPlayerAttachedObjectSlotUsed(playerid, GetWeaponObjectSlot(weaponid)))
                RemovePlayerAttachedObject(playerid, GetWeaponObjectSlot(weaponid));

            format(string, sizeof(string), "You have set your %s not to show.", weaponname);
            WeaponInfo[playerid][index][Hidden] = true;
        }
        SendClientMessage(playerid, COLOR_GREEN, string);
    }
    else SendErrorMessage(playerid, "You have specified an invalid option.");
    return 1;
}