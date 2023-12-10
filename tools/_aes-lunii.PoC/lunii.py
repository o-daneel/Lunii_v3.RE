import os
import glob
import pathlib

from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad
import binascii


def reverse_bytes(input_bytes):
    if len(input_bytes) %4 != 0:
        print("Input buffer must be modulo 4")
        return None

    groups_of_4 = [input_bytes[i:i+4] for i in range(0, len(input_bytes), 4)]
    reversed_groups = [group[::-1] for group in groups_of_4]
    final_key = b''.join(reversed_groups)

    return final_key


# internal flash personnalized value @0x0800BE00
raw_dev_key = b'00112233445566770011223344556677'
dev_key = reverse_bytes(binascii.unhexlify(raw_dev_key))
# internal flash personnalized value @0x0800BE10
raw_dev_iv = b'8899AABBCCDDEEFF8899AABBCCDDEEFF'
dev_iv = reverse_bytes(binascii.unhexlify(raw_dev_iv))



def load_bt(key, iv, filename):
    plain_bt = filename + ".plain"

    # need to decipher the file first ?
    if not os.path.isfile(plain_bt):
        dec_file(dev_key, dev_iv, filename, ".plain")

    # parsing the keys
    with open(plain_bt, "rb") as fp:
        story_key = reverse_bytes(fp.read(0x10))
        story_iv = reverse_bytes(fp.read(0x10))

    return story_key, story_iv


def dec_md(key, iv, filename, extension):
    dec_file(key, iv, filename, extension, 0x40, 0x30)

    with open(filename+extension, "rb") as fp:
        fp.seek(0x1A)
        plain_snu = fp.read(0x18)
        fp.seek(0x40)
        ciph_snu1 = fp.read(0x18)
        ciph_snu2 = fp.read(0x18)
        assert plain_snu == ciph_snu1, f"\nplain : {binascii.hexlify(plain_snu)}\nciph1 : {binascii.hexlify(ciph_snu1)}"
        assert plain_snu == ciph_snu2, f"\nplain : {binascii.hexlify(plain_snu)}\nciph1 : {binascii.hexlify(ciph_snu2)}"
        print(f"Deciphered and validated for " +  plain_snu.decode("utf-8"))


def dec_cmd(key, iv, filename, extension):
    if pathlib.Path(filename).is_dir():
        return

    print(f"Processing {filename}", end="")

    with open(filename + extension, "wb") as fo:
        # duplicating the file
        with open(filename, "rb") as fp:
            # processing header
            header = fp.read(0x10)
            decipher = AES.new(key, AES.MODE_CBC, iv)
            plain = decipher.decrypt(header)
            fo.write(plain)

            # processing commands
            cmd_data = ".."
            while cmd_data:
                cmd_data = fp.read(0x30)
                decipher = AES.new(key, AES.MODE_CBC, iv)
                plain = decipher.decrypt(cmd_data)
                fo.write(plain)

    print(f" as {filename}{extension}")



# Decipher one file
def dec_file(key, iv, filename, extension, offset = 0, dec_len = 0):
    if not pathlib.Path(filename).is_file():
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

        decipher = AES.new(key, AES.MODE_CBC, iv)
        plain_data = decipher.decrypt(ciphered_data)

        fo.seek(offset)
        fo.write(plain_data)

    print(f" as {filename}{extension}")


# Decipher one directory
def dec_dir(key, iv, dirname, extension):
    res_list = glob.glob(dirname + "*")
    res_list = [item for item in res_list if os.path.splitext(item)[1] == ""]
    # print(res_list)

    for file in res_list:
        dec_file(key, iv, file, extension, 0, 0x200)


def dec_story(key, iv, path):
    story_key, story_iv = load_bt(key, iv, path + "/bt")
    dec_dir(story_key, story_iv, path + "/rf/000/", ".bmp")
    dec_dir(story_key, story_iv, path + "/sf/000/", ".mp3")
    dec_file(story_key, story_iv, path + "/ri", ".plain", 0, 512)
    dec_file(story_key, story_iv, path + "/si", ".plain", 0, 512)
    dec_file(story_key, story_iv, path + "/li", ".plain", 0, 512)


def dec_all_bt(key, iv, path):
    bt_list = glob.glob(path + "/*/bt")
    for bt in bt_list:
        dec_file(key, iv, bt, ".plain")
        uuid_dir = os.path.basename(os.path.dirname(bt))
        os.rename(bt + ".plain", f"{path}/bt.{uuid_dir.upper()}.plain")


