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
#include <afk/fileformats/pex/pexfactory.hpp>
#include <afk/fileformats/pex/pexfallout4.hpp>
#include <afk/fileformats/pex/pexskyrim.hpp>
#include <afk/fileformats/pex/pexskyrimse.hpp>

namespace afk { namespace fileformats { namespace pex {

std::unique_ptr<PexBase> PexFactory::createUniquePex(const std::string &game)
{
    std::unique_ptr<PexBase> pexBase;

    if (game == "skyrim")
        pexBase = std::make_unique<PexSkyrim>();
    else if (game == "skyrimSE")
        pexBase = std::make_unique<PexSkyrimSE>();
    else if (game == "fallout4")
        pexBase = std::make_unique<PexFallout4>();
    else
        pexBase = nullptr;

    return std::move(pexBase);
}

std::unique_ptr<PexBase> PexFactory::createUniquePex(const GameID &gameID)
{
    std::unique_ptr<PexBase> pexBase = nullptr;

    if (gameID == GameID::skyrim)
        pexBase = std::make_unique<PexSkyrim>();
    else if (gameID == GameID::skyrimSE)
        pexBase = std::make_unique<PexSkyrimSE>();
    else if (gameID == GameID::fallout4)
        pexBase = std::make_unique<PexFallout4>();

    return std::move(pexBase);
}

std::unique_ptr<PexBase> PexFactory::createUniquePex(const PexHeader &pexHeader)
{
    std::unique_ptr<PexBase> pexBase;

    if (PexSkyrim().isPex(pexHeader))
        pexBase = std::make_unique<PexSkyrim>();
    else if (PexSkyrimSE().isPex(pexHeader))
        pexBase = std::make_unique<PexSkyrimSE>();
    else if (PexFallout4().isPex(pexHeader))
        pexBase = std::make_unique<PexFallout4>();
    else
        pexBase = nullptr;

    return pexBase;
}

std::unique_ptr<PexBase> PexFactory::createUniquePex(std::istream &instream)
{
    std::unique_ptr<PexBase> pexBase = nullptr;

    PexHeader pexHeader;
    if (pexHeader.read(instream))
    {
        if (PexSkyrim().isPex(pexHeader))
            pexBase = std::make_unique<PexSkyrim>();
        else if (PexSkyrimSE().isPex(pexHeader))
            pexBase = std::make_unique<PexSkyrimSE>();
        else if (PexFallout4().isPex(pexHeader))
            pexBase = std::make_unique<PexFallout4>();
    }

    return std::move(pexBase);
}

} // pex namespace
} // fileformats namespace
} // afk namespace
