ifeq (, $(shell which jemalloc-config))
JEMALLOC =
else
JEMALLOCLD = $(shell jemalloc-config --libdir)
JEMALLOC = -L$(JEMALLOCLD) -ljemalloc
endif

OPT = -O3 -g

CONCEPTS = -fconcepts -DCONCEPTS
#CFLAGS = -DEDGELONG -mcx16 $(OPT) -ldl -std=c++17 -march=native -Wall -Wno-subobject-linkage -DUSEMALLOC -DNDEBUG
CFLAGS = -DEDGELONG -mcx16 $(OPT) -ldl -std=c++17 -march=native -Wno-subobject-linkage -DUSEMALLOC -DNDEBUG

OMPFLAGS = -DOPENMP -fopenmp
CILKFLAGS = -DCILK -fcilkplus
HGFLAGS = -DHOMEGROWN -pthread

ifdef CLANG
CC = clang++
PFLAGS = $(CILKFLAGS)
else ifdef CILK
CC = g++
PFLAGS = $(CILKFLAGS)
else ifdef OPENMP
CC = g++
PFLAGS = $(OMPFLAGS)
else ifdef HOMEGROWN
CC = g++
PFLAGS = $(HGFLAGS)
else ifdef SERIAL
CC = g++
PFLAGS =
else # default is homegrown
CC = g++
PFLAGS = $(HGFLAGS)
endif

memory_footprint:  tools/memory_footprint.cpp
	$(CC) $(CFLAGS) $(PFLAGS) tools/memory_footprint.cpp -o memory_footprint

run_static_algorithm:  tools/run_static_algorithm.cpp
	$(CC) $(CFLAGS) $(PFLAGS) tools/run_static_algorithm.cpp -o run_static_algorithm

run_batch_updates:  tools/run_batch_updates.cpp
	$(CC) $(CFLAGS) $(PFLAGS) tools/run_batch_updates.cpp -o run_batch_updates

run_kv_batch_updates:  tools/run_kv_batch_updates.cpp
	$(CC) $(CFLAGS) $(PFLAGS) tools/run_kv_batch_updates.cpp -o run_kv_batch_updates

run_kv_insert: tools/run_kv_insert.cpp
    $(CC) $(CFLAGS) $(PFLAGS) tools/run_kv_insert.cpp -o run_kv_insert

run_kv_insert_btree: tools/run_kv_insert_btree.cpp
    $(CC) $(CFLAGS) $(PFLAGS) tools/run_kv_insert_btree.cpp -o run_kv_insert_btree

run_simultaneous_updates_queries:  tools/run_simultaneous_updates_queries.cpp
	$(CC) $(CFLAGS) $(PFLAGS) tools/run_simultaneous_updates_queries.cpp -o run_simultaneous_updates_queries


ALL= memory_footprint run_static_algorithm run_batch_updates run_kv_batch_updates run_simultaneous_updates_queries run_kv_insert run_kv_insert_btree
all: $(ALL)

% : tools/%.cpp
	$(CC) $(CFLAGS) $(PFLAGS) -o $@ $< $(JEMALLOC)


.PHONY : clean

clean:
	rm -f *.o $(ALL)
