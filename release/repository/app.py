from flask import Flask
from flask_autoindex import AutoIndex

app = Flask(__name__)
AutoIndex(app, browse_root="auto_index/")

if __name__ == "__main__":
    app.run()
