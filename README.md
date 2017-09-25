DcomPermEx
====

Command-line alternative to Windows built-in "Component Services" management console,
including the "DCOM Config" for administration of DCOM applications, accessible from
the "Administrative Tools" Control Panel, `dcomcnfg.exe`, or
`mmc.exe C:\WINDOWS\system32\comexp.msc`.

This is a slightly extended version of Microsoft's DCOM permission command line utility sample
https://github.com/Microsoft/Windows-classic-samples/tree/master/Samples/Win7Samples/com/fundamentals/dcom/dcomperm


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

TODO...

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

