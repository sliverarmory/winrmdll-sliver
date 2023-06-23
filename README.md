# winrmdll-sliver

C++ WinRM API via Sliver's reflective DLL loader, ported from [mez-0/winrmdll](https://github.com/mez-0/winrmdll) to add Sliver extension support.

## Usage

```
sliver > winrm dc.snaplabs.local whoami snaplabs.local\\domain_admin P@ssword1

[*] Successfully executed winrm
[*] Got output:
[+] Arguments processed
       hostname: dc.snaplabs.local
        command: whoami
       username: snaplabs.local\domain_admin
       password: P@ssword1

snaplabs\domain_admin
```

## Resources
* [mez-0/winrmdll](https://github.com/mez-0/winrmdll)
* [Learning Sliver C2 (12) - Extensions](https://dominicbreuker.com/post/learning_sliver_c2_12_extensions/)
* [trustedsec/SliverKeylogger](https://github.com/trustedsec/SliverKeylogger)
* [MrAle98/Sliver-PortBender](https://github.com/MrAle98/Sliver-PortBender)