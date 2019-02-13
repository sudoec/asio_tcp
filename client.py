from socket import *

def fxoperate(str):
    tctimeClient = socket(AF_INET,SOCK_STREAM)
    tctimeClient.connect(("localhost",2012))
    tctimeClient.send(str.encode())
    data = tctimeClient.recv(128).decode()
    tctimeClient.close()
    return data

print(fxoperate("get"))