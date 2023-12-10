from django.http import HttpResponse
from django.shortcuts import render, redirect
from django.urls import reverse
from django.core import serializers
from django.contrib.auth.models import User
import os
from app.models import Upload
from app.forms import UploadForm
from pdf2image import convert_from_path
from PIL import Image
import numpy as np
import json
import time
from string import digits
import subprocess
from django.http import JsonResponse
import concurrent.futures
import pretty_midi
import multiprocessing
from pvrecorder import PvRecorder
import random

from django.db import connection



# backend imports
import matplotlib.pyplot as plt
from libfmp.b.b_plot import plot_signal, plot_chromagram
from pvrecorder import PvRecorder
import librosa
from synctoolbox.dtw.cost import cosine_distance
from synctoolbox.dtw.core import compute_warping_path

# Sexy Global Variables
page_number = 2
pdf_path = ""
midi_path = ""
instrument = ""
images_list = []
total_pages = 0
my_variable = 0
bar = 0
row = 0
turnPage = 0
eyeData = []
prevAudio = None
start = False


# For Audoi AJAX Calls
pageNum = 0
relMeasureNum = 0
measureNum = 0
relBeatNum = 0
absBeatNum = 0



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
    global total_pages
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
            total_pages = len(images_list)
            print("images list in big function ", images_list)
            context['images_list'] = images_list
            context['image'] = 'page'
            page_number = 1
            context['page_number'] = page_number
            # return render(request, 'app/play.html', context)
            return redirect(reverse('play'))
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
    # runBackend()
    return render(request, 'app/play.html', context)


def flip_forward(request):
    context = {}
    global page_number
    global images_list
    global total_pages
    global row
    global bar
    bar = 0
    row = 0
    page_number += 1
    print("BEFORE images length", total_pages, "page num", page_number)
    # TODO: find total_pages and make it persistent
    if page_number > 3:
        page_number = 3
    print("AFTER images length", total_pages, "page num", page_number)

    context['images_list'] = images_list
    context['image'] = 'page'
    context['page_number'] = page_number
    # get_variable(request, page_number)
    return render(request, 'app/play.html', context)


def flip_backward(request):
    context = {}
    global page_number
    global images_list
    global row
    global bar
    bar = 0
    row = 0
    page_number -= 1
    if page_number <= 0:
        page_number = 1
    context['images_list'] = images_list
    context['image'] = 'page'
    context['page_number'] = page_number
   # get_variable(request, page_number)
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

def highest_average_in_window(vector, window_size):
    """
    Find the highest average within a moving window of a given size for a vector.

    Parameters:
    - vector (list or numpy.ndarray): Input vector.
    - window_size (int): Size of the moving window.

    Returns:
    - float: Highest average within the moving window.
    - int: Starting index of the window with the highest average.
    """
    if len(vector) < window_size or window_size <= 0:
        raise ValueError("Invalid window size or vector length.")

    max_average = float('-inf')
    max_average_index = 0

    for i in range(len(vector) - window_size + 1):
        window_sum = sum(vector[i:i+window_size])
        window_average = window_sum / window_size

        if window_average > max_average:
            max_average = window_average
            max_average_index = i

    return max_average, max_average_index

def threshold_and_zero(array, threshold):
    """
    Set every element in a NumPy array greater than a threshold to zero.

    Parameters:
    - array (numpy.ndarray): Input NumPy array.
    - threshold (float): Threshold value.

    Returns:
    - numpy.ndarray: Modified array.
    """
    return np.where(array > threshold, 0, array)

def almostEqual(a, b, rel_tol=1e-09, abs_tol=0.0):
    return abs(a-b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol)

