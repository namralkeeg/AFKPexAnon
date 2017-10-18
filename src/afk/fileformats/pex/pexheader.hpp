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
#ifndef PEXHEADER_HPP
#define PEXHEADER_HPP

#include <cstdint>
#include <iostream>

namespace afk { namespace fileformats { namespace pex {

struct PexHeader
{
    uint32_t magic;             // 00	FA57C0DE
    uint8_t  majorVersion;      // 04	03=Skyrim, SSE, Fallout 4
    uint8_t  minorVersion;      // 05	02=Skyrim, 01=SkyrimSE, 09=Fallout 4
    uint16_t gameId;            // 06	0001=Skyrim and SSE, 0002=Fallout 4
    uint64_t compilationTime;   // 08	time_t usually uint64_t

    std::size_t read(std::istream &instream);
};

std::ostream & operator <<(std::ostream &o, const PexHeader &pexHeader);
bool operator ==(const PexHeader &lhs, const PexHeader &rhs);

} // pex namespace
} // fileformats namespace
} // afk namespace

#endif // PEXHEADER_HPP
