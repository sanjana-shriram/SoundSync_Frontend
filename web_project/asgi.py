"""
ASGI config for web_project project.

It exposes the ASGI callable as a module-level variable named ``application``.

For more information on this file, see
https://docs.djangoproject.com/en/4.2/howto/deployment/asgi/
"""

import os

from django.core.asgi import get_asgi_application

# new imports for application part
from channels.routing import ProtocolTypeRouter, URLRouter
from your_app import routing
# idk if I need this
from app.consumers import ChatRoomConsumer
from django.urls import path


os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'web_project.settings')

# application = get_asgi_application()
application = ProtocolTypeRouter(
    {
        "http": get_asgi_application(),
        "websocket": URLRouter([
            # routing.websocket_urlpatterns
            path("app/<str:room_name>/", ChatRoomConsumer.as_asgi()),
        ]
        ),
    }
)
