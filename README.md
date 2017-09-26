DcomPermEx
====

Command-line alternative to Windows built-in "Component Services" management console,
including the "DCOM Config" for administration of DCOM applications, accessible from
the "Administrative Tools" Control Panel, `dcomcnfg.exe`, or
`mmc.exe C:\WINDOWS\system32\comexp.msc`.

DcomPermEx is an extended version of Microsoft's DCOM permission command line utility DcomPerm,
published only as source code - a sample that demonstrate the API used in Windows
classic desktop applications. Originally published in Windows 7 SDK, but also
available on Microsoft's GitHub account: https://github.com/Microsoft/Windows-classic-samples/tree/master/Samples/Win7Samples/com/fundamentals/dcom/dcomperm.

Microsoft has never published the utility in any other form than source code,
so if you don't want to build it yourself you can use my latest build from the
releases section, and you will get those [additional features](#extended-features) included as well.

The application is stand-alone single-binary, with the Visual C++ runtime libraries
statically linked in. This makes it easy to copy around where you need it, and also
easy to use in an installer custom action.

### Extended features

#### List permissions as XML

All list operations support an additional argument "xml" which will return the permission
list XML formatted, for easier processing in PowerShell scripts etc.

#### Setting application permissions to none

The original version supports setting application level permissions to machine default,
or customizing it by adding or removing permissions for a specified principal. When you
customize the permissions for an application for the first time it will be initialized
to some proper defaults before the customization is applied (from code comment: "Sets
the default ACEs in a ACL for the enhanced COM security model."). Note that these defaults
are not necessarily the same as the machine defaults, which is what will be set when you
do the same from the "DCOM Config" part of the "Component Services" user interface.

If you for instance want to set permissions only for a single principal you have to
remove all others from the default set. To simplify this I introduced a new option to set
permissions to "none", which clears the ACL - effectively setting deny for everyone.
Due to risk of getting yourself into problems, this option is only possible on specific AppID,
and even then; please use with care!

Usage
=====

*Note that many of the operations needs the application to be run as admininstrator (elevated) to work properly!*

The main purpose of this utility is to be able to script setting of DCOM permissions.
But I also found it very useful when I unintentionally removed the special group "SELF"
from the machine level access permissions in DCOM Config. I could not find any way
to add that special group back into the permission list using the DCOM Config user
interface. DcomPerm(Ex) to the rescue:

`DComPermEx.exe -da set "NT AUTHORITY\SELF" permit level:l,r`

The complete syntax description as reported by the command line utility itself:
 
```
Syntax: DcomPermEx.exe <option> [...]

Options:
   -ma <"set" or "remove"> <Principal Name> ["permit" or "deny"] ["level:l,r"]
   -ma list [xml]
       Modify or list the machine access permission list

   -ml <"set" or "remove"> <Principal Name> ["permit" or "deny"] ["level:l,r,ll,la,rl,ra"]
   -ml list [xml]
       Modify or list the machine launch permission list

   -da <"set" or "remove"> <Principal Name> ["permit" or "deny"] ["level:l,r"]
   -da list [xml]
       Modify or list the default access permission list

   -dl <"set" or "remove"> <Principal Name> ["permit" or "deny"] ["level:l,r,ll,la,rl,ra"]
   -dl list [xml]
       Modify or list the default launch permission list

   -aa <AppID> <"set" or "remove"> <Principal Name> ["permit" or "deny"] ["level:l,r"]
   -aa <AppID> default
   -aa <AppID> none
   -aa <AppID> list [xml]
       Modify or list the access permission list for a specific AppID

   -al <AppID> <"set" or "remove"> <Principal Name> ["permit" or "deny"] ["level:l,r,ll,la,rl,ra"]
   -al <AppID> default
   -al <AppID> none
   -al <AppID> list [xml]
       Modify or list the launch permission list for a specific AppID

   -runas <AppID> <Principal Name> <Password>
   -runas <AppID> "Interactive User"
   -runas <AppID> "Launching User"
       Set the RunAs information for a specific AppID

   -help
   -version
       Show this help and the application version

level:
   ll - local launch (only applies to {ml, dl, al} options)  
   rl - remote launch (only applies to {ml, dl, al} options)  
   la - local activate (only applies to {ml, dl, al} options)  
   ra - remote activate (only applies to {ml, dl, al} options)  
   l  - local (local access - means launch and activate when used with {ml, dl, al} options)  
   r  - remote (remote access - means launch and activate when used with {ml, dl, al} options)  

Examples:
   dcomperm -da set redmond\t-miken permit level:r
   dcomperm -dl set redmond\jdoe deny level:rl,ra
   dcomperm -aa {12345678-1234-1234-1234-00aa00bbf7c7} list
   dcomperm -al {12345678-1234-1234-1234-00aa00bbf7c7} remove redmond\t-miken
   dcomperm -runas {12345678-1234-1234-1234-00aa00bbf7c7} redmond\jdoe password
```

Contributing
============

See the original [readme.htm](http://htmlpreview.github.io/?https://github.com/albertony/dcompermex/blob/master/readme.htm)
for basic introduction to the source code.

History
=======

### Version 0.1.2

Added option to set application permissions to none, which clears the ACL effectively setting deny for everyone.
Due to risk of getting yourself into problems, this option is only possible on specific AppID, and
even then use with care!

### Version 0.1.1

Added list option to return as xml

### Version 0.1.0

The initial version with untouched copy of source code from [Microsoft's Windows-classic-samples](https://github.com/Microsoft/Windows-classic-samples/tree/master/Samples/Win7Samples/com/fundamentals/dcom/dcomperm)

License
=======

All code is licensed under MIT License.

