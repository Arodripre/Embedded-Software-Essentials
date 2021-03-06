#     *-Makefile-*    #

# Compiler selection depending on platform
ifeq ($(platform),bbb)
cc := arm-linux-gnueabihf-gcc
else ifeq ($(platform),frdm)
cc := arm-none-eabi-gcc
MCFLAG= -mcpu=cortex-m0plus -march=armv6-m --specs=nosys.specs -mthumb
else
cc := gcc
endif


# Compiler options

CFLAGS = -Wall
CFLAGS += -g
CFLAGS += -O0
CFLAGS += -I $(include)

# Settinf standard of compiler to c99
STD= -std=c99

#Beaglebone scp path : IP address: 192.168.7.2 , Root directory
BBB_PATH= project root@192.168.7.2:/root

# Compiler flag to generate dependancy flag


#including souce.mk file 
include sources.mk

#   PHONY Protection
.PHONY= clean all preprocessor asm-file compile-all build upload build-lib map_dep 

# This will build all the targets given in the prerequisites
all: preprocess asm-file compile-all build build-lib map_dep
#     Pre-Processing  


# Preprocess will give .i files from the .c files either individually or all of them at once

preprocess: $(i_files)
%.i:%.c 
	@mkdir -p -- "$(predir)"
	$(cc) $(STD) $(CFLAGS) -E -o $(predir)/$@ $<

# This wil generate .s files from .i files. If i files are not there ; they will be automatically generated
 
asm-file: $(s_files)
%.s: %.i
	@mkdir -p -- "$(asmdir)"
	$(cc) $(STD) $(CFLAGS) -S -o $(asmdir)/$@ $(predir)/$<
	
# Generates output files from .s files; if .s files are not present they are generated

compile-all: $(o_files)
%.o: %.s
	@mkdir -p -- "$(objdir)"
	$(cc) $(STD) $(CFLAGS) -c -o $(objdir)/$@ $(asmdir)/$<

	
# Generated Executable file by linking object files, Also uses GNU utilities size and objdump

build: $(o_files)
	$(cc) $(STD) $(MCFLAG) $(CFLAGS) $(objdir)/*.o -o project	
	size -d project
	objdump -a ./$(objdir)/*.o
	
# Generates dependancy and map files	

map_dep:
	$(cc) $(MCFLAG) -Wl,-Map=project.map $(objdir)/*.o --output project.map
	$(cc) $(CFLAGS) $(MCFLAG) -M ./source/*.c > project.dep

	

# Builds static library into the archive libproject1.a	
		
build-lib: libproject1.a
libproject1.a: data.c memory.c
	ar cr $@ source/*.c 


#	Cross compiles for BBB and copies executable into root directory of beagle bone

upload: clean build
	@scp $(BBB_PATH)
	

# Removes all the files generated by the build Such as .o,.s, .i,.map ,.dep etc
clean:
	rm -rf $(predir)
	rm -rf $(asmdir)
	rm -rf $(objdir)
	rm  -f project
	rm -f libproject1.a
	rm -f *.dep
	rm -f *.map
	rm -f *.o

