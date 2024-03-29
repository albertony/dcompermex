// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.

#include "targetver.h"
#include <windows.h>
#include <locale>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include "ntsecapi.h"
#include "dcomperm.h"
#include "dcomperm_rc.h"

/*---------------------------------------------------------------------------*\
 * NAME: HandleDefaultLaunchOption                                           *
 * --------------------------------------------------------------------------*
 * DESCRIPTION: 
\*---------------------------------------------------------------------------*/

void ShowUsage ( )
{
    BOOL fLegacy = IsLegacySecurityModel();

    _tprintf (_T("Syntax: %s <option> [...]\n\n"), _T(APPLICATION_FILENAME));
    _tprintf (_T("Options:\n"));

    if(!fLegacy)
    {
        _tprintf (_T("   -ma <\"set\" or \"remove\"> <Principal Name> [\"permit\" or \"deny\"] [\"level:l,r\"]\n"));
        _tprintf (_T("   -ma list [xml]\n"));
        _tprintf (_T("       Modify or list the machine access permission list\n\n"));

        _tprintf (_T("   -ml <\"set\" or \"remove\"> <Principal Name> [\"permit\" or \"deny\"] [\"level:l,r,ll,la,rl,ra\"]\n"));
        _tprintf (_T("   -ml list [xml]\n"));
        _tprintf (_T("       Modify or list the machine launch permission list\n\n"));
    }

    _tprintf (_T("   -da <\"set\" or \"remove\"> <Principal Name> [\"permit\" or \"deny\"] "));
    if(!fLegacy) _tprintf (_T("[\"level:l,r\"]\n")); else _tprintf (_T("\n"));
    
    _tprintf (_T("   -da list [xml]\n"));
    _tprintf (_T("       Modify or list the default access permission list\n\n"));

    _tprintf (_T("   -dl <\"set\" or \"remove\"> <Principal Name> [\"permit\" or \"deny\"] "));
    if(!fLegacy) _tprintf (_T("[\"level:l,r,ll,la,rl,ra\"]\n")); else _tprintf (_T("\n"));
    _tprintf (_T("   -dl list [xml]\n"));
    _tprintf (_T("       Modify or list the default launch permission list\n\n"));

    _tprintf (_T("   -aa <AppID> <\"set\" or \"remove\"> <Principal Name> [\"permit\" or \"deny\"] "));
    if(!fLegacy) _tprintf (_T("[\"level:l,r\"]\n")); else _tprintf (_T("\n"));
    _tprintf (_T("   -aa <AppID> default\n"));
    _tprintf (_T("   -aa <AppID> none\n"));
    _tprintf (_T("   -aa <AppID> list [xml]\n"));
    _tprintf (_T("       Modify or list the access permission list for a specific AppID\n\n"));

    _tprintf (_T("   -al <AppID> <\"set\" or \"remove\"> <Principal Name> [\"permit\" or \"deny\"] "));
    if(!fLegacy) _tprintf (_T("[\"level:l,r,ll,la,rl,ra\"]\n")); else _tprintf (_T("\n"));
    _tprintf (_T("   -al <AppID> default\n"));
    _tprintf (_T("   -al <AppID> none\n"));
    _tprintf (_T("   -al <AppID> list [xml]\n"));
    _tprintf (_T("       Modify or list the launch permission list for a specific AppID\n\n"));

    _tprintf (_T("Press any key to continue. . ."));
    _getch();
    _tprintf (_T("\n\n"));

    _tprintf (_T("   -runas <AppID>\n"));
    _tprintf (_T("   -runas <AppID> <Principal Name> <Password>\n"));
    _tprintf (_T("   -runas <AppID> \"Interactive User\"\n"));
    _tprintf (_T("   -runas <AppID> \"Launching User\"\n"));
    _tprintf (_T("       Get or set the RunAs information for a specific AppID\n\n"));

    _tprintf(_T("   -help\n"));
    _tprintf(_T("   -version\n"));
    _tprintf (_T("       Show this help and the application version\n\n"));

    if(!fLegacy)
    {
        _tprintf (_T("level:\n"));
        _tprintf (_T("   ll - local launch (only applies to {ml, dl, al} options)  \n"));
        _tprintf (_T("   rl - remote launch (only applies to {ml, dl, al} options)  \n"));
        _tprintf (_T("   la - local activate (only applies to {ml, dl, al} options)  \n"));
        _tprintf (_T("   ra - remote activate (only applies to {ml, dl, al} options)  \n"));
        _tprintf (_T("   l  - local (local access - means launch and activate when used with {ml, dl, al} options)  \n"));
        _tprintf (_T("   r  - remote (remote access - means launch and activate when used with {ml, dl, al} options)  \n\n"));
    }

    _tprintf (_T("Examples:\n"));
    _tprintf (_T("   dcomperm -da set redmond\\t-miken permit"));
    if(!fLegacy) _tprintf (_T(" level:r\n")); else _tprintf (_T("\n"));
    _tprintf (_T("   dcomperm -dl set redmond\\jdoe deny"));
    if(!fLegacy) _tprintf (_T(" level:rl,ra\n")); else _tprintf (_T("\n"));
    _tprintf (_T("   dcomperm -aa {12345678-1234-1234-1234-00aa00bbf7c7} list\n"));
    _tprintf (_T("   dcomperm -al {12345678-1234-1234-1234-00aa00bbf7c7} remove redmond\\t-miken\n"));
    _tprintf (_T("   dcomperm -runas {12345678-1234-1234-1234-00aa00bbf7c7} redmond\\jdoe password\n"));

    exit (0);
}

