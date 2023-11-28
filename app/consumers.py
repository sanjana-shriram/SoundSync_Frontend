# consumers.py
from asgiref.sync import async_to_sync
from channels.layers import get_channel_layer
from . import consumers
from django.urls import re_path
import json
import asyncio
from channels.generic.websocket import AsyncWebsocketConsumer


class ChatRoomConsumer(AsyncWebsocketConsumer):

    async def connect(self):
        self.room_name = self.scope['url_route']['kwargs']['room_name']
        self.room_group_name = 'app_%s' % self.room_name
        await self.channel_layer.group_add(
            self.room_group_name,
            self.channel_name
        )

        await self.accept()

        await self.channel_layer.group_send(
            self.room_group_name,
            {
                'type': 'tester_message',
                'tester': 'hello capybara world',
            }
        )

    async def tester_message(self, event):
        tester = event['tester']

        await self.send(text_data=json.dumps({
            'tester': tester,
        }))

    async def disconnect(self, close_code):
        await self.channel_layer.group_discard(
            self.room_group_name,
            self.channel_name
        )

    # async def update_cursor(self, event):
    #     measure_num = event['measure_num']

    #     # Send the measure_num to the WebSocket
    #     await self.send(text_data=json.dumps({
    #         'measure_num': measure_num
    #     }))

    async def receive(self, text_data):
        text_data_json = json.loads(text_data)
        message = text_data_json['message']  # passed in data as message
        # Handle incoming WebSocket messages if needed


# tasks.py or wherever your periodic task is defined


async def update_data_periodically(room_name):
    channel_layer = get_channel_layer()
    while True:
        data = fetch_updated_data()  # Your function to fetch updated data
        measure_num = data['measure_num']

        await channel_layer.group_send(
            'SoundSync',
            {
                'type': 'update_measure_num',
                'measure_num': measure_num,
            }
        )
        console.log("the update_data_periodically function is running")
        await asyncio.sleep(0.5)  # Wait for 500 milliseconds
