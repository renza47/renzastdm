#include <YSI_Coding\y_hooks>

forward VPNCheck(playerid, response_code, data[]);
public VPNCheck(playerid, response_code, data[])
{
	if(response_code == 200)
	{	
		if(data[0] == 'Y')
		{
			SendClientMessage(playerid, COLOR_RED, "[IP Check] Using VPN: Yes");
		}
		if(data[0] == 'N')
		{
			SendClientMessage(playerid, COLOR_GREEN, "[IP Check] Using VPN: No");
		}
	}
	return 1;
}

forward ShowStaffActivities(playerid);
public ShowStaffActivities(playerid)
{
	if(cache_num_rows() > 0)
	{
		new temp_str[1096], primary_str[1096], string[128], name[64], activity[3];
		strcat(primary_str, "Name\tHours\n");
		for(new rows; rows < cache_num_rows(); rows++)
		{
			for(new id = 1; id <= 3; id++)
			{
				format(string, sizeof string, "activity_%d", id);
				cache_get_value_name_int(rows, string, activity[id -1]);
			}
			cache_get_value_name(rows, "username", name);

			format(temp_str, sizeof temp_str, "%s\t%d hours %d minutes %d seconds\n", name, activity[0], activity[1], activity[2]);
            strcat(primary_str, temp_str);
		}
        ShowPlayerDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_TABLIST_HEADERS, "Staff Activities", primary_str, "Close", "");
	}
	return 1;
}