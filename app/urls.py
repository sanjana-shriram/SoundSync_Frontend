from django.urls import path
# from . import views
from app import views


urlpatterns = [
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
    path('get-var', views.get_variable, name='get-var'),
    path('usingAudio', views.using_audio, name='usingAudio'),
    path('usingEye', views.using_eye, name='usingEye'),

]
