# Pateer Basic Anti Cheat

A SAMP Basic Anti Cheat - Plug and Easy to Use

## Callback & Function

```pawn
// Callback
public OnPlayerCheat(playerid, cheatid)

// Function
GetCheatName(cheatid)
GetPlayerWeaponEx(playerid)
GetPlayerAmmoEx(playerid)
```

Include in your code and you must use YSI Y_Timers
```pawn
#include <YSI\y_timers>
#include <PAC>
```

## How To Use

```pawn
#include <a_samp>
#include <YSI\y_timers>
#include <PAC>

public OnPlayerCheat(playerid, cheatid)
{
    new name[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, name, sizeof(name));
    printf("%s(%d) has possibly using '%s'", name, playerid, GetCheatName(cheatid));
    return 1;
}
````