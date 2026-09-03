all:

build-mcp23017-demo:
upload-mcp23017-demo:
monitor-mcp23017-demo:

build-load-cell:
upload-load-cell:

build-function-generator-esp32s3:
upload-function-generator-esp32s3:
monitor-function-generator-esp32s3:

##############
# PlatformIO
##############

build-%:
	pio run -e $*

upload-%:
	pio run -e $* --target=upload --target=monitor

monitor-%:
	pio run -e $* --target=monitor

###########
# IDE: Zed
###########
.clangd: make-clangd.sh
	./make-clangd.sh
