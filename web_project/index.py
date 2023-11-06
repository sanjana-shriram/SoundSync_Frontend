from django.http import HttpResponse
from django.shortcuts import render


def consentPage(request):
    return render(request, 'consentPage.html')


def eyeCalibrationPage(request):
    return render(request, 'eyeCalibrationPage.html')


def instrumentPage(request):
    return render(request, 'instrumentPage.html')


def playPage(request):
    return render(request, 'playPage.html')
