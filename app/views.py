from django.http import HttpResponse
from django.shortcuts import render, redirect
from django.urls import reverse
import os
from app.models import Upload
from app.forms import UploadForm
from pdf2image import convert_from_path
from PIL import Image

# Sexy Global Variables
page_number = 1
pdf_path = ""
midi_path = ""
instrument = ""
images_list = []


def home(request):
    context = {}
    return render(request, 'app/consent.html', context)


def consent_action(request):
    context = {}
    return render(request, 'app/consent.html', context)


def calibrate_action(request):
    context = {}
    return render(request, 'app/calibrate.html', context)


def upload_action(request):
    context = {}
    return render(request, 'app/upload.html', context)


def upload_pdf(request):
    context = {}
    global page_number
    global pdf_path
    global midi_path
    global instrument
    global images_list
    if request.method == 'POST':
        form = UploadForm(request.POST, request.FILES)
        if form.is_valid():
            pdf_file = form.cleaned_data['pdf']
            # Save the uploaded PDF file to a specific folder
            output_folder = os.path.join(os.path.dirname(
                os.path.abspath(__file__)),  'outputs')

            with open(os.path.join(output_folder, pdf_file.name), 'wb+') as destination:
                for chunk in pdf_file.chunks():
                    destination.write(chunk)

            # Save PDF as images
            pdf_path = os.path.join(output_folder, pdf_file.name)
            print("pdf path!", type(pdf_path))

            # MIDI File
            midi_file = form.cleaned_data['midi']
            # Save the uploaded PDF file to a specific folder
            midi_output_folder = os.path.join(os.path.dirname(
                os.path.abspath(__file__)),  'outputs')

            with open(os.path.join(midi_output_folder, midi_file.name), 'wb+') as destination:
                for chunk in midi_file.chunks():
                    destination.write(chunk)

            # Save PDF as images
            midi_path = os.path.join(output_folder, midi_file.name)
            print("MIDI path!", midi_path)

            # Get the instrument
            instrument = form.cleaned_data['instrument']
            context['instrument'] = instrument

            # Iterate through each page
            images = convert_from_path(pdf_path)
            images_list = []

            # Save the uploaded PDF file to a specific folder
            image_output_folder = os.path.join(os.path.dirname(
                os.path.abspath(__file__)),  'static')

            # Iterate through each page
            for i, image in enumerate(images):
                # Create a unique file name for each page
                image_file_name = f'page{i + 1}.jpg'

                # Construct the full path for the image file
                image_path = os.path.join(image_output_folder, image_file_name)

                # Save the image
                image.save(image_path)

                # Optionally, if you need to perform other operations:
                # Open the saved image using Pillow (PIL)
                with Image.open(image_path) as img:
                    # Perform operations if needed, e.g., resizing
                    # img = img.resize((new_width, new_height))

                    # Save the modified image
                    img.save(image_path)

                # images_list.append(image_file_name) # works
                images_list.append('page')

            context['images_list'] = images_list
            context['image'] = 'page'
            page_number = 1
            context['page_number'] = page_number
            return render(request, 'app/play.html', context)
    else:
        form = UploadForm()
    return render(request, 'app/upload.html', {'uploadForm': form})


def play_action(request):
    pdf_files = ['page0.jpg', 'page1.jpg']
    # for obj in Upload.objects.all():
    #     pdf_files.append(obj.pdf)
    return render(request, 'app/play.html', {"pdf_files": pdf_files})


def flip_forward(request):
    context = {}
    global page_number
    global images_list
    page_number += 1
    print("YOI ", len(images_list))
    context['images_list'] = images_list
    context['image'] = 'page'
    context['page_number'] = page_number
    return render(request, 'app/play.html', context)


def flip_backward(request):
    context = {}
    global page_number
    global images_list
    page_number -= 1
    if page_number <= 0:
        page_number = 1
    context['images_list'] = images_list
    context['image'] = 'page'
    context['page_number'] = page_number
    return render(request, 'app/play.html', context)

# Audio Alignment Code Here
# run when page shows on


def backend(request):
    context = {}
    global page_number
    global images_list
    page_number += 1

    # audio_1, _ = librosa.load(midi_path, sr=Fs)

    context['images_list'] = images_list
    context['image'] = 'page'
    context['page_number'] = page_number
    return render(request, 'app/play.html', context)
