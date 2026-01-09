import socket
import threading


class Server:
    def __init__(self):
        self.host = 'localhost'
        self.port = 5050
        self.clients = []
        self.nicknames = []

    def start_server(self):
        try:
            self.sock =  socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.bind((self.host, self.port))
            self.sock.listen()
            self.running = True
            print(f"Server started on {self.host}:{self.port}")

            threading.Thread(target=self.accept_clients, daemon=True).start()
        except self.sock.error as e:
            return 0
    
    def accept_clients(self):
        while self.running:
            try:
                client , addr = self.sock.accept()
                print(f"Connected with {str(addr)}")
                client.send("NICK".encode('utf-8'))
                nickname = client.recv(1024).decode('utf-8')
                self.nicknames.append(nickname)
                self.clients.append(client)
                thread = threading.Thread(target=self.handle_client, args=(client,))
                thread.daemon = True
                thread.start()
            except Exception as e:
                print(f"Error accepting client: {e}")
                break

    def broadcast(self, message):
        for client in self.clients[:]:  # Create a copy to avoid modification during iteration
            try:
                client.send(message)
            except:
                self.remove_client(client)
                
    def remove_client(self, client):
        if client in self.clients:
            index = self.clients.index(client)
            self.clients.remove(client)
            nickname = self.nicknames[index]
            self.nicknames.remove(nickname)
            self.broadcast(f"{nickname} left the chat!".encode('utf-8'))
            client.close()
            
    def handle_client(self, client):
        while True:
            try:
                message = client.recv(1024)
                if message:
                    self.broadcast(message)
                else:
                    self.remove_client(client)
                    break
            except:
                self.remove_client(client)
                break

if __name__ == "__main__":
    server = Server()
    server.start_server()

    try:
        while True:
            pass
    except KeyboardInterrupt:
        print("Server shutting down.")