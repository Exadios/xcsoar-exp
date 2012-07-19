/* Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2012 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
 */
#ifndef EQN_EXPANSION_HPP
#define EQN_EXPANSION_HPP

#include <string>
#include "Math/MatrixT.hpp"
#include "Math/VectorT.hpp"

template <typename T> class Term;
template <typename T> Term<T> operator+(const Term<T>& lhs, const Term<T>& rhs);
template <typename T> Term<T> operator-(const Term<T>& lhs, const Term<T>& rhs);
template <typename T> Term<T> operator*(const Term<T>& lhs, const Term<T>& rhs);

template <typename T> class Term
{
  public:
    Term();
    Term(int a);
    Term(const std::string& name);
    Term(const Term& rhs);
    virtual ~Term();

    Term& operator=(const Term& rhs);
    void operator=(int a);
    void operator=(std::string name);
    void operator()(int a);
//    void operator+(const Term& rhs);
//    void operator-(const Term& rhs);
//    void operator*(const Term& rhs);
    friend Term<T> operator+<>(const Term<T>& lhs, const Term<T>& rhs);
    friend Term<T> operator-<>(const Term<T>& lhs, const Term<T>& rhs);
    friend Term<T> operator*<>(const Term<T>& lhs, const Term<T>& rhs);

    void Name(const std::string &name);
    std::string Name() const;

  private:
    std::string name;
};

template <typename T> std::ostream& operator<<(std::ostream& out, const Term<T>&);

//#include "eqn-expansion.i.cpp"

#endif // EQN_EXPANSION_HPP
