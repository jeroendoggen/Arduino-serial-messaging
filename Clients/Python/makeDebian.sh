#!/usr/bin/env bash
#
# Small script to create the Debian package & install it when it is done

echo "Starting package build process"

python setup.py --command-packages=stdeb.command sdist_dsc

cd deb_dist/sp-controller-0.0.1/

dpkg-buildpackage -rfakeroot -uc -us

cd ..

sudo dpkg -i python-sp-controller_0.0.1-1_all.deb

