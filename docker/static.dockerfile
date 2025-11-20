ARG BOOST_VERSION="1.89.0"
ARG BOOST_VARIANT="release"
ARG LINK="shared"

FROM ghcr.io/morphgrid/framework:1.0.2-boost-${BOOST_VERSION}-${BOOST_VARIANT}-${LINK} AS builder

ARG BOOST_VERSION
ARG BOOST_VARIANT
ARG LINK

WORKDIR /srv

COPY . .

RUN bash scripts/build.sh ${BOOST_VARIANT} ${LINK}

FROM alpine:3.22.2 AS runtime
WORKDIR /srv
COPY --from=builder /srv/build/base /usr/local/bin/base

EXPOSE 9000
ENTRYPOINT ["/usr/local/bin/base"]