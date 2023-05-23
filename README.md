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

## Schedule process counter and save to procfs
### 
Apply patch for `vmlinuz-6.4.0-rc3-dirty` and check proc folder. You should find something like this - `/proc/<xxx>/schedule_counter` where store counter number of task_struct field

Light way
```
tar xzf linux6.4.0.tar.gz
```
Run VM with kernel
```
emu-system-x86_64 -kernel ./build/vmlinuz-6.4.0-rc3-dirty \
                  -m 512m 
                  -initrd ./build/initramfs.gz 
                  -nographic 
                  -append "console=ttyS0" 
                  --cpu host 
                  --enable-kvm
```
Check `/proc` space with `/proc/<xxx>/schedule_counter`