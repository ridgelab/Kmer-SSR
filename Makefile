# MAKEFILE

# ------ VARIABLES -------------------
CXX?=g++
CXXFLAGS+=-g -Wall -Wextra -std=c++11 -pthread -pedantic
LIBS?=
PREFIX=/usr/local/bin

# ------ TARGETS ---------------------
.PHONY: all prep permissions clean realclean install test
all: prep bin/sa-ssr permissions

clean:
	@rm -rf bin/* test/bin/* || true
	@if [ -e $(PREFIX)/sa-ssr -a -r $(PREFIX)/sa-ssr -a -w $(PREFIX)/sa-ssr ] ; then rm -f $(PREFIX)/sa-ssr || true; fi

realclean: clean
	@rm -rf bin obj test/bin || true

prep:
	@mkdir -p obj bin test/bin || true

obj/Arguments.o: src/Arguments.cpp include/Arguments.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/OutputFile.o: src/OutputFile.cpp include/OutputFile.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/AtomicityChecker.o: src/AtomicityChecker.cpp include/AtomicityChecker.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/FastaSequences.o: src/FastaSequences.cpp include/FastaSequences.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/SingleResult.o: src/SingleResult.cpp include/SingleResult.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/Results.o: src/Results.cpp include/Results.h include/SingleResult.h include/Arguments.h include/OutputFile.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/FindSSRs.o: src/FindSSRs.cpp include/FindSSRs.h include/Arguments.h include/Results.h include/SingleResult.h include/OutputFile.h include/FastaSequences.h lib/sais-lite-lcp/sais.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/main.o: src/main.cpp include/FindSSRs.h include/Arguments.h include/Results.h include/SingleResult.h include/OutputFile.h include/FastaSequences.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/ProgressMeter.o: src/ProgressMeter.cpp include/ProgressMeter.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

bin/sa-ssr: obj/main.o obj/OutputFile.o obj/FastaSequences.o obj/FindSSRs.o obj/Results.o obj/SingleResult.o obj/Arguments.o obj/ProgressMeter.o
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

permissions:
	@chmod 750 bin bin/sa-ssr test/bin || true
	@chmod 750 include src obj test test/src || true
	@chmod 640 include/* src/* obj/* example/* test/src/* || true

install:
	@if [ -e bin/sa-ssr ]; then cp bin/sa-ssr $(PREFIX)/sa-ssr || true; else echo "ERROR: \`bin/sa-ssr' does not exist. Did you forget to run \`make' first?"; fi

test: test/bin/atomicityChecker

test/bin/atomicityChecker: test/src/atomicityChecker.cpp obj/AtomicityChecker.o
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@
	@$@
