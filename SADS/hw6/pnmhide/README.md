# PNMHide - SADS Assignment Sheet 6

### Contributor: Dushan Terzikj (d.terzikj@jacobs-university.de)

`pnmhide` is a program which uses libnetpbm to read images and encode some hidden text into the image which is not visible to the naked human eye. The program also has the ability to decode the hidden text (or detect that there is no hidden text at all) which was encoded using the exact same algorithm as `pnmhide` program.

## Report
Please find the .pdf report for the assignment (includes solutions to a, b, and c parts). The report can be found in `report/`.

## Build and usage

### Build
To build the program do the following:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

### Usage
Encode
```
$ ./pnmhide -e "the hidden text" covertext.pnm > stegotext.pnm
```

The original image is covertext.pnm. The encoded image is stegotext.pnm.

Decode
```
$ ./pnmhide -d stegotext.pnm
```

You should get the hidden text as a result if stegotext.pnm (as an encoded image) has a hidden text. If not, then there will be a blank line (no gibberish).

## Documentation

To build the documentation, do the "Build" part and then:
```
$ make doc
```
The documentation can be found in `doc/html`

## Techincal support
For any questions, contact the contributor.