#TODO: might need to change percentage metric
def needHarmonic(chroma_matrix):
    harmonics = False
    threshold = 0.8
    count = 0
    for col_index in range(chroma_matrix.shape[1]):
        column = chroma_matrix[:, col_index]
        if np.max(column) > threshold:
            count+=1
    
    total = chroma_matrix.shape[1]
    percentage = (2 * total) / 3.0 #checking if 2/3 or more of the
                                  #vectors meet the threshold

    if( total > percentage or almostEqual(total,percentage)):
        harmonics = True

    return harmonics 

def mergeCommon(list):
    newList = []
    curNote = "????"
    curD = 0
    for i in range(len(list)):
        note, d = list[i]
        if curNote == note:
            curD += d
        else:
            newList.append((curNote, curD)) #appends on note transitions
            curNote = note
            curD = d
    newList.append((curNote, curD)) #add the final note
    return newList[1:]




def getLiveAudioList(chroma_matrix, threshold=0.8, min_frames=100):
    selected_columns = []
    durationThreshold = 10
    for col_index in range(chroma_matrix.shape[1]):
        column = chroma_matrix[:, col_index]
        if np.max(column) > threshold:
            selected_columns.append(column)
    if(selected_columns == []):
        return (selected_columns)

    # Convert the list of selected columns back to a numpy array
    chromaCopy = np.column_stack(selected_columns)
    # plot_chromagram(chromaCopy[:, :30 * 50], Fs=50, title='Chroma representation for version 2', figsize=(9,3))
    # plt.show()
    # print(result_array.shape, chroma_matrix.shape)
    note_list = []
    count = 0
    prevNote = -1
    for col_index in range(chromaCopy.shape[1]):
        col = chromaCopy[:, col_index]
        curNote = np.argmax(col)
        if curNote != prevNote:
            note_list.append((prevNote, count))
            prevNote = curNote
            count = 1
        else:
            count += 1
    note_list.append((prevNote, count))
    note_list.pop(0)

    res = []
    mapping = ['C','C#','D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B']

    for (noteNum, noteTime) in note_list:
        if noteTime > durationThreshold:
            res.append((noteNum, noteTime))
    finalRes = []
    for (noteNum, noteTime) in res:
        finalRes.append((mapping[noteNum], noteTime))

    finalRes = mergeCommon(finalRes)
    return finalRes

def getSublist(midi_list, small_list):
    ptr1, ptr2 = 0, 0
    while ptr1 < len(small_list) and ptr2 < len(midi_list):
        if small_list[ptr1] == midi_list[ptr2]:
            ptr1 += 1
            ptr2 += 1
            if ptr1 >= len(small_list):
                return ptr2 - 1
        else:
            if ptr2 + 1 >= len(midi_list): return -1
            ptr2 += 1 
    return -1

def returnSmallest(arr):
    if arr == []:
        return []
    res = []
    minDiff = 1000000000
    for i in range(len(arr)):
        (start,end) = arr[i]
        if end-start == minDiff:
            minDiff = end-start
            res.append(arr[i])
        elif end-start < minDiff:
            res = []
            res.append(arr[i])
            minDiff = end-start
    return res

# def bestMidiMatch(midi_total_list,note_gen_list):
def bestMidiMatch(refMidiList, liveMidiList):

    midi_list = []
    for item in refMidiList:
        (note,_,_,_) = item
        midi_list.append(note)
    
    note_list = []
    for item in liveMidiList:
        (note,_) = item
        note_list.append(note) 
    if note_list == []:
        return (-1, note_list)

    ptr1 = 0
    possibles = []
    for i in range(len(midi_list)):
        if(note_list[0]==midi_list[i]):
            ptr1 = i 
            relEndIndex = getSublist(midi_list[ptr1:], note_list)
            if relEndIndex < 0: continue
            startIndex = i
            endIndex = startIndex + relEndIndex
            possibles.append((startIndex,endIndex))

    res = returnSmallest(possibles)
    print(f"res: {res}, possibles: {possibles}, noteList: {note_list}, midiList: {midi_list}")
    print("\n\n\n\n\n")
    #TODO: change this to not always give the first instance of match
    if res == []:
        confidence = 0 #TODO: eans no matches but still found notes
    else: 
        numWrongNotes = (res[0][1] - res[0][0]) - (len(note_list) - 1)
        confidence = (len(note_list) - 1 - numWrongNotes) / len(note_list)
    return confidence, res

