import socket
import threading

class Chat:
    def __init__(self):
        self.host = 'localhost'
        self.port = 5050
        self.client = None
        self.nickname = None
        self.connected = False

    def connect_to_server(self, nickname):
        self.nickname = nickname
        if not nickname:
            return 0
        
        else:
            self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.client.connect((self.host, self.port))
            self.connected = True
            threading.Thread(target=self.receive_messages, daemon=True).start()
            print(f"connected as {self.nickname}")

    def receive_messages(self):
        while self.connected:
            try:
                message = self.client.recv(1024).decode('utf-8')
                if message == "NICK":
                    self.client.send(self.nickname.encode('utf-8'))
                else:
                    self.display_message(message)
            except:
                 if self.connected:
                    self.display_message("Connection lost!")
                    self.disconnect_from_server()
                    break
    def disconnect_from_server(self):
        self.connected = False
        if self.client:
            self.client.close()
        
        print("Disconnected from server")

    def display_message(self,recvmsg):
        print(recvmsg)

    def send_message(self):
        if not self.connected:
            return
        else:
            print("Enter your message: ", end="")
            try:
                you = input("")
                message = f"{self.nickname}: {you}"
                self.client.send(message.encode('utf-8'))
            except:
                print("Failed to send message")
                self.disconnect_from_server()

if __name__ == "__main__":
    client = Chat()
    client.connect_to_server(input("Enter your nickname: "))

    while client.connected:
        client.send_message()