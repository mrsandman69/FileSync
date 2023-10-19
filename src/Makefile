# FILEPATH: /Users/garethmallinson/Library/CloudStorage/OneDrive-TheUniversityofWesternAustralia/University/2023/Semester 2/CITS2002/project2/make

PROJECT =   mysync
HEADERS =   $(PROJECT).h
OBJ     =   mysync.o args_parser.o reporter.o file_dir_handler.o

C11     =   cc -std=c11
CFLAGS  =   -Wall -Werror

$(PROJECT) : $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)

%.o : %.c $(HEADERS)
	$(C11) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) $(OBJ)