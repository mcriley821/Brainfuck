ifndef TARGET
	TARGET := run_tests
endif

ifndef ROOT
$(error Must provide ROOT path to project!)
endif

.DEFAULT_GOAL := $(TARGET)

CXXC := @g++

CXXFLAGS = -std=c++20 \
					 -Wall \
					 -Werror \
					 -Wextra \
					 -Wfatal-errors \
					 -I$(SRC_DIR) \
					 -I$(INC_DIR)
					 
DEFINES = -DBOOST_TEST_DYN_LINK

ifndef $(CONF)
	CONF := release
endif

ifeq ($(CONF), debug)
	CXXFLAGS += -g -ggdb -O0
	DEFINES += -DDEBUG
else
	CXXFLAGS += -O3
endif

LDFLAGS = -lboost_unit_test_framework

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.Td
POSTCOMPILE = @-mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d && touch $@

SRC_DIR := $(ROOT)/src
DEP_DIR := $(ROOT)/dep
OBJ_DIR := $(ROOT)/obj
INC_DIR := $(ROOT)/include

SRCS := $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
INCS := $(wildcard $(SRC_DIR)/*.h)

TSTS := $(wildcard *.cpp)
OBJS := $(patsubst %, $(OBJ_DIR)/%, $(TSTS:.cpp=.o)) \
				$(subst $(SRC_DIR), $(OBJ_DIR), $(SRCS:.cpp=.o))

$(TARGET): $(OBJS)
	@-echo "Building $@"
	$(CXXC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
$(OBJ_DIR)/%.o: %.cpp $(DEP_DIR)/%.d | $(DEP_DIR)
	@-echo "Building $<"
	@-mkdir -p $(shell dirname $@)
	$(CXXC) $(DEPFLAGS) $(DEFINES) $(CXXFLAGS) -c $< -o $@
	$(POSTCOMPILE)

$(OBJ_DIR):
	@-mkdir -p $(OBJ_DIR)

$(DEP_DIR):
	@-mkdir -p $(DEP_DIR)


DEPS := $(subst $(OBJ_DIR), $(DEP_DIR), $(OBJS:.o=.d))
$(DEPS):

include $(wildcard $(DEPS))

