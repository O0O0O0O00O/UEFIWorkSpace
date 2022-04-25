#fs0:
#cd EFI
#cd Boot
##真实环境下的网络配置
#load E9602X3.efi
#load SnpDxe.efi ArpDxe.efi Ip4Dxe.efi VlanConfigDxe.efi Udp4Dxe.efi Dhcp4Dxe.efi Mtftp4Dxe.efi TcpDxe.efi
#ifconfig -s eth0 static 192.168.0.5 255.255.255.0 192.168.0.1
#ifconfig -s eth0 dhcp
#ifconfig -l eth0 
#
#Security_lab.efi #后面书写ip地址和端口
#if %lasterror% ne 0 then
#    reset
#else
#    Win_BootX64.efi
#endif 
#echo %lasterror%


