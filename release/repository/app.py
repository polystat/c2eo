# from http.server import HTTPServer, BaseHTTPRequestHandler
#
#
# def run(server_class=HTTPServer, handler_class=BaseHTTPRequestHandler):
#     server_address = ('', 8000)
#     httpd = server_class(server_address, handler_class)
#     httpd.serve_forever()

# import http.server
# import socketserver
#
# PORT = 8000
#
# Handler = http.server.SimpleHTTPRequestHandler
#
# with socketserver.TCPServer(("", PORT), Handler) as httpd:
#     print("serving at port", PORT)
#     httpd.serve_forever()

# from flask import Flask
# app = Flask(__name__)
#
#
# @app.route('/')
# def hello_world():
#     return 'Hello, World!'

from flask import Flask
from flask_autoindex import AutoIndex

app = Flask(__name__)

ppath = "debian/"

app = Flask(__name__)
AutoIndex(app, browse_root=ppath)

if __name__ == "__main__":
    app.run()
