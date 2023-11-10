from django import forms
from app.models import PDF

MAX_UPLOAD_SIZE = 2500000


class PDFForm(forms.ModelForm):
    class Meta:
        model = PDF
        # fields = ('pdf', 'text')
        fields = ('pdf', )

    def clean_pdf(self):
        pdf = self.cleaned_data['pdf']
        if not pdf or not hasattr(pdf, 'content_type'):
            raise forms.ValidationError('You must upload a file')
        if not pdf.content_type or not pdf.content_type.startswith('application/pdf'):
            raise forms.ValidationError('File type is not PDF')
        if pdf.size > MAX_UPLOAD_SIZE:
            raise forms.ValidationError(
                'File too big (max size is {0} bytes)'.format(MAX_UPLOAD_SIZE))
        return pdf
