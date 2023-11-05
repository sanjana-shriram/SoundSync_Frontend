from PIL import Image, ImageTk
import PyPDF2
import tkinter as tk
from tkinter.filedialog import askopenfile
# from tkinter import font as tkfont
from pdf2image import convert_from_path

'''
class MainFrame(tk.Tk):
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)

        self.titlefont = tkfont.Font(
            family='Verdana', size=12, weight="bold", slant="roman")

        container = tk.Frame()
        container.grid(row=0, column=0, sticky='nesw')

        self.id = tk.StringVar()
        self.id.set("Current Screen")

        self.listing = {}  # (name of our pages)

        for p in (ConsentPage, InstrumentPage, CalibrationPage, DisplayPage):
            page_name = p.__name__
            # create object for each class
            frame = p(parent=container, controller=self)
            frame.grid(row=0, column=0, sticky='nsew')
            self.listing[page_name] = frame
        self.up_frame("ConsentPage")

    def up_frame(self, page_name):
        page = self.listing[page_name]
        page.tkraise()


class ConsentPage(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        self.id = sticky

        logo = Image.open('logo.png')
        logo = logo.resize((350, 85))
        logo = ImageTk.PhotoImage(logo)
        logo_label = tk.Label(image=logo)
        logo_label.image = logo
        logo_label.grid(column=0, row=0)

        label = tk.Label(self, text="Consent Page \n",
                         font=controller.titlefont)
        label.grid(column=0, row=1)

        btn = tk.Button(self, text="to page 2",
                        command=lambda: controller.up_frame("InstrumentPage"))
        btn.grid(column=0, row=2)


class InstrumentPage(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        self.id = controller.id

        label = tk.Label(self, text="Instrument Selection Page \n",
                         font=controller.titlefont)
        label.pack()
        btn = tk.Button(self, text="to page 3",
                        command=lambda: controller.up_frame("CalibrationPage"))
        btn.pack()


class CalibrationPage(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        self.id = controller.id

        label = tk.Label(self, text="Calibration Page \n",
                         font=controller.titlefont)
        label.pack()

        btn = tk.Button(self, text="to page 4",
                        command=lambda: controller.up_frame("DisplayPage"))
        btn.pack()


class DisplayPage(tk.Frame):
    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        self.controller = controller
        self.id = controller.id

        label = tk.Label(self, text="Display Page \n",
                         font=controller.titlefont)
        label.pack()

        btn = tk.Button(self, text="to page 1",
                        command=lambda: controller.up_frame("ConsentPage"))
        btn.pack()


if __name__ == '__main__':
    app = MainFrame()
    app.mainloop()
'''
root = tk.Tk()
root.geometry('+%d+%d' % (350, 10))

canvas = tk.Canvas(root, width=600, height=600)
canvas.grid(columnspan=3, rowspan=3)

logo = Image.open('logo.png')
logo = logo.resize((350, 85))
logo = ImageTk.PhotoImage(logo)
logo_label = tk.Label(image=logo)
logo_label.image = logo
logo_label.grid(column=1, row=0)

# Instructions/welcome
instructions = tk.Label(
    root, text="Select a PDF file of your music from your computer.", font="Raleway")
instructions.grid(columnspan=3, column=0, row=1)

pages = convert_from_path('scale.pdf', 500)
for count, page in enumerate(pages):
    page.save(f'out{count}.jpg', 'JPEG')

p = Image.open('out0.jpg')
p = p.resize((600, 800))
p = ImageTk.PhotoImage(p)
p_label = tk.Label(image=p)
p_label.image = p
p_label.grid(column=1, row=1)


def pdf_to_img(page_num):
    page = doc.load_page(page_num)
    pix = page.get_pixmap(matrix=mat)
    return Image.frombytes("RGB", [pix.width, pix.height], pix.samples)


def show_image():
    try:
        page_num = int(entry.get()) - 1
        assert page_num >= 0 and page_num < num_pages
        im = pdf_to_img(page_num)
        img_tk = ImageTk.PhotoImage(im)
        frame = Frame(canvas)
        panel = Label(frame, image=img_tk)
        panel.pack(side="bottom", fill="both", expand="yes")
        frame.image = img_tk
        canvas.create_window(0, 0, anchor='nw', window=frame)
        frame.update_idletasks()
        canvas.config(scrollregion=canvas.bbox("all"))
    except:
        pass


def open_file():
    # browse_text.set("loading...")
    # file = askopenfile(parent=root, mode='rb', title='Select a PDF file', filetypes=[
    #                    ('PDF files', '*.pdf')])

    # if file:
    #     read_pdf = PyPDF2.PdfReader(file)
    #     page = read_pdf.pages[0]
    #     page_content = page.extract_text()
    #     print(page_content)

    #     # text box
    #     text_box = tk.Text(root, height=10, width=50, padx=15, pady=15)
    #     text_box.insert(1.0, page_content)
    #     # text_box.grid(column=1, row=3)
    #     browse_text.set("Browse")

    #     set_pdf_pages(read_pdf)
    pass


def set_pdf_pages(read_pdf):

    read_pdf[0].save('page0.jpg', 'JPEG')
    mypage = Image.open('page0.jpg')
    mypage = mypage.resize((350, 85))
    mypage = ImageTk.PhotoImage(mypage)
    page_label = tk.Label(image=mypage)
    page_label.image = mypage
    page_label.grid(column=1, row=2)


browse_text = tk.StringVar()
browse_btn = tk.Button(root, textvariable=browse_text, command=lambda: open_file(),
                       font="Raleway", bg="#20bebe", fg="white", height=2, width=15)
browse_text.set("Browse Files")
browse_btn.grid(column=1, row=2)


canvas = tk.Canvas(root, width=600, height=250)
canvas.grid(columnspan=3)


root.mainloop()
