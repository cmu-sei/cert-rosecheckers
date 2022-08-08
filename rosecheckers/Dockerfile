# docker build -t rosebud .
# docker run -it --rm  rosebud rosecheckers example.c

# <legal>CERT Rosecheckers# 
# Copyright 2022 Carnegie Mellon University.# 
# NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING INSTITUTE MATERIAL IS FURNISHED ON AN 'AS-IS' BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.# 
# Released under a BSD (SEI)-style license, please see license.txt or contact permission@sei.cmu.edu for full terms.# 
# [DISTRIBUTION STATEMENT A] This material has been approved for public release and unlimited distribution.  Please see Copyright notice for non-US Government use and distribution.# 
# CERT(R) is registered in the U.S. Patent and Trademark Office by Carnegie Mellon University.# 
# This Software includes and/or makes use of Additional Software Dependencies, each subject to its own license.# 
# DM21-0505</legal># 

FROM ubuntu:bionic

RUN apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get -y install --no-install-recommends \
        software-properties-common \
    && apt-get clean
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install --no-install-recommends \
        gcc g++ perl python make bison flex \
    && apt-get clean
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install --no-install-recommends \
        build-essential fontconfig ghostscript gnupg groff imagemagick \
        openjdk-8-jre-headless patch xz-utils \
    && apt-get clean
RUN add-apt-repository ppa:rosecompiler/rose-stable \
    && DEBIAN_FRONTEND=noninteractive apt-get -y install --no-install-recommends \
        rose \
    && apt-get clean
# This used to also install rose-tools, but that dependency is currently broken

# Needed by rosecheckers, but not rose
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install --no-install-recommends \
        libtool libtool-bin doxygen \
    && apt-get clean

ENV ROSE=/usr/rose
ENV ROSECHECKERS=/usr/rosecheckers
WORKDIR $ROSECHECKERS
COPY . $ROSECHECKERS

RUN make \
        && make install \
        && make doc \
        && make clean


CMD ["/bin/bash"]
