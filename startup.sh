#/bin/sh
sudo apt-get install qemu
sudo apt-get install libc6-dev:i386
git clone https://github.com/mit-pdos/xv6-public.git xv6
chmod 700 -R xv6
cd xv6
make
make qemu