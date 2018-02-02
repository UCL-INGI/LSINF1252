SUBDIRS = reference student

.PHONY: subdirs $(SUBDIRS) mrproper

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

mrproper:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir mrproper; \
	done
