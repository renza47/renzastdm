new vehName[][] =       // array for vehicle names to be displayed
{
    "Landstalker", "Bravura", "Buffalo", "Linerunner", "Perrenial", "Sentinel", "Dumper", "Firetruck", "Trashmaster",
    "Stretch", "Manana", "Infernus", "Voodoo", "Pony", "Mule", "Cheetah", "Ambulance", "Leviathan", "Moonbeam",
    "Esperanto", "Taxi", "Washington", "Bobcat", "Whoopee", "BF Injection", "Hunter", "Premier", "Enforcer",
    "Securicar", "Banshee", "Predator", "Bus", "Rhino", "Barracks", "Hotknife", "Trailer", "Previon", "Coach",
    "Cabbie", "Stallion", "Rumpo", "RC Bandit", "Romero", "Packer", "Monster", "Admiral", "Squalo", "Seasparrow",
    "Pizzaboy", "Tram", "Trailer", "Turismo", "Speeder", "Reefer", "Tropic", "Flatbed", "Yankee", "Caddy", "Solair",
    "Berkley's RC Van", "Skimmer", "PCJ-600", "Faggio", "Freeway", "RC Baron", "RC Raider", "Glendale", "Oceanic",
    "Sanchez", "Sparrow", "Patriot", "Quad", "Coastguard", "Dinghy", "Hermes", "Sabre", "Rustler", "ZR-350", "Walton",
    "Regina", "Comet", "BMX", "Burrito", "Camper", "Marquis", "Baggage", "Dozer", "Maverick", "News Chopper", "Rancher",
    "Rancher", "Virgo", "Greenwood", "Jetmax", "Hotring", "Sandking", "Blista Compact", "Police Maverick",
    "Boxville", "Benson", "Mesa", "RC Goblin", "Hotring Racer A", "Hotring Racer B", "Bloodring Banger", "Rancher",
    "Super GT", "Elegant", "Journey", "Bike", "Mountain Bike", "Beagle", "Cropduster", "Stunt", "Tanker", "Roadtrain",
    "Nebula", "Majestic", "Buccaneer", "Shamal", "Hydra", "FCR-900", "NRG-500", "HPV1000", "Cement Truck", "Tow Truck",
    "Fortune", "Cadrona", "SWAT Truck", "Willard", "Forklift", "Tractor", "Combine", "Feltzer", "Remington", "Slamvan",
    "Blade", "Streak", "Freight", "Vortex", "Vincent", "Bullet", "Clover", "Sadler", "Firetruck", "Hustler", "Intruder",
    "Primo", "Cargobob", "Tampa", "Sunrise", "Merit", "Utility", "Nevada", "Yosemite", "Windsor", "Monster", "Monster",
    "Uranus", "Jester", "Sultan", "Stratium", "Elegy", "Raindance", "RC Tiger", "Flash", "Tahoma", "Savanna", "Bandito",
    "Freight Flat", "Streak Carriage", "Kart", "Mower", "Dune", "Sweeper", "Broadway", "Tornado", "AT-400", "DFT-30",
    "Huntley", "Stafford", "BF-400", "News Van", "Tug", "Trailer", "Emperor", "Wayfarer", "Euros", "Hotdog", "Club",
    "Freight Box", "Trailer", "Andromada", "Dodo", "RC Cam", "Launch", "LSPD Car", "SFPD Car", "LVPD Car",
    "Police Rancher", "Picador", "S.W.A.T", "Alpha", "Phoenix", "Glendale", "Sadler", "Luggage", "Luggage", "Stairs",
    "Boxville", "Tiller", "Utility Trailer"
};

new const VehicleSeats[] = {
    4,2,2,2,4,4,1,2,2,4,2,2,2,4,2,2,4,2,4,2,4,4,2,2,2,1,4,4,4,2,1,7,1,2,2,0,2,7,4,2,4,1,2,2,2,4,1,2,1,0,0,2,1,1,1,2,2,2,4,4,
    2,2,2,2,1,1,4,4,2,2,4,2,1,1,2,2,1,2,2,4,2,1,4,3,1,1,1,4,2,2,4,2,4,1,2,2,2,4,4,2,2,1,2,2,2,2,2,4,2,1,1,2,1,1,2,2,4,2,2,1,
    1,2,2,2,2,2,2,2,2,4,1,1,1,2,2,2,2,7,7,1,4,2,2,2,2,2,4,4,2,2,4,4,2,1,2,2,2,2,2,2,4,4,2,2,1,2,4,4,1,0,0,1,1,2,1,2,2,1,2,4,
    4,2,4,1,0,4,2,2,2,2,0,0,7,2,2,1,4,4,4,2,2,2,2,2,4,2,0,0,0,4,0,0
};

stock GetVehicleNameByModel(modelid)
{
    new output[64];
    if(modelid) format(output, sizeof output, vehName[modelid - 400]);
    return output;
}

stock GetVehicleSeats(vehicleid)
{
    new output = VehicleSeats[GetVehicleModel(vehicleid) - 400];
    return output;
}

stock IsVehicleSeatTaken(vehicleid, seatid)
{
    if((seatid + 1) > GetVehicleSeats(vehicleid)) return 1;

    foreach(new i : Player)
    {
        if(IsPlayerConnected(i))
        {
            if(GetPlayerVehicleID(i) == vehicleid)
            {
                if(GetPlayerVehicleSeat(i) == seatid) return 1;
            }
        }
    }
    return 0;
}

stock GetVehicleSpeed(vehicleid, bool:kmh = false)
{
    new
        Float:x,
        Float:y,
        Float:z,
        Float:speed;
        
    GetVehicleVelocity(vehicleid, x, y, z);
    speed = VectorSize(x, y, z);
    
    return kmh ? floatround(speed * 195.12) : floatround(speed * 121.30); 
}

stock ToggleVehicleEngine(vehicleid, toggle)
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, toggle, lights, alarm, doors, bonnet, boot, objective);
	return true;
}
stock ToggleVehicleLights(vehicleid, toggle)
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, engine, toggle, alarm, doors, bonnet, boot, objective);
	return true;
}
stock ToggleVehicleTrunk(vehicleid, toggle)
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, toggle, objective);
	return true;
}
stock ToggleVehicleBonnet(vehicleid, toggle)
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, toggle, boot, objective);
	return true;
}
stock ToggleVehicleLock(vehicleid, toggle)
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, toggle, bonnet, boot, objective);
	return true;
}

ResetVehicleWeapon(vehicleid)
{
    for(new i; i < 10; i++)
    {
        VehicleInfo[vehicleid][vGun][i] = 0;
        VehicleInfo[vehicleid][vGunAmmo][i] = 0;
    }
    return 1;
}

GetClosestVehicle(playerid)
{
	static
	    Float:fX,
	    Float:fY,
	    Float:fZ;

	for (new i = 0; i != MAX_VEHICLES; i ++) if (IsValidVehicle(i)) {
		GetVehiclePos(i, fX, fY, fZ);

		if (IsPlayerInRangeOfPoint(playerid, 3.0, fX, fY, fZ) && GetPlayerVirtualWorld(playerid) == GetVehicleVirtualWorld(i)) {
		    return i;
		}
	}
	return -1;
}

IsValidWeapon(weaponid)
{
	if (weaponid < 1 || weaponid > 46 || (weaponid > 18 && weaponid < 22)) return false;

	return true;
}
