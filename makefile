TARGET :=  brainfuck
TST_TARGET := run_tests
.DEFAULT_GOAL = $(TARGET)

CXXC := @g++

CXXFLAGS = -std=c++20 \
					 -Wall \
					 -Wextra \
					 -Werror \
					 -Wfatal-errors \
					 -I$(SRC_DIR) \
					 -I$(INC_DIR)
					 
DEFINES =

LDFLAGS = -lboost_program_options

SRC_DIR := src
INC_DIR := include
TST_DIR := tests
DEP_DIR := dep
OBJ_DIR := obj

#======================#

ifndef CONF
	CONF := release
endif

ifeq ($(CONF), debug)
	CXXFLAGS += -g -ggdb -O0
	DEFINES += -DDEBUG
else
	CXXFLAGS += -O3
endif

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.Td
POSTCOMPILE = @-mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d && touch $@

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(subst $(SRC_DIR), $(OBJ_DIR), $(SRCS:.cpp=.o))

$(TARGET): $(OBJS)
ifdef OBJS
	@-echo "Building $@"
	$(CXXC) $(LDFLAGS) -o $@ $^
else
	@-echo "make: Nothing to be done for '$@'. No sources."
endif

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d | $(OBJ_DIR) $(DEP_DIR)
	@-echo "Building $<"
	$(CXXC) $(DEPFLAGS) $(DEFINES) $(CXXFLAGS) -c $< -o $@
	$(POSTCOMPILE)

.PHONY: debug
debug: cleanall
	@$(MAKE) CONF=debug

.PHONY: tests
tests: $(OBJS) 
ifeq ($(CONF), debug)
	$(MAKE) debug
endif
	$(MAKE) -C $(TST_DIR) \
		TARGET=$(shell realpath --relative-to=$(TST_DIR) ./$(TST_TARGET)) \
		ROOT=$(shell pwd) \
		CONF=$(CONF)

.PHONY: clean
clean:
	@-echo "Cleaning objects"
	-@rm -rf $(OBJ_DIR)

.PHONY: cleanall
cleanall: clean
	@-echo "Cleaning all"
	-@rm -rf $(DEP_DIR) $(TARGET)
	-@rm -f $(TST_TARGET)

$(OBJ_DIR):
	@-mkdir -p $(OBJ_DIR)

$(DEP_DIR):
	@-mkdir -p $(DEP_DIR)

DEPS := $(subst $(SRC_DIR), $(DEP_DIR), $(SRCS:.cpp=.d))
$(DEPS):

include $(wildcard $(DEPS))

