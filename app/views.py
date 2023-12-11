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
import copy
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
total_pages = 3
my_variable = 0
bar = 0
row = 0
turnPage = 0
eyeData = [] #TODO delete this
prevAudio = None
start = False
process = None
usingAudio = False
usingEye = False
isSound = False
cursorMoves = True
globalEyeData = b'-1:-1:0,-1:-1:0,-1:-1:0,-1:-1:0,-1:-1:0,5:0:0,2:0:0,0:0:0,-1:-1:0,-1:-1:0\r\n'



# For Audio AJAX Calls
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


def using_audio(request):
    context = {}
    global usingAudio
    usingAudio = not usingAudio
    return render(request, 'app/play.html', context)

def using_eye(request):
    context = {}
    global usingEye
    usingEye = not usingEye
    return render(request, 'app/play.html', context)


def upload_pdf(request):
    context = {}
    global page_number
    global pdf_path
    global midi_path
    global instrument
    global images_list
    global total_pages
    global usingAudio
    global usingEye
    if request.method == 'POST':
        form = UploadForm(request.POST, request.FILES)
        if form.is_valid():
            # reset usingAudio and usingEye
            usingAudio = False
            usingEye = False
            
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

def almostEqual(x, y):
    return abs(x - y) < 10**-9

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

def bothCalculateConfidence(sI, eI, prevAbsBeatNum, midi_list, note_list, curEyeData):
    global page_number
    confidence = 0

    numWrongNotes = (eI - sI) - (len(note_list) - 1)
    noteName, pageNum, measureNum, beatNum, _ = midi_list[eI]
    absBeatNum = ((pageNum - 1) * 32 * 4) + ((measureNum - 1) * 4) + (beatNum - 1) #THIS IS 0-INDEXED; BEAT 128 is page 2, measure 1, beat 1
    if absBeatNum < prevAbsBeatNum:
        confidence = 0.5
        if pageNum == page_number:
            confidence += 0.4
    elif 0 <= absBeatNum - prevAbsBeatNum <= 8:
        confidence += 2
    elif 8 < absBeatNum - prevAbsBeatNum <= 16:
        confidence += 1
    else:
        dif = (absBeatNum - prevAbsBeatNum) - 16 #how many beats after 4 measures it is
        # confidence = 0.5 + (1 / (2 * (dif - 16)))
        confidence = 0.5
        if pageNum == page_number:
            confidence += 0.4
    eyePageMeasureNum = (int(curEyeData.split(":")[0]) * 4) + int(curEyeData.split(":")[1])
    if abs(eyePageMeasureNum - measureNum) <= 3:
        confidence += 10

    return confidence

def calculateConfidence(sI, eI, prevAbsBeatNum, midi_list, note_list):
    #case where predicted eI note is less than beatNum should have bad confidence
    #case where predicted eI note is = to beatNum should have bad confidence
    global page_number
    confidence = 0


    numWrongNotes = (eI - sI) - (len(note_list) - 1)
    noteName, pageNum, measureNum, beatNum, _ = midi_list[eI]
    absBeatNum = ((pageNum - 1) * 32 * 4) + ((measureNum - 1) * 4) + (beatNum - 1) #THIS IS 0-INDEXED; BEAT 128 is page 2, measure 1, beat 1
    if absBeatNum < prevAbsBeatNum:
        confidence = 0.5
        if pageNum == page_number:
            confidence += 0.4
    elif 0 <= absBeatNum - prevAbsBeatNum <= 8:
        confidence += 2
    elif 8 < absBeatNum - prevAbsBeatNum <= 16:
        confidence += 1
    else:
        dif = (absBeatNum - prevAbsBeatNum) - 16 #how many beats after 4 measures it is
        # confidence = 0.5 + (1 / (2 * (dif - 16)))
        confidence = 0.5
        if pageNum == page_number:
            confidence += 0.4


    return confidence

