#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include "raylib.h"

// example code, exclude from project

class MiscTools
{
private:
	// private constructor -- can't create from outside
	MiscTools() {}

	// delete copy constructor and assignment operator
	// This prevents duplication
	MiscTools(const MiscTools&) = delete;
	MiscTools& operator=(const MiscTools&) = delete;

public:

	// the ONLY way to get the instance
	static MiscTools& Instance() {
		static MiscTools instance; // created once, lives forever
		return instance;
	}

	std::string GetUUID()
	{
        std::stringstream ss;
        ss << std::hex << std::setfill('0');

        for (int i = 0; i < 16; i++) {
            // Get a random byte (0-255)
            unsigned char byte = (unsigned char)GetRandomValue(0, 255);

            // Apply UUID v4 variants
            if (i == 6) {
                // High nibble must be 4
                byte = 0x40 | (byte & 0x0F);
            }
            else if (i == 8) {
                // High nibble must be 8, 9, A, or B
                byte = 0x80 | (byte & 0x3F);
            }

            ss << std::setw(2) << (int)byte;

            // Insert hyphens at specific positions
            if (i == 3 || i == 5 || i == 7 || i == 9) {
                ss << "-";
            }
        }

        return ss.str();
	}

};