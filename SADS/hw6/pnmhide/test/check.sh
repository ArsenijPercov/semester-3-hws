#!/bin/sh

PNMHIDE=../build/src/pnmhide

$PNMHIDE -e "hello world" -d cover.pnm > stego.pnm && \
    pnmtojpeg stego.pnm > stego.jpg && \
    pnmhistmap cover.pnm | pnmtojpeg > cover-hist.jpg && \
    pnmhistmap stego.pnm | pnmtojpeg > stego-hist.jpg

exit
