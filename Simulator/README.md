The command of running simulators can be represent by:

xxx.exe (the running file)
-f the input format: for example, -f image%06d.png will take image000000.png, image0000001.png, ...... as input
-w -h the width and height of the simulator OR use -u to set the size equal to the images
-s -e the start and end index of the input images
-o output data stream file
-i -d thelta_i and thelta_d


Example arguments:
-f
"/home/andrew/Documents/CableFrames/0_index/%04d.png"
-u
-o
out.bin
-i
1720
-d
10
-m
"/home/andrew/Code/retinomorphic-sensing/Simulator/cmake-build-debug/out\DAVIS\merge%06d.png"
-s
0
-e
500