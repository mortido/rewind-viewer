FBS_DIR := $(CURDIR)/fbs
FBS_MSG_FILE := $(FBS_DIR)/rewind_message.fbs $(FBS_DIR)/vector2f.fbs $(FBS_DIR)/rewind_event.fbs

VIEWER_OUT := $(CURDIR)/src/gateway/messages
CPP_DIR := $(CURDIR)/clients/cpp
CPP_CMAKE_DIR := $(CURDIR)/clients/cpp_cmake
CSHARP_DIR := $(CURDIR)/clients/csharp
JAVA_DIR := $(CURDIR)/clients/java
KOTLIN_DIR := $(CURDIR)/clients/kotlin
GO_DIR := $(CURDIR)/clients/go
TS_DIR := $(CURDIR)/clients/typescript
RUST_DIR := $(CURDIR)/clients/rust

all: viewer cpp_client csharp_client java_client kotlin_client go_client python_client ts_client rust_client

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
	@flatc --cpp -o $(CPP_CMAKE_DIR)/rewind_viewer/include/rewind_viewer/messages --filename-suffix ".fbs" -I $(FBS_DIR) $(FBS_MSG_FILE)
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

csharp_client:
	@echo "Cleaning $(CSHARP_DIR)/messages..."
	@rm -rf $(CSHARP_DIR)/messages
	@mkdir -p $(CSHARP_DIR)/messages
	@echo "Compiling FlatBuffers schemas to C#..."
	@flatc --csharp -o $(CSHARP_DIR)/messages -I $(FBS_DIR) $(FBS_MSG_FILE)

java_client:
	@echo "Cleaning $(JAVA_DIR)/messages..."
	@rm -rf $(JAVA_DIR)/messages
	@mkdir -p $(JAVA_DIR)/messages
	@echo "Compiling FlatBuffers schemas to Java..."
	@flatc --java -o $(JAVA_DIR)/messages -I $(FBS_DIR) $(FBS_MSG_FILE)

kotlin_client:
	@echo "Cleaning $(KOTLIN_DIR)/messages..."
	@rm -rf $(KOTLIN_DIR)/messages
	@mkdir -p $(KOTLIN_DIR)/messages
	@echo "Compiling FlatBuffers schemas to KOTLIN..."
	@flatc --java -o $(KOTLIN_DIR)/messages -I $(FBS_DIR) $(FBS_MSG_FILE)

go_client:
	@echo "Cleaning $(GO_DIR)/messages..."
	@rm -rf $(GO_DIR)/messages
	@mkdir -p $(GO_DIR)/messages
	@echo "Compiling FlatBuffers schemas to Go..."
	@flatc --go -o $(GO_DIR)/messages -I $(FBS_DIR) $(FBS_MSG_FILE)

ts_client:
	@echo "Cleaning $(TS_DIR)/messages..."
	@rm -rf $(TS_DIR)/messages
	@mkdir -p $(TS_DIR)/messages
	@echo "Compiling FlatBuffers schemas to TypeScript..."
	@flatc --ts -o $(TS_DIR)/messages -I $(FBS_DIR) $(FBS_MSG_FILE)

rust_client:
	@echo "Cleaning $(RUST_DIR)/messages..."
	@rm -rf $(RUST_DIR)/messages
	@mkdir -p $(RUST_DIR)/messages
	@echo "Compiling FlatBuffers schemas to Rust..."
	@flatc --rust -o $(RUST_DIR)/messages --filename-suffix ".fbs" -I $(FBS_DIR) $(FBS_MSG_FILE)


.PHONY: all viewer cpp_client csharp_client java_client kotlin_client go_client python_client ts_client rust_client