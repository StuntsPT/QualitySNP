/*
* This File is part of QualitySNP; a program to detect Single Nucleotide Variations
* https://trac.nbic.nl/qualitysnp/
*
*   Copyright (C) 2012 Harm Nijveen
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <algorithm>
#include <iostream>
#include <vector>
#include "Variation.h"
#include "Contig.h"
#include "SeqRead.h"
#include "Configuration.h"

using namespace std;

SeqRead::SeqRead(string name, Contig* pParent) : 
	_parent(pParent), _name(name), _startPosition(0), _cSNP(0), _cHQSNP(0), _pHaploType(NULL), _endPosition(-1)
{
}

SeqRead::~SeqRead(void)
{
}

int SeqRead::getEndPosition() {
    if(_endPosition == -1) {
        _endPosition = _startPosition + _sequence.length();
    }

    return _endPosition;
}

void SeqRead::setSequence(const string& sequence) {
	_sequence = sequence;	

	transform (_sequence.begin (), _sequence.end (), _sequence.begin (), (int(*)(int)) toupper);

	_qualClipStart = 0;
	_qualClipEnd = sequence.length() - 1;

	_lowQual5p = _qualClipStart;
	_lowQual3p = sequence.length() - 1 + _qualClipStart;
}

void SeqRead::setQualitySanger(const string& quality) {
	_quality.reserve(_sequence.length());
	for(unsigned int i = 0; i < quality.length(); i++) {
		_quality.push_back(quality[i] - 33);
	}
}

const string SeqRead::toString() {
	string sequence;
	if(_startPosition + _qualClipStart < 0) {
		sequence  = _sequence.substr(abs(_startPosition), _qualClipEnd + _startPosition + 1);
	} else {
		sequence  = _sequence.substr(_qualClipStart, _qualClipEnd - _qualClipStart + 1);
        sequence.insert(0,_startPosition + _qualClipStart,' ');
	}
	for(unsigned int i = 0; i < sequence.length(); i++) {
		if(!isHighQuality(i)) {
			sequence[i] = tolower(sequence[i]);
		}
	}
	return sequence;
}

// returns the nucleotide at pos, where pos is the position in the contig reference sequence
// pos first needs to be translated to the position in the sequence
char SeqRead::getNucleotideAt(int pos) {
	if(pos < _startPosition) {
		return ' ';
	}
	
	pos -= _startPosition;
	return (pos > _qualClipEnd || pos < _qualClipStart) ?  ' ' : _sequence[pos];
}

// set the quality clip region, only this region of the read will be used
void SeqRead::setQualClip(unsigned int qualClipStart, unsigned int qualClipEnd) { 
	_qualClipStart = (qualClipStart < _sequence.length()) ? qualClipStart - 1 : 0;
	_qualClipEnd = (qualClipEnd < _sequence.length()) ? qualClipEnd - 1 : _sequence.length() - 1;
	setLowQualityBounds();
}


bool SeqRead::isHighQuality(int pos) {
	if(pos < _startPosition) {
        return false;
    }

	pos -= _startPosition;
	if (pos > _lowQual3p || pos < _lowQual5p) {
		return false;
	}

	if (pos < _quality.size()) {
		return (_quality[pos] >= Configuration::getConfig()->getInt("minSNPQualityScore"));
	}

	// if there is no quality information available for this position, consider it high quality
	return true;
}

// set the parts at the 5' and at the 3' ends that are low quality
// _lowQual5p is the position the low quality region 5' ends, 
// _lowQual3p is the postion the low quality region 3' begins
void SeqRead::setLowQualityBounds() {
	_lowQual5p = Configuration::getConfig()->getInt("lowQualityRegion5prime") + _qualClipStart;
	int lowQual3p = Configuration::getConfig()->getInt("lowQualityRegion3prime");

	if (lowQual3p == 0) {
		double lqPerc = Configuration::getConfig()->getDouble("lowQualityRegion3primePerc");
		_lowQual3p = int((1.0 - lqPerc) * getClippedSequenceLength()) + _qualClipStart;
	} else {
		_lowQual3p = getClippedSequenceLength() - lowQual3p - 1  + _qualClipStart;
	}
}

bool SeqRead::calculateSNPCount() {
	if (_parent == NULL) {
		// read is not part of a contig...
		return false;
	}

	_cSNP = 0;

	vector<Variation*>::const_iterator itVar;
	const vector<Variation*>& variations = _parent->getVariations();
	for ( itVar=variations.begin(); itVar != variations.end(); itVar++) {
		int pos = (*itVar)->getPos();
		char cNuc = getNucleotideAt(pos);
		if(cNuc != ' ') {
			_cSNP++;
			if((*itVar)->isHighConfidence()) {
				_cHQSNP++;
			}
		}
	}

	return true;
}

string SeqRead::toCSV() {
	if (_parent == NULL) {
		return string();
	}

	const char sep = Configuration::getConfig()->getChar("fieldSeparator");
	
	const int hapid = (_pHaploType == NULL) ? -1 :  _pHaploType->getID();
    const string group = (_group.empty()) ? "-" : _group;
	stringstream csv;
	csv << _name << sep;
	csv << hapid << sep;
	csv << _parent->getName() << sep;
    csv << (_startPosition + 1) << sep; // _startPosition is zero based
    csv << group << sep;
    csv << _sequence << NEWLINE;
	return csv.str();
}

const string& SeqRead::getGroupFromName(const string& separator) {
    size_t sepPos = _name.find(separator);

    if(sepPos != string::npos) {
        _group = _name.substr(0,sepPos);
    }

    return _group;
}
