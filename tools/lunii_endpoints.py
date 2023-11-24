#!/usr/bin/env python

from lunii_lib import *

# login = sys.argv[1]
# pwd = sys.argv[2]


def get_wifiPsk():
    auth = get_authToken(login, pwd)
    devices = get_devices(auth)
    prod_getWifiPsk(auth, devices[0])

def get_allCmd():
    auth = get_authToken(login, pwd)
    devices = get_devices(auth)
    user_audioBooks(auth)
    dev_getCmd(auth, devices[0], "RESET")
    dev_getCmd(auth, devices[0], "ONBOARDED")

def get_settings():
    auth = get_authToken(login, pwd)
    devices = get_devices(auth)
    dev_getSettings(auth, devices[0])

def get_update():
    auth = get_authToken(login, pwd)
    devices = get_devices(auth)
    dev_getFW(auth, devices[0], "3.1.2")
    dev_getFW(auth, devices[0], "3.1.3")

def session():
    auth = get_authToken(login, pwd)
    devices = get_devices(auth)
    dev_signin(auth, devices[0], True)
    dev_signout(auth, devices[0])

def start_pairing():
    auth = get_authToken(login, pwd)
    user_devicePair(auth, "23023030000001", True)


if __name__ == '__main__':
    # get_wifiPsk()
    # get_allCmd()
    # get_settings()
    # get_update()
    # session()
    start_pairing()

