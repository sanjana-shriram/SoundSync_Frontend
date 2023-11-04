from PIL import Image, ImageTk
import PyPDF2
import tkinter as tk
import os

# os.environ['TK_SILENCE_DEPRECATION'] = '1'

# Rest of your Python script or Django project code goes here


root = tk.Tk()  # beginning of interface

canvas = tk.Canvas(root, width=700, height=900)
canvas.grid(columnspan=3)

logo = Image.open('logo.png')
logo = ImageTk.PhotoImage(logo)
logo_label = tk.Label(image=logo)
logo_label.image = logo
logo_label.grid(column=0, row=0)

# label = tk.Label(
#     text="Hello, Tkinter",
#     fg="white",
#     bg="black",
#     width=10,
#     height=10
# )
# button = tk.Button(
#     text="Click me!",
#     width=25,
#     height=5,
#     bg="blue",
#     fg="yellow",
# )

# # greeting.pack()

# # # logo
# # try:
# #     logo = Image.open('logo.png')
# #     logo = ImageTk.PhotoImage(logo)
# #     logo_label = tk.Label(image=logo)
# #     logo_label.image = logo
# #     logo_label.grid(column=0, row=0)
# # except Exception as e:
# #     print(f"Error: {e}")

root.mainloop()
