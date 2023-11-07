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
]
