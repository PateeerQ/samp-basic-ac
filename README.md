# Pateeer Basic Anti Cheat

## A SAMP Basic Anti Cheat - Plug and Easy to Use

Anti Cheat Including
```bash
- Money Hack
- Health Hack
- Armour Hack
- Weapon Hack
```

## Callback & Function

```pawn
// Callback
public OnPlayerCheat(playerid, cheatid)

// Function
GetCheatName(cheatid)
GetPlayerWeaponVar(playerid)
GetPlayerAmmoVar(playerid)
```

## Usage

```pawn
#include <a_samp>
#include <PAC>

public OnPlayerCheat(playerid, cheatid)
{
    new name[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, name, sizeof(name));
    printf("%s(%d) has possibly using '%s'", name, playerid, GetCheatName(cheatid));
    return 0; // always return 0 for desync the player.
}
````
