FROM centos

RUN yum -y install make automake gcc libtool

WORKDIR /opt/status/
COPY . .
RUN cd lib/rabbitmq-c; autoreconf -i && ./configure && make install
RUN cd src && make

ENV LD_LIBRARY_PATH /usr/local/lib

CMD [ "/opt/status/src/producer" ] 