from server import Server
import cv2

if __name__ == "__main__":
    server = Server("127.0.0.1", 5002)

    # Check that connection works
    message = server.receive()
    print("[CLIENT]: " + message)
    server.send('Shut up and send an image')