include Makefile.inc

.PHONY: clean

all: vista player10 master

vista:
	cd $(VISTA_DIR) && make all

player10:
	cd $(PLAYER10_DIR) && make all

master:
	cd $(MASTER_DIR) && make all

clean:
	cd $(BIN_DIR) && rm -f *.o

get_master = $$( \
	if [ "$(m)" = "original" ] || [ "$(m)" = "o" ]; then \
		if [ "$(a)" = "arm" ]; then \
			echo "$(MASTER_ORIGINAL_ARM)"; \
		else \
			echo "$(MASTER_ORIGINAL)"; \
		fi; \
	else \
		echo "$(MASTER_CUSTOM)"; \
	fi \
)

w ?= 10
h ?= 10
d ?= 200
t ?= 10
p ?= 1

run:
	@echo "Running with $(p) players..."; \
	make all && sleep 1 && $(call get_master) -d $(d) -t $(t) -w $(w) -h $(h)  -v $(VISTA_O) -p $$(for i in $$(seq 1 $(p)); do echo -n "$(PLAYER10_O) "; done)

run_valgrind:
	@echo "Running with $(p) players..."; \
	make all && sleep 1 && valgrind $(call get_master) -d $(d) -t $(t) -v $(VISTA_O) -p $$(for i in $$(seq 1 $(p)); do echo -n "$(PLAYER10_O) "; done)


# Make testee todo con valgrind y pvs studio

pvs: $(CREDENTIALS)
	make clean
	pvs-studio-analyzer trace -- make
	pvs-studio-analyzer analyze
	plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o report.tasks PVS-Studio.log

cpp-check:
	cppcheck --quiet --enable=all --force --inconclusive --suppress=missingIncludeSystem .
