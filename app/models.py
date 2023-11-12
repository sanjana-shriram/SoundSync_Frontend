from django.db import models
from django.contrib.auth.models import User


def files_upload_path(instance, filename):
    return f'outputs/{filename}'


class Upload(models.Model):
    # text = models.CharField(max_length=200)
    pdf = models.FileField(upload_to=files_upload_path)
    # midi = models.FileField(blank=True)
    # ORCHESTRA_INSTRUMENT_CHOICES = [
    #     ('violin', 'Violin'),
    #     ('viola', 'Viola'),
    #     ('cello', 'Cello'),
    #     ('flute', 'Flute'),
    #     ('clarinet', 'Clarinet'),
    #     ('oboe', 'Oboe'),
    # ]

    # instrument = models.CharField(
    #     max_length=50, choices=ORCHESTRA_INSTRUMENT_CHOICES)
    # content_type_instrument = models.CharField(max_length=200)
    content_type_pdf = models.CharField(max_length=200)
    # content_type_midi = models.CharField(max_length=200)

    def __str__(self):
        return 'id=' + str(self.id) + ',text="' + self.text + '"'
