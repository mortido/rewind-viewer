FBS_DIR := $(CURDIR)/fbs
FBS_MSG_FILE := $(FBS_DIR)/rewind_message.fbs

VIEWER_OUT := $(CURDIR)/src/net/flatbuffers/messages
CPP_DIR := $(CURDIR)/clients/cpp
CPP_CMAKE_DIR := $(CURDIR)/clients/cpp_cmake

all: viewer cpp_client

viewer:
	@echo "Compiling FlatBuffers schemas for rewind viewer..."
	@echo "Cleaning $(VIEWER_OUT)..."
	@rm -rf $(VIEWER_OUT)/*
	@mkdir -p $(VIEWER_OUT)
	@flatc --cpp -o $(VIEWER_OUT) --filename-suffix ".fbs" -I $(FBS_DIR) $(FBS_MSG_FILE)

cpp_client:
	@echo "Cleaning $(CPP_CMAKE_DIR)/rewind_viewer/include/rewind_viewer/messages..."
	@rm -rf $(CPP_CMAKE_DIR)/rewind_viewer/include/rewind_viewer/messages/*
	@mkdir -p $(CPP_CMAKE_DIR)/rewind_viewer/include/rewind_viewer/messages
	@echo "Compiling FlatBuffers schemas to C++ client..."
	@flatc --cpp -o $(CPP_CMAKE_DIR)/rewind_viewer/include/rewind_viewer/messages --gen-object-api --filename-suffix ".fbs" -I $(FBS_DIR) $(FBS_MSG_FILE)
	@echo "Copying clsocket..."
	@rm -rf $(CPP_CMAKE_DIR)/clsocket
	@cp -r $(CURDIR)/3rdparty/clsocket $(CPP_CMAKE_DIR)
	@echo "Copying flatbuffers..."
	@rm -rf $(CPP_CMAKE_DIR)/flatbuffers
	@cp -r $(CURDIR)/3rdparty/flatbuffers $(CPP_CMAKE_DIR)

	@echo "Cleaning $(CPP_DIR)..."
	@rm -rf $(CPP_DIR)/*
	@mkdir -p $(CPP_DIR)
	@echo "Constructing $(CPP_DIR) from $(CPP_CMAKE_DIR)..."
	@cp -r $(CPP_CMAKE_DIR)/rewind_viewer/include/rewind_viewer $(CPP_DIR)
	@cp -r $(CPP_CMAKE_DIR)/flatbuffers/include/flatbuffers $(CPP_DIR)
	@cp -r $(CPP_CMAKE_DIR)/clsocket/include/clsocket $(CPP_DIR)
	@cp -r $(CPP_CMAKE_DIR)/clsocket/src/* $(CPP_DIR)/clsocket/
	@cp $(CPP_CMAKE_DIR)/main.cpp $(CPP_DIR)

.PHONY: all cpp_client