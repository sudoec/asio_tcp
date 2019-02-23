from socket import *

def fxopt(str):
    tctimeClient = socket(AF_INET,SOCK_STREAM)
    tctimeClient.connect(("127.0.0.1",2014))
    tctimeClient.sendall(((len(str)).to_bytes(4, byteorder="little", signed=True).decode()+"get").encode())
    data = tctimeClient.recv(128).decode()
    tctimeClient.close()
    return data

print(fxopt("get"))

# while(True):
#     try:
#         print(fxopt("get"))
#     except Exception:
#         print("Catched.")
