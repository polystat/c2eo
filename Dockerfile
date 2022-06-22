# The MIT License (MIT)
#
# Copyright (c) 2021-2022 c2eo team
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

FROM ubuntu:20.04
MAINTAINER Yegor Bugayenko <yegor256@gmail.com>
WORKDIR /eo

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get clean && \
  apt-get update -y --fix-missing && \
  apt-get install -y locales && \
  locale-gen en_US.UTF-8 && \
  dpkg-reconfigure locales && \
  echo "LC_ALL=en_US.UTF-8\nLANG=en_US.UTF-8\nLANGUAGE=en_US.UTF-8" > /etc/default/locale
ENV LC_ALL en_US.UTF-8
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US.UTF-8

RUN apt install -y software-properties-common
RUN apt-key adv --keyserver keyserver.ubuntu.com --recv-keys F7C91591CC543ECA
RUN add-apt-repository 'deb http://c2eo.polystat.org/debian/ c2eo-rep non-free main contrib'
RUN apt-get install -y clang
RUN apt-get install -y c2eo

ENTRYPOINT ["/usr/bin/c2eo"]
