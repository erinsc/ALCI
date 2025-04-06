CFLAGS :=  -std=c11 -g -MMD -MP -W -Wall -Wextra -Wpedantic
LFLAGS :=

TARGET := ALCI
SRCS   := $(wildcard src/*.c)
OBJS   := $(patsubst src/%.c,obj/%.o,$(SRCS))
DEPS   := $(patsubst src/%.c,obj/%.d,$(SRCS))

.PHONY: run clean compile

run: $(TARGET)
	clear
	./$(TARGET)

-include $(DEPS)

compile: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

$(TARGET): $(OBJS)
	gcc $(OBJS) -o $@ $(LFLAGS)

obj/%.o: src/%.c
	@mkdir -p obj
	gcc -c $< -o $@ $(CFLAGS)
