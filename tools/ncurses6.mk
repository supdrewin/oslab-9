NCURSES_CONFIG = $(shell command -v ncurses6-config)

ifeq ($(NCURSES_CONFIG),)
  NCURSES_CONFIG = $(shell command -v ncursesw6-config)
endif
