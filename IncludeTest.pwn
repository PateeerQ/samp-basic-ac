#include <a_samp>
#include <YSI\y_timers>
#include "PAC.inc"

main() {}

public OnPlayerCheat(playerid, cheatid)
{
    new name[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, name, sizeof(name));
    printf("%s(%d) has possibly use '%s'", name, playerid, GetCheatName(cheatid));
    return 1;
}