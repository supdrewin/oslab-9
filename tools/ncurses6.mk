NCURSES_CONFIG = $(shell which ncurses6-config)

ifeq ($(NCURSES_CONFIG),)
  NCURSES_CONFIG = $(shell which ncursesw6-config)
endif
