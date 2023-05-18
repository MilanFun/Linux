# Linux Kernel
## Telephone Book
### Installation
Compile and install Kernel module
```
./init.sh
```
Execute command for identify `Major`
```
cat /proc/devices | grep chardev | awk '{print $1}'
```
Create symbol device
```
mknod /dev/chardev c <Major> 0
```
Compile user-space program for communicate with module
```
gcc system_func.c -o run
```
Usage
```
./run add_user <name> <surname> <age> <email> <phone>
./run get_user <surname>
./run del_user <surname>
```
Check module output
```
dmesg
```

## Keyboard interrupt handler
### Run
Execute command
```
./init.sh
```
Check module output
```
dmesg
```
You should see the counter of pressed keyboard key
