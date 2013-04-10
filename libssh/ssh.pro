QT -= gui
TEMPLATE = lib
CONFIG += staticlib
DEFINES += WITH_SFTP HAVE_POLL HAVE_SELECT HAVE_LIBCRYPTO HAVE_PTHREAD WITH_ZLIB HAVE_GETADDRINFO
TARGET = ../ssh

SOURCES = agent.c buffer.c config.c error.c kex.c libgcrypt.c options.c pcap.c scp.c socket.c auth.c callbacks.c connect.c gcrypt_missing.c kex1.c log.c packet.c pki.c server.c string.c auth1.c channels.c crc32.c getpass.c known_hosts.c match.c packet1.c pki_crypto.c session.c threads.c base64.c channels1.c dh.c gzip.c legacy.c messages.c packet_cb.c pki_gcrypt.c sftp.c wrapper.c bind.c client.c ecdh.c init.c libcrypto.c misc.c packet_crypt.c poll.c sftpserver.c

HEADERS = libssh/agent.h libssh/callbacks.h libssh/dh.h libssh/legacy.h libssh/messages.h libssh/pcap.h libssh/priv.h libssh/sftp.h libssh/string.h libssh/auth.h libssh/channels.h libssh/ecdh.h libssh/libcrypto.h libssh/misc.h libssh/pki.h libssh/scp.h libssh/socket.h libssh/threads.h libssh/bind.h libssh/crc32.h libssh/kex.h libssh/libgcrypt.h libssh/options.h libssh/pki_priv.h libssh/server.h libssh/ssh1.h libssh/wrapper.h libssh/buffer.h libssh/crypto.h libssh/keys.h libssh/libssh.h libssh/packet.h libssh/poll.h libssh/session.h libssh/ssh2.h config.h
