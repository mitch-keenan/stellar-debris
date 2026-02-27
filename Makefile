RACK_DIR ?= /Users/mitch/vcv-rack-dev/Rack-SDK
FLAGS +=
SOURCES += $(wildcard src/*.cpp)
DISTRIBUTABLES += res
include $(RACK_DIR)/plugin.mk