def harmMidiMatch(refMidiList, liveAudio, liveSr):
    tempAudio = librosa.effects.harmonic(y = liveAudio, margin=1)
    return bestMidiMatch(refMidiList, tempAudio, liveSr)

def recordAudio(recorder):
    recorder.start()
    liveAudio = None
    for _ in range(40):
        frame = recorder.read()
        npFrame = np.asarray(frame)
        npFrame = np.divide(npFrame, 2**15)
        if type(liveAudio) == type(None):
            liveAudio = npFrame
        else: 
            liveAudio = np.concatenate((liveAudio, npFrame), axis = None)
    return liveAudio

# def getDTW(refChroma, liveAudio):
#     H = 512
#     Fs = 16000
#     liveChroma = compute_chroma(y = liveAudio, sr = 16000, N = 1024, H = 512)
#     cosDis = cosine_distance(refChroma, liveChroma)
#     _, _, wp_full = compute_warping_path(C = cosDis, implementation = "synctoolbox")
#     coordinates = np.column_stack((wp_full[0, :], wp_full[1, :]))
#     dy = np.gradient(coordinates[:, 1])
#     tempCoordinates = threshold_and_zero(dy, 1.01)
#     maxAvg, startPoint = highest_average_in_window(tempCoordinates, liveChroma.shape[1])
#     # print(f"maxAvg : {maxAvg}, startFrame: {startPoint}, startTime: {startPoint * (H/Fs)}")
#     return (maxAvg, startPoint, startPoint * (H/Fs))

# def getMIDIList(path):
#     midi_data = pretty_midi.PrettyMIDI(path)
#     midiList = []
#     # print("duration:",midi_data.get_end_time())
#     # print(f'{"note":>10} {"start":>10} {"end":>10}')
#     for instrument in midi_data.instruments:
#         # print("instrument:", instrument.program);
#         for note in instrument.notes:
#             # print(f'{note.pitch:10} {pretty_midi.note_number_to_name(note.pitch)} {note.start:10} {note.end:10} {note.get_duration():10}')
#             noteName = pretty_midi.note_number_to_name(note.pitch)
#             remove_digits = str.maketrans('', '', digits)
#             noteName = noteName.translate(remove_digits)
#             tempo = 120
#             secPerBeat = (tempo / (60)) ** -1
#             absBeatNum = round((round(note.start * 2) / 2) / secPerBeat)
#             relBeatNum = absBeatNum % 4
#             measureNum = absBeatNum // 4
#             relMeasureNum = measureNum % 32
#             pageNum = (measureNum // 32) + 1
#             midiList.append((noteName, pageNum, relMeasureNum, relBeatNum))
    

#     return midiList

