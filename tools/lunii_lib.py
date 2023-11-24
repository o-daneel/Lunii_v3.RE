#!/usr/bin/env python

import sys
import json
import requests
import hexdump
import base64

sys.path.insert(1, './tools/_aes-lunii.PoC/')

from aes_keys import *
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad


def get_authToken(login, pwd, debug = False):
    url1 = "https://server-auth-prod.lunii.com/auth/signin"
    args1 = {'application':"luniistore_mobile",
            'email':login,
            'password':pwd
            }
    auth = requests.post(url1, json = args1)
    token = auth.json()['response']['tokens']['access_tokens']['user']['server']
    user_id = auth.json()['response']['user_id']

    print("\nToken: {0}".format(token))
    if debug:
        raw = base64.b64decode(token.encode('utf-8') + b'===')
        hexdump.hexdump(raw)

    print("\nUser ID: {0}".format(user_id))
    if debug:
        raw = base64.b64decode(user_id.encode('utf-8') + b'===')
        hexdump.hexdump(raw)


    header_auth = {'x-auth-token':auth.json()['response']['tokens']['access_tokens']['user']['server'],
                   'authorization': 'Bearer {0}'.format(auth.json()['response']['tokens']['access_tokens']['user']['server'])
                  }

    return header_auth


def get_devices(json_auth, debug=False):
    url2 = "https://server-user-prod.lunii.com/v2/users"
    user = requests.get(url2, headers=json_auth)
    print("\nList of luniis")
    luniis = []

    devices =  user.json()['response'].get('luniis')
    if not devices:
        return luniis
    
    for lun in devices:
        one_lunii = devices[lun]
        lunii_serial = one_lunii.get("serial_number", None)
        if lunii_serial:
            luniis.append(lunii_serial)
            print("  - {0}: id: {1}, SNU: {2}, version: {3}".format(one_lunii['name'], lun, lunii_serial, one_lunii['version']))

    return luniis


def dev_getFW(json_auth, snu, installed, debug=False):
    fw = requests.get(f"https://server-backend-prod.lunii.com/devices/{snu}/firmware?installed={installed}", headers=json_auth)
    if fw.status_code == 200:
        with open(f"fa.{snu}.bin", "wb") as fa:
            fa.write(fw.content)
            print(f"Firmware downloaded : {fa.name} ({fa.tell()} bytes)")

    elif fw.status_code == 204:
        print("No updates available...")
    else:
        print(fw.json())


def dev_getCmd(json_auth, snu, command):
    fw = requests.get(f"https://server-backend-prod.lunii.com/devices/{snu}/boot?command={command.upper()}", headers=json_auth)
    if fw.status_code == 200:
        with open(f"cmd.{snu}.{command}", "wb") as fa:
            fa.write(fw.content)
            print(f"Script downloaded : {fa.name} ({fa.tell()} bytes)")

    else:
        print(fw.json())


def dev_getSettings(json_auth, snu, debug=False):
    rqt_books = requests.get(f"https://server-backend-prod.lunii.com/devices/{snu}/settings", headers=json_auth)
    print(rqt_books.json())


def dev_signin(json_auth, snu, debug=False):
    body = {"vendorId": "0x0483", "productId": "0xa341", "firmwareVersion": "3.1.2", "sdCardSize": 1024, "sdCardFree": 1000, "sdCardUsed": 24,"batteryLevel": 95, "batteryCharging": False, "wifiLevel": 5, "wifiSsid": "LUNII_AP"}
    rqt_challenge = requests.post(f"https://server-backend-prod.lunii.com/devices/{snu}/signin", headers=json_auth, json = body)
    print(rqt_challenge.json())

    challenge = rqt_challenge.json()['challenge']
    if debug:
        raw = base64.urlsafe_b64decode(challenge + '==')
        hexdump.hexdump(raw)

    cipher = AES.new(dev_key, AES.MODE_CBC, dev_iv)
    challenge_plain = cipher.decrypt(raw)

    print(f"Plain token : {challenge_plain}")
    hexdump.hexdump(challenge_plain)


def dev_signout(json_auth, snu, debug=False):
    rqt_out = requests.get(f"https://server-backend-prod.lunii.com/devices/{snu}/signout", headers=json_auth)
    print(rqt_out.json())


def prod_getWifiPsk(json_auth, snu, debug=False):
    fw = requests.get(f"https://server-backend-prod.lunii.com/factory/products/{snu}", headers=json_auth)
    print(fw.json())


def user_devicePair(json_auth, snu, debug=False):
    body_pair = {'serialNumber': snu }
    rqt_ptoken = requests.post(f"https://server-backend-prod.lunii.com/user/devices", headers=json_auth, data=body_pair)
    if rqt_ptoken.status_code != 200:
        return
    deviceId = rqt_ptoken.json().get('deviceId')
    pairingToken = rqt_ptoken.json().get('pairingToken')

    print("Device ID: {0}".format(deviceId))
    print("pairingToken: {0}".format(pairingToken))
    
    cipher = AES.new(dev_key, AES.MODE_CBC, dev_iv)

    ciphered_token = cipher.encrypt(pairingToken.encode('utf-8'))
    print("cipheredToken: {0}".format(ciphered_token))
    hexdump.hexdump(ciphered_token)

    b64_token = base64.urlsafe_b64encode(ciphered_token)
    final_token = b64_token.decode("utf-8")
    print(f"pairingToken : {final_token}")

    body_pair.update({'pairingToken': final_token})

    fw = requests.post(f"https://server-backend-prod.lunii.com/devices", headers=json_auth, data=body_pair)
    print(fw.status_code)
    print(fw.json())


def user_audioBooks(json_auth, debug=False):
    rqt_books = requests.get(f"https://server-backend-prod.lunii.com/user/audiobooks", headers=json_auth)
    print(rqt_books.json())