void ShowHelp ( )
{
    _tprintf (_T("%s: %s\n"), _T(PRODUCT_NAME), _T(APPLICATION_DESCRIPTION));
    _tprintf(_T("Version %s\n\n"), _T(PRODUCT_VERSION_STRING));
    ShowUsage ();
}

void ShowWrongUsage (
    LPTSTR tszErrorString
    )
{
    _tprintf (_T("%s\n"), tszErrorString);
    ShowUsage();
}

void ShowVersion ()
{
    _tprintf (_T("%s version %s\n"), _T(PRODUCT_NAME), _T(PRODUCT_VERSION_STRING));
}
/*---------------------------------------------------------------------------*\
 * NAME: HandleDefaultLaunchOption                                           *
 * --------------------------------------------------------------------------*
 * DESCRIPTION: 
\*---------------------------------------------------------------------------*/
void Error (
    LPTSTR tszErrorMessage,
    DWORD dwErrorCode
    )
{
    TCHAR tszMessageBuffer [SIZE_MSG_BUFFER] = {0};

    _tprintf (_T("%s\n%s"), tszErrorMessage, SystemMessage (tszMessageBuffer,  RTL_NUMBER_OF(tszMessageBuffer), dwErrorCode));
    exit (0);
}


/*---------------------------------------------------------------------------*\
 * NAME: HandleDefaultLaunchOption                                           *
 * --------------------------------------------------------------------------*
 * DESCRIPTION: 
\*---------------------------------------------------------------------------*/
void SetAccessMaskFromCommandLine (
    LPTSTR ptszArg,
    DWORD *pdwAccessMask,
    DWORD dwSDType
    )
{
    LPTSTR ptszToken = NULL;
    LPTSTR ptszContext = NULL;

    *pdwAccessMask = 0;

    ptszToken = _tcstok_s(ptszArg, _T(":"), &ptszContext);

    if(!ptszToken || _tcsicmp (ptszToken, _T("LEVEL")) != 0)
    {
        ShowWrongUsage(_T("Invalid LEVEL argument"));
    }

    ptszToken = _tcstok_s(NULL, _T(","), &ptszContext);

    if(!ptszToken)
    {
        ShowWrongUsage(_T("Invalid LEVEL argument"));
    }

    if(dwSDType & SDTYPE_ACCESS) do
    {

        if(_tcsicmp (ptszToken, _T("L")) == 0)
        {
            *pdwAccessMask |= COM_RIGHTS_EXECUTE_LOCAL | COM_RIGHTS_EXECUTE;
        }
        else if(_tcsicmp (ptszToken, _T("R")) == 0)
        {
            *pdwAccessMask |= COM_RIGHTS_EXECUTE_REMOTE | COM_RIGHTS_EXECUTE;
        }
        else
        {
            ShowWrongUsage(_T("Invalid LEVEL argument"));
        }

    }while(ptszToken = _tcstok_s(NULL, _T(","), &ptszContext));
    else do
    {

        if(_tcsicmp (ptszToken, _T("LL")) == 0)
        {
            *pdwAccessMask |= COM_RIGHTS_EXECUTE_LOCAL | COM_RIGHTS_EXECUTE;
        }
        else if(_tcsicmp (ptszToken, _T("LA")) == 0)
        {
            *pdwAccessMask |= COM_RIGHTS_ACTIVATE_LOCAL | COM_RIGHTS_EXECUTE;
        }
        else if(_tcsicmp (ptszToken, _T("RL")) == 0)
        {
            *pdwAccessMask |= COM_RIGHTS_EXECUTE_REMOTE | COM_RIGHTS_EXECUTE;
        }
        else if(_tcsicmp (ptszToken, _T("RA")) == 0)
        {
            *pdwAccessMask |= COM_RIGHTS_ACTIVATE_REMOTE | COM_RIGHTS_EXECUTE;
        }
        else if(_tcsicmp (ptszToken, _T("L")) == 0)
        {
            *pdwAccessMask |= COM_RIGHTS_ACTIVATE_LOCAL | COM_RIGHTS_EXECUTE_LOCAL | COM_RIGHTS_EXECUTE;
        }
        else if(_tcsicmp (ptszToken, _T("R")) == 0)
        {
            *pdwAccessMask |= COM_RIGHTS_ACTIVATE_REMOTE | COM_RIGHTS_EXECUTE_REMOTE | COM_RIGHTS_EXECUTE;
        }
        else
        {
            ShowWrongUsage(_T("Invalid LEVEL argument"));
        }

    }while(ptszToken = _tcstok_s(NULL, _T(","), &ptszContext));
    
}

