SOURCES=main.c translator.c parser.c
EXECUTABLE=VMtranslator

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	gcc -o $(EXECUTABLE) $(SOURCES)

clean:
	rm -f $(EXECUTABLE) *.asm
