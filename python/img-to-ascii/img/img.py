from PIL import Image
import tkinter as tk
from tkinter import Text,filedialog

root = tk.Tk()
root.title("Image to ASCII Art")
root.geometry("800x600")
root.minsize(800, 600)

text_box = tk.Text(root, wrap=tk.NONE, font=("Courier", 8))
text_box.pack(expand=True, fill=tk.BOTH)


def convert_to_ascii(path):
    chars = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjt/\|()1{}[]?-_+~<>i!lI;:,^`'. "
    img = Image.open(path).convert("L")
    img = img.resize((100, 50))  # Adjust size as needed
    pixels = img.getdata()
    ascii_str = "".join([chars[pixel // 25] for pixel in pixels])
    ascii_lines = [ascii_str[i:i+100] for i in range(0, len(ascii_str), 100)]
    return "\n".join(ascii_lines)

def open_image():
    file_path = filedialog.askopenfilename()
    if file_path:
        ascii_art = convert_to_ascii(file_path)
        text_box.delete("1.0", tk.END)
        text_box.insert(tk.END, ascii_art)

button = tk.Button(root, text="Open Image", command=open_image)
button.pack()

root.mainloop()
