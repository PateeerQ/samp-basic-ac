/*

                Pateeer Basic Anti Cheat
            version 0.3

        Thanks to Mr Michael for giving feedback

        Including:
        - Anti Health Hack
        - Anti Armor Hack
        - Anti Money Hack
        - Anti Weapon Hack

*/

/*************************************************
*                                                *
* Callbacks:                                     *
* OnPlayerCheat(playerid, cheatid)               *
*                                                *
* Functions:                                     *
* GetPlayerWeaponVar(playerid)                   *
* GetPlayerAmmoVar(playerid)                     *
* ResetPlayerWeaponVar(playerid)                 *
* GetCheatName(cheatid)                          *
* IsWeaponHasAmmo(weaponid)                      *
*                                                *
* Hooked Functions:                              *
* GivePlayerWeapon(playerid)                     *
* SetPlayerHealth(playerid, Float:amount)        *
* SetPlayerArmour(playerid, Float:amount)        *
* SetPlayerMoney(playerid, amount)               *
* GivePlayerMoney(playerid, amount)              *
*                                                *
**************************************************/

// Include guards
#if defined PAT_Included
    #endinput
#endif 

#define PAT_Included

#if !defined _samp_included
    #error Please include a_samp before this.
#endif

// can be changed before including this
#if !defined MAX_WEAPON_SLOT
    #define MAX_WEAPON_SLOT 13
#endif

#if !defined DEFAULT_MONEY
    #define DEFAULT_MONEY 0
#endif

// cannot be changed!
#if defined INVALID_CHEAT_ID
	#undef INVALID_CHEAT_ID
#endif

#define INVALID_CHEAT_ID -1

enum _:E_CHEAT_TYPE {
    MONEY_HACKS,
    HEALTH_HACKS,
    ARMOUR_HACKS,
    WEAPON_HACKS
};

#if defined OnPlayerCheat
	forward OnPlayerCheat(playerid, cheatid);
#endif

static const PAT_WeaponSlots[] = {
    0, 0, 1, 1, 1, 1, 1, 1, 1, 
    1, 10, 10, 10, 10, 10, 10, 
    8, 8, 8, 0, 0, 0, 2, 2, 2, 
    3, 3, 3, 4, 4, 5, 5, 4, 6, 
    6, 7, 7, 7, 7, 8, 12, 9, 9, 
    9, 11, 11, 11
};

static IsWeaponHasAmmo(weaponid)
{
	static const PAT_WeaponID[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0
	};

	if(!(0 < weaponid < 54))
		return 0;

	return (PAT_WeaponID[weaponid]);
}

enum E_PAT_PLAYERS {
    Float:PAT_Health,
    Float:PAT_Armor,
    PAT_Money,
    PAT_Gun,
    PAT_Weapon[MAX_WEAPON_SLOT],
    PAT_Ammo[MAX_WEAPON_SLOT]
};
new pat_player[MAX_PLAYERS][E_PAT_PLAYERS];

// Anti Cheat API
stock PAT_GetCheatName(cheatid)
{
    new str[32];
    
    switch(cheatid) {
        case HEALTH_HACKS: str = "Health Hack";
        case ARMOUR_HACKS: str = "Armour Hack";
        case MONEY_HACKS: str = "Money Hack";
        case WEAPON_HACKS: str = "Weapon Hack";
        default: str = "Unknown";
    }
    return str;
}

stock PAT_GetPlayerWeaponVar(playerid)
{
    new weaponid = GetPlayerWeapon(playerid);

    if(1 <= weaponid <= 46 && pat_player[playerid][PAT_Weapon][PAT_WeaponSlots[weaponid]] == weaponid)
        return weaponid;

    return 0;
}

stock PAT_GetPlayerAmmoVar(playerid)
{
    new weaponid = GetPlayerWeapon(playerid);
	new ammo = pat_player[playerid][PAT_Ammo][PAT_WeaponSlots[weaponid]];
	if(1 <= weaponid <= 46 && pat_player[playerid][PAT_Weapon][PAT_WeaponSlots[weaponid]] == weaponid)
	{
		if(pat_player[playerid][PAT_Weapon][PAT_WeaponSlots[weaponid]] != 0 && pat_player[playerid][PAT_Ammo][PAT_WeaponSlots[weaponid]] > 0)
		{
			return ammo;
		}
	}
	return 0;
}

stock PAT_ResetPlayerWeaponsVar(playerid)
{
    ResetPlayerWeapons(playerid);
    for(new i = 0; i < MAX_WEAPON_SLOT; i++) {
        pat_player[playerid][PAT_Weapon][i] = 0;
        pat_player[playerid][PAT_Ammo][i] = 0;
    }
    return 1;
}

// Server-side Controller
stock PAT_SetPlayerHealth(playerid, Float:health)
{
    pat_player[playerid][PAT_Health] = health;
    return SetPlayerHealth(playerid, health);
}

