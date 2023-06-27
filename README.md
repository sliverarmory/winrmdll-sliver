# winrmdll-sliver

C++ WinRM API via Sliver's reflective DLL loader, ported from [mez-0/winrmdll](https://github.com/mez-0/winrmdll) to add Sliver extension support.

## Usage

```
sliver > winrm -- -i dc.snaplabs.local -u snaplabs.local\\domain_admin -p P@ssword1 -c whoami /all

[*] Successfully executed winrm
[*] Got output:
[+] Arguments processed
       hostname: dc.snaplabs.local
        command: whoami /all
       username: snaplabs.local\domain_admin
       password: P@ssword1


USER INFORMATION
----------------

User Name             SID
===================== ==============================================
snaplabs\domain_admin S-1-5-21-2574976019-2108132042-3243833284-1113
[...]
```

## Resources
* [mez-0/winrmdll](https://github.com/mez-0/winrmdll)
* [Learning Sliver C2 (12) - Extensions](https://dominicbreuker.com/post/learning_sliver_c2_12_extensions/)
* [trustedsec/SliverKeylogger](https://github.com/trustedsec/SliverKeylogger)
* [MrAle98/Sliver-PortBender](https://github.com/MrAle98/Sliver-PortBender)