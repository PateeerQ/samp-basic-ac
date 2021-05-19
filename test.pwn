#include <a_samp>
#include "PAC.inc"

main() 
{
	print("Testing...");
}

public OnPlayerCheat(playerid, cheatid)
{
    new name[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, name, sizeof(name));
    printf("%s(%d) has possibly using '%s'", name, playerid, GetCheatName(cheatid));
    return 1;
}
