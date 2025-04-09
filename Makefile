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
	cd $(BIN_DIR) && rm *.o

get_master = $$( \
	if [ "$(m)" = "custom" ] || [ "$(m)" = "c" ]; then \
		echo "$(MASTER_CUSTOM)"; \
	else \
		if [ "$(a)" = "arm" ]; then \
			echo "$(MASTER_ORIGINAL_ARM)"; \
		else \
			echo "$(MASTER_ORIGINAL)"; \
		fi; \
	fi \
)

p ?= 1

run:
	@echo "Running with $(p) players..."; \
	make all && sleep 1 && $(call get_master) -d 1 -t 10 -v $(VISTA_O) -p $$(for i in $$(seq 1 $(p)); do echo -n "$(PLAYER10_O) "; done)

run_valgrind:
	@echo "Running with $(p) players..."; \
	make all && sleep 1 && valgrind --leak_check=yes $(call get_master) -d 1 -t 10 -v $(VISTA_O) -p $$(for i in $$(seq 1 $(p)); do echo -n "$(PLAYER10_O) "; done)


# Make testee todo con valgrind y pvs studio
