import time
import paho.mqtt.client as mqtt_client
import random

broker="broker.emqx.io"

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
    else:
        print("Failed to connect, return code %d\n", rc)


client= mqtt_client.Client("client")
client.on_connect = on_connect
client.connect(broker)
client.loop_start() 


for _ in range(10):
    state = "u" if random.randint(0,1) else "d"
    # rnd = random.randint(4, 10)
    client.publish("testData", state)
    print(f"publish state is {state}")
    time.sleep(random.randint(4, 10))
    
client.disconnect()
client.loop_stop()



