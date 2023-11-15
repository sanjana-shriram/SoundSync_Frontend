from django.http import HttpResponse
from django.shortcuts import render, redirect
from django.urls import reverse
import os
# from app.models import PDF, Midi, Instrument
# from app.forms import PDFForm, MidiForm, InstrumentForm
from app.models import Upload
from app.forms import UploadForm
from pdf2image import convert_from_path
from django.http import FileResponse, Http404
import PyPDF2
import fitz
from PIL import Image


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
            print("pdf path!", pdf_path)

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

                images_list.append(image_file_name)

            # for i in range(len(images)):
            #     image_file_name = 'page' + str(i) + '.jpg'
            #     with open(image_path, 'wb+') as destination:
            #         destination.write(images[i])

            # # Optionally, if you need to resize the image or perform other operations:
            #     with images[i].open(image_path) as img:
            #         # Perform operations if needed, e.g., resizing
            #         # img = img.resize((new_width, new_height))

            #         # Save the modified image
            #         img.save(image_path)

            # for i in range(len(images)):
            #     image_path = os.path.join(os.path.dirname(
            #         os.path.abspath(__file__)),  'static')
            #     images[i].save(image_path + 'page' + str(i) + '.jpg',  'JPEG')
            #     images_list.append(os.path.join(
            #         image_path, 'page' + str(i) + '.jpg'))

                print("testing out different paths \n")

            print("IMAGES LIST YOI", images_list)
            return render(request, 'app/play.html', {'images_list': images_list})
    else:
        form = UploadForm()
    return render(request, 'app/upload.html', {'uploadForm': form})


def play_action(request):
    pdf_files = ['page0.jpg', 'page1.jpg']
    # for obj in Upload.objects.all():
    #     pdf_files.append(obj.pdf)
    return render(request, 'app/play.html', {"pdf_files": pdf_files})


# def choose_instrument(request):
#     if request.method == 'POST':
#         form = InstrumentForm(request.POST, request.FILES)
#         if form.is_valid():
#             instrument = form.cleaned_data['instrument']
#             # Save the uploaded PDF file to a specific folder
#             output_folder = os.path.join(os.path.dirname(
#                 os.path.abspath(__file__)),  'outputs')
#             with open(os.path.join(output_folder, instrument), 'wb+') as destination:
#                 destination.write(instrument)
#             return render(request, 'upload.html', {'form': form})
#     else:
#         form = InstrumentForm()
#     return render(request, 'upload.html', {'form': form})


# def upload_music(request):
#     if request.method == 'POST':
#         form = PDFForm(request.POST, request.FILES)
#         if form.is_valid():
#             pdf_file = form.cleaned_data['pdf']
#             # Save the uploaded PDF file to a specific folder
#             output_folder = os.path.join(os.path.dirname(
#                 os.path.abspath(__file__)),  'outputs')
#             with open(os.path.join(output_folder, pdf_file.name), 'wb+') as destination:
#                 for chunk in pdf_file.chunks():
#                     destination.write(chunk)

#             return render(request, 'upload.html', {'pdfForm': form})
#     else:
#         form = PDFForm()
#     return render(request, 'upload.html', {'pdfForm': form})


# def upload_midi(request):
#     if request.method == 'POST':
#         form = MidiForm(request.POST, request.FILES)
#         if form.is_valid():
#             midi_file = form.cleaned_data['midi']
#             # Save the uploaded MIDI file to a specific folder
#             output_folder = os.path.join(os.path.dirname(
#                 os.path.abspath(__file__)),  'outputs')
#             with open(os.path.join(output_folder, midi_file.name), 'wb+') as destination:
#                 for chunk in midi_file.chunks():
#                     destination.write(chunk)
#             return render(request, 'upload.html', {'midiForm': form})
#     else:
#         form = MidiForm()
#     return render(request, 'upload.html', {'midiForm': form})


# trying somehting new
# def upload_process(request):
#     if request.method == 'POST':
#         print("here is the request", request.POST)
#         new_item = Upload()
#         form = UploadForm(request.POST, request.FILES, instance=new_item)
#         print("PRINTED FORM", form)
#         context = {}
#         if not form.is_valid():
#             context['form'] = form
#             print("cappy-spidey")
#         else:
#             # form = UploadForm(request.POST, request.FILES)

#             instrument = form.clean_instrument()
#             pdf_file = form.clean_pdf()
#             midi_file = form.clean_midi()
#             # output_folder = os.path.join(os.path.dirname(
#             #     os.path.abspath(__file__)),  'outputs')

#             # with open(os.path.join(output_folder, pdf_file.name), 'wb+') as destination:
#             #     destination.write(pdf_file.read())
#             # with open(os.path.join(output_folder, midi_file.name), 'wb+') as destination:
#             #     destination.write(midi_file.read())
#             # with open(os.path.join(output_folder, pdf_file), 'wb+') as destination:
#             #     for chunk in pdf_file.chunks():
#             #         destination.write(chunk)
#             # with open(os.path.join(output_folder, midi), 'wb+') as destination:
#             #     for chunk in midi.chunks():
#             #         destination.write(chunk)
#             new_item.instrument = instrument  # this works :)
#             # new_item.pdf = pdf_file
#             # new_item.midi = midi_file
#             new_item.pdf = form.cleaned_data['pdf'].content_type
#             new_item.midi = form.cleaned_data['midi'].content_type
#             print('Uploaded pDF: {} (type={})'.format(pdf_file, type(pdf_file)))
#             print('Uploaded midi: {} (type={})'.format(
#                 midi_file, type(midi_file)))
#             form.save()

#         return render(request, 'upload.html', {'uploadForm': form})
#     else:
#         form = UploadForm()
#         return render(request, 'upload.html', {'uploadForm': form})


# def get_uploads(request, id):
#     item = get_object_or_404(Upload, id=id)
#     print('Picture #{} fetched from db: {} (type={})'.format(
#         id, item.upload, type(item.upload)))

#     # Maybe we don't need this check as form validation requires a picture be uploaded.
#     # But someone could have delete the picture leaving the DB with a bad references.
#     if not item.upload:
#         raise Http404

#     return HttpResponse(item.upload, content_type=item.content_type)


# def show_pdf(request):
#     # Path to the outputs folder
#     output_folder = os.path.join(settings.BASE_DIR, 'outputs')

#     # Get a list of PDF files in the outputs folder
#     pdf_files = [f for f in os.listdir(output_folder) if f.endswith('.pdf')]
#     print(pdf_files)
#     # Use the first PDF file found (you can modify this logic as needed)
#     if pdf_files:
#         pdf_file = pdf_files[0]
#         pdf_path = os.path.join(output_folder, pdf_file)
#     else:
#         pdf_path = None

#     return render(request, 'play.html', {'pdf_path': pdf_path})


# PASTE HERE TEMPLATE FUNCTION
# def add_post(request):
#     context = compute_context()
#     if 'post' not in request.POST or not request.POST['post']:
#         context['error'] = 'You type something to post bruv.'
#         return render(request, 'socialnetwork/stream.hmtl', context)

#     p = Post()
#     p.text = request.POST['post']
#     p.date = timezone.now()
#     p.user = request.user

#     create_post = CreateForm(request.POST, instance=p)
#     if not create_post.is_valid():
#         print("\nkillme\n")
#         return render(request, 'socialnetwork/stream.html', context)
#     create_post.save()
#     return redirect('stream')
