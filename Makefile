CC = gcc
#CC = clang
CFLAGS = -Wfatal-errors -Wall -Wextra -Wconversion -std=gnu11 -O0  #-fsanitize=address  

LDFLAGS = #-fsanitize=address -static-libasan 
LIBS = -lpthread

PROGRAMS  = test main ch15/join_thread  ch15/detach_thread ch15/race_corrupt_memory ch15/race ch15/race_to_data ch16/race_to_data_mutex ch16/race_barrier ch16/race_to_data_sem ch16/race_cond_var 

GCC_CFLAGS = -DMESSAGE='"Compiled with GCC"' 
CLANG_CFLAGS = -DMESSAGE='"Compiled with Clang"' -v

ifeq ($(CC), gcc)
CFLAGS +=    $(GCC_CFLAGS) 
else ifeq ($(CC), clang)
CFLAGS +=    $(CLANG_CFLAGS)  
endif

.PHONY: clean all

all: $(PROGRAMS) 

test: test.o system.o print.o c_string.o 
	$(CC) $(CFLAGS) $(LIBS) $(LDFLAGS) test.o system.o print.o c_string.o  -o test 
	
test.o: test.c print.h system.h result_codes.h c_string.h
	$(CC) $(CFLAGS) -c test.c -o test.o 

main: main.o print.o c_string.o system.o
	$(CC) $(CFLAGS) $(LIBS) $(LDFLAGS) main.o print.o c_string.o system.o -o main 
	
main.o: main.c print.h system.h result_codes.h c_string.h
	$(CC) $(CFLAGS) -c main.c -o main.o 
	
c_utility.o: utility.h utility.c
	$(CC) $(CFLAGS) -c utility.c -o c_utility.o 
	
system.o: system.c system.h print.h result_codes.h
	$(CC) $(CFLAGS) -c system.c 
	
shared_lib_open.o: shared_lib_open.h shared_lib_open.c
	$(CC) $(CFLAGS) -c shared_lib_open.c -o shared_lib_open.o 
	
print.o: print.h print.c
	$(CC) $(CFLAGS) -c -fPIC print.c 
	
c_string.o: c_string.h c_string.c
	$(CC) $(CFLAGS) -c -fPIC c_string.c 
	
allocate.o: allocate.h allocate.c
	$(CC) $(CFLAGS) -c -fPIC allocate.c 
	
ch15/%.o: ch15/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	
ch16/%.o: ch16/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	
ch15/join_thread: ch15/join_thread.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) ch15/join_thread.o -o ch15/join_thread
	
ch15/detach_thread: ch15/detach_thread.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) ch15/detach_thread.o -o ch15/detach_thread
	
ch15/race: ch15/race.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) ch15/race.o -o ch15/race
	
ch15/race_to_data: ch15/race_to_data.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) ch15/race_to_data.o -o ch15/race_to_data
	
ch15/race_corrupt_memory: ch15/race_corrupt_memory.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) ch15/race_corrupt_memory.o -o ch15/race_corrupt_memory
	
ch16/race_to_data_mutex: ch16/race_to_data_mutex.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) ch16/race_to_data_mutex.o -o ch16/race_to_data_mutex
	
ch16/race_cond_var: ch16/race_cond_var.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) ch16/race_cond_var.o -o ch16/race_cond_var

ch16/race_barrier: ch16/race_barrier.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) ch16/race_barrier.o -o ch16/race_barrier
	
ch16/race_to_data_sem: ch16/race_to_data_sem.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) ch16/race_to_data_sem.o -o ch16/race_to_data_sem

clean :
	rm $(PROGRAMS) *.o ch15/*.o ch16/*.o
