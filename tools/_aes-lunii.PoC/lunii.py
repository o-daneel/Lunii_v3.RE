import os
import glob
import pathlib

from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad
import binascii

def vectkey_to_bytes(key_vect):
    joined = [k.to_bytes(4, 'little') for k in key_vect]
    return b''.join(joined)

# internal flash personnalized value @0x0800BE00
# AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD
raw_key_device = [0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC, 0xDDDDDDDD]
dev_key = vectkey_to_bytes(raw_key_device)

# 55555555 66666666 77777777 88888888
raw_iv_device = [0x55555555, 0x66666666, 0x77777777, 0x88888888]
dev_iv = vectkey_to_bytes(raw_iv_device)

# generic value read from bt file (ciphered with device key)
# AAAAAAAA BBBBBBBB CCCCCCCC DDDDDDDD
raw_key_generic = [0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC, 0xDDDDDDDD]
gen_key = vectkey_to_bytes(raw_key_generic)

# 55555555 66666666 77777777 88888888
raw_iv_generic = [0x55555555, 0x66666666, 0x77777777, 0x88888888]
gen_iv = vectkey_to_bytes(raw_iv_generic)


def sample_code():

    print(f"Key : {binascii.hexlify(dev_key)} | IV : {binascii.hexlify(dev_iv)}")

    s = b"xxtea is good"

    # Crée un objet AES avec le mode CBC
    decipher = AES.new(dev_key, AES.MODE_CBC, dev_iv)
    cipher = AES.new(dev_key, AES.MODE_CBC, dev_iv)

    plain_hex = "323330323330333030313233343500000000000000000000323330323330333030313233343500000000000000000000"
    plain = binascii.unhexlify(plain_hex)

    # Chiffrage des données
    ciphered = cipher.encrypt(plain)
    ciphered_hex = binascii.hexlify(ciphered).decode('utf-8')

    # Déchiffre les données
    plain2 = decipher.decrypt(ciphered)
    plain2_hex = binascii.hexlify(plain2).decode('utf-8')

    # Affiche les données 
    print(f"Plain    : {plain_hex}")
    print(f"Ciphered : {ciphered_hex.upper()}")
    print(f"Plain    : {plain2_hex}")

    if plain == plain2:
        print("👍")
    else:
        print("👎")


def sample_file():
    print("hello")

    with open("../../dump/1362862A/rf/000/0ACBC5FB", "rb") as fp:
        ciphered = fp.read(0x200)
        print(len(ciphered))
        print(ciphered)
        ciph_hex = binascii.hexlify(ciphered)

        print(ciph_hex)
        dec = xxtea.decrypt(ciphered, lunii_generic_key, padding=False, rounds=lunii_tea_rounds(ciphered))
        hexdec = binascii.hexlify(dec)
        print(dec)
        print(hexdec)


# Decipher one file
def dec_file(key, iv, filename, extension, offset = 0, dec_len = 0):
    if pathlib.Path(filename).is_dir():
        return

    print(f"Processing {filename}", end="")

    with open(filename + extension, "wb") as fo:
        # duplicating the file
        with open(filename, "rb") as fp:
            contents = fp.read()
        fo.write(contents)

        # processing the ciphered part
        with open(filename, "rb") as fp:
            # checking dec_len and offset
            if not dec_len:
                file_stats = os.stat(filename)
                if offset < file_stats.st_size:
                    dec_len = file_stats.st_size - offset

            # processing the ciphered block
            fp.seek(offset)
            ciphered_data = fp.read(dec_len)

        decipher = AES.new(dev_key, AES.MODE_CBC, dev_iv)
        plain_data = decipher.decrypt(ciphered_data)

        fo.seek(offset)
        fo.write(plain_data)

    print(f" as {filename}{extension}")


# Decipher one directory
def dec_dir(key, iv, dirname, extension):
    res_list = glob.glob(dirname + "*")
    res_list = [item for item in res_list if os.path.splitext(item)[1] == ""]
    print(res_list)

    for file in res_list:
        dec_file(key, file, extension)


def dec_story():
    # dec_dir(lunii_generic_key, "../../dump/1362862A/", ".bin")
    # dec_file(lunii_generic_key, "../../dump/root_sd/.md_p2", ".bin")
    # dec_file(lunii_device_key, "../../dump/1362862A/bt_p1", ".bin")
    pass

if __name__ == '__main__':
    # sample_code()
    # dec_file(dev_key, dev_iv, "dump/_v3/fw/3.1.2/lunii1_main.bin", ".plain_hash1.bin", 0x92000, 0x20)
    # dec_file(dev_key, dev_iv, "dump/_v3/fw/3.1.2/lunii1_main.bin", ".plain_hash2.bin", 0x92020, 0x20)
    # dec_file(dev_key, dev_iv, "dump/_v3/fw/3.1.2/lunii1_main.bin", ".plain_hash3.bin", 0x92000, 0x40)
    dec_file(dev_key, dev_iv, "resources/flash_swap/md.Frederir.orig", ".plain", 0x40)
    