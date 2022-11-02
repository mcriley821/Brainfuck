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
INCS := $(wildcard $(SRC_DIR)/*.h)
OBJS := $(subst $(SRC_DIR), $(OBJ_DIR), $(SRCS:.cpp=.o))

ifeq ($(MAKECMDGOALS),tests)
DEFINES += -DBOOST_TEST_DYN_LINK
LDFLAGS += -lboost_unit_test_framework
OBJS := $(filter-out $(OBJ_DIR)/main.o, $(OBJS))
endif

TSTS := $(wildcard $(TST_DIR)/*.cpp)
TST_OBJS := $(subst $(TST_DIR), $(OBJ_DIR), $(TSTS:.cpp=.o))

$(TARGET): $(OBJS)
ifdef OBJS
	@-echo "Building $@"
	$(CXXC) $(LDFLAGS) -o $@ $^
else
	@-echo "make: Nothing to be done for '$@'. No sources."
endif

$(TST_TARGET): $(TST_OBJS) $(OBJS)
	@-echo "Building $@"
	$(CXXC) -o $@ $^ $(LDFLAGS)

.PHONY: tests
tests: $(TST_TARGET)

.PHONY: debug
debug: cleanall
	@$(MAKE) CONF=debug

VPATH := $(SRC_DIR):$(TST_DIR)
$(OBJ_DIR)/%.o: %.cpp $(DEP_DIR)/%.d | $(OBJ_DIR) $(DEP_DIR)
	@-echo "Building $<"
	$(CXXC) $(DEPFLAGS) $(DEFINES) $(CXXFLAGS) -c $< -o $@
	$(POSTCOMPILE)

.PHONY: clean
clean:
	@-echo "Cleaning objects"
	-@rm -rf $(OBJ_DIR)

.PHONY: cleantests
cleantests: clean
	@-echo "Cleaning test files"
	-@rm -rf $(TST_TARGET)

.PHONY: cleanall
cleanall: clean cleantests
	@-echo "Cleaning all"
	-@rm -rf $(DEP_DIR) $(TARGET)

$(OBJ_DIR):
	@-mkdir -p $(OBJ_DIR)

$(DEP_DIR):
	@-mkdir -p $(DEP_DIR)

DEPS := $(subst $(SRC_DIR), $(DEP_DIR), $(SRCS:.cpp=.d)) \
				$(subst $(TST_DIR), $(DEP_DIR), $(TSTS:.cpp=.d))

$(DEPS):

include $(wildcard $(DEPS))

