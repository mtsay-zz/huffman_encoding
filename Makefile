OUTPUT=encode decode
OBJECTS=common.o
HEADERS=common.h
INPUT=input.jpg
ENCODED=encoded
DECODED=decoded.jpg

default: $(OUTPUT)

help:
	echo "Target:"
	echo "  encode - builds the encoding binary."
	echo "  decode - builds the decoding binary."
	echo "  test - builds and tests both binaries using the sample input."
	echo "  clean - remove intermediate artifacts."
	echo "  distclean - remove all non-source artifacts."
	echo "  help - display this message."

encode: encode.o $(OBJECTS)
decode: decode.o $(OBJECTS)

enc: encode
	./encode < $(INPUT) > $(ENCODED)

dec: decode
	./decode < $(ENCODED) > $(DECODED)
	diff $(INPUT) $(DECODED)

test: enc dec

clean:
	-rm -rf *.o

distclean: clean
	-rm -rf $(OUTPUT) $(ENCODED) $(DECODED)

%.o: %.cc
	gcc -I $(HEADERS) -c $< -o $@

.PHONY: default enc dec test help clean distclean
