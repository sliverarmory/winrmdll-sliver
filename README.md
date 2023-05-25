# winrmdll-sliver

C++ WinRM API via Reflective DLL

This project was ported from [mez-0/winrmdll](https://github.com/mez-0/winrmdll) to add Sliver extension support.

## Usage

```
sliver > winrm <hostname> <command> <username> <password>
```

## Example

```
sliver > winrm dc.snaplabs.local whoami snaplabs.local\domain_admin P@ssw0rd1
```

## Resources
* [mez-0/winrmdll](https://github.com/mez-0/winrmdll)
* [Learning Sliver C2 (12) - Extensions](https://dominicbreuker.com/post/learning_sliver_c2_12_extensions/)
* [trustedsec/SliverKeylogger](https://github.com/trustedsec/SliverKeylogger)
* [MrAle98/Sliver-PortBender](https://github.com/MrAle98/Sliver-PortBender)