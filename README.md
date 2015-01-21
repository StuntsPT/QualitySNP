#QualitySNPng

This repository is a fork of [upstream](https://trac.nbic.nl/qualitysnp/).
It includes some changes to allow for building a static binary, which is very convenient for [4Pipe4](https://github.com/StuntsPT/4Pipe4).

Other than that, there's nothing else to see here that you cannot find in the upstream repository.

##Self contained binary
You can find a mostly self contained binary under bin/QualitySNPng, which was built against Qt 4.8.6 OSE.
Due to this a GPL license was added to the repository.


##Build instructions:

In order to build a static binary, you must use a statically built Qt.
Here is how I have done it to build the binary you can find in the repository (much of the information was gathred [here](http://www.formortals.com/how-to-statically-link-qt-4/):

1. Download Qt from [here](http://www.qt.io/download/) and extract the file.
2. cd into the source root dir and run: ./configure -static -qt-libpng -qt-libjpeg  -qt-libtiff -prefix ~path/to/qt-everywhere-opensource-src-version
3. make
4. Open "QualitySNPng.pro" with Qt creator and add the Qt version you just statically complied to your Qt versions.
5. Build your project as "release version" with the statically linked Qt version.
6. There yo go - a 16Mb binary that contains all the required libs (at least as far as Qt is concerned).

####Windows

* Install Microsoft Visual C++
* Start Visual Studio Command prompt
* Go to the directory with the source
* Run:
  * $QTDIR/qmake -o Makefile QualitySNPng.pro
  * nmake

---

####OS X

* Install QtCreator
* Load QualitySNPng.pro with QtCreator, if requested select the Desktop target
* Build project
* To package, run:
  * macdeployqt QualitySNPng.app -dmg

---

####Linux

* Install QtCreator
* Load QualitySNPng.pro with QtCreator, if requested select the Desktop target
* Build project


##Citation

QualitySNPng was published in the 2013 webserver issue of Nucleic Acids Research:
[QualitySNPng: a user-friendly SNP detection and visualization tool](http://nar.oxfordjournals.org/content/41/W1/W587)
Harm Nijveen, Martijn van Kaauwen, Danny G. Esselink, Brechtje Hoegen and Ben Vosman

For further information regarding the upstream project, please email: qsnpng @ bioinformatics.nl 
For further information regarding this fork, please open an issue in this repository.

