from django.http import HttpResponse
from django.shortcuts import render, redirect
from django.urls import reverse
from django.core import serializers
import os
from app.models import Upload
from app.forms import UploadForm
from pdf2image import convert_from_path
from PIL import Image
# import numpy as np
import json

# backend imports
# from pvrecorder import PvRecorder
# import librosa
# from synctoolbox.dtw.cost import cosine_distance
# from synctoolbox.dtw.core import compute_warping_path

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

                # images_list.append(image_file_name)  # works
                images_list.append('page')
            print("images list in big function ", images_list)
            context['images_list'] = images_list
            context['image'] = 'page'
            page_number = 1
            context['page_number'] = page_number
            return render(request, 'app/play.html', context)
    else:
        form = UploadForm()
    return render(request, 'app/upload.html', {'uploadForm': form})


def play_action(request):
    global page_number
    global pdf_path
    global midi_path
    global images_list
    context = {}
    context['images_list'] = images_list
    context['image'] = 'page'
    page_number = 1
    context['page_number'] = page_number
    runBackend()
    return render(request, 'app/play.html', context)


def flip_forward(request):
    context = {}
    global page_number
    global images_list
    total_pages = len(images_list)
    page_number += 1
    print("BEFORE images length", total_pages, "page num", page_number)
    # TODO: find total_pages and make it persistent
    if page_number > 3:
        page_number = 3
    print("AFTER images length", total_pages, "page num", page_number)

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

########################################################################################
# Audio Alignment Code Here
# run when page shows on


def getRefAudio(Fs):
    global midi_path
    # refAudio, Fs = librosa.load(midi_path, sr=Fs)
    # return refAudio, Fs
    return


def getLiveAudio(recorder):
    # access pv recorder object
    numFrames = 20  # change to change length of recorded segment
    allFrames = []
    for i in range(numFrames):
        frame = recorder.read()
        allFrames.extend(frame)
    npFrame = np.asarray(allFrames)
    # idk why, but this is the conversion factor
    npFrame = np.divide(npFrame, 2**15)
    return


def audioSetup():
    device_index = 0
    # recorder = PvRecorder(frame_length=1024, device_index=device_index)
    return


def alignAudio(liveAudio):
    N = 2048
    H = 512  # decrease this number for more precision but longer computation
    Fs = 48100  # or maybe 22050
    # refAudio = getRefAudio(Fs)
    # refChroma = librosa.feature.chroma_stft(y=refAudio, sr=Fs, n_fft=N,
    # hop_length=H, norm=2.0)
    # liveChroma = librosa.feature.chroma_stft(y=liveAudio, sr=Fs, n_fft=N,
    #  hop_length=H, norm=2.0)
    # C = cosine_distance(chroma_1, chroma_2)
    # _, _, wp_full = compute_warping_path(C = C, implementation = "synctoolbox")
    # wp_full = np.multiply(wp_full, (H/Fs))
    # alignedRefAudio = wp_full[0, :].tolist()
    # alignedLiveAudio = wp_full[1, :].tolist()
    # return wp_full.T
    return

# rows are (alignedRefAudio, alignedLiveAudio)


def findStartTime(alignMatrix):
    return


def musicStarted():
    return

# on backend, want to start right after files submitted
# first begin listening and make a function for music being played vs not
# do the chorma_stft for refAudio once at the beginning
# continue aligning and use eye tracking data to help make better guess
# change page_number appropriately
# need head tracking function call somehow
#


def runBackend():
    recorder = audioSetup()
    # recorder.start() #start recording
    while True:
        # frame = recorder.read()
        # npFrame = np.divide(np.asarray(frame), 2**15)
        # alignMatrix = alignAudio(liveAudio = npFrame)
        # startTime = findStartTime(alignMatrix)
        # pageNum, measureNum, beatNum = findMeasure(startTime)
        # if measureNum ==
        # update page to pageNum
        pass
    return


def backend(request):
    context = {}
    global page_number
    global images_list
    page_number += 1

    # print(os.path.dirname(os.path.abspath(__file__)))
    # p = subprocess.Popen(args = [r"/mnt/d/CppDemo/CppDemo.cpp"], shell = True,
    #                                stdin = subprocess.PIPE, stdout=subprocess.PIPE)

    # audioSetup()
    context['images_list'] = images_list
    context['image'] = 'page'
    context['page_number'] = page_number
    return render(request, 'app/play.html', context)

######################################################################################


def get_list_json_dumps_serializer(request):
    imagejs_list = []
    global images_list
    global page_number
    js_page_num = page_number
    i = 1
    for img in images_list:
        my_img = {
            'id': img.id,
            'page_number': i,
        }
        i += 1

    response_data = {'images': imagejs_list, 'page_number': js_page_num}
    response_json = json.dumps(response_data)
    return HttpResponse(response_json, content_type="applications/json")


def fake_backend(request):
    # update every 500 ms
    measure = random.randrange(0, 32)


def room(request, room_name):
    context = {}
    context['room_name'] = room_name
    return render(request, 'app/play.html', context)
