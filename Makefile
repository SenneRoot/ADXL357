#Credits to Nicholas Hamilton
#https://stackoverflow.com/questions/5178125/how-to-place-object-files-in-separate-subdirectory
#Credits to Scott McPeak
#http://scottmcpeak.com/autodepend/autodepend.html

#Compiler and Linker
CC	:= g++

#The Target Binary Program
TARGET	:= ADXL357_Logger

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR	:= src
INCDIR	:= include
OBJDIR	:= build
BINDIR	:= bin
SRCEXT	:= cpp
DEPEXT	:= d
OBJEXT	:= o

#Flags, Libraries and Includes
CFLAGS	:= -Wall -Wextra -Wno-psabi -Wno-missing-field-initializers -g -std=c++11
LIB	:= -lpthread -lrt -lfftw3
INCDEP	:= -I$(INCDIR)

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES	:= $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS	:= $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Defauilt Make
all: directories $(TARGET)

#Remake
remake: cleaner all

directories:
	@mkdir -p $(BINDIR)
	@mkdir -p $(OBJDIR)

#Clean only Objecst
clean:
	@$(RM) -rf $(OBJDIR)

#Full Clean, Objects and Binaries
cleaner: clean
	@$(RM) -rf $(BINDIR)

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Link
$(TARGET): $(OBJECTS)
	$(CC) -o $(BINDIR)/$(TARGET) $^ $(LIB)

#Compile
$(OBJDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCDEP) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(OBJDIR)/$*.$(DEPEXT)
	@cp -f $(OBJDIR)/$*.$(DEPEXT) $(OBJDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.$(OBJEXT):|' < $(OBJDIR)/$*.$(DEPEXT).tmp > $(OBJDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(OBJDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(OBJDIR)/$*.$(DEPEXT)
	@rm -f $(OBJDIR)/$*.$(DEPEXT).tmp

#Non-File Targets
.PHONY: all remake clean cleaner directories
