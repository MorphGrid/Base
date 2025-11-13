ARG BOOST_VERSION="1.89.0"
ARG BOOST_VARIANT="release"
ARG LINK="shared"

FROM ghcr.io/morphgrid/framework:1.0.0-boost-${BOOST_VERSION}-${BOOST_VARIANT}-${LINK}

ARG BOOST_VERSION
ARG BOOST_VARIANT
ARG LINK

WORKDIR /srv

COPY . .

RUN bash scripts/build.sh ${BOOST_VARIANT} ${LINK}

EXPOSE 9000
ENTRYPOINT ["/srv/build/base"]