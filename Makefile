SHELL = /bin/bash

all: wait-for-signal

wait-for-signal: wait-for-signal.c

check: wait-for-signal
	./wait-for-signal & sleep 1; kill -SIGUSR1 $$!
	./wait-for-signal SIGUSR1 SIGUSR2 & sleep 1; kill -USR1 $$!
	./wait-for-signal SIGUSR1 SIGUSR2 & sleep 1; kill -USR2 $$!
	./wait-for-signal USR1 USR2 & sleep 1; kill -USR1 $$!
	./wait-for-signal USR1 USR2 & sleep 1; kill -USR2 $$!
	./wait-for-signal SIGINT & sleep 1; kill -SIGINT $$!
	./wait-for-signal INT & sleep 1; kill -SIGINT $$!
	./wait-for-signal 2 & sleep 1; kill -SIGINT $$!
	:
	./wait-for-signal & kill -SIGUSR1 $$!
	./wait-for-signal SIGUSR1 SIGUSR2 & kill -USR1 $$!
	./wait-for-signal SIGUSR1 SIGUSR2 & kill -USR2 $$!
	./wait-for-signal USR1 USR2 & kill -USR1 $$!
	./wait-for-signal USR1 USR2 & kill -USR2 $$!
	./wait-for-signal SIGINT & kill -SIGINT $$!
	./wait-for-signal INT & kill -SIGINT $$!
	./wait-for-signal 2 & kill -SIGINT $$!
	sleep 1
	! ps -efww | grep -q '[^\]\./wait-for'

install: wait-for-signal
	if systemctl is-active inhibit-suspend >/dev/null; then				\
		systemctl stop inhibit-suspend;						\
		install -m 0755 wait-for-signal /usr/bin;				\
		install -m 0644 inhibit-suspend.service /usr/lib/systemd/system;	\
		systemctl daemon-reload;						\
		systemctl start inhibit-suspend;					\
	else										\
		install -m 0755 wait-for-signal /usr/bin;				\
		install -m 0644 inhibit-suspend.service /usr/lib/systemd/system;	\
		systemctl daemon-reload;						\
	fi

clean:
	rm -f wait-for-signal
