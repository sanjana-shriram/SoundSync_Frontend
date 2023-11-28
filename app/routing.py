from django.urls import re_path
from . import consumers

websocket_urlpatterns = [
    # re_path(r'ws/app/((?P<room_name>\w+)$', consumers.ChatRoomConsumer),
    # this is correct
    re_path(r'^ws/app/(?P<uri>[^/]+)/$', consumers.ChatRoomConsumer.as_asgi())
]
# w+ says anything after the / will be picked up and used as the room name
#
