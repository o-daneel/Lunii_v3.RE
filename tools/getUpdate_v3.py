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

fw = requests.get(f"https://server-backend-prod.lunii.com/devices/{luniis[0]}/firmware?installed=2.6", headers=header_auth)
with open("fa.bin", "wb") as fa:
    fa.write(fw.content)
