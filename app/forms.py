from django import forms
from app.models import PDF, Midi, Instrument

MAX_UPLOAD_SIZE = 2500000


class InstrumentForm(forms.ModelForm):
    class Meta:
        model = Instrument
        fields = ('instrument', )

    def clean_instrument(self):
        instrument = self.cleaned_data['instrument']
        return instrument


class PDFForm(forms.ModelForm):
    class Meta:
        model = PDF
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


class MidiForm(forms.ModelForm):
    class Meta:
        model = Midi
        fields = ('midi', )

    def clean_midi(self):
        midi = self.cleaned_data['midi']
        if not midi or not hasattr(midi, 'content_type'):
            raise forms.ValidationError('You must upload a MIDI file')
        # Check if the file has a .midi or .mid extension
        if not (midi.name.endswith('.midi') or midi.name.endswith('.mid')):
            raise forms.ValidationError(
                'File is not a MIDI file. You must upload a MIDI file!')
        if midi.size > MAX_UPLOAD_SIZE:
            raise forms.ValidationError(
                'File too big (max size is {0} bytes)'.format(MAX_UPLOAD_SIZE))
        return midi