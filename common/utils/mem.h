/*
 *  Copyright (C) 2005 Krzysztof Rzymkowski <rzymek@users.sourceforge.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef MEM_H
#define MEM_H

#include <memory>
template<typename T>
inline std::auto_ptr<T> make_auto_ptr(T* t) {
    return std::auto_ptr<T>(t);
}

template<typename T>
inline std::auto_ptr<T> make_auto_ptr(const T& t) {
    return std::auto_ptr<T>(new T(t));
}

#endif//MEM_H

