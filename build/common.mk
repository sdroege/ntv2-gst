#
# Copyright (C) 2004 - 2018 AJA Video Systems, Inc.
# Proprietary and Confidential information.
# All righs reserved
#

CXX ?= g++
CPP := $(CXX)

ifeq ($(AJA_USE_CCACHE),1)
	CXX := ccache $(CXX)
	CPP := ccache $(CPP)
	CC := ccache $(CC)
endif


CPPFLAGS += -DAJALinux -DAJA_LINUX -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 \
			-pedantic -Wall -Wno-long-long  -Wwrite-strings -c -pipe -fPIC $(DBG)

LD = $(CXX)
LDFLAGS = 

VPATH		:= $(VPATH):$(A_LIB_NTV2_INC):$(A_LIB_NTV2_SRC):$(A_LIB_NTV2_SRC_LINUX):$(A_UBER_LIB):$(SRCDIR)
INCLUDES	:= $(INCLUDES) -I$(A_LIB_NTV2_INC) -I$(A_LIB_NTV2_SRC) -I$(A_LIB_NTV2_SRC_LINUX) -I$(A_LIBRARIES_PATH) -I$(A_LIB_BASE_PATH)
LIBDIRS		:= $(LIBDIRS) -L$(A_UBER_LIB)


ifeq ($(AJA_DEBUG),1)
LIB_AJANTV2_FILENAME = ajantv2d
else
LIB_AJANTV2_FILENAME = ajantv2
endif

LIB_AJANTV2 = $(A_UBER_LIB)/lib$(LIB_AJANTV2_FILENAME).a

ifdef AJA_LIB_PATH
BUILD_AND_LINK_AGAINST_AJALIBS = 0
else
BUILD_AND_LINK_AGAINST_AJALIBS = 1
endif

ifeq ($(BUILD_AND_LINK_AGAINST_AJALIBS),1)
LIBS    := $(LIBS) -l$(LIB_AJANTV2_FILENAME) -lpthread -lrt
else
LIBS	:= $(LIBS) -lpthread -lrt
endif

OBJS = $(patsubst %.cpp,%.o,$(SRCS))
ifdef C_SRCS
OBJS += $(patsubst %.c,%.o,$(C_SRCS))
endif

ifdef SRCS2
  OBJS2 = $(patsubst %.cpp,%.o,$(SRCS2))
endif
ifdef C_SRCS2
  OBJS2 += $(patsubst %.c,%.o,$(C_SRCS2))
endif

ifdef SRCS3
  OBJS3 = $(patsubst %.cpp,%.o,$(SRCS3))
endif
ifdef C_SRCS3
  OBJS3 += $(patsubst %.c,%.o,$(C_SRCS3))
endif

ifdef SRCS4
  OBJS4 = $(patsubst %.cpp,%.o,$(SRCS4))
endif
ifdef C_SRCS4
  OBJS4 += $(patsubst %.c,%.o,$(C_SRCS4))
endif

ifdef AJA_LIB_SRCS
LIBOBJS = $(patsubst %.cpp,%.o,$(AJA_LIB_SRCS))
ifdef AJA_LIB_C_SRCS
LIBOBJS += $(patsubst %.c,%.o,$(AJA_LIB_C_SRCS))
endif
endif

%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(INCLUDES) -o $@ $<
	@$(CPP) -MM $(CPPFLAGS) $(INCLUDES) -MF $*.d $<

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDES) -o $@ $<
	@$(CC) -MM $(CFLAGS) $(INCLUDES) -MF $*.d $<

ifeq ($(BUILD_AND_LINK_AGAINST_AJALIBS),1)
all: $(LIB_AJANTV2) $(AJA_APP) $(AJA_APP2) $(AJA_APP3) $(AJA_APP4)
else
all: $(AJA_APP) $(AJA_APP2) $(AJA_APP3) $(AJA_APP4) $(AJA_LIB_PATH)
endif

-include $(patsubst %.cpp,%.d,$(SRCS))

ifdef SRCS2
  -include $(patsubst %.cpp,%.d,$(SRCS2))
endif

ifdef SRCS3
  -include $(patsubst %.cpp,%.d,$(SRCS3))
endif

ifdef SRCS4
  -include $(patsubst %.cpp,%.d,$(SRCS4))
endif

ifdef AJA_LIB_SRCS
  -include $(patsubst %.cpp,%.d,$(AJA_LIB_SRCS))
endif

ifeq ($(BUILD_AND_LINK_AGAINST_AJALIBS),1)
.PHONY : $(LIB_AJANTV2)
$(LIB_AJANTV2): $(SDK_SRCS)
	$(MAKE) -C $(A_LIB_NTV2_PATH)/build
endif

$(AJA_APP): $(OBJS) 
	$(LD) $(LDFLAGS) $(LIBDIRS) $(OBJS) -o $(AJA_APP) $(LIBS)

ifdef AJA_APP2
  $(AJA_APP2): $(OBJS2)
	  $(LD) $(LDFLAGS) $(LIBDIRS) $(OBJS2) -o $(AJA_APP2) $(LIBS)
endif

ifdef AJA_APP3
  $(AJA_APP3): $(OBJS3)
	  $(LD) $(LDFLAGS) $(LIBDIRS) $(OBJS3) -o $(AJA_APP3) $(LIBS)
endif

ifdef AJA_APP4
  $(AJA_APP4): $(OBJS4)
	  $(LD) $(LDFLAGS) $(LIBDIRS) $(OBJS4) -o $(AJA_APP4) $(LIBS)
endif

ifdef AJA_LIB_PATH
.PHONY : $(AJA_LIB_PATH)
$(AJA_LIB_PATH): $(LIBOBJS)
	+@[ -d $(A_UBER_LIB) ] || mkdir -p $(A_UBER_LIB)
	$(A_LIBCMD) $@ $(LIBOBJS)
endif

.PHONY: cleandeps clean realclean

realclean: clean cleandeps

clean:
	@echo "clean: removing .o and .d files"
	@rm -f *.o *.d *~ errors.txt
ifdef AJA_APP
	@[ -f $(AJA_APP) ] && rm -f $(AJA_APP) && echo "clean: removed $(AJA_APP)" || true
endif
ifdef AJA_APP2
	@[ -f $(AJA_APP2) ] && rm -f $(AJA_APP2) || true
endif
ifdef AJA_APP3
	@[ -f $(AJA_APP3) ] && rm -f $(AJA_APP3) || true
endif
ifdef AJA_APP4
	@[ -f $(AJA_APP4) ] && rm -f $(AJA_APP4) || true
endif
ifdef AJA_LIB_PATH
	@[ -f $(AJA_LIB_PATH) ] && rm -f $(AJA_LIB_PATH) || true
endif
ifeq ($(BUILD_AND_LINK_AGAINST_AJALIBS),1)
	@[ -f $(LIB_AJANTV2) ] && rm -f $(LIB_AJANTV2) || true
	@rm -f $(A_LIB_NTV2_PATH)/build/$(OBJDIR)/*.o
	@rm -f $(A_LIB_NTV2_PATH)/build/$(OBJDIR)/*.d
endif

cleandeps:
	@echo "cleandeps: removing .d files"
	@rm -f *.d
