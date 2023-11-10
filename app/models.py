from django.db import models
from django.contrib.auth.models import User


class PDF(models.Model):
    text = models.CharField(max_length=200)

    pdf = models.FileField(blank=True)
    content_type = models.CharField(max_length=50)

    def __str__(self):
        return 'id=' + str(self.id) + ',text="' + self.text + '"'

 # user = models.ForeignKey(User, default=None, on_delete=models.PROTECT)
# ip_addr = models.GenericIPAddressField()
