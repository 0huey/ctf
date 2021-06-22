export DEBIAN_FRONTEND=noninteractive

apt-get update
apt-get install -y firewalld ncat gdb gdbserver python3-pip strace openjdk-11-jdk openjdk-11-jre

pip3 install pwntools ROPGadget keystone-engine ropper

sudo -u vagrant mkdir /home/vagrant/Downloads

GHIDRA_VERSION=$(curl -Ls https://ghidra-sre.org/ | grep -oE "ghidra_.*zip" | head -n 1)
echo Downloading ${GHIDRA_VERSION}
wget -q -O /home/vagrant/Downloads/ghidra.zip https://ghidra-sre.org/${GHIDRA_VERSION}

sudo -u vagrant unzip -q /home/vagrant/Downloads/ghidra.zip -d /home/vagrant/Downloads

sudo -u vagrant sh -c "$(curl -fsSL http://gef.blah.cat/sh)"

gem install seccomp-tools

echo Setting up firewalld
systemctl --now enable firewalld
firewall-cmd --permanent --new-zone=vpn
firewall-cmd --permanent --zone=vpn --add-interface=tun0
firewall-cmd --permanent --zone=public --add-interface=eth0
firewall-cmd --reload

echo "PATH=$PATH:/home/vagrant/.local/bin" >> /home/vagrant/.bashrc

if [[ -f /vagrant/hackthebox/hackthebox.conf ]]; then
    cp /vagrant/hackthebox/hackthebox.conf /etc/openvpn/client/
fi
