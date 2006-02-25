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
class wrapper {
public:
    typedef void (*free_t)(void*);
private:
    struct value_t {
        value_t() : obj(0),func(0),ref_count(0)
            { }
        value_t(void* an_obj, free_t a_func)
            : obj(an_obj), func(a_func), ref_count(1)
            { }
        ~value_t()
            { /* func(obj); */ }
        void* obj;
        free_t func;
        unsigned ref_count;
    } *value;
public:
    wrapper();
    wrapper(void* an_obj, free_t a_func);
    wrapper(const wrapper& w);

    wrapper& operator=(const wrapper& w);
    ~wrapper();
    void* obj() const
        { return value->obj; }
};

