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

#include "eqn-expansion.hpp"
#include <sstream>
#include <typeinfo>
#include <string.h>

// Necessary to instantiate for gcc v4.7.1 ??
template class std::basic_string<char, std::char_traits<char>,
                                 std::allocator<char>>;

template <typename T>
Term<T>::Term()
  {
  std::ostringstream s;

  s << typeid(T).name() << "(" << 0 << ")";
  std::string x = s.str();
  this->name = x;
  }

template <typename T>
Term<T>::Term(int a)
  {
  std::ostringstream s;

  s << typeid(T).name() << "(" << a << ")";
  std::string x = s.str();
  this->name = x;
  }

template <typename T>
Term<T>::Term(const std::string& name)
  {
  this->name = name;
  }

template <typename T>
Term<T>::Term(const Term<T>& rhs)
  {
  this->name = rhs.name;
  }

template <typename T>
Term<T>::~Term()
  {
  }

template <typename T> Term<T>&
Term<T>::operator=(const Term<T>& rhs)
  {
  this->name = rhs.name;
  return *this;
  }

template <typename T> void
Term<T>::operator=(int a)
  {
  std::stringstream s;

  s << typeid(T).name() << "(" << a << ")";
  std::string x = s.str();
  this->name = x;
  }

template <typename T> void
Term<T>::operator=(std::string name)
  {
  this->name = name;
  }

template <typename T> void
Term<T>::operator()(int a)
  {
  std::stringstream s;

  s << typeid(T).name() << "(" << a << ")";
  std::string x = s.str();
  this->name = x;
  }

#if 0
void
Term::operator+(const Term& rhs)
  {
  this->name += " + " + rhs.name;
  }

void
Term::operator-(const Term& rhs)
  {
  this->name += " - " + rhs.name;
  }

void
Term::operator*(const Term& rhs)
  {
  this->name += " * " + rhs.name;
  }
#endif

template <typename T> void
Term<T>::Name(const std::string &name)
  {
  this->name = name;
  }

template <typename T> std::string
Term<T>::Name() const
  {
  return this->name;
  }

template <typename T> std::ostream&
operator<<(std::ostream& out, Term<T>& term)
  {
  out << term.Name();
  }

template class Term<double>;

template <typename T> Term<T>
operator+(const Term<T>& lhs, const Term<T>& rhs)
  {
  std::string x;
  x = "(" + lhs.Name() + std::string(" + ") + rhs.Name() + ")";
  Term<T> r(x);
  return r;
  }

template <typename T> Term<T>
operator-(const Term<T>& lhs, const Term<T>& rhs)
  {
  std::string x;
  x = "(" + lhs.Name() + " - " + rhs.Name() + ")";
  Term<T> r(x);
  return r;
  }

template <typename T> Term<T>
operator*(const Term<T>& lhs, const Term<T>& rhs)
  {
  std::string x;
  x = lhs.Name() + " * " + rhs.Name();
  Term<T> r(x);
  return r;
  }

template <typename T> std::ostream&
operator<<(std::ostream& out, const Term<T>& rhs)
  {
  out << rhs.Name();
  }

template Term<double> operator+<>(const Term<double>&, const Term<double>&);
template Term<double> operator-<>(const Term<double>&, const Term<double>&);
template Term<double> operator*<>(const Term<double>&, const Term<double>&);
template std::ostream& operator<<<>(std::ostream&, const Term<double>&);
