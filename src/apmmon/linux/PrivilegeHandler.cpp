/*
 * Copyright (c) 2018 David 'Mokon' Bond <mokon@mokon.net>
 * Author: David 'Mokon' Bond <mokon@mokon.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <linux/PrivilegeHandler.hpp>
#include <system_error>

namespace apm {

PrivilegeHandler::PrivilegeHandler(std::vector<Privilege>& _privileges)
    : caps(cap_get_proc())
    , privileges(_privileges)
{
    if (cap_clear(caps))
    {
        throw std::system_error(errno, std::generic_category());
    }

    for (auto& privilege : privileges)
    {
        setPrivilegesFlag(CAP_PERMITTED, privilege, CAP_SET);
    }
    setPrivileges();
}

void
PrivilegeHandler::setPrivileges()
{
    if (cap_set_proc(caps))
    {
        throw std::system_error(errno, std::generic_category());
    }
}

void
PrivilegeHandler::setPrivilegesFlag(
    cap_flag_t       flag,
    Privilege        privilege,
    cap_flag_value_t value)
{
    cap_value_t privs[1] = {static_cast<int>(privilege)};
    if (cap_set_flag(caps, flag, 1, privs, value))
    {
        throw std::system_error(errno, std::generic_category());
    }
}

void
PrivilegeHandler::raisePrivilege(Privilege privilege)
{
    setPrivilegesFlag(CAP_EFFECTIVE, privilege, CAP_SET);
    setPrivileges();
}

void
PrivilegeHandler::lowerPrivilege(Privilege privilege)
{
    setPrivilegesFlag(CAP_EFFECTIVE, privilege, CAP_CLEAR);
    setPrivileges();
}

void
PrivilegeHandler::raiseInputPrivilege()
{
    raisePrivilege(Privilege::Input);
}

void
PrivilegeHandler::lowerInputPrivilege()
{
    lowerPrivilege(Privilege::Input);
}

PrivilegeHandler::~PrivilegeHandler()
{
    if (cap_free(caps))
    {
        std::perror("cap_free failed");
    }
}

} // namespace apm
