# concurrent_n_body

Hello everyone!

Here are a few instructions on the files, what they do and how to run them.



### n_body.cpp
  
  That is the file containing the class NBody, which is included in all other files as it defines the bodies. 
  
  
 
### simulation.cpp

  That is the file containing the thread free algorithm and its visualization printed.
  To run it, do: 
  - make
  - ./simulation 
  
 
 
### simulation_visualize.cpp
  
  That is the file containing the thread free algorithm and its visualization using SFML.
  To run it, you need to: 
  - go into the makefile
  - change the variable PATH1 from PATH1 = -I/usr/local/Cellar/sfml/2.5.1_1/include/ to PATH1 = -I/YOUR_PATH/include/
    where YOUR_PATH is the path where SFML is installed on your computer (for me it is YOUR_PATH = usr/local/Cellar/sfml/2.5.1_1)
  - change the variable PATH2 from PATH2 = -L/usr/local/Cellar/sfml/2.5.1_1/lib/ to PATH2= -L/YOUR_PATH/lib/
  Then do:
  - make visualize
  - ./simulation_visualize.cpp



### simulation_threads.cpp
  That is the file containing the algorithm with threads and its visualization using SFML.
  To run it, you need to: 
  - go into the makefile
  - change the variable PATH1 from PATH1 = -I/usr/local/Cellar/sfml/2.5.1_1/include/ to PATH1 = -I/YOUR_PATH/include/
    where YOUR_PATH is the path where SFML is installed on your computer (for me it is YOUR_PATH = usr/local/Cellar/sfml/2.5.1_1)
  - change the variable PATH2 from PATH2 = -L/usr/local/Cellar/sfml/2.5.1_1/lib/ to PATH2= -L/YOUR_PATH/lib/
  Then do:
  - make threads
  - ./simulation_threads.cpp
  



  
  
