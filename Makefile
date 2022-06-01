CXX = g++
CFLAGS = -pthread -std=c++11 -Wall

# SOURCES = gradinglib/gradinglib.cpp grading/grading.cpp main.cpp 
# OBJECTS = gradinglib.o grading.o main.o 
# STUDENTS_SOURCES = CoarseSetList.cpp SetList.cpp td5.cpp

# grader: $(OBJECTS)
# 	$(CXX) $(CFLAGS) -o grader $(OBJECTS) 

# gradinglib.o: gradinglib/gradinglib.cpp gradinglib/gradinglib.hpp
# 	$(CXX) -c $(CFLAGS) -o gradinglib.o gradinglib/gradinglib.cpp

# grading.o: grading/grading.cpp gradinglib/gradinglib.hpp CoarseBST.cpp td6.cpp
# 	$(CXX) -c $(CFLAGS) -o grading.o grading/grading.cpp -I.

main.o: simulation.cpp 
	$(CXX) $(CFLAGS) simulation.cpp

clean:
	rm -f *.o