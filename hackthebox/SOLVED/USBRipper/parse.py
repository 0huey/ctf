#!/usr/bin/env python3

import json

class Device():
    def __init__(self):
        pass

    def __str__(self):
        data = list()

        data.append( "idVendor={}".format(self.id_vendor) )
        data.append( "idProduct={}".format(self.id_product) )
        data.append( "Product={}".format(self.product) )
        data.append( "Manufacturer={}".format(self.manufacturer) )
        data.append( "SerialNumber={}".format(self.serial) )

        return " ".join(data)


def Main():
    usbs = list()

    with open("usb-ripper/syslog") as f:
        for line in f.readlines():
            if ": New USB device found," in line:
                dev = Device()

                for item in line.split():
                    if "idVendor" in item:
                        dev.id_vendor = item.split('=')[1].strip(',')

                    elif "idProduct" in item:
                        dev.id_product = item.split('=')[1]

            elif ": Product:" in line:
                dev.product = line.split()[-1]

            elif ": Manufacturer:" in line:
                dev.manufacturer = line.split()[-1]

            elif ": SerialNumber:" in line:
                dev.serial = line.split()[-1]

            elif ": USB Mass Storage device detected" in line:
                usbs.append(dev)

    with open("usb-ripper/auth.json") as f:
        auth_data = json.load(f)

        auth_manf = set( auth_data["manufact"] )

        auth_prod = set( auth_data["prod"] )

        auth_serial = set( auth_data["serial"] )


    for device in usbs:
        if not device.serial in auth_serial:
            print(device)


if __name__ == '__main__':
    Main()
