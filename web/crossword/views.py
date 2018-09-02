from django.shortcuts import render
from django.http import HttpResponse

import socket
from struct import *
import json
from django.views.decorators.csrf import csrf_exempt
import random
import re
import urllib.request


def index(request):
    return render(request, "index.html", {"name": "Misha"})


def recvall(sock, n):
    # Helper function to recv n bytes or return None if EOF is hit
    data = b''
    while len(data) < n:
        packet = sock.recv(n - len(data))
        if not packet:
            return data
        data += packet
    return data


def recv_number(sock):
    data = recvall(sock, 4)
    number = unpack("i", data)

    return number


@csrf_exempt
def generate(request):
    print(request.POST)

    words = request.POST.getlist("words[]")
    print("Get words: ", words)

    descriptions = {}
    descriptions_ = request.POST.getlist("descriptions[]")
    for i in range(len(words)):
        descriptions[words[i]] = descriptions_[i];

    print("Get descriptions: ", descriptions)

    data = bytearray()

    words_number = 0
    for w in words:
        if (len(w) != 0):
            words_number += 1

    print("words_number =", words_number)

    words_number_bytes = words_number.to_bytes(4, 'little')
    data.extend(words_number_bytes)

    for word in words:
        if (len(word) == 0):
            print("zero")
            continue
        word = word.lower()
        data.extend(len(str.encode(word)).to_bytes(4, 'little'))
        data.extend(str.encode(word))

    sock = socket.socket()
    sock.connect(('localhost', 2809))
    sock.send(data)

    data = recvall(sock, 8)
    (H, W) = unpack("ii", data)

    for_java_script = []

    for i in range(words_number):
        data = recvall(sock, 16)
        (y, x, direction, length) = unpack("iiii", data)

        word = recvall(sock, length)

        print("y =", y, "x =", x, "direction =", direction, " length =", length, " word =", word.decode("utf-8"))
        for_java_script.append({
            "x": x,
            "y": y,
            "word": word.decode("utf-8"),
            "direction": direction,
            "description": descriptions[word.decode("utf-8")]
        })

    for_java_script = {"height": H, "width": W, "data": for_java_script}
    print("from socket: H(", H, "), W(", W, ")")

    sock.close()

    return HttpResponse(json.dumps(for_java_script), content_type="application/json")


@csrf_exempt
def save_page(request):
    html_before = request.POST.getlist("html")[0]
    html_after = ""

    html_split = html_before.split('\n')
    for line in html_split:
        res = re.match('.*link.*href=\"(.+)\".*', line)
        if (res):
            url = res.group(1)
            contents = urllib.request.urlopen(url).read().decode('utf-8')
            line = '<style>' + contents + '</style>'

        res = re.match('.*script.*src=\"(.+)\".*', line)
        if (res):
            print(line)
            url = res.group(1)
            try:
                contents = urllib.request.urlopen(url).read().decode('utf-8')
                line = '<script>' + contents + '</script>'
            except:
                continue

        html_after += line + '\n'

    random_number = int(random.random() * 10000000000)
    print(random_number)
    file = open("/tmp/%d" % random_number, "w+")
    file.write(html_after)
    file.close()

    return HttpResponse(json.dumps({"url": random_number}), content_type="application/json")


def download_page(request, random_number):
    print(random_number)
    print(int(random_number))
    filepath = '/tmp/%d' % int(random_number)
    with open(filepath, 'r') as fp:
        data = fp.read()
    filename = 'crossword.html'
    response = HttpResponse()
    response['Content-Disposition'] = 'attachment; filename=%s' % filename  # force browser to download file
    response.write(data)
    return response
