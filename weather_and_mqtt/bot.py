import requests
import time
#from  weather import get_city_id
import json

TOKEN = '7449023574:AAGJzL-XcaigYnIu0sTqazWMP0TPGjpnUn4'
URL = 'https://api.telegram.org/bot'

def get_updates(offset=0):
    result = requests.get(f'{URL}{TOKEN}/getUpdates?offset={offset}').json()
    return result['result']

def send_message(chat_id, text):
    requests.get(f'{URL}{TOKEN}/sendMessage?chat_id={chat_id}&text={text}')

def reply_keyboard(chat_id, text):
    reply_markup ={ "keyboard": [["Привет", "Hello"], [{"request_location":True, "text":"Где я нахожусь"}]], "resize_keyboard": True, "one_time_keyboard": True}
    data = {'chat_id': chat_id, 'text': text, 'reply_markup': json.dumps(reply_markup)}
    requests.post(f'{URL}{TOKEN}/sendMessage', data=data)

def check_message(chat_id, message):
    if message.lower() in ['привет', 'hello']:
        send_message(chat_id, 'Привет :)')
    else:
        reply_keyboard(chat_id, 'Я не понимаю тебя :(')

def geocoder(latitude, longitude):
    global city
    token = 'pk.b3175c30f0f65e020cd682b8e49ddf3f'
    headers = {"Accept-Language": "ru"}
    address = requests.get(f'https://eu1.locationiq.com/v1/reverse.php?key={token}&lat={latitude}&lon={longitude}&format=json', headers=headers).json()
    if city := address.get("address", {}).get("city"):
        print(city)
        return f'Твое местоположение: {city}'
    print(address)
    return f'Произошло ошибка при получении местоположения!'

def run():
    update_id = get_updates()[-1]['update_id'] # Присваиваем ID последнего отправленного сообщения боту
    while True:
        time.sleep(2)
        messages = get_updates(update_id) # Получаем обновления
        for message in messages:
            # Если в обновлении есть ID больше чем ID последнего сообщения, значит пришло новое сообщение
            if update_id < message['update_id']:
                update_id = message['update_id']# Присваиваем ID последнего отправленного сообщения боту
                if (user_message := message['message'].get('text')): # Проверим, есть ли текст в сообщении
                    check_message(message['message']['chat']['id'], user_message) # Отвечаем
                if (user_location := message['message'].get('location')): # Проверим, если ли location в сообщении
                    latitude = user_location['latitude']
                    longitude = user_location['longitude']
                    send_message(message['message']['chat']['id'], geocoder(latitude, longitude))

if __name__ == '__main__':
    run()

def get_city():
    return city