# MAKEFILE

# ------ VARIABLES -------------------
CXX?=g++
CXXFLAGS+=-g -Wall -Wextra -std=c++11 -pthread -pedantic
LIBS?=
PREFIX=/usr/local/bin

# ------ TARGETS ---------------------
.PHONY: all prep permissions clean realclean install test-setup test-prep test-permissions test supertest
all: prep bin/kmer-ssr permissions test-prep test-setup test-permissions

clean:
	@rm -rf bin/* test/bin/* || true
	@if [ -e $(PREFIX)/kmer-ssr -a -r $(PREFIX)/kmer-ssr -a -w $(PREFIX)/kmer-ssr ] ; then rm -f $(PREFIX)/kmer-ssr || true; fi

realclean: clean
	@rm -rf bin obj test/bin test/output/actual || true

prep:
	@mkdir -p obj bin || true

obj/Arguments.o: src/Arguments.cpp include/Arguments.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/OutputFile.o: src/OutputFile.cpp include/OutputFile.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/AtomicityChecker.o: src/AtomicityChecker.cpp include/AtomicityChecker.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/SSR.o: src/SSR.cpp include/SSR.h include/OutputFile.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/SSRcontainer.o: src/SSRcontainer.cpp include/SSRcontainer.h include/SSR.h include/OutputFile.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/Task.o: src/Task.cpp include/Task.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/TaskQueue.o: src/TaskQueue.cpp include/TaskQueue.h include/Task.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/ProgressMeter.o: src/ProgressMeter.cpp include/ProgressMeter.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/FindSSRs.o: src/FindSSRs.cpp include/FindSSRs.h include/Arguments.h include/SSR.h include/SSRcontainer.h include/OutputFile.h include/Task.h include/TaskQueue.h include/AtomicityChecker.h include/ProgressMeter.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/main.o: src/main.cpp include/FindSSRs.h include/Arguments.h include/SSR.h include/SSRcontainer.h include/OutputFile.h include/Task.h include/TaskQueue.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

bin/kmer-ssr: obj/main.o obj/OutputFile.o obj/SSR.o obj/SSRcontainer.o obj/AtomicityChecker.o obj/FindSSRs.o obj/Task.o obj/TaskQueue.o obj/Arguments.o obj/ProgressMeter.o
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

permissions:
	@chmod 750 bin bin/kmer-ssr || true
	@chmod 750 include src obj test test/src test/input test/output || true
	@chmod 640 include/* src/* obj/* example/* test/src/* || true
	@chmod 440 test/input/* test/output/expected/*

install:
	@if [ -e bin/kmer-ssr ]; then cp bin/kmer-ssr $(PREFIX)/kmer-ssr || true; else echo "ERROR: \`bin/kmer-ssr' does not exist. Did you forget to run \`make' first?"; fi

test-setup: test-prep test/bin/atomicityChecker test/bin/progressMeter test/bin/outputFile test/bin/ssr test/bin/ssrContainer test/bin/task test/bin/taskQueue test/bin/voidPointer test/bin/arguments test/bin/findSSRs test-permissions

test-prep:
	@mkdir -p obj test/bin test/output/actual || true

test/bin/atomicityChecker: test/src/atomicityChecker.cpp obj/AtomicityChecker.o
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

test/bin/progressMeter: test/src/progressMeter.cpp obj/ProgressMeter.o
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

test/bin/outputFile: test/src/outputFile.cpp obj/OutputFile.o
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

test/bin/ssr: test/src/ssr.cpp obj/SSR.o obj/OutputFile.o
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

test/bin/ssrContainer: test/src/ssrContainer.cpp obj/SSRcontainer.o obj/SSR.o obj/OutputFile.o
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

test/bin/task: test/src/task.cpp obj/Task.o
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

test/bin/taskQueue: test/src/taskQueue.cpp obj/TaskQueue.o obj/Task.o
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

test/bin/voidPointer: test/src/voidPointer.cpp
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

test/bin/arguments: test/src/arguments.cpp obj/Arguments.o
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

test/bin/findSSRs: test/src/findSSRs.cpp obj/FindSSRs.o obj/Arguments.o obj/TaskQueue.o obj/Task.o obj/SSRcontainer.o obj/SSR.o obj/ProgressMeter.o obj/AtomicityChecker.o obj/OutputFile.o
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

test-permissions:
	@chmod 750 test/bin || true
	@chmod 750 include src obj test test/src test/input test/output || true
	@chmod 770 test/output/actual || true
	@chmod 640 include/* src/* obj/* example/* test/src/* || true
	@chmod 440 test/input/* test/output/expected/*

test:
	@test/bin/atomicityChecker
	@test/bin/progressMeter
	@test/bin/outputFile
	@test/bin/ssr
	@test/bin/ssrContainer
	@test/bin/task
	@test/bin/taskQueue
	@test/bin/voidPointer
	@test/bin/arguments
	@test/bin/findSSRs
	@printf "\n"

supertest:
	@valgrind --leak-check=full test/bin/atomicityChecker
	@valgrind --leak-check=full test/bin/progressMeter
	@valgrind --leak-check=full test/bin/outputFile
	@valgrind --leak-check=full test/bin/ssr
	@valgrind --leak-check=full test/bin/ssrContainer
	@valgrind --leak-check=full test/bin/task
	@valgrind --leak-check=full test/bin/taskQueue
	@valgrind --leak-check=full test/bin/voidPointer
	@valgrind --leak-check=full test/bin/arguments
	@valgrind --leak-check=full test/bin/findSSRs
	@printf "\n"
