# UEFIWorkspace

# 客户端开发平台Linux

## 1.下载依赖软件

```bash
sudo apt install git python3 python3-distutils uuid-dev nasm biosn flex build-essential
```

## 2.版本展示

```bash
root@hcjzxh-virtual-machine:/home# git --version
git version 2.17.1

root@hcjzxh-virtual-machine:/home# python3 --version
Python 3.6.9

oot@hcjzxh-virtual-machine:/home# gcc --version #最好使用5.5版本，其他版本可能会有问题
gcc (Ubuntu 5.5.0-12ubuntu1) 5.5.0 20171010
Copyright (C) 2015 Free Software Foundation, Inc.

root@hcjzxh-virtual-machine:/home# make --version
GNU Make 4.1
Built for x86_64-pc-linux-gnu
```

## 3.下载客户端源代码

```bash
git clone https://github.com/O0O0O0O00O/UEFIWorkspace.git
```

## 4. 配置开发环境

```bash
root@hcjzxh-virtual-machine:/home/hcj# cd UEFIWorkspace/
root@hcjzxh-virtual-machine:/home/hcj/UEFIWorkspace# cd edk2
root@hcjzxh-virtual-machine:/home/hcj/UEFIWorkspace/edk2# make -C BaseTools/ #如果执行错误可以先执行后面的两个source命令，在回来执行该make命令
root@hcjzxh-virtual-machine:/home/hcj/UEFIWorkspace/edk2# cd ..
root@hcjzxh-virtual-machine:/home/hcj/UEFIWorkspace# source myexport.sh
root@hcjzxh-virtual-machine:/home/hcj/UEFIWorkspace# source edk2/edksetup.sh
```

## 5.编译测试

```bash
root@hcjzxh-virtual-machine:/home/hcj/UEFIWorkspace# build -p edk2/EmulatorPkg/EmulatorPkg.dsc -a X64 -t GCC5

```

- 看到done即编译通过

![](https://secure2.wostatic.cn/static/hUpmc9QaMmCQAyFMN6t5He/image.png)

## 6.仿真环境搭建

```bash
root@hcjzxh-virtual-machine:/home/hcj/UEFIWorkspace# mkdir xxx-ovmf #xxx为你的自定义名字
root@hcjzxh-virtual-machine:/home/hcj/UEFIWorkspace# cd xxx-ovmf
root@hcjzxh-virtual-machine:/home/hcj/UEFIWorkspace/xxx-ovmf# mkdir esp
root@hcjzxh-virtual-machine:/home/hcj/UEFIWorkspace/xxx-ovmf# cd esp
root@hcjzxh-virtual-machine:/home/hcj/UEFIWorkspace/xxx-ovmf# mkdir EFI
root@hcjzxh-virtual-machine:/home/hcj/UEFIWorkspace/xxx-ovmf# cd EFI
root@hcjzxh-virtual-machine:/home/hcj/UEFIWorkspace/xxx-ovmf/EFI# cp
root@hcjzxh-virtual-machine:/home/hcj/UEFIWorkspace/xxx-ovmf/esp/EFI# cp ../../../Build/EmulatorX64/DEBUG_GCC5/X64/Shell.efi ./
root@hcjzxh-virtual-machine:/home/hcj/UEFIWorkspace/xxx-ovmf/esp/EFI# mv Shell.efi BootX64.efi

root@hcjzxh-virtual-machine:/home/hcj/UEFIWorkspace/xxx-ovmf/esp/EFI# vim ~/.bashrc
在文末添加
alias xxx-qemuovmf="qemu-system-x86_64 -m 4096 -drive if=pflash,format=raw,file=/home/hcj/UEFIWorkspace/ovmf/OVMF_CODE.fd,readonly=on -drive if=pflash,format=raw,file=/home/hcj/UEFIWorkspace/ovmf/OVMF_VARS.fd,readonly=on -drive format=raw,file=fat:rw://home/hcj/UEFIWorkspace/xxx-ovmf/esp -net none"
# 这里需要将你的file替换成你自己的file
```

