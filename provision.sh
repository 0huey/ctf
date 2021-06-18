export DEBIAN_FRONTEND=noninteractive

apt-get update
#apt-get upgrade -y
apt-get install -y firewalld ncat gdb gdbserver python3-pip strace openjdk-11-jdk openjdk-11-jre gradle
#apt-get autoremove -y

pip3 install pwntools ROPGadget

sudo -u vagrant mkdir -p /home/vagrant/Downloads

GHIDRA_VERSION=$(curl -Ls https://ghidra-sre.org/ | grep -oE "ghidra_.*zip" | head -n 1)
echo Downloading ${GHIDRA_VERSION}
wget -q -O /home/vagrant/Downloads/ghidra.zip https://ghidra-sre.org/${GHIDRA_VERSION}

cd /home/vagrant/Downloads/
sudo -u vagrant unzip -q ghidra.zip

sudo -u vagrant sh -c "$(curl -fsSL http://gef.blah.cat/sh)"

echo Setting up firewalld
systemctl --now enable firewalld
firewall-cmd --permanent --new-zone=vpn
firewall-cmd --permanent --zone=vpn --add-interface=tun0
firewall-cmd --permanent --zone=public --add-interface=eth0
firewall-cmd --reload

if [[ -f /vagrant/hackthebox/hackthebox.conf ]]; then
    cp /vagrant/hackthebox/hackthebox.conf /etc/openvpn/client/
fi