/*---------------------------------------------------------------------------*\
 * NAME: HandleDefaultLaunchOption                                           *
 * --------------------------------------------------------------------------*
 * DESCRIPTION: 
\*---------------------------------------------------------------------------*/
void HandleMachineAccessOption (
    int cArgs,
    TCHAR **pptszArgv
    )
{

    DWORD dwReturnValue = ERROR_SUCCESS;
    DWORD dwAccessMask = COM_RIGHTS_EXECUTE;

    if (cArgs < 3) ShowWrongUsage(_T("Invalid number of arguments."));

    if (_tcsicmp (pptszArgv [2], _T("LIST")) == 0)
    {
        BOOL fXml = cArgs > 3 && _tcsicmp (pptszArgv [3], _T("XML")) == 0;
        if (!fXml) _tprintf (_T("Machine access permission list:\n\n"));
        dwReturnValue = ListMachineAccessACL(fXml);
        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot list default access permission list."), dwReturnValue);
        }
        return;
    }

    if (cArgs < 4) ShowWrongUsage(_T("Invalid number of arguments."));

    if (_tcsicmp (pptszArgv [2], _T("SET")) == 0)
    {
        if (cArgs < 5) ShowWrongUsage(_T("Invalid number of arguments."));

        if(cArgs == 6) 
        {
            SetAccessMaskFromCommandLine(pptszArgv[5], &dwAccessMask, SDTYPE_MACHINE_ACCESS);
        }
        else if(!IsLegacySecurityModel())
        {
            _tprintf (_T("WARNING: Default access flags designated on a system with an enhanced security model.\n"));
        }

        if (_tcsicmp (pptszArgv [4], _T("PERMIT")) == 0)
        {
            dwReturnValue = ChangeMachineAccessACL (pptszArgv [3], TRUE, TRUE, dwAccessMask);
        }
        else if (_tcsicmp (pptszArgv [4], _T("DENY")) == 0)
        {
            dwReturnValue = ChangeMachineAccessACL (pptszArgv [3], TRUE, FALSE, dwAccessMask); 
        }
        else
        {
            ShowWrongUsage (_T("You can only set a user's permissions to \"permit\" or \"deny\".\n\n"));
        }

        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot add user to machine access ACL."), dwReturnValue);
        }
    } 
    else if (_tcsicmp (pptszArgv [2], _T("REMOVE")) == 0)
    {
        dwReturnValue = ChangeMachineAccessACL (pptszArgv[3], FALSE, FALSE, dwAccessMask);

        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot remove user from machine access ACL."), dwReturnValue);
        }
    } 
    else
    {
        ShowWrongUsage (_T("You can only \"set\" or \"remove\" a user."));
    }

     _tprintf (_T("Successfully set the Machine Access ACL.\n"));

    ListMachineAccessACL(FALSE);

}