# Decipher one file
def enc_file(key, iv, filename, extension, offset = 0, enc_len = 0):
    if not pathlib.Path(filename).is_file():
        return

    print(f"Processing {filename}", end="")

    with open(filename + extension, "wb") as fo:
        # duplicating the file
        with open(filename, "rb") as fp:
            contents = fp.read()
        fo.write(contents)

        # processing the part to cipher
        with open(filename, "rb") as fp:
            # checking enc_len and offset
            if not enc_len:
                file_stats = os.stat(filename)
                if offset < file_stats.st_size:
                    enc_len = file_stats.st_size - offset

            # processing the ciphered block
            fp.seek(offset)
            plain_data = fp.read(enc_len)

        cipher = AES.new(key, AES.MODE_CBC, iv)
        ciphered_data = cipher.encrypt(plain_data)

        fo.seek(offset)
        fo.write(ciphered_data)

    print(f" as {filename}{extension}")


def enc_firmware(key, iv, snu, path):
    enc_file(key, iv, path, ".ciphered")

    fw_ciphered = f"{path.replace('.bin.plain', '')}.{snu[-5:]}.bin"
    if os.path.isfile(fw_ciphered):
        os.remove(fw_ciphered)
    os.rename(path + ".ciphered", fw_ciphered)
    print(f"Plain firmware {path} ciphered for {snu} : {fw_ciphered}")


def bl_update(filename, dev_key, dev_iv, snu, bssid, ssid, pwd):
    target = f"{filename.strip('.bin')}.{snu[-5:]}.bin"
    with open(filename, "rb") as fp:
        with open(target, "wb") as fo:
            fo.write(fp.read())
            fo.seek(0xBE00)
            fo.write(binascii.unhexlify(dev_key))
            fo.write(binascii.unhexlify(dev_iv))
            fo.write(b"\x00"*24)
            fo.seek(0xBE20)
            fo.write(snu.encode("utf-8"))
            fo.seek(0xBE38)
            # fo.write(bssid)
            fo.seek(0xBE48)
            fo.write(ssid.encode("utf-8"))
            fo.write(pwd.encode("utf-8"))


if __name__ == '__main__':
    from  aes_keys_daneel import *
    # enc_firmware(dev_key, dev_iv, dev_snu, "dump/_v3/fw/3.1.3/fa.bin.plain")
    # enc_firmware(dev_key, dev_iv, dev_snu, "dump/_v3/fw/3.1.2/fa.bin.plain")

    from  aes_keys_fred import *
    # dec_md(dev_key, dev_iv, "resources/flash_swap/md.Frederir.orig", ".plain")
    # dec_file(dev_key, dev_iv, "dump/_v3/fw/3.1.2/fa.23023030012345.bin", ".plain")
    # dec_cmd(dev_key, dev_iv, "cmd.23023030012345.ONBOARDED", ".plain")
    # dec_cmd(dev_key, dev_iv, "cmd.23023030012345.RESET", ".plain")

    from  aes_keys_daneel import *
    # dec_file(dev_key, dev_iv, "dump/_v3/fw/3.1.3/fa.23456.bin", ".plain")
    # dec_file(dev_key, dev_iv, "dump/_v3/sd/odaneel/root/wifi.prefs", ".plain", 0, 0X480)
    # dec_md(dev_key, dev_iv, "dump/_v3/sd/odaneel/root/.md", ".plain")
    # bl_update("dump/_v3/fw/BL 3.0/bl.frederir.orig.bin", raw_dev_key, raw_dev_iv, dev_snu, dev_bssid, dev_ssid, dev_pwd)

    # dec_story(dev_key, dev_iv, "dump/_v3/sd/odaneel/root/.content/1BBA473C")
    # dec_story(dev_key, dev_iv, "dump/_v3/sd/odaneel/root/.content/4CDF38C6")
    # dec_story(dev_key, dev_iv, "dump/_v3/sd/odaneel/root/.content/8171A964")
    # dec_story(dev_key, dev_iv, "dump/_v3/sd/odaneel/root/.content/DD2E29B7")
    # dec_story(dev_key, dev_iv, "dump/_v3/sd/odaneel/root/.content/FFB5D68A")