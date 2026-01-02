import sys
import time
import pygame
from multiprocessing import Process

def play_music(music):
    pygame.mixer.init()
    pygame.mixer.music.load(music)
    pygame.mixer.music.set_volume(0.7)
    pygame.mixer.music.play()
    pass

tkl = """Maybe it's the way you say my name
Maybe it's the way you play your game
But it's so good, I've never known anybody like you
But it's so good, I've never dreamed of nobody like you
And I've heard of a love that comes once in a lifetime
And I'm pretty sure that you are that love of mine
'Cause I'm in a field of dandelions
Wishing on every one that you'd be mine, mine
And I see forever in your eyes
I feel okay when I see you smile, smile
Wishing on dandelions all of the time
Praying to God that one day you'll be mine
Wishing on dandelions all of the time, all of the time
I think that you are the one for me
'Cause it gets so hard to breathe
When you're looking at me, I've never felt so alive and free
When you're looking at me, I've never felt so happy
And I've heard of a love that comes once in a lifetime
And I'm pretty sure that you are that love of mine
'Cause I'm in a field of dandelions
Wishing on every one that you'd be mine, mine
And I see forever in your eyes
I feel okay when I see you smile, smile
Wishing on dandelions all of the time
Praying to God that one day you'll be mine
Wishing on dandelions all of the time
All of the time
Dandelion, into the wind you go
Won't you let my darling know?
Dandelion, into the wind you go
Won't you let my darling know that?
I'm in a field of dandelions
Wishing on every one that you'd be mine, mine
Oh, and I see forever in your eyes
I feel okay when I see you smile, smile
Wishing on dandelions all of the time
Praying to God that one day you'll be mine
Wishing on dandelions all of the time, all of the time
I'm in a field of dandelions
Wishing on every one that you'd be mine, mine"""

def lyrics(song):
    timing = [0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5,]
    time.sleep(13)
    for char in song:
        sys.stdout.write(char)
        sys.stdout.flush()
        time.sleep(timing[char.count(char) % len(timing)])


if __name__ == "__main__":
    music_file = "<path_to_your_music_file>"  # Replace with the path to your music file
    play_music(music_file)
    lyrics(tkl)
    while pygame.mixer.music.get_busy():
        time.sleep(1)