#define MAX_DAMAGES 5000

enum dmgInfo
{
    bool:dmgExists,
    dmgDamaged,
    dmgBody,
    dmgIssuer[MAX_PLAYER_NAME],
    Float:dmgAmount,
    dmgWeapon
}
new DamageInfo[MAX_DAMAGES][dmgInfo];