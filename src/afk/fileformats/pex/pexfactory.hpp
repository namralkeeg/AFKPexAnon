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
#ifndef PEXFACTORY_HPP
#define PEXFACTORY_HPP

#include <afk/fileformats/pex/gameid.hpp>
#include <afk/fileformats/pex/pexbase.hpp>
#include <memory>
#include <istream>

namespace afk { namespace fileformats { namespace pex {

class PexFactory
{
public:
    static std::unique_ptr<PexBase> createUniquePex(const std::string &game);
    static std::unique_ptr<PexBase> createUniquePex(const GameID &gameID);
    static std::unique_ptr<PexBase> createUniquePex(const PexHeader &pexHeader);
    static std::unique_ptr<PexBase> createUniquePex(std::istream &instream);

protected:
    PexFactory() { }
};

} // pex namespace
} // fileformats namespace
} // afk namespace

#endif // PEXFACTORY_HPP
