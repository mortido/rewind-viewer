FBS_DIR := $(CURDIR)/fbs
FBS_FILES := $(wildcard $(FBS_DIR)/*.fbs)

# Output directories for each language
CPP_OUT_DIR := $(CURDIR)/clients/cpp/messages/
CPP_CMAKE_OUT_DIR := $(CURDIR)/clients/cpp_cmake/rewind_viewer/include/rewind_viewer/messages/

all: cpp_client

#build_cpp_msg:
#	@echo "Cleaning $(CPP_OUT_DIR)..."
#	@rm -rf $(CPP_OUT_DIR)/*
#	@mkdir -p $(CPP_OUT_DIR)
#	@echo "Compiling FlatBuffers schemas to C++..."
#	$(foreach fbs,$(FBS_FILES),flatc --cpp -o $(CPP_OUT_DIR) --gen-object-api --filename-suffix ".fbs" -I $(FBS_DIR) $(fbs);)

cpp_client:
	@echo "Cleaning $(CPP_CMAKE_OUT_DIR)..."
	@rm -rf $(CPP_CMAKE_OUT_DIR)/*
	@mkdir -p $(CPP_CMAKE_OUT_DIR)
	@echo "Compiling FlatBuffers schemas to C++..."
	$(foreach fbs,$(FBS_FILES),flatc --cpp -o $(CPP_CMAKE_OUT_DIR) --gen-object-api --filename-suffix ".fbs" -I $(FBS_DIR) $(fbs);)
# copy files.

.PHONY: all cpp_client