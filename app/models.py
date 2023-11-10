from django.db import models
from django.contrib.auth.models import User


class PDF(models.Model):
    text = models.CharField(max_length=200)
    pdf = models.FileField(blank=True)
    content_type = models.CharField(max_length=50)

    def __str__(self):
        return 'id=' + str(self.id) + ',text="' + self.text + '"'


class Midi(models.Model):
    text = models.CharField(max_length=200)
    midi = models.FileField(blank=True)
    content_type = models.CharField(max_length=50)

    def __str__(self):
        return 'id=' + str(self.id) + ',text="' + self.text + '"'


class Instrument(models.Model):

    ORCHESTRA_INSTRUMENT_CHOICES = [
        ('violin', 'Violin'),
        ('viola', 'Viola'),
        ('cello', 'Cello'),
        ('flute', 'Flute'),
        ('clarinet', 'Clarinet'),
        ('oboe', 'Oboe'),
    ]

    text = models.CharField(max_length=200)
    instrument = models.CharField(
        max_length=50, choices=ORCHESTRA_INSTRUMENT_CHOICES)
    content_type = models.CharField(max_length=50)
