The command of running reconstruction method can be represent by:

xxx.exe (the running file)
-i the input stream data
-w -h the width and height of the image
-f the total images to output
-o the format of the reconstruction frames: for example, -o image%06d.png will output image000000.png, image0000001.png, ...... 
-t -d thelta_i and thelta_d
-y the type of the reconstruction method

FFMPEG video to png images example: ffmpeg -i ./video.mp4 -start_number 0 -s 1280x720 ./0_index/%04d.png
(Must start at index 0)