/*---------------------------------------------------------------------------*\
 * NAME: HandleDefaultLaunchOption                                           *
 * --------------------------------------------------------------------------*
 * DESCRIPTION: 
\*---------------------------------------------------------------------------*/
void HandleMachineLaunchAndActivateOption (
    int cArgs,
    TCHAR **pptszArgv
    )
{
    DWORD dwReturnValue = ERROR_SUCCESS;
    DWORD dwAccessMask = COM_RIGHTS_EXECUTE;


    if (cArgs < 3) ShowWrongUsage (_T("Invalid number of arguments."));

    if (_tcsicmp (pptszArgv [2], _T("LIST")) == 0)
    {
        BOOL fXml = cArgs > 3 && _tcsicmp (pptszArgv [3], _T("XML")) == 0;
        if (!fXml) _tprintf (_T("Machine launch permission list:\n\n"));
        dwReturnValue = ListMachineLaunchACL(fXml);

        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot list default launch ACL."), dwReturnValue);
        }

        return;
    }

    if (cArgs < 4) ShowWrongUsage(_T("Invalid number of arguments."));

    if (_tcsicmp (pptszArgv [2], _T("SET")) == 0)
    {
        if (cArgs < 5) ShowWrongUsage(_T("Invalid number of arguments."));

        if(cArgs == 6) 
        {
            SetAccessMaskFromCommandLine(pptszArgv[5], &dwAccessMask, SDTYPE_MACHINE_LAUNCH);
        }
        else if(!IsLegacySecurityModel())
        {
            _tprintf (_T("WARNING: Default access flags designated on a system with an enhanced security model.\n"));
        }

        if (_tcsicmp (pptszArgv [4], _T("PERMIT")) == 0)
        {
            dwReturnValue = ChangeMachineLaunchAndActivateACL (pptszArgv [3], TRUE, TRUE, dwAccessMask); 
        }
        else if (_tcsicmp (pptszArgv [4], _T("DENY")) == 0)
        {
            dwReturnValue = ChangeMachineLaunchAndActivateACL (pptszArgv [3], TRUE, FALSE, dwAccessMask); 
        }
        else
        {
            ShowWrongUsage (_T("You can only set a user's permissions to \"permit\" or \"deny\".\n\n"));
        }

        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot add user to machine launch ACL."), dwReturnValue);
        }

    } 
    else if (_tcsicmp (pptszArgv [2], _T("REMOVE")) == 0)
    {
        dwReturnValue = ChangeMachineLaunchAndActivateACL (pptszArgv[3], FALSE, FALSE, dwAccessMask);

        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot remove user from machine launch ACL."), dwReturnValue);
        }
    } 
    else
    {
        ShowWrongUsage (_T("You can only \"set\" or \"remove\" a user."));
    }
    
     _tprintf (_T("Successfully set the Machine Launch ACL.\n"));

    ListMachineLaunchACL(FALSE);

}



/*---------------------------------------------------------------------------*\
 * NAME: HandleDefaultLaunchOption                                           *
 * --------------------------------------------------------------------------*
 * DESCRIPTION: 
\*---------------------------------------------------------------------------*/
void HandleDefaultAccessOption (
    int cArgs,
    TCHAR **pptszArgv
    )
{
    DWORD dwReturnValue = ERROR_SUCCESS;
    DWORD dwAccessMask = COM_RIGHTS_EXECUTE;

    if (cArgs < 3) ShowWrongUsage (_T("Invalid number of arguments."));

    if (_tcsicmp (pptszArgv [2], _T("LIST")) == 0)
    {
        BOOL fXml = cArgs > 3 && _tcsicmp (pptszArgv [3], _T("XML")) == 0;
        if (!fXml) _tprintf (_T("Default access permission list:\n\n"));
        dwReturnValue = ListDefaultAccessACL(fXml);
        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot list default access permission list."), dwReturnValue);
        }
        return;
    }

    if (cArgs < 4) ShowWrongUsage (_T("Invalid number of arguments."));

    if (_tcsicmp (pptszArgv [2], _T("SET")) == 0)
    {
        if (cArgs < 5) ShowWrongUsage (_T("Invalid number of arguments."));

        if(cArgs == 6) 
        {
            SetAccessMaskFromCommandLine(pptszArgv[5], &dwAccessMask, SDTYPE_DEFAULT_ACCESS);
        }
        else if(!IsLegacySecurityModel())
        {
            _tprintf (_T("WARNING: Default access flags designated on a system with an enhanced security model.\n"));
        }

        if (_tcsicmp (pptszArgv [4], _T("PERMIT")) == 0)
        {
            dwReturnValue = ChangeDefaultAccessACL (pptszArgv [3], TRUE, TRUE, dwAccessMask);
        }
        else if (_tcsicmp (pptszArgv [4], _T("DENY")) == 0)
        {
            dwReturnValue = ChangeDefaultAccessACL (pptszArgv [3], TRUE, FALSE, dwAccessMask); 
        }
        else
        {
            ShowWrongUsage (_T("You can only set a user's permissions to \"permit\" or \"deny\".\n\n"));
        }

        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot add user to default access ACL."), dwReturnValue);
        }
    } 
    else if (_tcsicmp (pptszArgv [2], _T("REMOVE")) == 0)
    {
        dwReturnValue = ChangeDefaultAccessACL (pptszArgv[3], FALSE, FALSE, dwAccessMask);

        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot remove user from default access ACL."), dwReturnValue);
        }
    } 
    else
    {
        ShowWrongUsage (_T("You can only \"set\" or \"remove\" a user."));
    }

     _tprintf (_T("Successfully set the Default Access ACL.\n"));

    ListDefaultAccessACL(FALSE);
}

