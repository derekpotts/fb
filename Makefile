# https://spin.atomicobject.com/2016/08/26/makefile-c-projects/

CXX = clang++
CPPFLAGS = -std=c++20 -Wall -Wextra
LDFLAGS = -lwthttp -lwt

TARGET_EXEC ?= fb

OBJ_DIR ?= ./obj
SRC_DIRS ?= .

SRCS := $(shell find $(SRC_DIRS) -name "*.cpp" -or -name "*.c" -or -name "*.s")
OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS += $(INC_FLAGS) -MMD -MP

$(OBJ_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# assembly
$(OBJ_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(OBJ_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(OBJ_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: run clean deploy

run: $(OBJ_DIR)/$(TARGET_EXEC)
	$(OBJ_DIR)/$(TARGET_EXEC) \
		--accesslog=- \
		--docroot . --resources-dir /usr/share/Wt/resources \
		--http-address localhost --http-port 1024

clean:
	$(RM) -r $(OBJ_DIR)

deploy:
	scp -p \
		*.h *.cpp \
		*.css \
		Makefile \
		root@t5.helloworld.sh:src/fb/
	ssh root@t5.helloworld.sh "cd src/fb && make clean"
	ssh root@t5.helloworld.sh "cd src/fb && make"
	ssh root@t5.helloworld.sh "systemctl restart fb"

-include $(DEPS)

MKDIR_P ?= mkdir -p
