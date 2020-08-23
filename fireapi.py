from flask import Flask,jsonify
import json

app = Flask(__name__)


with open('viirs.json') as f:
  fire = json.load(f)


@app.route("/fire", methods = ["GET"])
def getFire():
	#return jsonify(fire)
    return json.dumps(fire)
if __name__ == '__main__':
	app.run(port=8080,debug=True, host='192.168.10.170')
