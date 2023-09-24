cmd:engine(playerid, params[])
{
    if(PlayerInfo[playerid][State] == PLAYER_STATE_WOUNDED) return 1;

    new vehicleid = GetPlayerVehicleID(playerid), engine, lights, alarm, doors, bonnet, boot, objective;
    if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendErrorMessage(playerid, "You must be in a vehicle.");
    
    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

    new Float:vHP;
    GetVehicleHealth(vehicleid, vHP);

    if(vHP <= 300.0) return SendErrorMessage(playerid, "The vehicle engine is broken.");

    if(VehicleInfo[vehicleid][vTeam] != 0 && VehicleInfo[vehicleid][vTeam] != PlayerInfo[playerid][CC_Team]) return SendErrorMessage(playerid, "You don't have the key for the vehicle.");

    if (engine == VEHICLE_PARAMS_UNSET || engine == VEHICLE_PARAMS_OFF)
    {
        SendNearbyMessage(playerid, 20.0, 0xC2A2DAFF, "** %s {C2A2DA}starts the vehicle engine.", ReturnName(playerid));
        SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective);
    }
    else 
    {
        SendNearbyMessage(playerid, 20.0, 0xC2A2DAFF, "** %s {C2A2DA}stopped the vehicle engine.", ReturnName(playerid));
        SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);
    }
    return 1;
}

cmd:lights(playerid, params[])
{
    if(PlayerInfo[playerid][State] == PLAYER_STATE_WOUNDED) return 1;

    new vehicleid = GetPlayerVehicleID(playerid), engine, lights, alarm, doors, bonnet, boot, objective;
    if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendErrorMessage(playerid, "You must be in a vehicle.");
    
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
    return 1;
}

cmd:lock(playerid, params[])
{
    if(GetPlayerGameMode(playerid) != GAME_MODE_COPCHASE) return SendErrorMessage(playerid, "This command can only be used in copchase.");

    if(PlayerInfo[playerid][State] != PLAYER_STATE_ALIVE) return SendErrorMessage(playerid, "You must be alive to use this command.");

    new Float:vehPos[3], vehID = GetClosestVehicle(playerid), engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehiclePos(vehID, vehPos[0], vehPos[1], vehPos[2]);

	if (GetPlayerDistanceFromPoint(playerid, vehPos[0], vehPos[1], vehPos[2]) > 3.0) return SendErrorMessage(playerid, "You aren't near a vehicle.");

    if(VehicleInfo[vehID][vTeam] != 0 && VehicleInfo[vehID][vTeam] != PlayerInfo[playerid][CC_Team]) return SendErrorMessage(playerid, "You don't have the key for the vehicle.");

    GetVehicleParamsEx(vehID, engine, lights, alarm, doors, bonnet, boot, objective);
    if(!doors)
    {
        ToggleVehicleLock(vehID, true);
        SendNearbyMessage(playerid, 20.0, 0xC2A2DAFF, "** %s {C2A2DA}locks their vehicle.", ReturnName(playerid));
        GameTextForPlayer(playerid, "~r~VEHICLE LOCKED", 3000, 3);
    }
    else if(doors)
    {
        ToggleVehicleLock(vehID, false);
        SendNearbyMessage(playerid, 20.0, 0xC2A2DAFF, "** %s {C2A2DA}unlocks their vehicle.", ReturnName(playerid));
        GameTextForPlayer(playerid, "~g~VEHICLE UNLOCKED", 3000, 3);
    }
    return 1;
}