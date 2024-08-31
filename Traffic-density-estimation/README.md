# Traffic-density-estimation

Traffic density estimation using OpenCV functions.

## Subtask1: Camera angle correction and frame cropping.

    - Converting colored image into gray image.
    - Camera angle correction using Homography.
    - Cropping of the image.

### How to run?

    - give the address of the image as command line argument.

### Image of the sample video:
 ![](https://github.com/Amaniitd/Traffic-density-estimation/blob/main/traffic.jpg)
### Image after prespective correction:
 ![](https://github.com/Amaniitd/Traffic-density-estimation/blob/main/traffic_projected.jpg)

## Subtask2: Queue and dynamic density estimation from traffic video

    - The code will take a video as input and will estimate the density of the traffic.
    - The output will be queue density (blue line in the graph below) and dynamic density (orange line) for the video frames.
    - Queue density is density of all vehicles queued (either standing or moving) which are waiting for red signal in the straight stretch of road going towards north (output after angle correction and cropping in Subtask1). Dynamic density is the density of those vehicles which are not standing but moving in that same stretch.

### Output graph:
![](https://github.com/Amaniitd/Traffic-density-estimation/blob/main/density.jpg)


## Subtask3: Understanding and analyzing trade-offs in software design

    - Sub-sampling frames -- processing every x frame i.e. process frame N and then frame N+x, and for all intermediate frames just use the value obtained for N - total processing time will reduce, but utility might decrease as intermediate frames values might differ from baseline. Parameter for this method is x, how many frames you drop.
    - Reduce resolution for each frame. Lower resolution frames might be processed faster, but having higher errors. Parameter can be resolution XxY.
    - Split work spatially across threads (application level pthreads) by giving each thread part of a frame to process. Parameter can be number of splits i.e. number of threads, if each thread gets one split. You might need to take care of boundary pixels that different threads process for utility.
    - Split work temporally across threads (application level pthreads), by giving consecutive frames to different threads for processing. Parameter can be number of threads.
