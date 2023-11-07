from django.http import HttpResponse
from django.shortcuts import render, redirect
from django.urls import reverse

# def index(request):
#     return HttpResponse("Hello, world. You're at the APP index.")


def consent_action(request):
    context = {}
    return render(request, 'consentPage.html', context)


def calibrate_action(request):
    context = {}
    return render(request, 'eyeCalibrationPage.html', context)


def upload_action(request):
    context = {}
    return render(request, 'instrumentPage.html', context)


def play_action(request):
    context = {}
    return render(request, 'playPage.html', context)
