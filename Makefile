CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference -g3 -O0

SRCDIR=./

TESTFOLDER=$(SRCDIR)tests
ADD01=$(TESTFOLDER)/add01
ADD02=$(TESTFOLDER)/add02
CMP80=$(TESTFOLDER)/cmp80
JG95=$(TESTFOLDER)/jg95
JL93=$(TESTFOLDER)/jl93
JMP91=$(TESTFOLDER)/jmp91
MOV1A=$(TESTFOLDER)/mov1a
MOV1B=$(TESTFOLDER)/mov1b
MOV1C=$(TESTFOLDER)/mov1c

ADD01PARAMS=-i $(ADD01)/input.txt -o $(ADD01)/output.txt -t $(ADD01)/memory.csv -v $(ADD01)/verify.txt -q
ADD02PARAMS=-i $(ADD02)/input.txt -o $(ADD02)/output.txt -t $(ADD02)/memory.csv -v $(ADD02)/verify.txt -q
CMP80PARAMS=-i $(CMP80)/input.txt -o $(CMP80)/output.txt -t $(CMP80)/memory.csv -v $(CMP80)/verify.txt -q
JG95PARAMS=-i $(JG95)/input.txt -o $(JG95)/output.txt -t $(JG95)/memory.csv -v $(JG95)/verify.txt -q
JL93PARAMS=-i $(JL93)/input.txt -o $(JL93)/output.txt -t $(JL93)/memory.csv -v $(JL93)/verify.txt -q
JMP91PARAMS=-i $(JMP91)/input.txt -o $(JMP91)/output.txt -t $(JMP91)/memory.csv -v $(JMP91)/verify.txt -q
MOV1APARAMS=-i $(MOV1A)/input.txt -o $(MOV1A)/output.txt -t $(MOV1A)/memory.csv -v $(MOV1A)/verify.txt -q
MOV1BPARAMS=-i $(MOV1B)/input.txt -o $(MOV1B)/output.txt -t $(MOV1B)/memory.csv -v $(MOV1B)/verify.txt -q
MOV1CPARAMS=-i $(MOV1C)/input.txt -o $(MOV1C)/output.txt -t $(MOV1C)/memory.csv -v $(MOV1C)/verify.txt -q

SRC=$(wildcard $(SRCDIR)*.c)

OBJ=$(SRC:.c=.o)

EXECUTABLE=outDebug.exe

.PHONY: all run clean

all: run

run: build
	$(EXECUTABLE) 

test: build testadd01 testadd02 testcmp80 testjg95 testjl93 testjmp91 testmov1a testmov1b testmov1c

testadd01: 
	$(EXECUTABLE)  $(ADD01PARAMS)

testadd02:
	$(EXECUTABLE)  $(ADD02PARAMS)

testcmp80:
	$(EXECUTABLE)  $(CMP80PARAMS)

testjg95:
	$(EXECUTABLE)  $(JG95PARAMS)

testjl93:
	$(EXECUTABLE)  $(JL93PARAMS)

testjmp91:
	$(EXECUTABLE)  $(JMP91PARAMS)

testmov1a:
	$(EXECUTABLE)  $(MOV1APARAMS)

testmov1b:
	$(EXECUTABLE)  $(MOV1BPARAMS)

testmov1c:
	$(EXECUTABLE)  $(MOV1CPARAMS)

build: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)

cleanwin:
	del $(OBJ) $(EXECUTABLE)