/*---------------------------------------------------------------------------*\
 * NAME: HandleDefaultLaunchOption                                           *
 * --------------------------------------------------------------------------*
 * DESCRIPTION: 
\*---------------------------------------------------------------------------*/
void HandleDefaultLaunchOption (
    int cArgs,
    TCHAR **pptszArgv
    )
{
    DWORD dwReturnValue = ERROR_SUCCESS;
    DWORD dwAccessMask = COM_RIGHTS_EXECUTE;

    if (cArgs < 3) ShowWrongUsage (_T("Invalid number of arguments."));

    if (_tcsicmp (pptszArgv [2], _T("LIST")) == 0)
    {
        BOOL fXml = cArgs > 3 && _tcsicmp (pptszArgv [3], _T("XML")) == 0;
        if (!fXml) _tprintf (_T("Default launch permission list:\n\n"));
        dwReturnValue = ListDefaultLaunchACL(fXml);

        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot list default launch ACL."), dwReturnValue);
        }

        return;
    }

    if (cArgs < 4) ShowWrongUsage (_T("Invalid number of arguments."));

    if (_tcsicmp (pptszArgv [2], _T("SET")) == 0)
    {
        if (cArgs < 5) ShowWrongUsage (_T("Invalid number of arguments."));

        if(cArgs == 6) 
        {
            SetAccessMaskFromCommandLine(pptszArgv[5], &dwAccessMask, SDTYPE_DEFAULT_LAUNCH);
        }
        else if(!IsLegacySecurityModel())
        {
            _tprintf (_T("WARNING: Default access flags designated on a system with an enhanced security model.\n"));
        }

        if (_tcsicmp (pptszArgv [4], _T("PERMIT")) == 0)
        {
            dwReturnValue = ChangeDefaultLaunchAndActivateACL (pptszArgv [3], TRUE, TRUE, dwAccessMask); 
        }
        else if (_tcsicmp (pptszArgv [4], _T("DENY")) == 0)
        {
            dwReturnValue = ChangeDefaultLaunchAndActivateACL (pptszArgv [3], TRUE, FALSE, dwAccessMask); 
        }
        else
        {
            ShowWrongUsage (_T("You can only set a user's permissions to \"permit\" or \"deny\".\n\n"));
        }

        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot add user to default launch ACL."), dwReturnValue);
        }

    } 
    else if (_tcsicmp (pptszArgv [2], _T("REMOVE")) == 0)
    {
        dwReturnValue = ChangeDefaultLaunchAndActivateACL (pptszArgv[3], FALSE, FALSE, dwAccessMask);

        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot remove user from default launch ACL."), dwReturnValue);
        }
    } 
    else
    {
        ShowWrongUsage (_T("You can only \"set\" or \"remove\" a user."));
    }

     _tprintf (_T("Successfully set the Default Launch ACL.\n"));

    ListDefaultLaunchACL(FALSE);
}

