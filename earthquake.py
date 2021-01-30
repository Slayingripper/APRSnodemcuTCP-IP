from __future__ import unicode_literals

from tornado.websocket import websocket_connect
from tornado.ioloop import IOLoop
from tornado import gen
import aprslib
import logging
import json
import sys

echo_uri = 'wss://www.seismicportal.eu/standing_order/websocket'
PING_INTERVAL = 15

#You can modify this function to run custom process on the message
def myprocessing(message):
    try:
        data = json.loads(message)
        info = data['data']['properties']
        info['action'] = data['action']
        logging.info('>>>> {action:7} event from {auth:7},T0:{time},Lat{lat},Lon{lon}, Mag:{mag}, Region: {flynn_region}'.format(**info))
        mag = data['data']['properties']['mag']
        lat = data["data"]["properties"]["lat"]
        lon = data["data"]["properties"]["lon"]
        time = data["data"]["properties"]["time"]
        print("EDULEPSEN")
        print(lat,lon,mag)
        callback(lat,lon,mag)
    except Exception:
        logging.exception("Unable to parse json message")

@gen.coroutine
def listen(ws):
    while True:
        msg = yield ws.read_message()
        if msg is None:
            logging.info("close")
            self.ws = None
            break
            
        myprocessing(msg)
        
        print(msg)

@gen.coroutine
def launch_client():
    try:
        logging.info("Open WebSocket connection to %s", echo_uri)
        ws = yield websocket_connect(echo_uri, ping_interval=PING_INTERVAL)
    except Exception:
        logging.exception("connection error")
    else:
        logging.info("Waiting for messages...")
        listen(ws)
        

def callback(lat,lon,mag):
    #  print(packet)
    # # a valid passcode for the callsign is required in order to send
    AIS = aprslib.IS("5B4ANU", passwd="15540", host = "asia.aprs2.net" ,port=14580)

    AIS.connect()
    # # send a single status message
    AIS.sendall("5B4ANU-12>APRS15,WIDE1-1:>""3506.19N/""03321.63E")

    #AIS.sendall("5B4ANU-12>APRS15,WIDE1-1:@""3506.19N/""03321.63E""&""Magnitude:"+str(mag)+",Location:Latitude"+str(lat)+",Longitude"+str(lon)+",""Testing Phase")
   # AIS.sendall("5B4ANU-12>APDR15,WIDE1-1:="+lat+"N/""0""+lon+"E:"""EARTHQUAKE MAGNITUDE:"+mag+"")

if __name__ == '__main__':
    logging.basicConfig(stream=sys.stdout, level=logging.INFO)
    ioloop = IOLoop.instance()
    launch_client()
   	
    try:
        ioloop.start()
    except KeyboardInterrupt:
        logging.info("Close WebSocket")
        ioloop.stop()
