import socket
import json
from typing import Any

class Client:
    def __init__(self, SERVER_HOST='127.0.0.1', SERVER_PORT=5000):
        self.SERVER_HOST = SERVER_HOST
        self.SERVER_PORT = SERVER_PORT
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def SARMESSAGE(self, message: dict) -> Any | Exception: #SARMESSAGE= SEND AN RECEIVE MESSAGE
        try:
            self.client_socket.connect((self.SERVER_HOST, self.SERVER_PORT))
            query = json.dumps(message).encode('utf-8')

            # send message to the server
            self.client_socket.sendall(query)

            # catch the answer from the server 
            response = self.client_socket.recv(4096)
            response_json = json.loads(response.decode('utf-8'))

            self.client_socket.close()
            return response_json

        except Exception as e:
            return e

if __name__ == "__main__":
    c = Client()
    data = {"nom": "Joel", "action": "connect"}
    response = c.SARMESSAGE(data)
    print(response)
