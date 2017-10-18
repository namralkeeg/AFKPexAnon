/*
 * Copyright (C) 2017 Larry Lopez
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#include <afk/fileformats/pex/pexheader.hpp>
#include <keeg/io/binaryreaders.hpp>
#include <keeg/endian/conversion.hpp>
#include <keeg/common/stringutils.hpp>
#include <ctime>
#include <iomanip>

namespace afk { namespace fileformats { namespace pex {

std::size_t PexHeader::read(std::istream &instream)
{
    try
    {
        if (instream)
        {
            instream.seekg(0, std::ios::beg);
            if (keeg::io::readPODType<PexHeader>(instream, *this))
            {
                std::size_t status = 0;
                switch (magic) {
                case UINT32_C(0xFA57C0DE):
                    status = 1;
                    break;
                /// File data is in reverse endian of the host machine.
                case UINT32_C(0xDEC057FA):
                    magic = keeg::endian::swap(magic);
                    gameId = keeg::endian::swap(gameId);
                    compilationTime = keeg::endian::swap(compilationTime);
                    status = 1;
                    break;
                default:
                    break;
                }

                return status;
            }
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return 0;
    }

    return 0;
}

std::ostream & operator <<(std::ostream &o, const PexHeader &pexHeader)
{
    char buildTimeStr[27];
    time_t compTime = static_cast<time_t>(pexHeader.compilationTime);
    ctime_s(buildTimeStr, sizeof(buildTimeStr), &compTime);
    buildTimeStr[24] = 0;	// remove newline (format is fixed-length, no really)

    o << "Magic:\t" << std::hex << pexHeader.magic << std::endl;
    o << "Major:\t" << std::hex << std::setfill('0') << std::setw(2)
      << static_cast<uint16_t>(pexHeader.majorVersion) << std::endl;
    o << "Minor:\t" << std::hex << std::setfill('0') << std::setw(2)
      << static_cast<uint16_t>(pexHeader.minorVersion) << std::endl;
    o << "Game ID\t: " << std::hex << std::setfill('0') << std::setw(4) << pexHeader.gameId << std::endl;
    o << "Compile:\t" << std::string(buildTimeStr) << std::endl;

    return o;
}

bool operator ==(const PexHeader &lhs, const PexHeader &rhs)
{
    return  (lhs.magic == rhs.magic) &&
            (lhs.majorVersion == rhs.majorVersion) &&
            (lhs.minorVersion == rhs.minorVersion) &&
            (lhs.gameId == rhs.gameId) &&
            (lhs.compilationTime == rhs.compilationTime);
}

} // pex namespace
} // fileformats namespace
} // afk namespace
