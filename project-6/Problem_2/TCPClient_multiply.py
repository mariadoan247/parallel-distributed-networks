from socket import *
# Create a TCP socket
clientSocket = socket(AF_INET, SOCK_STREAM)
# Connect to a server process using a tuple of IP and port
serverIP = '127.0.0.1'
serverPort = 12345
serverAddress = (serverIP, serverPort)
clientSocket.connect(serverAddress)
# Send a byte string to the client socket
message = input('Input list of comma-separated numbers:')
clientSocket.send(message.encode("utf-8"))
# Receive a byte string from the client socket
modifiedMessageBytes = clientSocket.recv(1024)
print('From Server: ', modifiedMessageBytes.decode("utf-8"))
clientSocket.close()
