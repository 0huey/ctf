export DEBIAN_FRONTEND=noninteractive

set -x

apt-get update
apt-get install -y xxd terminator firewalld ncat gdb gdbserver jq python3-pip strace openjdk-17-jdk openjdk-17-jre gobuster

pip3 install pwntools ROPGadget keystone-engine ropper

sudo -u vagrant mkdir /home/vagrant/Downloads

curl -s https://api.github.com/repos/NationalSecurityAgency/ghidra/releases/latest | jq .assets[0].browser_download_url | tr -d '"' | wget -qO /home/vagrant/Downloads/ghidra.zip -i -

sudo -u vagrant unzip -q /home/vagrant/Downloads/ghidra.zip -d /home/vagrant/Downloads

set +x
sudo -u vagrant sh -c "$(curl -fsSL http://gef.blah.cat/sh)"
set -x

gem install seccomp-tools

systemctl --now enable firewalld
firewall-cmd --permanent --new-zone=vpn
firewall-cmd --permanent --zone=vpn --add-interface=tun0
firewall-cmd --permanent --zone=public --add-interface=eth0
firewall-cmd --reload

echo "PATH=$PATH:/home/vagrant/.local/bin" >> /home/vagrant/.bashrc

if [[ -f /vagrant/hackthebox/hackthebox.conf ]]; then
    cp /vagrant/hackthebox/hackthebox.conf /etc/openvpn/client/
fi

updatedb
