from django.http import HttpResponse
from django.shortcuts import render, redirect
from django.urls import reverse

# def index(request):
#     return HttpResponse("Hello, world. You're at the APP index.")


def home(request):
    context = {}
    return render(request, 'consent.html', context)


def consent_action(request):
    context = {}
    return render(request, 'consent.html', context)


def calibrate_action(request):
    context = {}
    return render(request, 'calibrate.html', context)


def upload_action(request):
    context = {}
    return render(request, 'upload.html', context)


def play_action(request):
    context = {}
    return render(request, 'play.html', context)
