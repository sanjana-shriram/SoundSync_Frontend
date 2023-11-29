from django.db import models
from django.contrib.auth.models import User


class Upload(models.Model):
    pdf = models.FileField(upload_to='outputs/')
    midi = models.FileField(blank=True)
    ORCHESTRA_INSTRUMENT_CHOICES = [
        ('violin', 'Violin'),
        ('viola', 'Viola'),
        ('cello', 'Cello'),
        ('flute', 'Flute'),
        ('clarinet', 'Clarinet'),
        ('oboe', 'Oboe'),
    ]

    instrument = models.CharField(
        max_length=50, choices=ORCHESTRA_INSTRUMENT_CHOICES)

    def __str__(self):
        return 'id=' + str(self.id) + ',text="' + self.text + '"'