def getMIDIList(path):
    midi_data = pretty_midi.PrettyMIDI(path)
    midiList = []
    # tempo = 108
    print(midi_data.estimate_tempo())
    print(int(round(midi_data.get_tempo_changes()[1][0])))
    tempo = int(round(midi_data.get_tempo_changes()[1][0]))
    # print("duration:",midi_data.get_end_time())
    # print(f'{"note":>10} {"start":>10} {"end":>10}')
    for instrument in midi_data.instruments:
        # print("instrument:", instrument.program);
        for note in instrument.notes:
            # print(f'{note.pitch:10} {pretty_midi.note_number_to_name(note.pitch)} {note.start:10} {note.end:10} {note.get_duration():10}')
            noteName = pretty_midi.note_number_to_name(note.pitch)
            remove_digits = str.maketrans('', '', digits)
            noteName = noteName.translate(remove_digits)
            secPerBeat = (tempo / (60)) ** -1
            absBeatNum = (round(((round(note.start * 16) / 16) / secPerBeat) * 4)) / 4
            # print((round(((round(note.start * 16) / 16) / secPerBeat) * 2)) / 2)
            # absBeatNum = round(note.start / secPerBeat)
            relBeatNum = absBeatNum % 4
            measureNum = absBeatNum // 4
            relMeasureNum = measureNum % 32
            pageNum = (measureNum // 32) + 1
            # print(f'NoteName: {noteName},absBeatNum: {absBeatNum}, relMeasureNum:{relMeasureNum + 1}, relBeatNum: {relBeatNum + 1}')
            midiList.append((noteName, pageNum, relMeasureNum + 1, relBeatNum + 1))

    return midiList

def getEyeData():
    global page_number
    global images_list
    global total_pages
    global row
    global bar
    flippedPage = False
    startTime = 0
    # p = subprocess.Popen(args = [r"./CppDemo/x64/Debug/CppDemo.exe"], shell = True, #Caleb Help
    #                                stdin = subprocess.PIPE, stdout=subprocess.PIPE)
    
    
    p = subprocess.Popen(args = [r".\TobiiDemo\x64\Debug\TobiiDemo.exe"], shell = True, #Rohan Help
                                   stdin = subprocess.PIPE, stdout=subprocess.PIPE)

    while True:
        output = p.stdout.readline()
        print("output: ", output)
        # print("total pages", total_pages)
        if output == b'' and p.poll() is not None:
            print("broke out of loop\n")
            break
        if output:
            if '1' in output.decode('utf-8'):
                print(f"saw a 1 should've flipped, {page_number}\n")
                if page_number < total_pages and not flippedPage:
                    flippedPage = True
                    startTime = time.time()
                    print(f"saw a 1 should've flipped, {page_number}\n")
                    page_number += 1
                bar = 0
                row = 0
            elif '2' in output.decode('utf-8'):
                print(f"saw a 2 should've flipped, {page_number}\n")
                if page_number > 1 and not flippedPage:
                    flippedPage = True
                    startTime = time.time()
                    page_number -= 1
                bar = 0
                row = 0
                   
        if flippedPage and (time.time() - startTime) > 1:
            flippedPage = False
        print("page number", page_number, "\n")
        print("total pages:", total_pages, "flippedPage", flippedPage,"\n")
         
    return 

# def getEyeData():
#     global page_number
#     global images_list
#     global total_pages
#     flippedPage = False
#     startTime = 0
#     # p = subprocess.Popen(args = [r"./CppDemo/x64/Debug/CppDemo.exe"], shell = True, #Caleb Help
#     #                                stdin = subprocess.PIPE, stdout=subprocess.PIPE)
    
    
#     p = subprocess.Popen(args = [r".\TobiiDemo\x64\Debug\TobiiDemo.exe"], shell = True, #Rohan Help
#                                    stdin = subprocess.PIPE, stdout=subprocess.PIPE)
#     outputList = []
#     while not (output == b'' and p.poll() is not None):
#         output = p.stdout.readline()
#         outputList.append(output)
         
#     return outputList

# #####################################################################################

def compute_chroma(audio, sr, N, H):
    
    return librosa.feature.chroma_stft(y=audio, sr=sr, n_fft=N, hop_length=H, norm=2.0)

def getMeasureNum(time):
    global relMeasureNum
    global measureNum
    global relBeatNum
    global absBeatNum
    
    
    tempo = 120
    secPerBeat = (tempo / (60)) ** -1
    absBeatNum = round((round(time * 2) / 2) / secPerBeat)
    relBeatNum = absBeatNum % 4
    measureNum = absBeatNum // 4
    relMeasureNum = measureNum % 32
    pageNum = (measureNum // 32) + 1
    return pageNum, relMeasureNum, measureNum, relBeatNum, absBeatNum,

# # NOTE: need to add entire folder into this github repo
# def getEyeData():
#     p = subprocess.Popen(args = [r"./CppDemo/x64/Debug/CppDemo.exe"], shell = True,
#                                    stdin = subprocess.PIPE, stdout=subprocess.PIPE)
#     stdout, stderr = p.communicate()
#     return stdout.decode('utf-8')

def getAudioData(recorder):
    recorder.start()
    startTime = time.time()
    liveAudio = None
    #NOTE: to change length of audio segemtn
    # audioSegLength = 0.25
    # while time.time() - startTime < audioSegLength:
    for _ in range(50):
        frame = recorder.read()
        # print(frame)    
        npFrame = np.asarray(frame)
        npFrame = np.divide(npFrame, 2**15)
        if type(liveAudio) == type(None):
            liveAudio = npFrame
        else:
            liveAudio = np.concatenate((liveAudio, npFrame), axis = None)
    return liveAudio


def alignAudio(refAudio, liveAudio):
    Fs_ref = 48000 #22050
    Fs_live = 16000
    N = 1024
    H = 512
    feature_rate = int(48100 / H)

    #make into chromas
    with concurrent.futures.ThreadPoolExecutor() as executor:
        # Submit the function calls to the thread pool
        future_chroma_1 = executor.submit(compute_chroma, refAudio, Fs_ref, N, H)
        future_chroma_2 = executor.submit(compute_chroma, liveAudio, Fs_live, N, H)

        # Get the results when they are ready
        refChroma = future_chroma_1.result()
        liveChroma = future_chroma_2.result()
    # refChroma = refAudio.get_chroma(fs = 100)
    # plot_chromagram(refChroma[:, :30 * feature_rate], Fs=feature_rate, title='Chroma representation for version 2', figsize=(9,3))
    # plt.show()
    # plot_chromagram(liveChroma[:, :30 * feature_rate], Fs=feature_rate, title='Chroma representation for liveChroms', figsize=(9,3))
    # plt.show()

    C = cosine_distance(refChroma, liveChroma)
    D, E, wp_full = compute_warping_path(C = C, implementation = "synctoolbox")
    coordinates = np.column_stack((wp_full[0, :], wp_full[1, :]))
    dx, dy = np.gradient(coordinates[:, 0]), np.gradient(coordinates[:, 1])
    maxAvg, startPoint = highest_average_in_window(threshold_and_zero(dy, 1), liveChroma.shape[1])

    getMeasureNum(startPoint * (H/Fs_ref))
    print(f"maxAvg: {maxAvg}, startFrame: {startPoint} , startTime: {startPoint * (H/Fs_ref)}, page, measure, beat num: {getMeasureNum(startPoint * (H/Fs_ref))}")
    return

# Sending Audio Data to Frontend AJAX
# def sendCursorData(request):
    # global pageNum
    # global measureNum
    # global relBeatNum
    # global absBeatNum

    # pageNum = 32
    # absBeatNum = 9

    # return JsonResponse({'paegNum': pageNum,
    #                      'measureNum': measureNum,
    #                      'relBeatNum': relBeatNum,
    #                      'absBeatNum': absBeatNum})


# # on backend, want to start right after files submitted
# # first begin listening and make a function for music being played vs not
# # do the chorma_stft for refAudio once at the beginning
# # continue aligning and use eye tracking data to help make better guess
# # change page_number appropriately
# # need head tracking function call somehow




def backend(request):
    context = {}
    global page_number
    global images_list
    # global prevAudio
    global refMidiList
    global midi_path
    global start
    global bar
    global row
    start = True
    
    
    #print("midi_path: ",midi_path) NOTE: use midipath
    refMidiList = getMIDIList(path = r"./app/outputs/SoundSync_Demo_Dec9.mid") #NOTE: change to midi path variable
    recorder = PvRecorder(frame_length=1024, device_index = 0)
    compute_chroma(np.zeros(1), recorder.sample_rate, 1024, 8192)
    recorder.start()
   
    #getEyeData()
    while True:
    # for _ in range(10):
        liveAudio = None
        for _ in range(50):
            frame = recorder.read()
            # print(time.time() - startTime)
            npFrame = np.asarray(frame)
            npFrame = np.divide(npFrame, 2**15)
            # print(npFrame)
            if type(liveAudio) == type(None):
                liveAudio = npFrame
            else: 
                liveAudio = np.concatenate((liveAudio, npFrame), axis = None)
        print(f"size of liveaudio array is {liveAudio.shape}")
        startTime = time.time()
        liveChroma = compute_chroma(liveAudio, 16000, 1024, 128)
        print(f"time it took to compute chroma: {time.time() - startTime}")
        # plot_chromagram(liveChroma[:, :30 * 50], Fs=50, title='Chroma representation for version 2', figsize=(9,3))
        # plt.show()
        liveMidiList = getLiveAudioList(liveChroma)
        print(f"liveMidiList: {liveMidiList}")
        print(f"time it took to compute liveMIDIList: {time.time() - startTime}")
        confidence, matchList = bestMidiMatch(refMidiList, liveMidiList)
        if confidence == -1 or matchList == []:
            # print(f"confidence: {confidence} matchlist: {matchList}")
            print("\n\n")
            print(f"continued... time it took to get to here is {time.time() - startTime}")
            continue
        print(f"time it took to get bestMidiMatch: {time.time() - startTime}")
        # if matchList == []:
        #     print(f"continued... time it took to get to here is {time.time() - startTime}")
        #     continue
        res = refMidiList[matchList[0][0]:matchList[0][1] + 1] 
        print(f"res: {res}")
        _, endNote = refMidiList[matchList[0][0]], refMidiList[matchList[0][1] + 1]
        tempMeasureNum = endNote[2]
        row, bar = tempMeasureNum // 4, (tempMeasureNum - 1) % 4
        bar += ((endNote[3] - 1) / 4)
        page_number = endNote[1] #endNote = (noteName, pageNum, measureNum, beatNum)
        print(f"total time = {time.time() - startTime}")
        print("mathcList: ", matchList)
        # print("refmidi: ",refMidiList, "\n\n")
        print(f"pageNum: {page_number} measureNum: {tempMeasureNum} (1-indexed) beatNum: {endNote[3]} row: {row} bar: {bar} endnote: {endNote}")

    recorder.delete()
    return render(request, 'app/play.html', context)

#     #do one offs here
#     print(midi_path)
#     refAudio, _ = librosa.load(path = midi_path, sr = 48000)
#     refMidiList = getMIDIList(midi_path)
#     refChroma = compute_chroma(librosa.effects.harmonic(y = refAudio, margin=1), sr = 48000,
#                                 N = 1024, H = 512)
#     prevAudio = None
#     recorder = PvRecorder(frame_length=1024, device_index=0)
#     storedAudio = None
#     print("\n done with backend one offs \n")

#     while True:
#         if prevAudio is None:
#             curAudio = recordAudio(recorder)
#             prevAudio = curAudio
#             continue
#         liveSr = 16000 #sampling rate of Pvrecorder
#         startTime = time.time()
#         print("\n\n starting to make processes \n\n")
#         # Create two processes with arguments
#         regMidiAlignProcess = multiprocessing.Process(target=bestMidiMatch, args=(refMidiList, prevAudio, liveSr))
#         harmMidiAlignProcess = multiprocessing.Process(target=harmMidiMatch, args=(refMidiList, prevAudio, liveSr))
#         curAudioProcess = multiprocessing.Process(target = recordAudio)
#         dtwProcess = multiprocessing.Process(target = getDTW, args = (refChroma, storedAudio))
#         eyeDataProcess = multiprocessing.Process(target = getEyeData)

#         print("\n\n starting processes \n\n")
#         # Start the processes
#         regMidiAlignProcess.start()
#         print("\n\n made it past one start call \n\n")
#         eyeDataProcess.start()
#         harmMidiAlignProcess.start()
#         curAudioProcess.start()

#         print("\n\n started proccessess \n\n")
#         ranDTW = False
#         if dtwCount >= 3:
#             dtwProcess.start()
#             ranDTW = True
#         else: 
#             dtwCount += 1

#         print("\n\n here here \n\n")
#         # Wait for process 1 to finish and get its result
#         regMidiConf, regMidiAlign = regMidiAlignProcess.join()

#         # Check the result of process 1
#         needHarmonicFlag = needHarmonic(regMidiAlign)
#         if not needHarmonicFlag:
#             # Process 1 returned True, terminate process 2
#             harmMidiAlignProcess.terminate()
#             harmMidiAlignProcess.join()
#             harmMidiAlign = 0, []
#             print("harmMidiAlign terminated")
#         else:
#             # Process 1 returned False, let process 2 continue
#             harmMidiConf, harmMidiAlign = harmMidiAlignProcess.join()
#             print("harmMidiAlign continued")
#         if ranDTW:
#             dtwRes = dtwProcess.join()
#             dtwConfidence, _, dtwStartTime = dtwRes
#         else:
#             dtwRes = None

#         audioAlignmentTime = time.time() - startTime
#         print(f"time for parallel audio alignment {audioAlignmentTime}")

#         eyeData = eyeDataProcess.join()
#         eyeDataTime = time.time() - startTime
#         print(f"time for parallel eye and audio {eyeDataTime}")


#         # finalSig = monitorSignals() #this will just update everything
#         # updateCursor(finalSig)

#         while curAudioProcess.is_alive():
#             tempEyeData = getEyeData()
#             # monitorSignals(eyeTrack = tempEyeData, midAlign = [], harMidAlign = [], dtw = None)


#         curAudio = curAudioProcess.join()
#         if not ranDTW:
#             if storedAudio is None:
#                 storedAudio = curAudio
#             else:
#                 storedAudio = np.concatenate((storedAudio, curAudio), axis = None)
#         ranDTW = False
#         prevAudio = curAudio
#     context['images_list'] = images_list
#     context['image'] = 'page'

      

# ######################################################################################


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




def get_variable(request):
    # Compute or get your variable here
    global my_variable
    global bar
    global row
    global page_number
    global total_pages
    # global turnPage

    # page_number = 1

    my_variable += 1
    # As long as these variables are updated in a timely manner, this will update the cursor on the page!
  
    # PAGENUM = 2


   
    global relMeasureNum    
    global measureNum
    global relBeatNum
    global absBeatNum


    # find row and bar from these 
    # row = relMeasureNum // 4
    # bar = relMeasureNum % 4 + (relBeatNum)/4
   
    # if(start):
    #     if(page_number==3 and row>=4):
    #         bar+=0.08
    #     else:
    #         bar+=0.0105 # 120 bpm
    #     #bar+= 0.05
    
    #bar+= 0.0105 #120 bpm
    if total_pages==0:
        total_pages = 3
    #bar += 0.025

    if(bar >=4 ):
        row+=1
        bar = 0
    
    if(row >=8):
        row = 0

        if(page_number<total_pages):
            page_number+=1

    
    # row = 0
    # bar = 0


    # print("bar: ", bar)
    # print ("row: ", row)
    # print("page_number", page_number)
    # print("total pages", total_pages)


    

    

    
    

    # pageNum = 32
    

    # Return the variable as JSON
    return JsonResponse({'my_variable': my_variable,
                         'row': row,
                         'bar': bar,
                         'page_number': page_number, #page_number 
                         'relMeasureNum': relMeasureNum,
                         'measureNum': measureNum,
                         'relBeatNum': relBeatNum,
                         'absBeatNum': absBeatNum})
