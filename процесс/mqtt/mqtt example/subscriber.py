import time
import paho.mqtt.client as mqtt_client
import random
import serial

broker="broker.emqx.io"

responses = {'d': 7, 
             'u': 6}

def on_message(client, userdata, message):
    global responses
    time.sleep(1)
    data = str(message.payload.decode("utf-8"))
    if data in list(responses.keys()):
        resp = send_command(data, responses[data])
        print(resp)
    print("received message =", data)
    
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
    else:
        print("Failed to connect, return code %d\n", rc)

port = "COM5"
connection = serial.Serial(port, timeout=1)



def send_command(cmd: str, response_len: int) -> str:
    str_resp = None
    connection.write(cmd.encode())
    if response_len != 0:
        resp = connection.read(response_len)
        str_resp = resp.decode()
    return str_resp

client= mqtt_client.Client(f'lab_{random.randint(10000, 99999)}') 
client.on_message=on_message
client.on_connect=on_connect

client.connect(broker) 
client.loop_start() 
print("Subcribing")
client.subscribe("labs")
time.sleep(50)
client.disconnect()
client.loop_stop()
