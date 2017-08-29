FW_VERSION := $(shell git tag -l --points-at HEAD)
ifeq ($(FW_VERSION),)
FW_VERSION := $(shell git log -1 --pretty=format:%h)
endif

all:
	@echo VERSION=$(FW_VERSION)
