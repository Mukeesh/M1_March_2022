# Name of the project
PROJECT_NAME = QUIZ_Project

#Doxyfile folder
DOC = documentation

#All source code files
SRC = Client.c\
src/Client_operations.c

#All test source files
TEST_SRC = src/Client_operations.c\
test/test_Client_operations.c\
unity/unity.c

TEST_OUTPUT = Test_a.out

#All include folders with header files
INC = -Iinc\
-Iunity\

#Project Output Name
PROJECT_OUTPUT = Client.out

#Document files
DOCUMENTATION_OUTPUT = documentation/html
Build:
	gcc Client.c test_quiz.c unity/unity.c -Iunity
Run:
	./a.out
