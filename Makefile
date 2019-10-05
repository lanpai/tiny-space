ifeq ($(CC),)
	CC := clang
endif

ifeq ($(CXX),)
	CXX := clang++
endif

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

BINARY=tiny-space
OBJS = GridShader.o GraphScreen.o GraphEngine.o main.o

BUILD_DIR = build
OBJ_DIR = obj

CPP_STD=c++11

LIB = -lGL -lglfw -lGLEW -ltiny-piyo

output: $(OBJS)
	@mkdir -p $(BUILD_DIR)
	cd $(OBJ_DIR); $(CXX) -Wall $(OBJS) -o ../$(BUILD_DIR)/$(BINARY) $(LIB)

$(OBJS): %.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	cd $(OBJ_DIR); $(CXX) -Wall -std=$(CPP_STD) -c ../$<

run:
	$(BUILD_DIR)/$(BINARY)

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)