void HandleApplicationAccessOption (
    int cArgs,
    TCHAR **pptszArgv
    )
{
    DWORD dwReturnValue                 = ERROR_SUCCESS;
    HKEY  hkeyRegistry                  = NULL;
    TCHAR tszAppID [SIZE_NAME_BUFFER]   = {0};
    TCHAR tszKeyName [SIZE_NAME_BUFFER] = {0};

    DWORD dwAccessMask = COM_RIGHTS_EXECUTE;

    if (cArgs < 4)
        ShowWrongUsage (_T("Invalid number of arguments."));

    if (_tcsicmp (pptszArgv[3], _T("LIST")) == 0)
    {
        if (cArgs < 4) ShowWrongUsage (_T("Invalid number of arguments."));
        BOOL fXml = cArgs > 4 && _tcsicmp (pptszArgv[4], _T("XML")) == 0;
        if (!fXml) _tprintf (_T("Access permission list for AppID %s:\n\n"), pptszArgv[2]);
        ListAppIDAccessACL (pptszArgv[2], fXml);
        return;
    }
    else if (_tcsicmp (pptszArgv[3], _T("DEFAULT")) == 0)
    {

        _stprintf_s (tszAppID, RTL_NUMBER_OF(tszAppID), pptszArgv [2][0] == '{' ? _T("%s") : _T("{%s}"), pptszArgv [2]);
        _stprintf_s (tszKeyName, RTL_NUMBER_OF(tszKeyName), _T("APPID\\%s"), tszAppID);

        dwReturnValue = RegOpenKeyEx (HKEY_CLASSES_ROOT, tszKeyName, 0, KEY_ALL_ACCESS, &hkeyRegistry);
        if (dwReturnValue != ERROR_SUCCESS && dwReturnValue != ERROR_FILE_NOT_FOUND)
        {
            Error (_T("ERROR: Cannot open AppID registry key."), dwReturnValue);
        }

        dwReturnValue = RegDeleteValue (hkeyRegistry, _T("AccessPermission"));
        if (dwReturnValue != ERROR_SUCCESS && dwReturnValue != ERROR_FILE_NOT_FOUND)
        {
            Error (_T("ERROR: Cannot delete AccessPermission value."), dwReturnValue);
        }

        if(hkeyRegistry) RegCloseKey (hkeyRegistry);

        _tprintf (_T("Successfully set the Application Access to the machine default.\n"));

        return;
    }
    else if (_tcsicmp(pptszArgv[3], _T("NONE")) == 0)
    {
        dwReturnValue = ClearAppIDAccessACL(pptszArgv[2]);
        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot clear the application access ACL."), dwReturnValue);
        }
        _tprintf (_T("Successfully cleared the application access ACL (Application Access is denied for everyone).\n"));
        return;
    }

    if (cArgs < 5) ShowWrongUsage (_T("Invalid number of arguments."));

    if (_tcsicmp (pptszArgv [3], _T("SET")) == 0)
    {
         if (cArgs < 6) ShowWrongUsage (_T("Invalid number of arguments."));

        if(cArgs == 7) 
        {
            SetAccessMaskFromCommandLine(pptszArgv[6], &dwAccessMask, SDTYPE_APPLICATION_ACCESS);
        }
        else if(!IsLegacySecurityModel())
        {
            _tprintf (_T("WARNING: Default access flags designated on a system with an enhanced security model.\n"));
        }

        if (_tcsicmp (pptszArgv [5], _T("PERMIT")) == 0)
        {
            dwReturnValue = ChangeAppIDAccessACL (pptszArgv[2], pptszArgv [4], TRUE, TRUE, dwAccessMask); 
        }
        else if (_tcsicmp (pptszArgv [5], _T("DENY")) == 0)
        {
            dwReturnValue = ChangeAppIDAccessACL (pptszArgv[2], pptszArgv [4], TRUE, FALSE, dwAccessMask); 
        }
        else
        {
            ShowWrongUsage (_T("You can only set a user's permissions to \"permit\" or \"deny\".\n\n"));
        }

        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot add user to application access ACL."), dwReturnValue);
        }
    } 
    else if (_tcsicmp (pptszArgv [3], _T("REMOVE")) == 0)
    {
        dwReturnValue = ChangeAppIDAccessACL (pptszArgv[2], pptszArgv[4], FALSE, FALSE, dwAccessMask);

        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot remove user from application access ACL."), dwReturnValue);
        }
    } 
    else
    {
        ShowWrongUsage (_T("You can only \"set\" or \"remove\" a user."));
    }

     _tprintf (_T("Successfully set the Application Access ACL.\n"));

    ListAppIDAccessACL(pptszArgv[2], FALSE);
}

