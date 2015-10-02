.PHONY: all clean run

EXE=level-editor
COMMON_OBJECTS=level.o pqueue.o pathfinder.o dwgraph.o gui.o common.o
OBJECTS=editor.o $(COMMON_OBJECTS)

TEST_EXE=level-editor-test
TEST_OBJECTS=min_unit.o test_editor.o $(COMMON_OBJECTS)

CC=gcc
STD="-std=c11"

all: $(EXE) $(TEST_EXE)

clean:
	-rm -f *.o *~ $(EXE) $(TEST_EXE)

ALLEGRO_PKG_LIBS="allegro-5.0 allegro_font-5.0 allegro_ttf-5.0 allegro_image-5.0 allegro_primitives-5.0 allegro_dialog-5.0"

%.o: %.c *.h
	$(CC) $(CFLAGS) `pkg-config --cflags $(ALLEGRO_PKG_LIBS)` -c -g -O0 ${STD} -Wall $< -o $@

$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ -g `pkg-config --libs --static $(ALLEGRO_PKG_LIBS)`

$(TEST_EXE): $(TEST_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@ -g `pkg-config --libs --static $(ALLEGRO_PKG_LIBS)`

run: $(EXE)
	./$(EXE)

test: $(TEST_EXE)
	./$(TEST_EXE)

