appid = '9d763e7cd096892b5a50b5df25faf8ed'# полученный при регистрации на OpenWeatherMap.org. Что-то вроде такого набора букв и цифр: '6d8e495ca73d5bbc1d6bf8ebd52c4123'

import requests
import serial
import time
import paho.mqtt.client as mqtt_client
import random


def serial_write_and_read(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data


arduino = serial.Serial(port='COM9', baudrate=9600, timeout=.1)


#-----------------------------------------------------------
# Проверка наличия в базе информации о нужном населенном пункте
def get_city_id(s_city_name):
    try:
        res = requests.get('http://api.openweathermap.org/data/2.5/find',
                     params={'q': s_city_name, 'type': 'like', 'units': 'metric', 'lang': 'ru', 'APPID': appid})
        data = res.json()
        cities = ['{} ({})'.format(d['name'], d['sys']['country'])
                  for d in data['list']]
        # print('city:', cities)
        city_id = data['list'][0]['id']
        # print('city_id=', city_id)
    except Exception as e:
        print('Exception (find):', e)
        pass
    assert isinstance(city_id, int)
    return city_id


# Запрос текущей погоды
def request_current_weather(city_id):
    weather_descriptions_array = ['Clear', 'Clouds', 'Rain'] # номер погоды от 0 до 2, 3 варианта
    weather_descriptions_array = ['Clear', 'Clouds', 'Rain', 'Snow', 'Atmosphere', 'Drizzle', 'Thunderstorm'] # номер погоды от 0 до 7 (8 вариантов)
    try:
        res = requests.get('http://api.openweathermap.org/data/2.5/weather',
                     params={'id': city_id, 'units': 'metric', 'lang': 'ru', 'APPID': appid})
        data = res.json()
        # weather = data['weather'][0]['description'] # точное описание погоды
        weather = data['weather'][0]['main'] #общее описание погоды
        weather_num = weather_descriptions_array.index(weather)
        temperature = data['main']['temp']
        # print('conditions:', weather)
        # print('номер в массиве:', weather_num)
        # print('temp:', temperature)
        # print('data:', data)
        return [temperature, weather_num]
    except Exception as e:
        print('Exception (weather):', e)
        pass

def get_weather():
    city = input('Введите город: ')
    # city = 'Иркутск'
    # city = 'Москва'
    city_id = get_city_id(city)
    temperature, weather_num = request_current_weather(city_id)

    separator = '-'
    # temperature = int(temperature)
    weather_string = f'{temperature}{separator}{weather_num}'
    weather_data = weather_num
    return weather_data


#-----------------------------------------------------------
broker="broker.emqx.io"
topic_leap_motion = "leap_motion_states"

def on_message(client, userdata, message):
    data = str(message.payload.decode("utf-8"))
    send_MQTT(data)
    
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
    else:
        print("Failed to connect, return code %d\n", rc)

def mqtt_connection():
    client= mqtt_client.Client(f'client_{random.randint(10000, 99999)}') 
    client.on_message=on_message
    client.on_connect=on_connect

    client.connect(broker) 
    client.loop_start()
    client.subscribe(topic_leap_motion)
    # client.publish(topic_leap_motion, value)
    time.sleep(50)
    client.disconnect()
    client.loop_stop()


#-----------------------------------------------------------
def send_weather():
    weather_num = get_weather()   
    value_from_arduino = serial_write_and_read(str(weather_num))
    print(f'weather_num = {weather_num}')
    print(f'weather from arduino = {value_from_arduino}')

def send_MQTT(data):
    value_from_arduino = serial_write_and_read(str(data))
    print(f'MQTT data = {data}')
    print(f'MQTT data from arduino = {value_from_arduino}')

# def send_mqtt_and_weather():