#include <YSI_Coding\y_hooks>

hook OnPlayerDisconnect(playerid, reason)
{
    if(IsValidVehicle(PlayerInfo[playerid][FreeroamVehicle])) 
    {
        DestroyVehicle(PlayerInfo[playerid][FreeroamVehicle]);
        PlayerInfo[playerid][FreeroamVehicle] = INVALID_VEHICLE_ID;
    }
    return 1;
}

hook OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	new Float:hp, Float:ar;
    GetPlayerHP(damagedid, hp);
    GetPlayerArmor(damagedid, ar);
	if(GetPlayerGameMode(playerid) == GAME_MODE_FREEROAM)
    {
        if(PlayerInfo[damagedid][AdminDuty] == true)
        {
            amount = 0;
            SetPlayerArmor(damagedid, ar);
            return SetPlayerHP(damagedid, hp);
        }

        if(PlayerInfo[damagedid][DamageFix]) 
        {
            amount = 0;
            SetPlayerArmor(damagedid, ar);
            return SetPlayerHP(damagedid, 30);
        }

        UpdateNameTag(damagedid, true);

        if(PlayerInfo[playerid][HitSound]) PlayerPlaySound(playerid, 17802, 0.0, 0.0, 0.0);

        HitMarkerForPlayer(playerid, damagedid, weaponid, amount);

        if(ar > 1)
        {
            SetPlayerArmor(damagedid, ar-amount);
            if(ar - amount <= 6)
            {
                SetPlayerArmor(damagedid, 0);
            }
        }
        else
        {
            SetPlayerArmor(damagedid, ar);
            SetPlayerHP(damagedid, hp-amount);
            AddPlayerDamage(damagedid, PlayerInfo[playerid][Name], weaponid, amount, bodypart);
        }

        if(hp - amount <= 6)
        {
            if(PlayerInfo[damagedid][State] == PLAYER_STATE_ALIVE)
            {
                if(bodypart != 9) ApplyAnimation(damagedid, "PED","KO_shot_front", 4.1, 0, 0, 0, 1, 0, 1);
                else ApplyAnimation(damagedid, "PED", "KO_shot_face", 4.1, 0, 0, 0, 1, 0, 1);

                new
                    s_Name[MAX_PLAYER_NAME],
                    s_Str[144]
                ;
                
                //signature for player (the one who died)
                GetPlayerName(playerid, s_Name, MAX_PLAYER_NAME);
                format(s_Str, sizeof(s_Str), "You got killed by ~r~%s(%i)", s_Name, playerid);

                PlayerTextDrawSetString(damagedid, PlayerInfo[damagedid][i_Textdraw][0], s_Str);
                PlayerTextDrawShow(damagedid, PlayerInfo[damagedid][i_Textdraw][0]);

                ShowPlayerSignature(damagedid, playerid, 3000);

                GetPlayerName(damagedid, s_Name, MAX_PLAYER_NAME);
                format(s_Str, sizeof(s_Str), "You killed ~r~%s(%i)", s_Name, damagedid);

                PlayerTextDrawSetString(playerid, PlayerInfo[playerid][i_Textdraw][0], s_Str);
                PlayerTextDrawShow(playerid, PlayerInfo[playerid][i_Textdraw][0]);

                if(PlayerInfo[playerid][i_ExpireTimer] != -1)
                {
                    KillTimer(PlayerInfo[playerid][i_ExpireTimer]);
                    PlayerInfo[playerid][i_ExpireTimer] = -1;
                }

                PlayerInfo[playerid][i_ExpireTimer] = SetTimerEx("OnPlayerSignatureExpire", 3000, false, "i", playerid);
                
                PlayerInfo[damagedid][State] = PLAYER_STATE_WOUNDED;
                PlayerInfo[damagedid][DamageFix] = 4;

                PlayerInfo[damagedid][KillStreak] = 0;

                PlayerInfo[damagedid][MedkitTime] = 0;

                ClearPlayerDamages(damagedid);

                SetPlayerSkillLevel(damagedid, WEAPONSKILL_PISTOL, 0);
                SetPlayerSkillLevel(damagedid, WEAPONSKILL_MICRO_UZI, 0);
                SetPlayerSkillLevel(damagedid, WEAPONSKILL_SPAS12_SHOTGUN, 0);

                foreach(new i : Player)
                {
                    if(GetPlayerGameMode(i) == GetPlayerGameMode(playerid)) SendDeathMessageToPlayer(i, playerid, damagedid, weaponid);
                }
                SetPlayerHP(damagedid, 30);
                return 1;
            }
        }
    }
	return 1;
}