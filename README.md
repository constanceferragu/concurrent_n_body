# concurrent_n_body

Here are a few instructions on the files, what they do and how to run them.



### n_body.cpp
  
  That is the file containing the class NBody, which is included in all other files as it defines the bodies. 
  
  
 
### simulation.cpp

  That is the file containing the thread free algorithm and its visualization printed.
  To run it, do: 
  - make
  - ./simulation 
  - ./simulation N, where N is the number of random bodies to generate. 

If it is run without arguments, it will simulate the Earth-Moon system which we used extensively to debug, and is quite satisfying to see work. Automatically the simulation runs only for one day, which in the case of earth and moon is not so interesting. Instead, changing the line 77 to be "while(time<WEEK)" you can simulate one week. To do one month, do "while(time<4*WEEK)". The program will print the start position and end position of the bodies on the console. To see a graphical visualisation, refer to the simulation_visualise file. 

It can also be run with an argument N, where N is the number of random bodies you want to simulate.
  
 
 
### simulation_visualize.cpp
  
  That is the file containing the thread free algorithm and its visualization using SFML.
  To run it, you need to: 
  - go into the makefile
  - change the variable PATH1 from PATH1 = -I/usr/local/Cellar/sfml/2.5.1_1/include/ to PATH1 = -I/YOUR_PATH/include/
    where YOUR_PATH is the path where SFML is installed on your computer (for me it is YOUR_PATH = usr/local/Cellar/sfml/2.5.1_1)
  - change the variable PATH2 from PATH2 = -L/usr/local/Cellar/sfml/2.5.1_1/lib/ to PATH2= -L/YOUR_PATH/lib/
  Then do:
  - make visualize
  - ./simulation_visualize
  
  - In the beginning of the file there are two blocks of code, one commented and the other not. You can run the file twice for two examples by uncommenting random bodies and commenting eart moon.
 


### simulation_threads.cpp
the file simulation_threads.cpp has the parallel version of the algorithm. Build it with the command 'make threads'. It will create the file ./simulation_threads which can be run with arguments num_bodies and num_threads. 

There is a catch though, the num_bodies has to be an even number and it has to be divisible by num_threads. This is due to a problem in the implementation where we divide the tasks evenly among the threads, and if the last block is larger than the rest we have issues. 

A good working example would be ".\simulation_threads 4 2" but  ".\simulation_threads 9 2" will not work properly. 

In order to run the parallel algorithm with an earth moon system, uncomment the lines in simulation_threads.cpp saying ' TO RUN WITH EARTH MOON SIMULATION (3 MOONS ACTUALLY) UNCOMMENT THESE LINES'

  To run:
  - make threads
  - ./simulation_threads num_bodies




### simulation_threads_visualize.cpp

  That is the file containing the algorithm with threads and its visualization using SFML.
  To run it, you need to: 
  - go into the makefile
  - change the variable PATH1 from PATH1 = -I/usr/local/Cellar/sfml/2.5.1_1/include/ to PATH1 = -I/YOUR_PATH/include/
    where YOUR_PATH is the path where SFML is installed on your computer (for me it is YOUR_PATH = usr/local/Cellar/sfml/2.5.1_1)
  - change the variable PATH2 from PATH2 = -L/usr/local/Cellar/sfml/2.5.1_1/lib/ to PATH2= -L/YOUR_PATH/lib/
  Then do:
  - make threadsvisualize
  - ./simulation_threads_visualize



### simulation_moving_bodies_visualize.cpp

  That is the file containing the visualization of the Barnes-Hutt simulation
  To run it, you need to: 
  - go into the makefile
  - change the variable PATH1 from PATH1 = -I/usr/local/Cellar/sfml/2.5.1_1/include/ to PATH1 = -I/YOUR_PATH/include/
    where YOUR_PATH is the path where SFML is installed on your computer (for me it is YOUR_PATH = usr/local/Cellar/sfml/2.5.1_1)
  - change the variable PATH2 from PATH2 = -L/usr/local/Cellar/sfml/2.5.1_1/lib/ to PATH2= -L/YOUR_PATH/lib/
  Then do:
  - make
  - ./simulation_visualize
  
  

## Screen Recording
  You can watch the videos to check that the code runs well for us. 
  - Thread_free_earth_moon.mp4 is a simulation for the file simulation_visualize.cpp with two bodies: the Earth and the moon
  - Thread_free_random_Bodies.mp4 is a simulation for the file simulation_visualize.cpp with 20 random bodies. Note that it takes some time to run
  - Threads_free_earth_moon.mp4 is a simulation for the file simulation_threads.cpp with two bodies: the Earth and the moon
  - Threads_random_bodies.mp4 is a simulation for the file simulation_threads.cpp with 3 random bodies and 2 threads. 
  - Barnes-Hutt.mp4 is a simulation for the file simulation_moving_bodies_visualize.cpp 

  



  
  
