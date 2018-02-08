SUBDIRS = reference student

.PHONY: subdirs $(SUBDIRS) mrproper mrproper2

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

make2: mrproper2
	$(MAKE) -C student -f Makefile2

mrproper:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir mrproper; \
	done

mrproper2:
	rm -f student/*.o calc
