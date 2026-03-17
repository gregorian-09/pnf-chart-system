/// \file version.hpp
/// \brief Version query helpers.

//
// Created by gregorian-rayne on 12/01/2026.
//

#ifndef VERSION_HPP
#define VERSION_HPP

#define PNF_VERSION_MAJOR 0
#define PNF_VERSION_MINOR 1
#define PNF_VERSION_PATCH 2
#define PNF_VERSION_STRING "0.1.2"

namespace pnf {

    struct Version {
        static constexpr int major = PNF_VERSION_MAJOR;
        static constexpr int minor = PNF_VERSION_MINOR;
        static constexpr int patch = PNF_VERSION_PATCH;
        static constexpr auto string = PNF_VERSION_STRING;
    };

}

#endif //VERSION_HPP
