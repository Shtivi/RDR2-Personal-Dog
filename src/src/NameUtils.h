#pragma once

const int WHITE_FIRSTNAMES_MALES_COUNT = 3;
const int WHITE_LASTNAMES_COUNT = 3;
const char* const WHITE_FIRSTNAMES_MALES[WHITE_FIRSTNAMES_MALES_COUNT] = { "Jim", "Andrew", "Joshua" };
const char* const WHITE_LASTNAMES[WHITE_LASTNAMES_COUNT] = { "Hemilton", "Green", "Brown" };

const int SPANISH_FIRSTNAMES_MALES_COUNT = 1;
const int SPANISH_LASTNAMES_COUNT = 1;
const char* const SPANISH_FIRSTNAMES_MALES[SPANISH_FIRSTNAMES_MALES_COUNT] = { "Alejandro" };
const char* const SPANISH_LASTNAMES[SPANISH_LASTNAMES_COUNT] = { "Hernendez" };

const char* randomWhiteName()
{
	const char* firstname = WHITE_FIRSTNAMES_MALES[rndInt(0, WHITE_FIRSTNAMES_MALES_COUNT)];
	const char* lastname = WHITE_LASTNAMES[rndInt(0, WHITE_LASTNAMES_COUNT)];

	stringstream fullname;
	fullname << firstname << " " << lastname;
	return fullname.str().c_str();
}

const char* randomSpanishName()
{
	const char* firstname = SPANISH_FIRSTNAMES_MALES[rndInt(0, SPANISH_FIRSTNAMES_MALES_COUNT)];
	const char* lastname = SPANISH_LASTNAMES[rndInt(0, SPANISH_LASTNAMES_COUNT)];

	stringstream fullname;
	fullname << firstname << " " << lastname;
	return fullname.str().c_str();
}