def bothBestMidiMatch(refMidiList, liveMidiList):
    
    midi_list = []
    for item in refMidiList:
        (note,_,_,_,_) = item
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

    smallPossibles = returnSmallest(possibles)
    # print(f"res: {smallPossibles}, possibles: {possibles}, noteList: {note_list}, midiList: {midi_list}")
    # print("\n\n\n\n\n")
    res = []
    for startIndex, endIndex in smallPossibles:
        #math then append
        # global eyeData
        # eyeDataCopy = copy.copy(eyeData) #so that it doens't change in the middle of calcuating
        curEyeData = globalEyeData.decode('utf-8')
        # confidence = bothCalculateConfidence(startIndex, endIndex, absBeatNum, refMidiList, liveMidiList, curEyeData)
        confidence = calculateConfidence(startIndex, endIndex, absBeatNum, refMidiList, liveMidiList)
        res.append((confidence,startIndex,endIndex))
    return res

# def bestMidiMatch(midi_total_list,note_gen_list):
def bestMidiMatch(refMidiList, liveMidiList):

    midi_list = []
    for item in refMidiList:
        (note,_,_,_,_) = item
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

    smallPossibles = returnSmallest(possibles)
    # print(f"res: {smallPossibles}, possibles: {possibles}, noteList: {note_list}, midiList: {midi_list}")
    # print("\n\n\n\n\n")
    res = []
    for startIndex, endIndex in smallPossibles:
        #math then append
        # global eyeData
        # eyeDataCopy = copy.copy(eyeData) #so that it doens't change in the middle of calcuating
        confidence = calculateConfidence(startIndex, endIndex, absBeatNum, refMidiList, liveMidiList)
        res.append((confidence,startIndex,endIndex))
    return res

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
            absBeatNum = ((pageNum - 1) * 32 * 4) + ((relMeasureNum) * 4) + (relBeatNum)
            midiList.append((noteName, pageNum, relMeasureNum + 1, relBeatNum + 1, absBeatNum+1)) #sending all 1-indexed
    
    

    return midiList

def getEyeData():
    global process

    process = subprocess.Popen(args = [r".\TobiiDemo\x64\Debug\TobiiDemo.exe"], shell = True, #Rohan Help
                                   stdin = subprocess.PIPE, stdout=subprocess.PIPE)
    return

 



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

def getBestMatch(matchList):
    bestMatch = None
    bestConfidence = -1 * np.inf
    for elem in matchList:
        confidence, startIndex, endIndex = elem
        if confidence > bestConfidence:
            bestMatch = elem
    return bestMatch

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

# def backend(request):
#     context = {}
#     global page_number
#     global images_list
#     # global prevAudio
#     global refMidiList
#     global midi_path
#     global start
#     global bar
#     global row
#     start = True
    
#     # SETUP
#     #print("midi_path: ",midi_path) NOTE: use midipath
#     refMidiList = getMIDIList(path = r"./app/outputs/SoundSync_Demo_Dec9.mid") #NOTE: change to midi path variable
#     recorder = PvRecorder(frame_length=1024, device_index = 0)
#     compute_chroma(np.zeros(1), recorder.sample_rate, 1024, 8192)
#     recorder.start()
#     getEyeData() #start eyetracking pipes in global process

