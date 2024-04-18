SECTIONS = \
	00-bootstrap \
	01-helloRVOS \
	02-memanagement \
	03-contextswitch \
	04-multitask \
	05-traps

.DEFAULT_GOAL := all

all :
	@for section in $(SECTIONS); do \
		echo "Building $$section"; \
		$(MAKE) -C $$section; \
	done

.PHONY :clean
clean :
	for dir in $(SECTIONS); do \
		$(MAKE) -C $$dir clean; \
	done