stock PAT_SetPlayerArmour(playerid, Float:armour)
{
    pat_player[playerid][PAT_Armor] = armour;
    return SetPlayerArmour(playerid, armour);
}

stock PAT_SetPlayerMoney(playerid, amount)
{
    pat_player[playerid][PAT_Money] = amount;
    ResetPlayerMoney(playerid);
    return GivePlayerMoney(playerid, amount);
}

stock PAT_GivePlayerMoney(playerid, amount)
{
    pat_player[playerid][PAT_Money] += amount;
    return GivePlayerMoney(playerid, amount);
}

stock PAT_ResetPlayerMoney(playerid)
{
    pat_player[playerid][PAT_Money] = DEFAULT_MONEY;
    return ResetPlayerMoney(playerid);
}

stock PAT_GivePlayerWeapon(playerid, weaponid, ammo)
{
    if(!(1 <= weaponid <= 46))
        return 0;

    pat_player[playerid][PAT_Weapon][PAT_WeaponSlots[weaponid]] = weaponid;
    pat_player[playerid][PAT_Ammo][PAT_WeaponSlots[weaponid]] += ammo;
    return GivePlayerWeapon(playerid, weaponid, ammo);
}

stock PAT_SetSpawnInfo(playerid, team, skin, Float:x, Float:y, Float:z, Float:rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
{
    pat_player[playerid][PAT_Weapon][0] = weapon1;
    pat_player[playerid][PAT_Ammo][0] += weapon1_ammo;
    pat_player[playerid][PAT_Weapon][1] = weapon2;
    pat_player[playerid][PAT_Ammo][1] += weapon2_ammo;
    pat_player[playerid][PAT_Weapon][2] = weapon3;
    pat_player[playerid][PAT_Ammo][2] += weapon3_ammo;
    return SetSpawnInfo(playerid, team, skin, Float:x, Float:y, Float:z, Float:rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
}


public OnGameModeInit(){
	print("=====================================");
    print("|     PATEEER BASIC ANTI CHEATS     |");
    print("|               v3.0                |");
    print("=====================================");

	#if defined PAT_OnGameModeInit
		PAT_OnGameModeInit();
	#endif
	return 1;
}

public OnPlayerConnect(playerid){

    new pat_empty[E_PAT_PLAYERS];
    pat_player[playerid] = pat_empty;

    pat_player[playerid][PAT_Health] = 100.0;
    pat_player[playerid][PAT_Armor] = 0.0;

	#if defined PAT_OnPlayerConnect
		return PAT_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if(!IsWeaponHasAmmo(weaponid)) return 0;
	if(1 <= weaponid <= 46 && pat_player[playerid][PAT_Weapon][PAT_WeaponSlots[weaponid]] == weaponid)
	{
		pat_player[playerid][PAT_Ammo][PAT_WeaponSlots[weaponid]]--;
		if(pat_player[playerid][PAT_Weapon][PAT_WeaponSlots[weaponid]] != 0 && !pat_player[playerid][PAT_Ammo][PAT_WeaponSlots[weaponid]])
		{
			pat_player[playerid][PAT_Weapon][PAT_WeaponSlots[weaponid]] = 0;
		}
	}
	#if defined PAT_OnPlayerWeaponShot
		return PAT_OnPlayerWeaponShot(playerid,weaponid,hittype,hitid,fX,fY,fZ);
	#else
		return 1;
	#endif
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
    new Float:PAT_armour;
    GetPlayerArmour(playerid, PAT_armour);
    if(PAT_armour > 0)
    {
        pat_player[playerid][PAT_Armor] -= amount;
        SetPlayerArmour(playerid, PAT_armour-amount);
    }
    else
    {
        new Float:PAT_health;
        GetPlayerHealth(playerid, PAT_health);
        pat_player[playerid][PAT_Health] -= amount;
        SetPlayerHealth(playerid, PAT_health-amount);
    }
	#if defined PAT_OnPlayerTakeDamage
		return PAT_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
	#else
		return 1;
	#endif
}

/* 
    This? better check it under OnPlayerUpdate.
    Why? you need a better check every 30x per 1 second.

    But people say OnPlayerUpdate is not good? yes i know but
    still you need OnPlayerUpdate to constantly checking the player data.

    You do not need timer 1 second because cheater can actually bypass it by making some script to set
    health 500ms before the next timer starts and the player can reconnect to the server then the thing
    he hacked will be in your db.
*/
public OnPlayerUpdate(playerid)
{
    if(GetPlayerMoney(playerid) > pat_player[playerid][PAT_Money] || GetPlayerMoney(playerid) < pat_player[playerid][PAT_Money])
    {
        SetPlayerMoney(playerid, pat_player[playerid][PAT_Money]);
        return CallLocalFunction("OnPlayerCheat", "ii", playerid, MONEY_HACKS);
    }

    new Float:PAT_health, Float:PAT_armour;
    GetPlayerHealth(playerid, PAT_health);
    GetPlayerArmour(playerid, PAT_armour);

    if(PAT_health > pat_player[playerid][PAT_Health])
    {
        SetPlayerHealth(playerid, pat_player[playerid][PAT_Health]);
        return CallLocalFunction("OnPlayerCheat", "ii", playerid, HEALTH_HACKS);
    }

    if(PAT_armour > pat_player[playerid][PAT_Armor])
    {
        SetPlayerArmour(playerid, pat_player[playerid][PAT_Armor]);
        return CallLocalFunction("OnPlayerCheat", "ii", playerid, ARMOUR_HACKS);
    }

    if(GetPlayerWeapon(playerid) != pat_player[playerid][PAT_Gun])
    {
        pat_player[playerid][PAT_Gun] = GetPlayerWeapon(playerid);

        if(pat_player[playerid][PAT_Gun] >= 1 && pat_player[playerid][PAT_Gun] <= 45 && pat_player[playerid][PAT_Gun] != 40 && pat_player[playerid][PAT_Gun] != 2 && pat_player[playerid][PAT_Weapon][PAT_WeaponSlots[pat_player[playerid][PAT_Gun]]] != GetPlayerWeapon(playerid) && GetPlayerWeapon(playerid) != 41)
        {
            ResetPlayerWeapons(playerid);

            for (new i = 0; i < MAX_WEAPON_SLOT; i ++) if(pat_player[playerid][PAT_Weapon][i] > 0 && pat_player[playerid][PAT_Ammo][i] > 0) {
                GivePlayerWeapon(playerid, pat_player[playerid][PAT_Weapon][i], pat_player[playerid][PAT_Ammo][i]);
            }
            return CallLocalFunction("OnPlayerCheat", "ii", playerid, WEAPON_HACKS);
        }
    }
   	#if defined PAT_OnPlayerUpdate
		return PAT_OnPlayerUpdate(playerid);
	#else
    	return 1;
    #endif
}

// Rename functions, i do not think this was necessary
#define GetCheatName PAT_GetCheatName
#define SetPlayerMoney PAT_SetPlayerMoney
#define GetPlayerWeaponVar PAT_GetPlayerWeaponVar
#define GetPlayerAmmoVar PAT_GetPlayerAmmoVar
#define ResetPlayerWeaponsVar PAT_ResetPlayerWeaponsVar

// ALS hook functions
#if defined _ALS_SetPlayerHealth
	#undef SetPlayerHealth
#else
	#define _ALS_SetPlayerHealth
#endif
#define SetPlayerHealth PAT_SetPlayerHealth

#if defined _ALS_SetPlayerArmour
	#undef SetPlayerArmour
#else
	#define _ALS_SetPlayerArmour
#endif
#define SetPlayerArmour PAT_SetPlayerArmour

#if defined _ALS_GivePlayerMoney
	#undef GivePlayerMoney
#else
	#define _ALS_GivePlayerMoney
#endif
#define GivePlayerMoney PAT_GivePlayerMoney


#if defined _ALS_ResetPlayerMoney
	#undef ResetPlayerMoney
#else
	#define _ALS_ResetPlayerMoney
#endif
#define ResetPlayerMoney PAT_ResetPlayerMoney

#if defined _ALS_GivePlayerWeapon
	#undef GivePlayerWeapon
#else
	#define _ALS_GivePlayerWeapon
#endif
#define GivePlayerWeapon PAT_GivePlayerWeapon

#if defined _ALS_SetSpawnInfo
	#undef SetSpawnInfo
#else
	#define _ALS_SetSpawnInfo
#endif
#define SetSpawnInfo PAT_SetSpawnInfo


// ALS hook callbacks
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit PAT_OnGameModeInit
#if defined PAT_OnGameModeInit
	forward PAT_OnGameModeInit();
#endif

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect PAT_OnPlayerConnect

#if defined PAT_OnPlayerConnect
	forward PAT_OnPlayerConnect(playerid);
#endif

#if defined _ALS_OnPlayerWeaponShot
	#undef OnPlayerWeaponShot
#else
	#define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot PAT_OnPlayerWeaponShot
#if defined PAT_OnPlayerWeaponShot
	forward PAT_OnPlayerWeaponShot(playerid,weaponid,hittype,hitid,Float:fX,Float:fY,Float:fZ);
#endif

#if defined _ALS_OnPlayerTakeDamage
	#undef OnPlayerTakeDamage
#else
	#define _ALS_OnPlayerTakeDamage
#endif
#define OnPlayerTakeDamage PAT_OnPlayerTakeDamage
#if defined PAT_OnPlayerTakeDamage
	forward PAT_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif

#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif

#define OnPlayerUpdate PAT_OnPlayerUpdate
#if defined PAT_OnPlayerUpdate
	forward PAT_OnPlayerUpdate(playerid);
#endif
