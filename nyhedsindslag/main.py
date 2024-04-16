'''
IMPORTANT: the following line should be changed in the pillow library:

resized_pil = pilim.resize(newsize[::-1], Image.ANTIALIAS)

to:

resized_pil = pilim.resize(newsize[::-1], Image.LANCZOS)

in the file located at venv/lib/python3.11/site-packages/moviepy/video/fx/resize.py on line 37

DO NOT update the library
'''

from moviepy.editor import *

def create_news(profilepicturefilename):
    clip = VideoFileClip("Base - Nyhedsindslag.mp4").subclip(0,-3) # subtract last 3 seconds of the video clip

    image = (ImageClip(profilepicturefilename).set_start(13).set_duration(20).set_position((1000, 200)).resize(height=700))

    new_clip = CompositeVideoClip([clip, image]) # compose the image ontop of the video

    # store videofile
    new_clip.write_videofile("test.mp4",
                             fps=24,
                             codec='libx264',
                             audio=True,
                             audio_codec='aac',
                             temp_audiofile='temp-audio.m4a',
                             remove_temp=True)


picturefilename = '../scrapere/selected_image.jpg'
create_news(picturefilename)
