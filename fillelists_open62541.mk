# define freertos dir
62541_DIR = $(ROOT_DIR)/open62541

# add freertos source
62541_SRC  = 	$(62541_DIR)/open62541.c			
# include directories
INC			+= 	$(62541_DIR)

C_SOURCES  += 	$(62541_SRC)
C_INCLUDES	+= 	$(INC)