void HandleApplicationLaunchAndActivateOption (
    int cArgs,
    TCHAR **pptszArgv
    )
{
    DWORD dwReturnValue                 = ERROR_SUCCESS;
    HKEY  hkeyRegistry                  = NULL;
    TCHAR tszAppID [SIZE_NAME_BUFFER]   = {0};
    TCHAR tszKeyName [SIZE_NAME_BUFFER] = {0};

    DWORD dwAccessMask = COM_RIGHTS_EXECUTE;
    
    if (cArgs < 4) ShowWrongUsage (_T("Invalid number of arguments."));

    if (_tcsicmp (pptszArgv[3], _T("LIST")) == 0)
    {
        if (cArgs < 4) ShowWrongUsage (_T("Invalid number of arguments.\n"));
        BOOL fXml = cArgs > 4 && _tcsicmp (pptszArgv[4], _T("XML")) == 0;
        if (!fXml) _tprintf (_T("Launch permission list for AppID %s:\n\n"), pptszArgv[2]);
        ListAppIDLaunchACL (pptszArgv[2], fXml);
        return;
    }
    else if (_tcsicmp (pptszArgv[3], _T("DEFAULT")) == 0)
    {

        _stprintf_s (tszAppID, RTL_NUMBER_OF(tszAppID), pptszArgv [2][0] == '{' ? _T("%s") : _T("{%s}"), pptszArgv [2]);
        _stprintf_s (tszKeyName, RTL_NUMBER_OF(tszKeyName), _T("APPID\\%s"), tszAppID);

        dwReturnValue = RegOpenKeyEx (HKEY_CLASSES_ROOT, tszKeyName, 0, KEY_ALL_ACCESS, &hkeyRegistry);
        if (dwReturnValue != ERROR_SUCCESS && dwReturnValue != ERROR_FILE_NOT_FOUND)
        {
            Error (_T("ERROR: Cannot open AppID registry key."), dwReturnValue);
        }

        dwReturnValue = RegDeleteValue (hkeyRegistry, _T("LaunchPermission"));
        if (dwReturnValue != ERROR_SUCCESS && dwReturnValue != ERROR_FILE_NOT_FOUND)
        {
            Error (_T("ERROR: Cannot delete LaunchPermission value."), dwReturnValue);
        }

        if(hkeyRegistry) RegCloseKey (hkeyRegistry);

        _tprintf (_T("Successfully set the Application Launch to the machine default.\n"));

        return;
    }
    else if (_tcsicmp(pptszArgv[3], _T("NONE")) == 0)
    {
        dwReturnValue = ClearAppIDLaunchAndActivateACL(pptszArgv[2]);
        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot clear the application launch ACL."), dwReturnValue);
        }
         _tprintf (_T("Successfully cleared the application launch ACL (Application Launch is denied for everyone).\n"));
        return;
    }

    if (cArgs < 5) ShowWrongUsage (_T("Invalid number of arguments."));

    if (_tcsicmp (pptszArgv [3], _T("SET")) == 0)
    {
        if (cArgs < 6) ShowWrongUsage (_T("Invalid number of arguments."));

        if(cArgs == 7) 
        {
            SetAccessMaskFromCommandLine(pptszArgv[6], &dwAccessMask, SDTYPE_APPLICATION_LAUNCH);
        }
        else if(!IsLegacySecurityModel())
        {
            _tprintf (_T("WARNING: Default access flags designated on a system with an enhanced security model.\n"));
        }

        if (_tcsicmp (pptszArgv [5], _T("PERMIT")) == 0)
        {
            dwReturnValue = ChangeAppIDLaunchAndActivateACL (pptszArgv[2], pptszArgv [4], TRUE, TRUE, dwAccessMask);
        }
        else if (_tcsicmp (pptszArgv [5], _T("DENY")) == 0)
        {
            dwReturnValue = ChangeAppIDLaunchAndActivateACL (pptszArgv[2], pptszArgv [4], TRUE, FALSE, dwAccessMask);
        }
        else
        {
            ShowWrongUsage (_T("You can only set a user's permissions to \"permit\" or \"deny\".\n\n"));
        }

        if (dwReturnValue != ERROR_SUCCESS)
            Error (_T("ERROR: Cannot add user to application launch ACL."), dwReturnValue);
    } 
    else if (_tcsicmp (pptszArgv [3], _T("REMOVE")) == 0)
    {
        dwReturnValue = ChangeAppIDLaunchAndActivateACL (pptszArgv[2], pptszArgv[4], FALSE, FALSE, dwAccessMask);

        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot remove user from application launch ACL."), dwReturnValue);
        }
    } 
    else
    {
        ShowWrongUsage (_T("You can only \"set\" or \"remove\" a user."));
    }

     _tprintf (_T("Successfully set the Application Launch ACL.\n"));

    ListAppIDLaunchACL(pptszArgv[2], FALSE);
}

