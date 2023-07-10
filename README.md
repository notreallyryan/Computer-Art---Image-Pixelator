# Computer-Art---Image-Pixelator

The final assignment in Compter Science 221 - Data Structures and Algorithms. This one was a lot of fun to do, so I decided to put it on github.

The objective of the assignment was to pixelize a square image of dimensions 2^n to produce an image with varying resolution:

![stanley-totem-poles](https://github.com/notreallyryan/Computer-Art---Image-Pixelator/assets/96549151/321f7b2d-5fca-4035-a5ba-521d9ee4937a)

However, instead of uniformly decreasing the resolution of the image, the resolution of each general area was to be based on the variation of the contained pixels. 
This created an art-like effect where the detailed sections of the image are higher quality, while the backgrounds are made blocky:

![given-comp-count-stanley](https://github.com/notreallyryan/Computer-Art---Image-Pixelator/assets/96549151/fcd961ce-1b12-42d6-b8f3-e5e9de21be3e)

To do this, I created a quadtree structure based on the four quadrants of the square image, where each node stored the sum of its children's pixel values:

![image](https://github.com/notreallyryan/Computer-Art---Image-Pixelator/assets/96549151/8d1b6d57-982a-4b82-aadf-d2ebc62ad31d)

To create the effect, the structure was traversed, and any node that had a variance less than a user-inputed threshold was pruned. When the image was reprinted, all pixels in the node would be replaced with the respective
average value pixel. 
