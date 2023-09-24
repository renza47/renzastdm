CreateProperty(name[], Float:x, Float:y, Float:z, Float:int_x, Float:int_y, Float:int_z, interior)
{
    new idx, world, string[256];
    world = (idx + randomEx(10000, 50000));
    for(new i = 1; i < MAX_PROPERTIES; i++)
	{
		if(!PropertyInfo[i][PropID])
			continue;
			
		idx = i; 
		break;
	}
    new query[512];
    mysql_format(g_SQL, query, sizeof query, "INSERT INTO `properties` (`name`, `entranceX`, `entranceY`, `entranceZ`, `exitX`, `exitY`, `exitZ`, `world`, `interior`) VALUES ('%e', '%f', '%f', '%f', '%f', '%f', '%f', '%d', '%d')", name, x, y, z, int_x, int_y, int_z, world, interior);
    mysql_tquery(g_SQL, query);

    PropertyInfo[idx][PropID] = idx;
    format(PropertyInfo[idx][PropName], 64, name);

    PropertyInfo[idx][PropEntrance][0] = x;
    PropertyInfo[idx][PropEntrance][1] = y;
    PropertyInfo[idx][PropEntrance][2] = z;

    PropertyInfo[idx][PropExit][0] =int_x;
    PropertyInfo[idx][PropExit][1] =int_y;
    PropertyInfo[idx][PropExit][2] =int_z;

    PropertyInfo[idx][PropWorld] = world;
    PropertyInfo[idx][PropInterior] = interior;

    PropertyInfo[idx][PropCheckpoint] = CreateDynamicCP(PropertyInfo[idx][PropEntrance][0], PropertyInfo[idx][PropEntrance][1], PropertyInfo[idx][PropEntrance][2], 1.5, 2, 0);
    format(string, sizeof string, "[#%d]\n%s\n{FFFFFF}Available commands: /enter", PropertyInfo[idx][PropID], PropertyInfo[idx][PropName]);
    PropertyInfo[idx][PropLabel] = CreateDynamic3DTextLabel(string, COLOR_GREEN, PropertyInfo[idx][PropEntrance][0], PropertyInfo[idx][PropEntrance][1], PropertyInfo[idx][PropEntrance][2], 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 2);
    return 1;
}

forward LoadProperties();
public LoadProperties()
{
    new total, string[256];
    if(cache_num_rows())
    {
        for(new i = 0; i < cache_num_rows() && i < MAX_PROPERTIES; i++)
        {
            cache_get_value_int(i, "id", PropertyInfo[i + 1][PropID]);
	        cache_get_value_name(i, "name", PropertyInfo[i + 1][PropName]);

            cache_get_value_float(i, "entranceX", PropertyInfo[i + 1][PropEntrance][0]);
            cache_get_value_float(i, "entranceY", PropertyInfo[i + 1][PropEntrance][1]);
            cache_get_value_float(i, "entranceZ", PropertyInfo[i + 1][PropEntrance][2]);

            cache_get_value_float(i, "exitX", PropertyInfo[i + 1][PropExit][0]);
            cache_get_value_float(i, "exitY", PropertyInfo[i + 1][PropExit][1]);
            cache_get_value_float(i, "exitZ", PropertyInfo[i + 1][PropExit][2]);

            cache_get_value_int(i, "world", PropertyInfo[i + 1][PropWorld]);
            cache_get_value_int(i, "interior", PropertyInfo[i + 1][PropInterior]);

            PropertyInfo[i + 1][PropCheckpoint] = CreateDynamicCP(PropertyInfo[i + 1][PropEntrance][0], PropertyInfo[i + 1][PropEntrance][1], PropertyInfo[i + 1][PropEntrance][2], 1.5, 2, 0);
            format(string, sizeof string, "[#%d]\n%s\n{FFFFFF}Available commands: /enter", i + 1, PropertyInfo[i + 1][PropName]);
            PropertyInfo[i + 1][PropLabel] = CreateDynamic3DTextLabel(string, COLOR_GREEN, PropertyInfo[i + 1][PropEntrance][0], PropertyInfo[i + 1][PropEntrance][1], PropertyInfo[i + 1][PropEntrance][2], 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 2);
            total++;
        }
    }
    printf("%d property were loaded!", total);
    return 1;
}

SaveProperties()
{
    for(new i = 1; i < MAX_PROPERTIES; i++)
    {
        new query[512];
        mysql_format(g_SQL, query, sizeof query, "UPDATE `properties` SET `name` = %e, `entranceX` = %f, `entranceY` = %f, `entranceZ` = %f WHERE `id` = %d LIMIT 1", PropertyInfo[i][PropName], PropertyInfo[i][PropEntrance][0], PropertyInfo[i][PropEntrance][1], PropertyInfo[i][PropEntrance][2], PropertyInfo[i][PropID]);
        mysql_tquery(g_SQL, query);

        mysql_format(g_SQL, query, sizeof query, "UPDATE `properties` SET `exitX` = %f, `exitY` = %f, `exitZ` = %f, `world` = %d, `interior` = %d WHERE `id` = %d LIMIT 1", PropertyInfo[i][PropExit][0], PropertyInfo[i][PropExit][1],PropertyInfo[i][PropExit][2], PropertyInfo[i][PropWorld], PropertyInfo[i][PropInterior], PropertyInfo[i][PropID]);
        mysql_tquery(g_SQL, query);
    }
    return 1;
}

stock DeleteProperty(id)
{
    new query[128];
    mysql_format(g_SQL, query, sizeof query, "SELECT * FROM `properties` WHERE `id` = %d LIMIT 1", id);
    mysql_tquery(g_SQL, query, "OnPropertyDestroyed", "i", id);
    return 1;
}

forward OnPropertyDestroyed(id);
public OnPropertyDestroyed(id)
{
    if(cache_num_rows())
    {
        DestroyDynamicCP(PropertyInfo[id][PropCheckpoint]);
        DestroyDynamic3DTextLabel(PropertyInfo[id][PropLabel]);

        PropertyInfo[id][PropEntrance][0] = 0.0;
        PropertyInfo[id][PropEntrance][1] = 0.0;
        PropertyInfo[id][PropEntrance][2] = 0.0;

        PropertyInfo[id][PropExit][0] = 0.0;
        PropertyInfo[id][PropExit][1] = 0.0;
        PropertyInfo[id][PropExit][2] = 0.0;

        PropertyInfo[id][PropWorld] = 100;
        PropertyInfo[id][PropInterior]= 100;

        new query[128];
        mysql_format(g_SQL, query, sizeof query, "DELETE * FROM `properties` WHERE `id` = %d LIMIT 1", id);
        mysql_tquery(g_SQL, query);
    }
    return 1;
}