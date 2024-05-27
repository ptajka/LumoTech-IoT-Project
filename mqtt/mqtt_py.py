import time
import paho.mqtt.client as mqtt_client
import random
import serial

broker="broker.emqx.io"

responses = {'d': 7, 
             'u': 6}

def on_message(client, userdata, message):
    data = str(message.payload.decode("utf-8"))
    print("received message =", data)
    
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
    else:
        print("Failed to connect, return code %d\n", rc)


client= mqtt_client.Client(f'client_{random.randint(10000, 99999)}') 
client.on_message=on_message
client.on_connect=on_connect

client.connect(broker) 
client.loop_start() 
print("Subcribing")
client.subscribe("testData")
time.sleep(50)
client.disconnect()
client.loop_stop()