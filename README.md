![Build Status](https://github.com/revolution-robotics/inhibit-suspend/actions/workflows/inhibit-suspend.yml/badge.svg)

# Inhibit Suspend

- [GDM suspend](#gdm-suspend)
- [Systemd-inhibit](#systemd-inhibit)
- [inhibit-suspend.service](#inhibit-suspendservice)
- [Installation](#installation)

## GDM suspend

Recent versions of Fedora running GNOME desktop unexpectedly suspend.
For systems that also happen to be servers, this is a problem. The
apparent solution is to add a `gdm` profile, as outlined on
StackExchange
[Disable GDM suspend on lock screen](https://unix.stackexchange.com/a/746767).
Such a `gdm` profile works great until `gdm` itself crashes
and the system again suspends.

## Systemd-inhibit

Systemd provides a utility for blocking suspend: `systemd-inhibit`.
Similar to `nohup`, it's intent is to prevent interruption of
a long-running process.

## inhibit-suspend.service

This package provides a systemd service, *inhibit-suspend.service*,
that invokes `wait-for-signal`, which simply sleeps until it receives
a designated signal (SIGUSR1, by default). By invoking
`wait-for-signal` via `systemd-inhibit`, this is enough to prevent a
slightly unstable desktop from suspending the system. *Hurrah!*

## Installation

Provided your **systemd**-based OS has **GNU make** and a C compiler, run:

```bash
git clone https://github.com/revolution-robotics/inhibit-suspend.git
make -C inhibit-suspend
sudo make -C inhibit-suspend install
sudo systemctl enable --now inhibit-suspsend
```
