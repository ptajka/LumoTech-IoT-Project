import asyncio
import websockets
async def test():
    async with websockets.connect('wss://free.blr2.piesocket.com/v3/1?api_key=mewvJjc5Mq1zSr5OlPE2X9jcGDyOg25fVRPnmWgs&notify_self=1') as websocket:
        await websocket.send("hello")
        response = await websocket.recv()
        print(response)
asyncio.get_event_loop().run_until_complete(test())