CC     :=  gcc
CFLAGS :=  -std=c11 -g -MMD -MP -W -Wall -Wextra -Wpedantic
LFLAGS :=

TARGET := ALCI
SRCS   := $(wildcard src/*.c)
OBJS   := $(patsubst src/%.c,obj/%.o,$(SRCS))
DEPS   := $(patsubst src/%.c,obj/%.d,$(SRCS))

-include $(DEPS)

.PHONY: run all clean compile

run: $(TARGET)
	clear
	./$(TARGET)

compile: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LFLAGS)

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) -c $< -o $@ $(CFLAGS)

