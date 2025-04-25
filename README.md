What is 3GPPDecoder?
=====================

3GPPDecoder is a free open source decoder to decode wireless control plane messages and PDUs (Protocol Data Units).
This fork is mainly tested in Linux, although Windows and macOS should be working.

You can use 3GPPDecoder to decode 3GPP NR, LTE, UMTS and GSM messages for RRC and NAS. Also it supports RLC (Radio Link Control) protocol PDU header decoding.

## How to build 3GPPDecoder

3GPPDecoder is developed using Qt and main programming language is C++.
Use the `qmake` or `cmake` (Requires Qt 6+) to build the decoder program.
Currently both Qt5 and Qt6 are supported.

I do not provide precompiled binaries for Windows and macOS.
