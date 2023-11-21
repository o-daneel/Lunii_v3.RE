#!/usr/bin/env python

import sys
import json
import requests

login = sys.argv[1]
pwd = sys.argv[2]

url1 = "https://server-auth-prod.lunii.com/auth/signin"
args1 = {'application':"luniistore_mobile",
         'email':login,
         'password':pwd
        }
auth = requests.post(url1, json = args1)
token = auth.json()['response']['tokens']['access_tokens']['user']['server']
user_id = auth.json()['response']['user_id']
print("Token: {0}".format(token))
print("User ID: {0}".format(user_id))

url2 = "https://server-user-prod.lunii.com/v2/users"
header_auth = {'x-auth-token':auth.json()['response']['tokens']['access_tokens']['user']['server'],
            'authorization': 'Bearer {0}'.format(auth.json()['response']['tokens']['access_tokens']['user']['server'])
           }
user = requests.get(url2, headers=header_auth)
print("List of luniis")
luniis = []
for l in user.json()['response']['luniis']:
    ll = user.json()['response']['luniis'][l]
    ll_serial = ll.get("serial_number", None)
    if ll_serial:
        luniis.append(ll_serial)
        print("  - {0}: id: {1}, SNU: {2}, version: {3}".format(ll['name'], l, ll_serial, ll['version']))

url3 = "https://server-backend-prod.lunii.com/user/devices"
devices = requests.get(url3, headers=header_auth)
print("List of luniis")
for l in devices.json():
    print("  - {0}, SNU: {1}, id: {2}, productId: {3}, vendorId: {4} ".format(l['name'], l.get('serialNumber','unknown'), l['id'], l['metadata']['productId'] if 'metadata' in l else None, l['metadata']['vendorId'] if 'metadata' in l else None))

for snu in luniis:
    url4 = "https://server-backend-prod.lunii.com/factory/products/"
    wap = requests.get(url4 + snu, headers=header_auth)

    if wap.json()['version'] == 'v3':
        print("Wifi PSK for {0} is {1} and ssid is {2}".format(snu, wap.json()['wap']['secret'], wap.json()['wap']['ssid']))

# for snu in luniis:
    # books = requests.get(f"https://server-backend-prod.lunii.com/devices/{snu}/audiobooks/", headers=header_auth)
    # print(books.json())
# userdev = requests.get(f"https://server-backend-prod.lunii.com/user/devices", headers=header_auth)
# print(userdev.json())

# userbooks = requests.get(f"https://server-backend-prod.lunii.com/user/audiobooks", headers=header_auth)
# print(userbooks.json())

# fw = requests.get(f"https://server-backend-prod.lunii.com/devices/{luniis[0]}/firmware?installed=2.6", headers=header_auth)
# print(fw.json())

# set = requests.get(f"https://server-backend-prod.lunii.com/devices/{luniis[0]}/settings", headers=header_auth)
# print(set.json())


# boot = requests.get(f"https://server-backend-prod.lunii.com/devices/{luniis[0]}/boot?command=help", headers=header_auth)
# print(boot.json())


# avatar = requests.get(f"https://server-backend-prod.lunii.com/devices/{luniis[0]}/avatar/", headers=header_auth)
# print(avatar.json())

# books = requests.get(f"https://server-backend-prod.lunii.com/devices/{luniis[0]}/audiobooks/c4139d59-872a-4d15-8cf1-76d34cdf38c6", headers=header_auth)
# print(books.json())