void HandleRunAsOption (
    int cArgs,
    TCHAR **pptszArgv
    )
{
    DWORD dwReturnValue                 = ERROR_SUCCESS;
    HKEY  hkeyRegistry                  = NULL;
    TCHAR tszAppID [SIZE_NAME_BUFFER]   = {0};
    TCHAR tszKeyName [SIZE_NAME_BUFFER] = {0};

    if (cArgs < 3) ShowWrongUsage(_T("Invalid number of arguments."));

    _stprintf_s(tszAppID, RTL_NUMBER_OF(tszAppID), pptszArgv[2][0] == '{' ? _T("%s") : _T("{%s}"), pptszArgv[2]);
    _stprintf_s(tszKeyName, RTL_NUMBER_OF(tszKeyName), _T("APPID\\%s"), tszAppID);

    if (cArgs < 4)
    {
        dwReturnValue = RegOpenKeyEx (HKEY_CLASSES_ROOT, tszKeyName, 0, KEY_READ, &hkeyRegistry);
        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot open AppID registry key."), dwReturnValue);
        }

        TCHAR tszPrincipalName[SIZE_NAME_BUFFER] = { 0 };
        TCHAR tszPassword[SIZE_NAME_BUFFER] = { 0 };
        DWORD dwSize = SIZE_NAME_BUFFER;

        dwReturnValue = RegGetValue (hkeyRegistry, NULL, _T("RunAs"), RRF_RT_ANY, NULL, tszPrincipalName, &dwSize);
        if (dwReturnValue == ERROR_SUCCESS)
        {
            _tprintf (_T("Principal Name: %s\n"), tszPrincipalName);
        }
        else if (dwReturnValue == ERROR_FILE_NOT_FOUND)
        {
            _tprintf (_T("Principal Name: Launching User\n"));
        }
        else
        {
            Error (_T("ERROR: Cannot get RunAs registry value."), dwReturnValue);
        }

        dwReturnValue = GetRunAsPassword(pptszArgv[2], tszPassword);
        if (dwReturnValue == ERROR_SUCCESS)
        {
            _tprintf (_T("Password: %s\n"), tszPassword);
        }
        else if (dwReturnValue == ERROR_FILE_NOT_FOUND)
        {
            _tprintf (_T("Password: N/A\n"));
        }
        else
        {
            Error (_T("ERROR: Cannot get RunAs password."), dwReturnValue);
        }
    }
    else
    {
        dwReturnValue = RegOpenKeyEx (HKEY_CLASSES_ROOT, tszKeyName, 0, KEY_ALL_ACCESS, &hkeyRegistry);
        if (dwReturnValue != ERROR_SUCCESS)
        {
            Error (_T("ERROR: Cannot open AppID registry key."), dwReturnValue);
        }

        if (_tcsicmp (pptszArgv[3], _T("LAUNCHING USER")) == 0)
        {
            dwReturnValue = RegDeleteValue(hkeyRegistry, _T("RunAs"));
            if(dwReturnValue == ERROR_FILE_NOT_FOUND)
            {
                dwReturnValue = ERROR_SUCCESS;
            }
            else if (dwReturnValue != ERROR_SUCCESS)
            {
                Error (_T("ERROR: Cannot remove RunAs registry value."), dwReturnValue);
            }

            dwReturnValue = RemoveRunAsPassword(pptszArgv[2]);
            if(dwReturnValue == ERROR_FILE_NOT_FOUND)
            {
                dwReturnValue = ERROR_SUCCESS;
            }
            else if (dwReturnValue != ERROR_SUCCESS)
            {
                Error (_T("ERROR: Cannot remove RunAs password."), dwReturnValue);
            }
        }
        else
        {
            if (_tcsicmp (pptszArgv[3], _T("INTERACTIVE USER")) == 0)
            {
                dwReturnValue = RemoveRunAsPassword(pptszArgv[2]);
                if(dwReturnValue == ERROR_FILE_NOT_FOUND)
                {
                    dwReturnValue = ERROR_SUCCESS;
                }
                else if (dwReturnValue != ERROR_SUCCESS)
                {
                    Error (_T("ERROR: Cannot remove RunAs password."), dwReturnValue);
                }
            }
            else
            {
                if (cArgs < 5) ShowWrongUsage (_T("Invalid number of arguments."));

                dwReturnValue = SetRunAsPassword (pptszArgv[2], pptszArgv[3], pptszArgv[4]);
                if (dwReturnValue != ERROR_SUCCESS)
                {
                    Error (_T("ERROR: Cannot set RunAs password."), dwReturnValue);
                }
            }

            dwReturnValue = RegSetValueEx (hkeyRegistry, _T("RunAs"), 0, REG_SZ, (LPBYTE) pptszArgv[3], (DWORD)_tcslen (pptszArgv[3]) * sizeof (TCHAR));
            if (dwReturnValue != ERROR_SUCCESS)
            {
                Error (_T("ERROR: Cannot set RunAs registry value."), dwReturnValue);
            }
        }
    }

    if(hkeyRegistry) RegCloseKey (hkeyRegistry);
}

extern "C" void _tmain (
    int cArgs,
    TCHAR **pptszArgv
    )
{
    std::locale::global(std::locale(""));

    if (cArgs < 2) ShowWrongUsage (_T("No option specified."));

    if (_tcsicmp (pptszArgv[1], _T("-HELP")) == 0)
    {
        ShowHelp ();
    }
    else if (_tcsicmp (pptszArgv[1], _T("-VERSION")) == 0)
    {
        ShowVersion ();
    }
    else if (_tcsicmp (pptszArgv [1], _T("-DA")) == 0)
    {
        HandleDefaultAccessOption (cArgs, pptszArgv); 
    }
    else if (_tcsicmp (pptszArgv [1], _T("-DL")) == 0)
    {
        HandleDefaultLaunchOption (cArgs, pptszArgv); 
    }
    else if (_tcsicmp (pptszArgv [1], _T("-AA")) == 0)
    {
        HandleApplicationAccessOption (cArgs, pptszArgv);
    }
    else if (_tcsicmp (pptszArgv [1], _T("-AL")) == 0)
    {
        HandleApplicationLaunchAndActivateOption (cArgs, pptszArgv); 
    }
    else if (_tcsicmp (pptszArgv [1], _T("-MA")) == 0)
    {
        HandleMachineAccessOption (cArgs, pptszArgv);
    }
    else if (_tcsicmp (pptszArgv [1], _T("-ML")) == 0)
    {
        HandleMachineLaunchAndActivateOption (cArgs, pptszArgv); 
    }
    else if (_tcsicmp (pptszArgv [1], _T("-RUNAS")) == 0)
    {
        HandleRunAsOption (cArgs, pptszArgv); 
    }
    else
    {
        ShowWrongUsage (_T("Invalid option specified."));
    }
    
}
