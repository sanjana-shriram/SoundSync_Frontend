import tkinter as tk
from tkinter.filedialog import askopenfilename
import fitz  # PyMuPDF library


class PDFViewer(tk.Tk):
    def __init__(self):
        super().__init__()
        self.geometry("800x600")
        self.canvas = tk.Canvas(self, width=600, height=400)
        self.canvas.grid(row=0, column=0, columnspan=3)
        self.current_page = 0

        # Create buttons for navigation
        prev_button = tk.Button(
            self, text="Previous Page", command=self.show_previous_page)
        prev_button.grid(row=1, column=0)
        next_button = tk.Button(self, text="Next Page",
                                command=self.show_next_page)
        next_button.grid(row=1, column=1)

        browse_button = tk.Button(
            self, text="Browse PDF", command=self.open_pdf)
        browse_button.grid(row=1, column=2)

    def open_pdf(self):
        file_path = askopenfilename(filetypes=[("PDF files", "*.pdf")])
        if file_path:
            self.pdf_document = fitz.open(file_path)
            self.show_current_page()

    def show_current_page(self):
        page = self.pdf_document.load_page(self.current_page)
        pix = page.get_pixmap()
        img = tk.PhotoImage(data=pix.samples)
        self.canvas.create_image(0, 0, anchor="nw", image=img)
        self.canvas.image = img  # Keep a reference to the image object

    def show_previous_page(self):
        if self.current_page > 0:
            self.current_page -= 1
            self.show_current_page()

    def show_next_page(self):
        if self.current_page < len(self.pdf_document) - 1:
            self.current_page += 1
            self.show_current_page()


if __name__ == "__main__":
    app = PDFViewer()
    app.mainloop()
