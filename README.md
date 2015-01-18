#QualitySNPng

This repository is a fork of [upstream](https://trac.nbic.nl/qualitysnp/).
It includes some changes to allow for building a static binary, which is very convinient for [4Pipe4](https://github.com/StuntsPT/4Pipe4).

Other than that, there's nothing else to sse here that you cannot find in the upstream repository.

##Self contained binary
You can find a mostly self contained binary under bin/QualitySNPng, which was build against Qt 4.8.6 OSE.
Due to this a GPL license was added to the repository.


##Build instructions:

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

For further information please email: qsnpng @ bioinformatics.nl 
