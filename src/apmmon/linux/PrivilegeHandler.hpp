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

#pragma once

#include <sys/capability.h>
#include <vector>

namespace apm {

enum class Privilege
{
    Input = CAP_DAC_READ_SEARCH
};

class PrivilegeHandler final
{
  public:
    explicit PrivilegeHandler(std::vector<Privilege>& _privileges);
    PrivilegeHandler() = delete;
    ~PrivilegeHandler();
    PrivilegeHandler(const PrivilegeHandler&) = delete;
    PrivilegeHandler& operator=(const PrivilegeHandler&) = delete;
    PrivilegeHandler(PrivilegeHandler&&)                 = delete;
    PrivilegeHandler& operator=(PrivilegeHandler&&) = delete;

  public:
    void raisePrivilege(Privilege privilege);
    void lowerPrivilege(Privilege privilege);
    void raiseInputPrivilege();
    void lowerInputPrivilege();

  private:
    void setPrivileges();
    void setPrivilegesFlag(
        cap_flag_t       flag,
        Privilege        privilege,
        cap_flag_value_t value);

  private:
    cap_t                  caps;
    std::vector<Privilege> privileges;
};

} // namespace apm
