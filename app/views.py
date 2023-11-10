from django.http import HttpResponse
from django.shortcuts import render, redirect
from django.urls import reverse
import os
from app.models import PDF
from app.forms import PDFForm


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


def upload_music(request):
    if request.method == 'POST':
        form = PDFForm(request.POST, request.FILES)
        if form.is_valid():
            pdf_file = form.cleaned_data['pdf']
            # Save the uploaded PDF file to a specific folder
            output_folder = os.path.join(os.path.dirname(
                os.path.abspath(__file__)),  'outputs')
            with open(os.path.join(output_folder, pdf_file.name), 'wb+') as destination:
                for chunk in pdf_file.chunks():
                    destination.write(chunk)

            return render(request, 'upload.html', {'form': form})
    else:
        form = PDFForm()
    return render(request, 'upload.html', {'form': form})


def upload_midi(request):
    if request.method == 'POST':
        print("request method was post yoi")
    context = {}
    return render(request, 'upload.html', context)
