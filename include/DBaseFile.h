#ifndef DBASEFILE_H
#define DBASEFILE_H

#include <string>
#include <vector>
#include <stdint.h>
#include <bitset>
#include <ctime>
#include <exception>
#include <stdexcept>

#include "DBaseRecord.h"
#include "DBaseFieldDescArray.h"

class DBaseFile
{
    public:
        bool openFile(const std::string fileName);

    private:
    	//Helper function: converts char to bitset
    	std::bitset<8> ToBits(char* byte);
        //File contents
		std::string m_rawData = "";
		const unsigned int blockSize = 32;
        //--------------------------------------------------
        //Level 5 DOS Header for dBase III - dBase VII files
        //--------------------------------------------------
        std::string fileType;
        char firstByte;                     //BYPTE 0 temporary storage, c++ can only read bytes
            uint16_t versionNr = 3;         //BYTE 0 bit 0-2 version number of file
            bool memoFilePresent = false;   //BYTE 0 bit 3 is a memo present or not
            bool sqlFilePresent = false;    //BYTE 0 bit 4-6 is a .sql file present or not
            bool anyMemoFilePresent = false;//BYTE 0 bit 7 is one of the above true
        time_t lastUpdated = time(0);       //BYTE 1-3 YYMMDD format
        uint32_t numRecordsInDB = 0;        //BYTE 4-7 32-bit nr of records
        uint16_t numBytesInHeader = 0;      //BYTE 8-9 16 bit nr of bytes in header
        uint16_t numBytesInRecord = 0;      //BYTE 10-11 16 bit nr of bytes in record
                                            //BYTE 12-13: 0, ignore
        bool incompleteTransaction = false; //BYTE 14, set by BEGIN TRANSACTION
        bool encrypted = false;             //BYTE 15, dBase IV encryption flag
                                            //BYTE 16-27 dBase DOS in multi-user environment
        bool productionMDX = false;         //BYTE 28 0x01 = production .mdx available, else not
                                            //BYTE 30-31:0, ignore
											//BYTE 64-67:0, ignore
		std::vector<DBaseFieldDescArray> fieldDescriptors;	//BYTE 68-n Field Descriptor Array, 48 bytes each
                                            //BYTE n+1: 0x0D (13) field descriptor terminator
                                            //BYTE n+2 start of the field properties

		bool hasStructuralCDX = false;
		bool hasMemoField = false;
		bool isDatabase =  false;
		uint8_t fieldDescArrayNum = 0;
		uint8_t codePageMark;

		//helper function
		std::string getFileContents(std::ifstream& iFile);
};

//Exceptions
//no memory available on system
class noMemoryAvailableEx : public std::runtime_error{};

//file could not be read
class fileNotFoundEx : public std::runtime_error{};

//header is not a calculatable, misaligned bytes
class unexpectedHeaderEndEx : public std::runtime_error{
	int m_byteHeaderFailed = 0;
	bool m_isFoxBaseHeader = false;
};

//blockSize is too short or too long, calculate expected byte layout
class unexpectedBlockSize	: public unexpectedHeaderEndEx{
	int m_blockSizePrev;
	int calculateNextBlockSize(int prev, int totalStringSize, ...);
};

class badFileEx : public std::runtime_error{};
class incompleteTransactionEx : public badFileEx{};

#endif // DBASEFILE_H
