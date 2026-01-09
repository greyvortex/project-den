import tkinter as tk
from tkinter import filedialog
from PIL import Image
import numpy as np
import cv2  # for edge detection

# ASCII ramps
SHADE_RAMP = " .:-=+*#%@"

def map_angle_to_char(angle):
    if -22.5 <= angle <= 22.5 or angle >= 157.5 or angle <= -157.5:
        return "-"
    elif 22.5 < angle <= 67.5:
        return "/"
    elif -67.5 >= angle >= -112.5:
        return "\\"
    elif 67.5 < angle <= 112.5 or -112.5 <= angle <= -67.5:
        return "|"
    else:
        return "+"

def image_to_ascii(path, width=100):
    img = Image.open(path).convert("RGB")
    aspect_ratio = img.height / img.width
    height = int(width * aspect_ratio * 0.5)
    img = img.resize((width, height))
    arr = np.array(img)

    # Grayscale for edges
    gray = cv2.cvtColor(arr, cv2.COLOR_RGB2GRAY)
    gx = cv2.Sobel(gray, cv2.CV_32F, 1, 0)
    gy = cv2.Sobel(gray, cv2.CV_32F, 0, 1)
    mag, ang = cv2.cartToPolar(gx, gy, angleInDegrees=True)

    ascii_art = ""
    for y in range(height):
        line = ""
        for x in range(width):
            if mag[y, x] > 50:  # edge threshold
                ch = map_angle_to_char(ang[y, x])
            else:
                lum = gray[y, x]
                idx = int(lum / 255 * (len(SHADE_RAMP) - 1))
                ch = SHADE_RAMP[idx]
            line += ch
        ascii_art += line + "\n"
    return ascii_art

# Tkinter GUI
def open_image():
    path = filedialog.askopenfilename()
    if path:
        art = image_to_ascii(path, width=120)
        text.delete("1.0", tk.END)
        text.insert(tk.END, art)

root = tk.Tk()
root.title("ASCII Art Generator")

btn = tk.Button(root, text="Open Image", command=open_image)
btn.pack()

text = tk.Text(root, font=("Courier", 8))
text.pack(fill="both", expand=True)

root.mainloop()