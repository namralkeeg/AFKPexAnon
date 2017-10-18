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
#include <afk/fileformats/pex/pexskyrimse.hpp>
#include <afk/fileformats/pex/gameid.hpp>
#include <keeg/common/enums.hpp>
#include <ctime>

namespace afk { namespace fileformats { namespace pex {

namespace kc = keeg::common;

/// SkyrimSE pex files use Big-Endian ordering for numbers.
PexSkyrimSE::PexSkyrimSE() : PexBase(keeg::endian::Order::big)
{
    m_header.magic = UINT32_C(0xFA57C0DE);
    m_header.majorVersion = 3;
    m_header.minorVersion = 1;
    m_header.gameId = kc::enumToIntegral<GameID>(GameID::skyrimSE);
    m_header.compilationTime = static_cast<uint64_t>(time(NULL));
}

bool PexSkyrimSE::isPex(const PexHeader &pexHeader)
{
    try
    {
        if ((pexHeader.magic == UINT32_C(0xFA57C0DE)) &&
                (pexHeader.majorVersion == 3) &&
                (pexHeader.minorVersion == 1) &&
                (pexHeader.gameId == kc::enumToIntegral<GameID>(GameID::skyrimSE)))
        {
            return true;
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
        return false;
    }

    return false;
}

} // pex namespace
} // fileformats namespace
} // afk namespace
