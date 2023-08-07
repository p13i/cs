import http.server
import socketserver
import os

PORT = 8080
OUTPUT_DIR = "cs/app/server/"

os.chdir(OUTPUT_DIR)

httpd = socketserver.TCPServer(("", PORT), http.server.SimpleHTTPRequestHandler)

print(f"Serving {OUTPUT_DIR} on port {PORT}.")
while True:
    httpd.handle_request()
