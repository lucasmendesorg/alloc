     CC	= gcc
     RM	= rm -Rf
 CFLAGS	= -Wall

   OBJS	= main.o
   AOUT	= alloc

all: $(OBJS)
	$(CC) -o $(AOUT) $(OBJS)

clean:
	$(RM) $(OBJS) $(AOUT)
