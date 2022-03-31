TARGET:=button_client
SRCS:=button_client.c
OBJS:=$(SRCS:.c=.o)
CFLAGS:=-DDEBUG -O
LIBS:= -lpthread

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(OBJS) $(TARGET)