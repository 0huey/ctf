# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|

  config.vm.box = "kalilinux/rolling"

   config.vm.provider "virtualbox" do |vb|
     vb.gui = true
     vb.memory = "4096"
   end

   config.vm.provision "shell", path: "provision.sh"
end
