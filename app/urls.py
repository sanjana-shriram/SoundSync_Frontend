from django.urls import path
# from . import views
from app import views


urlpatterns = [
    # path("", views.index, name="index"),
    path('', views.consent_action, name='home'),
    path('consent', views.consent_action, name='consent'),
    path('calibrate', views.calibrate_action, name='calibrate'),
    path('upload', views.upload_action, name='upload'),
    path('play', views.play_action, name='play'),
    path('upload-pdf', views.upload_pdf, name='upload-pdf'),
    path('forwardPageFlip', views.flip_forward, name='forwardPageFlip'),
    path('backwardPageFlip', views.flip_backward, name='backwardPageFlip'),
    path('backend', views.backend, name='backend'),
    path('get-global', views.get_list_json_dumps_serializer),
    path('<str:room_name>/', views.room, name='room'),

    # path('upload-process', views.upload_process, name='upload-process'),
    # path('upload/<int:id>', views.get_uploads, name='get-upload'),

    # path('show-pdf/', views.show_pdf, name='show-pdf'),
    # path('choose-instrument', views.choose_instrument, name='choose-instrument'),
    # path('upload-music', views.upload_music, name='upload-music'),
    # path('upload-midi', views.upload_midi, name='upload-midi'),
]