#     flippedPage = False
#     flipCounter = 0
#     count = 0
#     startFlag = False
#     while True:
#         if usingAudio:
#             liveAudio = None
#             for _ in range(50):
#                 frame = recorder.read()
#                 # print(time.time() - startTime)
#                 npFrame = np.asarray(frame)
#                 npFrame = np.divide(npFrame, 2**15)
#                 # print(npFrame)
#                 if type(liveAudio) == type(None):
#                     liveAudio = npFrame
#                 else: 
#                     liveAudio = np.concatenate((liveAudio, npFrame), axis = None)
#             # if count % 5 == 0:
#             #     eyeData = process.stdout.readline()
#             #     if not flippedPage and eyeData.split(":")[2] != 0:
#             #         if eyeData.split(":")[2]:
#             #             page_number -= 1
#             #         else:
#             #             page_number += 1
#             #         flippedPage = True
#             #         flipCounter = 0
#             # if flipCounter >= 15:
#             #     flippedPage = False
#             # flipCounter += 1
#             # count += 1
#         print(f"size of liveaudio array is {liveAudio.shape}")
#         startTime = time.time()
#         liveChroma = compute_chroma(liveAudio, 16000, 1024, 128)
#         print(f"time it took to compute chroma: {time.time() - startTime}")
#         # plot_chromagram(liveChroma[:, :30 * 50], Fs=50, title='Chroma representation for version 2', figsize=(9,3))
#         # plt.show()
#         liveMidiList = getLiveAudioList(liveChroma)
#         print(f"liveMidiList: {liveMidiList}")
#         print(f"time it took to compute liveMIDIList: {time.time() - startTime}")
#         confidence, matchList = bestMidiMatch(refMidiList, liveMidiList)
#         if confidence == -1 or matchList == []:
#             # print(f"confidence: {confidence} matchlist: {matchList}")
#             print("\n\n")
#             print(f"continued... time it took to get to here is {time.time() - startTime}")
#             continue
#         elif not startFlag:
#             startFlag = True
#         #doesn't update if music has not been heard yet   
#         if not startFlag: #TODO: style better later
#             continue
#         print(f"time it took to get bestMidiMatch: {time.time() - startTime}")
#         # if matchList == []:
#         #     print(f"continued... time it took to get to here is {time.time() - startTime}")
#         #     continue
#         bestMatch = getBestMatch(matchList)
#         # res = refMidiList[matchList[0][0]:matchList[0][1] + 1] 
#         # print(f"res: {res}")
#         # _, endNote = refMidiList[matchList[0][0]], refMidiList[matchList[0][1] + 1]
#         endNote =  refMidiList[bestMatch[2] + 1]
#         tempMeasureNum = endNote[2]
#         row, bar = tempMeasureNum // 4, (tempMeasureNum - 1) % 4
#         bar += ((endNote[3] - 1) / 4)
#         page_number = endNote[1] #endNote = (noteName, pageNum, measureNum, beatNum)
#         print(f"total time = {time.time() - startTime}")
#         print("mathcList: ", matchList)
#         # print("refmidi: ",refMidiList, "\n\n")
#         print(f"pageNum: {page_number} measureNum: {tempMeasureNum} (1-indexed) beatNum: {endNote[3]} row: {row} bar: {bar} endnote: {endNote}")

#     recorder.delete()
#     return render(request, 'app/play.html', context)




    
  
# def checkWrongPrediction(refMidi, bestMatch):
#     conf,start,end = bestMatch
#     #(_,_,_,_, absBeatNum) = refMidi[end]
#     for elem in refMidi:
#         (_,_,_,_, absBeatNum) = elem
#         if(almostEqual(predBeatNum,absBeatNum)):
#             return True
    
