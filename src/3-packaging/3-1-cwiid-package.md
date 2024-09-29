# 构建 cwiid 软件包

0.6.91

[cwiid source code](https://salsa.debian.org/georgesk/cwiid/-/archive/upstream/0.6.91/cwiid-upstream-0.6.91.tar.gz)

[openEuler packaging guidelines](https://gitee.com/openeuler/community/blob/master/zh/contributors/packaging.md)

[EulerMaker用户指南](https://docs.openeuler.org/zh/docs/24.03_LTS/docs/EulerMaker/EulerMaker%E7%94%A8%E6%88%B7%E6%8C%87%E5%8D%97.html)

[Downloading source code archives - GitHub Docs](https://docs.github.com/en/repositories/working-with-files/using-files/downloading-source-code-archives#source-code-archive-urls)

[Building an RPM Package](https://docs.openeuler.org/en/docs/22.03_LTS_SP1/docs/ApplicationDev/building-an-rpm-package.html)

Format: 3.0 (quilt)
Source: cwiid
Binary: python3-cwiid, libcwiid1t64, libcwiid-dev, lswm, wminput
Architecture: any
Version: 0.6.91-9
Maintainer: Georges Khaznadar <georgesk@debian.org>
Homepage: https://github.com/abstrakraft/cwiid
Standards-Version: 4.6.2
Vcs-Browser: https://salsa.debian.org/georgesk/cwiid
Vcs-Git: https://salsa.debian.org/georgesk/cwiid.git
Build-Depends: dpkg-dev (>= 1.22.5), debhelper-compat (= 13), libtool, automake, autoconf, python3-dev, python3, libpython3-dev, dh-python, libbluetooth-dev, flex, bison
Package-List:
 libcwiid-dev deb libdevel optional arch=any
 libcwiid1t64 deb libs optional arch=any
 lswm deb utils optional arch=any
 python3-cwiid deb python optional arch=any
 wminput deb utils optional arch=any
