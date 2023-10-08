all: wait-for-signal

wait-for-signal: wait-for-signal.c

install: wait-for-signal
	if systemctl is-active inhibit-suspend; then					\
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
