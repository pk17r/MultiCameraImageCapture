-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: v4l-utils
Binary: libv4l-0, libv4lconvert0, libv4l2rds0, libv4l-dev, v4l-utils, libdvbv5-0, libdvbv5-dev, libdvbv5-doc, dvb-tools, qv4l2, ir-keytable
Architecture: linux-any kfreebsd-any all
Version: 1.10.0-1
Maintainer: Gregor Jasny <gjasny@googlemail.com>
Uploaders: Martin Pitt <mpitt@debian.org>, Loic Minier <lool@dooz.org>
Homepage: http://linuxtv.org/downloads/v4l-utils/
Standards-Version: 3.9.6
Vcs-Browser: http://anonscm.debian.org/gitweb/?p=collab-maint/libv4l.git
Vcs-Git: git://anonscm.debian.org/collab-maint/libv4l.git
Build-Depends: debhelper (>= 8.1.3), dh-autoreconf, autotools-dev, doxygen, graphviz, libasound2-dev, libtool, libjpeg-dev, libqt4-dev, libqt4-opengl-dev, libudev-dev [linux-any], libx11-dev, pkg-config, udev [linux-any]
Package-List:
 dvb-tools deb utils optional arch=linux-any
 ir-keytable deb utils optional arch=linux-any
 libdvbv5-0 deb libs optional arch=linux-any
 libdvbv5-dev deb libdevel optional arch=linux-any
 libdvbv5-doc deb doc optional arch=all
 libv4l-0 deb libs optional arch=linux-any,kfreebsd-any
 libv4l-dev deb libdevel optional arch=linux-any,kfreebsd-any
 libv4l2rds0 deb libs optional arch=linux-any,kfreebsd-any
 libv4lconvert0 deb libs optional arch=linux-any,kfreebsd-any
 qv4l2 deb utils optional arch=linux-any
 v4l-utils deb utils optional arch=linux-any
Checksums-Sha1:
 79b5d9f42e5926729e93fb7064dc46d3b862b13e 1335798 v4l-utils_1.10.0.orig.tar.bz2
 353b4578b22aa5d5dbaefcc6636dc1b87cad1d7b 20172 v4l-utils_1.10.0-1.debian.tar.xz
Checksums-Sha256:
 78ead27ee58a701d7c6342303cf4520bdd4a2b88a7813bc99a0b389307e4336b 1335798 v4l-utils_1.10.0.orig.tar.bz2
 c74f692365018d37040797d552b4288eaa898441bc8750961c67d47e176b15a1 20172 v4l-utils_1.10.0-1.debian.tar.xz
Files:
 945a38979138997e805828a4a53e53ec 1335798 v4l-utils_1.10.0.orig.tar.bz2
 34887c4fdfe064ff058b56111cb7dfab 20172 v4l-utils_1.10.0-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCAAGBQJWpIyhAAoJEBmaZPrftQD/IcgP/1B9L+EgQR1mH/0F3EocTdjZ
KKHYrsXvRYFI5mxMh820hqsRnBNlt3t8y1BxXxTT7sNnIvQLhLwBtd2R2i48ffYS
SP5Faiu5T3vzA1FgroO1zs6mr7ld/UEq6+ZaG2Mm825e4bO1SxB4tSyArDKnfsC+
HDXFX6saufNIb1B1sHkok8p88sNEfVBWZWmBdoPc1OjZICsXZl50w90ur84YY2lB
ChJlNZdPU1Np3DBYFD/NzmoqxluFoZJqS24n/iZfmAUVEM9VPmng/zldD6crccs5
XLvBTt5i/8KVZHM5BVHfE0PsO+4ir5Vpqa6w5v36dki0h6XZfGUSIrPG5Od+Xs3g
h1+FALy1dQdeaIMPFdRhdmfsrJbIMRpZWD3sSptxtr8S0m66fzHieSywXsUqn6BU
joQpHwBh4Twe2r887I+ChxnUnq9ovuG2BHWTHGvKzKW+WYO4dBOaEEkJz7dAD44E
tPgILSN1fy8opNiaztMGmARShqNCixhvtVCeRDcZijDyBlomBu90N//kj3jZjHKC
1tM7qXde/GTZY9rkILDjMXX59o9JRZdRrhlJMkOYPhN0EObV/vvJ2O+4wU7hS/qy
mcSFZ5B+OKqsEcuaVGg/959hwAPbx9PxF88/IEiKT6xs/pCwcurFgGZE7sDZEfoK
n4xfQoBRroitC+xfFzSN
=QhqH
-----END PGP SIGNATURE-----
