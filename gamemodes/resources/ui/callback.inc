#include <YSI_Coding\y_hooks>

hook OnPlayerDisconnect(playerid, reason)
{
    PlayerTextDrawDestroy(playerid, ui_clock[playerid]);
	PlayerTextDrawDestroy(playerid, ui_dayNotifier[playerid]);
	PlayerTextDrawDestroy(playerid, ui_serverName[playerid]);
	PlayerTextDrawDestroy(playerid, ui_serverWeb[playerid]);
	
	PlayerTextDrawDestroy(playerid, ui_class_skinName[playerid]);
	PlayerTextDrawDestroy(playerid, ui_class_arrowLeft[playerid]);
	PlayerTextDrawDestroy(playerid, ui_class_arrowRight[playerid]);
	PlayerTextDrawDestroy(playerid, ui_class_select[playerid]);
	PlayerTextDrawDestroy(playerid, ui_class_name[playerid]);

	PlayerTextDrawDestroy(playerid, ui_box[playerid]);
    return 1;
}