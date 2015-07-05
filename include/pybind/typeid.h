/*
    pybind/typeid.h: Compiler-independent access to type identifiers

    Copyright (c) 2015 Wenzel Jakob <wenzel@inf.ethz.ch>

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE file.
*/

#if !defined(__PYBIND_TYPEID_H)
#define __PYBIND_TYPEID_H

#include "common.h"
#include <cstdio>
#include <cstdlib>
#if defined(__GNUG__)
#include <cxxabi.h>
#endif

NAMESPACE_BEGIN(pybind)
NAMESPACE_BEGIN(detail)
/// Erase all occurrences of a substring
inline void erase_all(std::string &string, const std::string &search) {
    for (size_t pos = 0;;) {
        pos = string.find(search, pos);
        if (pos == std::string::npos) break;
        string.erase(pos, search.length());
    }
}
NAMESPACE_END(detail)

/// Return a string representation of a C++ type
template <typename T> static std::string type_id() {
    std::string name(typeid(T).name());
    #if defined(__GNUG__)
        int status = 0;
        std::unique_ptr<char, void (*)(void *)> res {
            abi::__cxa_demangle(name.c_str(), nullptr, nullptr, &status), std::free };
        if (status == 0)
            name = res.get();
    #else
        detail::erase_all(name, "class ");
        detail::erase_all(name, "struct ");
        detail::erase_all(name, "enum ");
    #endif
    detail::erase_all(name, "pybind::");
    return name;
}

NAMESPACE_END(pybind)

#endif /* __PYBIND_TYPEID_H */
