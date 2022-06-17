CXX = g++
CFLAGS = -pthread -std=c++11 -Wall
PATH1 = -I/usr/local/Cellar/sfml/2.5.1_1/include/
PATH2 = -L/usr/local/Cellar/sfml/2.5.1_1/lib/
CFLAGS2 = -lsfml-graphics -lsfml-window -lsfml-system

# SOURCES = gradinglib/gradinglib.cpp grading/grading.cpp main.cpp 
# OBJECTS = gradinglib.o grading.o main.o 
# STUDENTS_SOURCES = CoarseSetList.cpp SetList.cpp td5.cpp

# grader: $(OBJECTS)
# 	$(CXX) $(CFLAGS) -o grader $(OBJECTS) 

# gradinglib.o: gradinglib/gradinglib.cpp gradinglib/gradinglib.hpp
# 	$(CXX) -c $(CFLAGS) -o gradinglib.o gradinglib/gradinglib.cpp

# grading.o: grading/grading.cpp gradinglib/gradinglib.hpp CoarseBST.cpp td6.cpp
# 	$(CXX) -c $(CFLAGS) -o grading.o grading/grading.cpp -I.


main.o: simulation_moving_bodies_visualize.cpp 
	$(CXX) $(CFLAGS) simulation_moving_bodies_visualize.cpp $(PATH1) -o simulation_visualize $(PATH2) $(CFLAGS2)

clean:
	rm -f *.o