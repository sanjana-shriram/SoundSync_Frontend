from django.http import HttpResponse
from django.shortcuts import render, redirect
from django.urls import reverse
import os
from app.models import PDF, Midi, Instrument
from app.forms import PDFForm, MidiForm, InstrumentForm


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


def choose_instrument(request):
    if request.method == 'POST':
        form = InstrumentForm(request.POST, request.FILES)
        if form.is_valid():
            instrument = form.cleaned_data['instrument']
            # Save the uploaded PDF file to a specific folder
            output_folder = os.path.join(os.path.dirname(
                os.path.abspath(__file__)),  'outputs')
            with open(os.path.join(output_folder, instrument), 'wb+') as destination:
                destination.write(instrument)
            return render(request, 'upload.html', {'form': form})
    else:
        form = InstrumentForm()
    return render(request, 'upload.html', {'form': form})


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

            return render(request, 'upload.html', {'pdfForm': form})
    else:
        form = PDFForm()
    return render(request, 'upload.html', {'pdfForm': form})


def upload_midi(request):
    if request.method == 'POST':
        form = MidiForm(request.POST, request.FILES)
        if form.is_valid():
            midi_file = form.cleaned_data['midi']
            # Save the uploaded MIDI file to a specific folder
            output_folder = os.path.join(os.path.dirname(
                os.path.abspath(__file__)),  'outputs')
            with open(os.path.join(output_folder, midi_file.name), 'wb+') as destination:
                for chunk in midi_file.chunks():
                    destination.write(chunk)

            return render(request, 'upload.html', {'midiForm': form})
    else:
        form = MidiForm()
    return render(request, 'upload.html', {'midiForm': form})
