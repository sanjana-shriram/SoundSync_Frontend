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

########################################################################################
# Helper Functions

# def trim_chroma_matrix(chroma_matrix, amplitude_threshold=0.75, min_duration=400):
#     """
#     Trim a chroma matrix by removing the initial portion where there is no solid harmonic component.

#     Parameters:
#     - chroma_matrix (numpy.ndarray): Chroma matrix where each column represents a chroma vector.
#     - amplitude_threshold (float): Minimum amplitude threshold for considering a chroma vector.
#     - min_duration (int): Minimum consecutive duration of chroma vectors with sufficient amplitude.

#     Returns:
#     - numpy.ndarray: Trimmed chroma matrix.
#     """
#     # Find the maximum value and its index in each chroma vector
#     vector_max_values = np.max(chroma_matrix, axis=0)
#     vector_max_indices = np.argmax(chroma_matrix, axis=0)

#     # Find the first index where the maximum value exceeds the threshold for a consecutive duration
#     start_index = 0
#     while start_index < len(vector_max_values) and vector_max_values[start_index] <= amplitude_threshold:
#         start_index += 1

#     # Find the end index based on the minimum consecutive duration
#     end_index = start_index
#     consecutive_duration = 0
#     while end_index < len(vector_max_values) and consecutive_duration < min_duration:
#         if vector_max_values[end_index] > amplitude_threshold and vector_max_indices[end_index] == vector_max_indices[start_index]:
#             consecutive_duration += 1
#         else:
#             consecutive_duration = 0
#         end_index += 1

#     # Extract the valid chroma vectors
#     trimmed_chroma_matrix = chroma_matrix[:, start_index:end_index]
#     print(start_index, end_index)

#     return trimmed_chroma_matrix

# def highest_average_in_window(vector, window_size):
#     """
#     Find the highest average within a moving window of a given size for a vector.

#     Parameters:
#     - vector (list or numpy.ndarray): Input vector.
#     - window_size (int): Size of the moving window.

#     Returns:
#     - float: Highest average within the moving window.
#     - int: Starting index of the window with the highest average.
#     """
#     if len(vector) < window_size or window_size <= 0:
#         raise ValueError("Invalid window size or vector length.")

#     max_average = float('-inf')
#     max_average_index = 0

#     for i in range(len(vector) - window_size + 1):
#         window_sum = sum(vector[i:i+window_size])
#         window_average = window_sum / window_size

#         if window_average > max_average:
#             max_average = window_average
#             max_average_index = i

#     return max_average, max_average_index

# def threshold_and_zero(array, threshold):
#     """
#     Set every element in a NumPy array greater than a threshold to zero.

#     Parameters:
#     - array (numpy.ndarray): Input NumPy array.
#     - threshold (float): Threshold value.

#     Returns:
#     - numpy.ndarray: Modified array.
#     """
#     return np.where(array > threshold, 0, array)
# #####################################################################################

# def compute_chroma(audio, sr, N, H):
#     return librosa.feature.chroma_stft(y=audio, sr=sr, n_fft=N, hop_length=H, norm=2.0)

# # NOTE: need to add entire folder into this github repo
# def getEyeData():
#     p = subprocess.Popen(args = [r"./CppDemo/x64/Debug/CppDemo.exe"], shell = True,
#                                    stdin = subprocess.PIPE, stdout=subprocess.PIPE)
#     stdout, stderr = p.communicate()
#     return stdout.decode('utf-8')

# def getAudioData(recorder):
#     recorder.start()
#     startTime = time.time()
#     liveAudio = None
#     #NOTE: to change length of audio segemtn
#     audioSegLength = 0.25
#     while time.time() - startTime < audioSegLength:
#         frame = recorder.read()
#         npFrame = np.asarray(frame)
#         npFrame = np.divide(npFrame, 2**15)
#         if type(liveAudio) == type(None):
#             liveAudio = npFrame
#         else:
#             liveAudio = np.concatenate((liveAudio, npFrame), axis = None)
#     return liveAudio


# def alignAudio(refAudio, liveAudio):
#     Fs_ref = 48100 #22050
#     Fs_live = 16000
#     N = 1024
#     H = 512
#     feature_rate = int(48100 / H)

#     #make into chromas
#     with concurrent.futures.ThreadPoolExecutor() as executor:
#         # Submit the function calls to the thread pool
#         print(Fs_live)
#         future_chroma_1 = executor.submit(compute_chroma, refAudio, Fs_ref, N, H)
#         future_chroma_2 = executor.submit(compute_chroma, liveAudio, Fs_ref, N, H)

#         # Get the results when they are ready
#         refChroma = future_chroma_1.result()
#         liveChroma = future_chroma_2.result()

#     plot_chromagram(refChroma[:, :30 * feature_rate], Fs=feature_rate, title='Chroma representation for version 2', figsize=(9,3))
#     # plt.show()
#     plot_chromagram(liveChroma[:, :30 * feature_rate], Fs=feature_rate, title='Chroma representation for version 2', figsize=(9,3))
#     # plt.show()

#     C = cosine_distance(refChroma, liveChroma)
#     D, E, wp_full = compute_warping_path(C = C, implementation = "synctoolbox")
#     coordinates = np.column_stack((wp_full[0, :], wp_full[1, :]))
#     dx, dy = np.gradient(coordinates[:, 0]), np.gradient(coordinates[:, 1])
#     maxAvg, startPoint = highest_average_in_window(threshold_and_zero(dy, 1), liveChroma.shape[1])

#     print(f"maxAvg: {maxAvg}, startFrame: {startPoint} , startTime: {startPoint * (H/Fs_ref)}")
#     return


# # on backend, want to start right after files submitted
# # first begin listening and make a function for music being played vs not
# # do the chorma_stft for refAudio once at the beginning
# # continue aligning and use eye tracking data to help make better guess
# # change page_number appropriately
# # need head tracking function call somehow


# def runBackend():
#     #establish pvrecorder
#     recorder = PvRecorder(frame_length=1024, device_index=0)
#     print("pvrecorder version: %s" % recorder.version)
#     #get ref Audio
#     # global midi_path
#     midi_path = "" #NOTE: delete this and uncomment prev line
#     refAudio, _ = librosa.load(midi_path, sr = 48100)

#     #get first sample
#     with concurrent.futures.ThreadPoolExecutor() as executor:
#         eyeDataThread = executor.submit(getEyeData)
#         audioDataThread = executor.submit(getAudioData, recorder)

#         eyeData = eyeDataThread.result()
#         prevLiveAudio = audioDataThread.result()

#     # now run this on a loop
#     while True:
#         with concurrent.futures.ThreadPoolExecutor() as executor:

#             # Submit the function calls to the thread pool
#             eyeDataThread = executor.submit(getEyeData)
#             audioDataThread = executor.submit(getAudioData, recorder)
#             alignAudioThread = executor.submit(alignAudio, refAudio, prevLiveAudio)


#             # Get the results when they are ready
#             eyeData = eyeDataThread.result()
#             audioData = audioDataThread.result()
#             alignedAudio = alignAudioThread.result()

#         #NOTE: do something with the alignedAudio data
#         # do something here
#         # update measure num etc

#         prevLiveAudio = audioData


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
