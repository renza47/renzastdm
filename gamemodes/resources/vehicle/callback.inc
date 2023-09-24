#include <YSI_Coding\y_hooks>

hook OnGameModeInit()
{
	// LOS SANTOS
    LoadStaticVehiclesFromFile("vehicles/ls_law.txt");
    LoadStaticVehiclesFromFile("vehicles/ls_airport.txt");
    LoadStaticVehiclesFromFile("vehicles/ls_gen_inner.txt");
    LoadStaticVehiclesFromFile("vehicles/ls_gen_outer.txt");


	// sbf
	AddStaticVehicleEx(405,2785.1057,-2000.0519,13.3041,269.7271,39,1,60);
	AddStaticVehicleEx(405,2774.5493,-1969.9019,13.3023,359.0316,39,1,60);
	AddStaticVehicleEx(405,2774.7444,-1933.3691,13.2753,359.4951,39,1,60);
	AddStaticVehicleEx(405,2794.5205,-1944.4742,13.4219,89.8047,75,1,60); 
	AddStaticVehicleEx(405,2764.6831,-1931.9960,13.2798,180.5796,75,1,60);
	AddStaticVehicleEx(405,2764.3423,-1970.9294,13.3035,179.7076,75,1,60);
	AddStaticVehicleEx(405,2731.0615,-1970.3612,13.4219,270.3466,75,1,60);

	AddStaticVehicleEx(492,2509.6294,-1666.6212,13.2637,9.0844,7, 1,60); // gsf car
	AddStaticVehicleEx(566,2199.3298,-1225.4409,23.6685,270.3154,30,8,60); // rhb vehicle
	AddStaticVehicleEx(566,2251.6025,-1225.8481,23.6703,269.1742,30,8,60); // rhb vehicle
	AddStaticVehicleEx(566,2242.2659,-1236.4421,24.5554,359.7455,30,8,60); // rhb vehicle
	AddStaticVehicleEx(566,2246.0845,-1215.9581,23.6640,89.5084,30,8,60); // rhb vehicle
	AddStaticVehicleEx(566,2215.2451,-1215.8320,23.6735,89.5142,30,8,60); // rhb vehicle
	AddStaticVehicleEx(566,2193.4707,-1215.8241,23.6653,89.2372,30,8,60); // rhb vehicle
	AddStaticVehicleEx(566,2223.5776,-1236.5197,24.4606,359.5408,30,8,60); // rhb vehicle
	AddStaticVehicleEx(566,1923.8318,-1130.8458,24.7298,89.4305,30,8,60); // ktb vehicle (31, 31)
	AddStaticVehicleEx(566,1906.1351,-1130.6653,24.2888,89.4205,30,8,60); // ktb vehicle (31, 31)
	AddStaticVehicleEx(566,1886.5116,-1130.8569,23.7937,88.6358,30,8,60); // ktb vehicle (31, 31)
	AddStaticVehicleEx(566,1914.5245,-1140.8492,24.4949,269.8146,30,8,60); // ktb vehicle (31, 31)
	AddStaticVehicleEx(566,1894.3549,-1140.7842,23.9903,269.9606,30,8,60); // ktb vehicle (31, 31)
	AddStaticVehicleEx(566,2186.0498,-1667.5831,14.4009,347.2808,30,8,60); // fyb veh
	AddStaticVehicleEx(566,2182.6560,-1682.6230,13.6950,347.2183,30,8,60); // fyb veh
	AddStaticVehicleEx(566,2180.8667,-1699.1919,13.3734,355.8385,30,8,60); // fyb veh
	AddStaticVehicleEx(566,2179.8350,-1714.8027,13.2039,358.4282,30,8,60); // fyb veh
	AddStaticVehicleEx(566,1276.8405,-918.5677,41.7846,97.0915,30,8,60); // tdb veh
	AddStaticVehicleEx(566,1259.6559,-920.9862,42.3928,98.7476,30,8,60); // tdb veh
	AddStaticVehicleEx(566,1236.2811,-925.3774,42.3926,101.3790,30,8,60); // tdb veh
	AddStaticVehicleEx(492,1077.3540,-1087.5969,25.9077,179.8733,7,1,60); // tdf veh
	AddStaticVehicleEx(492,1077.3378,-1070.4209,27.9314,179.7958,7,1,60); // tdf veh
	AddStaticVehicleEx(492,1077.4172,-1113.0009,24.0359,179.9433,7,1,60); // tdf veh
	AddStaticVehicleEx(492,1087.7805,-1113.0424,24.0345,359.5824,7,1,60); // tdf veh
	AddStaticVehicleEx(492,1087.7281,-1093.4186,25.2409,0.0684,7,1,60); // tdf veh
	AddStaticVehicleEx(492,1087.6766,-1075.1155,27.3279,0.0049,7,1,60); // tdf veh
	AddStaticVehicleEx(492,1108.3750,-1078.6487,29.1812,90.5249,7,1,60); // tdf veh
	AddStaticVehicleEx(467,2445.7922,-1272.6169,23.6481,179.8352,61,61,60); // lsv veh (kuning gelap)
	AddStaticVehicleEx(467,2445.7144,-1298.9614,23.6475,179.5936,61,61,60); // lsv veh (kuning gelap)
	AddStaticVehicleEx(467,2445.9485,-1330.8607,23.6477,179.7248,61,61,60); // lsv veh (kuning gelap)
	AddStaticVehicleEx(467,2445.9829,-1353.5244,23.6477,180.0533,61,61,60); // lsv veh (kuning gelap)
	AddStaticVehicleEx(467,2445.9944,-1384.7323,23.6477,180.0770,61,61,60); // lsv veh (kuning gelap)
	AddStaticVehicleEx(467,2456.1770,-1353.6974,23.6524,359.8295,61,61,60); // lsv veh (kuning gelap)
	AddStaticVehicleEx(467,2456.1770,-1324.1310,23.6540,359.8525,61,61,60); // lsv veh (kuning gelap)
	AddStaticVehicleEx(467,2456.1758,-1291.9723,23.6540,359.8215,61,61,60); // lsv veh (kuning gelap)
	AddStaticVehicleEx(467,2467.1960,-1311.1621,25.3614,87.8431,61,61,60); // lsv veh (kuning gelap)
	AddStaticVehicleEx(467,1909.2971,-2047.5201,13.2009,89.8750,87,87,60); // vla veh (cyan gelap)
	AddStaticVehicleEx(467,1862.8766,-2047.8390,13.2006,89.8183,87,87,60); // vla veh (cyan gelap)
	AddStaticVehicleEx(467,1888.0100,-2039.1066,13.2050,180.2430,87,87,60); // vla veh (cyan gelap)
	AddStaticVehicleEx(467,1887.9253,-2022.9309,13.2015,180.2540,87,87,60); // vla veh (cyan gelap)
	AddStaticVehicleEx(467,1877.9767,-2024.4019,13.2015,179.4252,87,87,60); // vla veh (cyan gelap)
	AddStaticVehicleEx(467,1878.0531,-2036.7438,13.2033,180.2234,87,87,60); // vla veh (cyan gelap)
	AddStaticVehicleEx(467,1901.2362,-2057.7517,13.2009,268.8854,87,87,60); // vla veh (cyan gelap)
    return 1;
}

hook OnPlayerStateChange(playerid, newstate, oldstate)
{
    new vehicleid = GetPlayerVehicleID(playerid), engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

	GameTextForPlayer_(playerid, GetVehicleNameByModel(GetVehicleModel(vehicleid)), 5000, 8);

    if(oldstate == PLAYER_STATE_ONFOOT && newstate == PLAYER_STATE_DRIVER)
    {
        if (engine == VEHICLE_PARAMS_UNSET || engine == VEHICLE_PARAMS_OFF)
        {
            ShowPlayerBox(playerid, "The engine is off. Use /engine turn it on.", 3000);
        }
    }
	return 1;
}

hook OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

IsVehicleOccupied(vehicleid)
{
    foreach(new i : Player)
    {
        if(GetPlayerState(i) == PLAYER_STATE_DRIVER && GetPlayerVehicleID(i) == vehicleid) return 1;
    }
    return 0;
}