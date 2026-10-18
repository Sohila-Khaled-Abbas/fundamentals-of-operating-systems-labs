# ==============================================================================
# Fundamentals of Operating Systems - Root Makefile
# Instructor: Hussein Nasser
# ==============================================================================

SUBDIRS = labs/01-process-anatomy \
          labs/02-stack-memory \
          labs/03-data-segment \
          labs/04-heap-allocation \
          labs/05-allocation-benchmarks \
          labs/06-process-fork-cow \
          labs/07-socket-webserver \
          labs/08-cpu-cache-locality \
          labs/09-storage-page-cache

.PHONY: all clean test help $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	@echo "=========================================="
	@echo " Building in $@"
	@echo "=========================================="
	$(MAKE) -C $@

clean:
	@for dir in $(SUBDIRS); do \
		echo "Cleaning in $$dir"; \
		$(MAKE) -C $$dir clean; \
	done

test: all
	@echo "=========================================="
	@echo " Running safe benchmarks & tests...       "
	@echo "=========================================="
	$(MAKE) -C labs/05-allocation-benchmarks run
	$(MAKE) -C labs/08-cpu-cache-locality run
	$(MAKE) -C labs/09-storage-page-cache run

help:
	@echo "Available commands:"
	@echo "  make all    - Compile all lab executables and assembly"
	@echo "  make clean  - Remove all compiled binaries and temporary artifacts"
	@echo "  make test   - Run benchmarks (allocations, CPU cache, page cache)"
	@echo "  make help   - Display this help message"
