# NOTE: this is a GNU Makefile.  You must use "gmake" rather than "make".
#
# Makefile for the threads assignment.  The threads assignment must
#  be done first!
#
# Copyright (c) 1992 The Regents of the University of California.
# All rights reserved.  See copyright.h for copyright notice and limitation 
# of liability and disclaimer of warranty provisions.

DEFINES = -DTHREADS
INCPATH = -I../threads -I../machine
HFILES = $(THREAD_H)
CFILES = $(THREAD_C)
C_OFILES = $(THREAD_O)

include ../Makefile.common
include ../Makefile.dep
#-----------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend uses it
# DEPENDENCIES MUST END AT END OF FILE
main.o: ../threads/main.cc ../threads/copyright.h ../threads/utility.h \
 ../threads/bool.h ../machine/sysdep.h ../threads/copyright.h \
 ../threads/system.h ../threads/thread.h ../threads/scheduler.h \
 ../threads/list.h ../machine/interrupt.h ../threads/list.h \
 ../machine/stats.h ../machine/timer.h ../threads/utility.h \
 ../threads/Alarm.h ../threads/hello.h ../threads/../dllist/dllist.h
list.o: ../threads/list.cc ../threads/copyright.h ../threads/list.h \
 ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
 ../threads/copyright.h
scheduler.o: ../threads/scheduler.cc ../threads/copyright.h \
 ../threads/scheduler.h ../threads/list.h ../threads/utility.h \
 ../threads/bool.h ../machine/sysdep.h ../threads/copyright.h \
 ../threads/thread.h ../threads/system.h ../machine/interrupt.h \
 ../threads/list.h ../machine/stats.h ../machine/timer.h \
 ../threads/utility.h ../threads/Alarm.h
synch.o: ../threads/synch.cc ../threads/copyright.h ../threads/synch.h \
 ../threads/thread.h ../threads/utility.h ../threads/bool.h \
 ../machine/sysdep.h ../threads/copyright.h ../threads/list.h \
 ../threads/system.h ../threads/scheduler.h ../machine/interrupt.h \
 ../threads/list.h ../machine/stats.h ../machine/timer.h \
 ../threads/utility.h ../threads/Alarm.h
synchlist.o: ../threads/synchlist.cc ../threads/copyright.h \
 ../threads/synchlist.h ../threads/list.h ../threads/utility.h \
 ../threads/bool.h ../machine/sysdep.h ../threads/copyright.h \
 ../threads/synch.h ../threads/thread.h
system.o: ../threads/system.cc ../threads/copyright.h ../threads/system.h \
 ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
 ../threads/copyright.h ../threads/thread.h ../threads/scheduler.h \
 ../threads/list.h ../machine/interrupt.h ../threads/list.h \
 ../machine/stats.h ../machine/timer.h ../threads/utility.h \
 ../threads/Alarm.h
thread.o: ../threads/thread.cc ../threads/copyright.h ../threads/thread.h \
 ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
 ../threads/copyright.h ../threads/switch.h ../threads/synch.h \
 ../threads/list.h ../threads/system.h ../threads/scheduler.h \
 ../machine/interrupt.h ../threads/list.h ../machine/stats.h \
 ../machine/timer.h ../threads/utility.h ../threads/Alarm.h
utility.o: ../threads/utility.cc ../threads/copyright.h \
 ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
 ../threads/copyright.h ../threads/stdarg.h
threadtest.o: ../threads/threadtest.cc ../threads/copyright.h \
 ../threads/system.h ../threads/utility.h ../threads/bool.h \
 ../machine/sysdep.h ../threads/copyright.h ../threads/thread.h \
 ../threads/scheduler.h ../threads/list.h ../machine/interrupt.h \
 ../threads/list.h ../machine/stats.h ../machine/timer.h \
 ../threads/utility.h ../threads/Alarm.h ../threads/dllist.h \
 ../threads/synch.h ../threads/Table.h ../threads/BoundedBuffer.h \
 ../threads/EventBarrier.h
interrupt.o: ../machine/interrupt.cc ../threads/copyright.h \
 ../machine/interrupt.h ../threads/list.h ../threads/copyright.h \
 ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
 ../threads/system.h ../threads/thread.h ../threads/scheduler.h \
 ../threads/list.h ../machine/interrupt.h ../machine/stats.h \
 ../machine/timer.h ../threads/utility.h ../threads/Alarm.h
sysdep.o: ../machine/sysdep.cc ../threads/copyright.h \
 ../machine/interrupt.h ../threads/list.h ../threads/copyright.h \
 ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
 ../threads/system.h ../threads/thread.h ../threads/scheduler.h \
 ../threads/list.h ../machine/interrupt.h ../machine/stats.h \
 ../machine/timer.h ../threads/utility.h ../threads/Alarm.h
stats.o: ../machine/stats.cc ../threads/copyright.h ../threads/utility.h \
 ../threads/copyright.h ../threads/bool.h ../machine/sysdep.h \
 ../machine/stats.h
timer.o: ../machine/timer.cc ../threads/copyright.h ../machine/timer.h \
 ../threads/utility.h ../threads/copyright.h ../threads/bool.h \
 ../machine/sysdep.h ../threads/system.h ../threads/utility.h \
 ../threads/thread.h ../threads/scheduler.h ../threads/list.h \
 ../machine/interrupt.h ../threads/list.h ../machine/stats.h \
 ../machine/timer.h ../threads/Alarm.h
hello.o: ../threads/hello.c ../threads/hello.h
Table.o: ../threads/Table.cc ../threads/Table.h ../threads/synch.h \
 ../threads/copyright.h ../threads/thread.h ../threads/utility.h \
 ../threads/bool.h ../machine/sysdep.h ../threads/copyright.h \
 ../threads/list.h
BoundedBuffer.o: ../threads/BoundedBuffer.cc ../threads/BoundedBuffer.h \
 ../threads/synch.h ../threads/copyright.h ../threads/thread.h \
 ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
 ../threads/copyright.h ../threads/list.h
dllist.o: ../threads/dllist.cc ../threads/dllist.h ../threads/synch.h \
 ../threads/copyright.h ../threads/thread.h ../threads/utility.h \
 ../threads/bool.h ../machine/sysdep.h ../threads/copyright.h \
 ../threads/list.h
dllist-driver.o: ../threads/dllist-driver.cc ../threads/dllist.h \
 ../threads/synch.h ../threads/copyright.h ../threads/thread.h \
 ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
 ../threads/copyright.h ../threads/list.h
EventBarrier.o: ../threads/EventBarrier.cc ../threads/EventBarrier.h \
 ../threads/synch.h ../threads/copyright.h ../threads/thread.h \
 ../threads/utility.h ../threads/bool.h ../machine/sysdep.h \
 ../threads/copyright.h ../threads/list.h ../threads/system.h \
 ../threads/scheduler.h ../machine/interrupt.h ../threads/list.h \
 ../machine/stats.h ../machine/timer.h ../threads/utility.h \
 ../threads/Alarm.h
Alarm.o: ../threads/Alarm.cc ../threads/Alarm.h ../threads/list.h \
 ../threads/copyright.h ../threads/utility.h ../threads/bool.h \
 ../machine/sysdep.h ../threads/copyright.h ../threads/thread.h \
 ../threads/system.h ../threads/scheduler.h ../machine/interrupt.h \
 ../threads/list.h ../machine/stats.h ../machine/timer.h \
 ../threads/utility.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
