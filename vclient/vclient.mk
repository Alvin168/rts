# encoding: UTF-8

VCLIENT_BIN := $(BIN_DIR)/vclient

VCLIENT_INC_DIR := $(VCLIENT_DIR)/inc
VCLIENT_SRC_DIR := $(VCLIENT_DIR)/src
VCLIENT_OBJ_DIR := $(VCLIENT_DIR)/.obj
VCLIENT_DEP_DIR := $(VCLIENT_DIR)/.dep

# VCLIENT INCLUDES
VCLIENT_INCLUDES := -I $(VCLIENT_INC_DIR)
VCLIENT_INCLUDES += 

# VCLIENT DEFINITIONS
VCLIENT_DEFINITIONS := 
VCLIENT_DEFINITIONS += 

# VCLIENT SOURCES
VCLIENT_SRCS_NOTDIR := $(notdir $(wildcard $(VCLIENT_SRC_DIR)/*.c))

# VCLIENT OBJECTS
VCLIENT_OBJS := $(patsubst %.c, $(VCLIENT_OBJ_DIR)/%.o, $(VCLIENT_SRCS_NOTDIR))

# VCLIENT DEPENDENCIES
VCLIENT_DEPS := $(patsubst %.c, $(VCLIENT_DEP_DIR)/%.d, $(VCLIENT_SRCS_NOTDIR))

$(VCLIENT_OBJ_DIR)/%.o: $(VCLIENT_SRC_DIR)/%.c
	$(CC) -c $< $(FLAGS) $(VCLIENT_INCLUDES) -MMD -MF $(VCLIENT_DEP_DIR)/$(notdir $(patsubst %.o, %.d, $@)) -o $@

-include $(VCLIENT_DEPS)