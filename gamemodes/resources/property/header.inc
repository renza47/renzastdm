#define MAX_PROPERTIES 500

enum E_PROPERTY_DATA
{
    PropID,
    PropName[64],
    Float:PropEntrance[3],
    Float:PropExit[3],
    PropWorld,
    PropInterior,
    PropCheckpoint,
    Text3D:PropLabel
}
new PropertyInfo[MAX_PROPERTIES][E_PROPERTY_DATA];