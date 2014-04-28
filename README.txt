You can run the code (including make and meshview) for Taubin smoothing using
./taubin.sh alpha mu iterations [mesh_file]

and Loop subdivision using:
./loop.sh [mesh_file]

Sample images are included in screenshots folder. Loop subdivision does not support non-triangular faces (such as all faces in both bump meshes), but it was tested on other meshes and works flawlessly.

Taubin smoothing was run using 10 iterations of .5 alpha and .6 mu. Do not provide negative coefficients. 

Abbas Naderi (an2wm@virginia.edu)