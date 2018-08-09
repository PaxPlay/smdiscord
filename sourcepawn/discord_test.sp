#pragma semicolon 1
#include <sourcemod>
#include <discord>

#pragma newdecls required

public Plugin myinfo =
{
   name = "this",
   description = "is",
   author = "a",
   version = "stupid",
   url = "test",
};

public void OnPluginStart()
{
    RegServerCmd("sm_testdiscord", Cmd_Test);
}

public Action Cmd_Test(int args)
{
    test();
    return Plugin_Handled;
}