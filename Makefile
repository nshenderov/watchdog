CC = gcc
CFLAGS = $(IDIRS) -ansi -pedantic-errors -Wall -Wextra
CFLAGS_DBG = -g

MODULENAME = watchdog
RUNNERNAME = wd_runner
MODULENAMEDBG = $(MODULENAME)_dbg
RUNNERNAMEDBG = $(RUNNERNAME)_dbg

SRCDIR = ./src
INCDIR = ./include
DEPDIR = ./deps
TESTDIR = ./test
DEPSDIRS = $(shell find $(DEPDIR) -type d -print)
IDIRS = $(addprefix -I,$(DEPSDIRS)) -I./include
EXPORTDIR = ./export
EXPORTDIRDBG = $(EXPORTDIR)_dbg

DEPS = $(filter-out %_test.c, $(wildcard $(DEPDIR)/*/*.c))
DEPS_OBJS = $(patsubst %.c, %.o, $(DEPS))
DEPS_OBJS_DBG = $(patsubst %.c, %.o, $(DEPS))

# REL

exp: dirs lib$(MODULENAME)
	cp -n ./lib$(MODULENAME).so $(EXPORTDIR)/bin/lib$(MODULENAME).so
	cp -n $(INCDIR)/$(MODULENAME).h $(EXPORTDIR)/include/$(MODULENAME).h
	cp -n ./$(MODULENAME).out $(EXPORTDIR)/$(MODULENAME).out

lib$(MODULENAME): CFLAGS += -fPIC
lib$(MODULENAME): lib$(MODULENAME).so
lib$(MODULENAME): $(MODULENAME).out

$(MODULENAME).o: $(SRCDIR)/$(MODULENAME).c $(INCDIR)/$(MODULENAME).h
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/$(MODULENAME).c
lib$(MODULENAME).so : $(MODULENAME).o $(DEPS_OBJS)
	$(CC) $(CFLAGS) -shared -o $@ $(MODULENAME).o $(DEPS_OBJS)

$(MODULENAME).out: $(RUNNERNAME).o $(INCDIR)/$(MODULENAME).h
	$(CC) $(CFLAGS) -o $@ $(RUNNERNAME).o -L. -Wl,-rpath=. -Wl,-rpath=./bin -l$(MODULENAME)
$(RUNNERNAME).o: $(SRCDIR)/$(RUNNERNAME).c $(INCDIR)/$(MODULENAME).h
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/$(RUNNERNAME).c

dirs:
	@mkdir -p $(EXPORTDIR)/bin $(EXPORTDIR)/include

# DBG

exp_dbg: dirs_dbg lib$(MODULENAMEDBG)
	cp -n ./lib$(MODULENAMEDBG).so $(EXPORTDIRDBG)/bin/lib$(MODULENAMEDBG).so
	cp -n $(INCDIR)/$(MODULENAME).h $(EXPORTDIRDBG)/include/$(MODULENAME).h
	cp -n ./$(MODULENAMEDBG).out $(EXPORTDIRDBG)/$(MODULENAMEDBG).out

lib$(MODULENAMEDBG): CFLAGS += $(CFLAGS_DBG) -fPIC
lib$(MODULENAMEDBG): lib$(MODULENAMEDBG).so
lib$(MODULENAMEDBG): $(MODULENAMEDBG).out

$(MODULENAMEDBG).o: $(SRCDIR)/$(MODULENAME).c $(INCDIR)/$(MODULENAME).h
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/$(MODULENAME).c
lib$(MODULENAMEDBG).so : $(MODULENAMEDBG).o $(DEPS_OBJS_DBG)
	$(CC) $(CFLAGS) -shared -o $@ $(MODULENAMEDBG).o $(DEPS_OBJS_DBG)

$(MODULENAMEDBG).out: $(RUNNERNAMEDBG).o $(INCDIR)/$(MODULENAME).h
	$(CC) $(CFLAGS) -o $@ $(RUNNERNAMEDBG).o -L. -Wl,-rpath=. -Wl,-rpath=./bin -l$(MODULENAMEDBG)
$(RUNNERNAMEDBG).o: $(SRCDIR)/$(RUNNERNAME).c $(INCDIR)/$(MODULENAME).h
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)/$(RUNNERNAME).c

test: CFLAGS += $(CFLAGS_DBG)
test: exp_dbg $(MODULENAME)_test.out
	cp -n ./$(MODULENAME)_test.out $(EXPORTDIRDBG)/$(MODULENAME)_test.out

$(MODULENAME)_test.out: $(MODULENAME)_test.o $(INCDIR)/$(MODULENAME).h
	$(CC) $(CFLAGS) -o $@ $(MODULENAME)_test.o -L. -Wl,-rpath=. -Wl,-rpath=./bin -l$(MODULENAMEDBG)
$(MODULENAME)_test.o: $(TESTDIR)/$(MODULENAME)_test.c $(INCDIR)/$(MODULENAME).h
	$(CC) $(CFLAGS) -c -o $@ $(TESTDIR)/$(MODULENAME)_test.c

dirs_dbg:
	@mkdir -p $(EXPORTDIRDBG)/bin $(EXPORTDIRDBG)/include

c: clean
clean:
	rm -f ./*.out ./*.so ./*.o ./*/*.o ./*/*/*.o ./*/*/*.out
	rm -rf $(EXPORTDIR) $(EXPORTDIRDBG)

.PHONY: exp exp_dbg dirs dirs_dbg libwatchdog libwatchdog_dbg test c clean