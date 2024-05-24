appid = '9d763e7cd096892b5a50b5df25faf8ed'# полученный при регистрации на OpenWeatherMap.org. Что-то вроде такого набора букв и цифр: '6d8e495ca73d5bbc1d6bf8ebd52c4123'

import requests

# Проверка наличия в базе информации о нужном населенном пункте
def get_city_id(s_city_name):
    try:
        res = requests.get('http://api.openweathermap.org/data/2.5/find',
                     params={'q': s_city_name, 'type': 'like', 'units': 'metric', 'lang': 'ru', 'APPID': appid})
        data = res.json()
        cities = ['{} ({})'.format(d['name'], d['sys']['country'])
                  for d in data['list']]
        print('city:', cities)
        city_id = data['list'][0]['id']
        print('city_id=', city_id)
    except Exception as e:
        print('Exception (find):', e)
        pass
    assert isinstance(city_id, int)
    return city_id


# Запрос текущей погоды
def request_current_weather(city_id):
    weather_descriptions_array = ['Thunderstorm', 'Drizzle', 'Rain', 'Snow', 'Atmosphere', 'Clear', 'Clouds']
    try:
        res = requests.get('http://api.openweathermap.org/data/2.5/weather',
                     params={'id': city_id, 'units': 'metric', 'lang': 'ru', 'APPID': appid})
        data = res.json()
        # weather = data['weather'][0]['description'] # точное описание погоды
        weather = data['weather'][0]['main'] #общее описание погоды
        weather_num = weather_descriptions_array.index(weather)
        temperature = data['main']['temp']
        print('conditions:', weather)
        print('номер в массиве:', weather_num)
        print('temp:', temperature)
        # print('data:', data)
    except Exception as e:
        print('Exception (weather):', e)
        pass


city = input('Введите город: ')
city_id = get_city_id(city)

request_current_weather(city_id)