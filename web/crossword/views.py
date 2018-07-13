from django.shortcuts import render
from django.http import HttpResponse

import socket
from struct import *
import json
from django.views.decorators.csrf import csrf_exempt


def index(request):
    return render(request, "index.html", {"name": "Misha"})

def recvall(sock, n):
    # Helper function to recv n bytes or return None if EOF is hit
    data = b''
    while len(data) < n:
        packet = sock.recv(n - len(data))
        if not packet:
            print("data = ", data)
            return data
        data += packet
    print("data = ", data)
    return data

def recv_number(sock):
    data = recvall(sock, 4)
    number = unpack("i", data)

    return number

@csrf_exempt
def generate(request):
    print(request.POST)

    words = request.POST.getlist("words[]")
    print("Get: ", words)

    data = bytearray()

    words_number = 0
    for w in words:
        if (len(w) != 0):
            words_number += 1

    print("words_number =", words_number)

    words_number = words_number.to_bytes(4, 'little')
    data.extend(words_number)

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

    for i in range(len(words)):
        data = recvall(sock, 16)
        (y, x, direction, length) = unpack("iiii", data)

        word = recvall(sock, length)

        print("y =", y, "x =", x, "direction =", direction, " length =", length, " word =", word.decode("utf-8"))
        for_java_script.append({"x": x, "y": y, "word": word.decode("utf-8"), "direction": direction})


    for_java_script = { "height": H, "width": W, "data": for_java_script }
    print("from socket: H(", H, "), W(", W, ")")

    sock.close()

    return HttpResponse(json.dumps(for_java_script), content_type="application/json")