#     return False
        



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
    global usingAudio
    global usingEye
    global isSound
    global globalEyeData

    # SETUP
    #print("midi_path: ",midi_path) NOTE: use midipath
    # refMidiList = getMIDIList(path = r"./app/outputs/SoundSync_Demo_Dec9.mid") #NOTE: change to midi path variable
    refMidiList = getMIDIList(path = midi_path)
    recorder = PvRecorder(frame_length=1024, device_index = 0)
    compute_chroma(np.zeros(1), recorder.sample_rate, 1024, 8192)
    recorder.start()
    getEyeData() #start eyetracking pipes in global process
    justEyeCount = 0
    bothCount = 0

    while True:
        # print(usingAudio, usingEye)
        if usingAudio and usingEye:
            justEyeCount = 0
            #do both
            liveAudio = None
            for i in range(45):
                frame = recorder.read()
                # print(time.time() - startTime)
                npFrame = np.asarray(frame)
                npFrame = np.divide(npFrame, 2**15)
                # print(npFrame)
                if type(liveAudio) == type(None):
                    liveAudio = npFrame
                else: 
                    liveAudio = np.concatenate((liveAudio, npFrame), axis = None)
                # _ = process.stdout.readline()
                startTime = time.time()
                globalEyeData = process.stdout.readline()
                print(f"time to readline is {time.time() - startTime}")
                print(globalEyeData.decode('utf-8').split(","))
                #check if turnPageSig says to turn the page
                turnPageSigList = []
                for elem in globalEyeData.decode('utf-8').split(","):
                    # print("stuff: ",elem, elem.split(":")[2])
                    elem = elem.split(":")
                    turnPageSigList.append(elem[2])
                if turnPageSigList.count('1') >= 1: #TODO fix later if need be
                    # print("turn page forward")
                    if page_number < 3 and bothCount > 100:
                        page_number += 1
                        bar, row = 0, 0
                        bothCount = 0
                elif turnPageSigList.count('2') >= 1:
                    # print("turn page backwards")
                    if page_number > 1 and bothCount > 100:
                        page_number -= 1 
                        bar, row = 0, 0
                        bothCount = 0
                bothCount += 1
            liveChroma = compute_chroma(liveAudio, 16000, 1024, 128)
            liveMidiList = getLiveAudioList(liveChroma)
            matchList = bothBestMidiMatch(refMidiList, liveMidiList)
            if matchList == [] or matchList[0] == -1:
                # print(f"confidence: {confidence} matchlist: {matchList}")
                # print("\n\n")
                # print(f"continued... time it took to get to here is {time.time() - startTime}")
                isSound = False
                continue
            isSound = True
            bestMatch = getBestMatch(matchList)  
            if bestMatch[2] + 1 >= len(refMidiList):
                endNote = refMidiList[-1]
            else:
                endNote =  refMidiList[bestMatch[2] + 1]
            tempMeasureNum = endNote[2] #is 1 indexed
            row, bar = (tempMeasureNum - 1) // 4, (tempMeasureNum - 1) % 4
            bar += ((endNote[3] - 1) / 4)
            page_number = endNote[1] #endNote = (noteName, pageNum, measureNum, beatNum)

        elif usingAudio and not usingEye:
            #do audio DONE!!!!
            bothCount = 0
            justEyeCount = 0
            liveAudio = None
            for _ in range(45):
                frame = recorder.read()
                # print(time.time() - startTime)
                npFrame = np.asarray(frame)
                npFrame = np.divide(npFrame, 2**15)
                # print(npFrame)
                if type(liveAudio) == type(None):
                    liveAudio = npFrame
                else: 
                    liveAudio = np.concatenate((liveAudio, npFrame), axis = None)
            liveChroma = compute_chroma(liveAudio, 16000, 1024, 128)
            liveMidiList = getLiveAudioList(liveChroma)
            matchList = bestMidiMatch(refMidiList, liveMidiList) 
            if matchList == [] or matchList[0] == -1:
                # print(f"confidence: {confidence} matchlist: {matchList}")
                # print("\n\n")
                # print(f"continued... time it took to get to here is {time.time() - startTime}")
                isSound = False
                continue
            isSound = True
            bestMatch = getBestMatch(matchList)  
            if bestMatch[2] + 1 >= len(refMidiList):
                endNote = refMidiList[-1]
            else:
                endNote =  refMidiList[bestMatch[2] + 1]
            tempMeasureNum = endNote[2] #is 1 indexed
            row, bar = (tempMeasureNum - 1) // 4, (tempMeasureNum - 1) % 4
            bar += ((endNote[3] - 1) / 4)
            page_number = endNote[1] #endNote = (noteName, pageNum, measureNum, beatNum)
            # print(f"capybara pageNum {page_number}, row {row}, bar {bar}, endNote {endNote}")
            

        elif usingEye and not usingAudio:
            bothCount = 0
            eyeDataList = process.stdout.readline().decode('utf-8').split(",")
            turnPageSigList = []
            for elem in eyeDataList:
                # print("stuff: ",elem, elem.split(":")[2])
                elem = elem.split(":")
                turnPageSigList.append(elem[2])
            if turnPageSigList.count('1') > 6: #TODO fix later if need be
                # print("turn page forward")
                if page_number < 3 and justEyeCount > 50:
                    page_number += 1  
                    justEyeCount = 0
            elif turnPageSigList.count('2') > 6:
                # print("turn page backwards")
                if page_number > 1 and justEyeCount > 50:
                    page_number -= 1 
                    justEyeCount = 0
            justEyeCount += 1      
        else:
            #not using either
            pass


      

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
    global cursorMoves
    global usingAudio
    global isSound


    # find row and bar from these 
    # row = relMeasureNum // 4
    # bar = relMeasureNum % 4 + (relBeatNum)/4
   
    # if(start):
    #     if(page_number==3 and row>=4):
    #         bar+=0.08
    #     else:
    #         bar+=0.0105 # 120 bpm
    #     #bar+= 0.05
    
   
    if total_pages==0:
        total_pages = 3
    
    
    if (isSound and usingAudio and cursorMoves):
        bar += 0.025 #1120 bpm

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
