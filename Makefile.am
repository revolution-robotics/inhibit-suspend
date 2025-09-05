# Makefile.am: Automake template for the inhibit-suspend service.
#
# Process this file with automake to create Makefile.in
SHELL = /bin/bash

bin_PROGRAMS = wait-for-signal
wait_for_signal_SOURCES = wait-for-signal.c

bin_SCRIPTS = restart

AM_DEFAULT_SOURCE_EXT = .in

USER := $(shell id -un)

check-local: wait-for-signal
	$(builddir)/wait-for-signal & $(SLEEP_CMD) 1; $(KILL_CMD) -SIGUSR1 $$!
	$(builddir)/wait-for-signal SIGUSR1 SIGUSR2 & $(SLEEP_CMD) 1; $(KILL_CMD) -USR1 $$!
	$(builddir)/wait-for-signal SIGUSR1 SIGUSR2 & $(SLEEP_CMD) 1; $(KILL_CMD) -USR2 $$!
	$(builddir)/wait-for-signal USR1 USR2 & $(SLEEP_CMD) 1; $(KILL_CMD) -USR1 $$!
	$(builddir)/wait-for-signal USR1 USR2 & $(SLEEP_CMD) 1; $(KILL_CMD) -USR2 $$!
	$(builddir)/wait-for-signal SIGINT & $(SLEEP_CMD) 1; $(KILL_CMD) -SIGINT $$!
	$(builddir)/wait-for-signal INT & $(SLEEP_CMD) 1; $(KILL_CMD) -SIGINT $$!
	$(builddir)/wait-for-signal 2 & $(SLEEP_CMD) 1; $(KILL_CMD) -SIGINT $$!
	:
	$(builddir)/wait-for-signal & $(KILL_CMD) -SIGUSR1 $$!
	$(builddir)/wait-for-signal SIGUSR1 SIGUSR2 & $(KILL_CMD) -USR1 $$!
	$(builddir)/wait-for-signal SIGUSR1 SIGUSR2 & $(KILL_CMD) -USR2 $$!
	$(builddir)/wait-for-signal USR1 USR2 & $(KILL_CMD) -USR1 $$!
	$(builddir)/wait-for-signal USR1 USR2 & $(KILL_CMD) -USR2 $$!
	$(builddir)/wait-for-signal SIGINT & $(KILL_CMD) -SIGINT $$!
	$(builddir)/wait-for-signal INT & $(KILL_CMD) -SIGINT $$!
	$(builddir)/wait-for-signal 2 & $(KILL_CMD) -SIGINT $$!
	$(SLEEP_CMD) 1
	$(PGREP_CMD) -U $(USER) wait-for-signal || exit 0

install: wait-for-signal restart
	if $(SYSTEMCTL_CMD) is-active inhibit-suspend >/dev/null; then				\
		$(SYSTEMCTL_CMD) stop inhibit-suspend;						\
		$(INSTALL_CMD) -m 0755 wait-for-signal $(bindir);				\
		$(INSTALL_CMD) -m 0644 inhibit-suspend.service $(libdir)/systemd/system;	\
		$(SYSTEMCTL_CMD) daemon-reload;							\
		$(SYSTEMCTL_CMD) start inhibit-suspend;						\
	else											\
		$(INSTALL_CMD) -m 0755 wait-for-signal $(bindir);				\
		$(INSTALL_CMD) -m 0644 inhibit-suspend.service $(libdir)/systemd/system;	\
		$(SYSTEMCTL_CMD) daemon-reload;							\
	fi
	$(INSTALL_CMD) -m 0755 restart $(bindir)
