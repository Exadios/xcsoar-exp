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

Term::Term()
  : type("fixed")
  {
  }

Term::Term(int a)
  : type("fixed")
  {
  std::stringstream s;

  s << this->type << "(" << a << ")";
  this->name = s.str();
  }

Term::Term(const std::string& name)
  : name(name),
    type("fixed")
  {
  }

Term::Term(const Term& rhs)
  {
  this->name = rhs.name;
  this->type = rhs.type;
  }

Term::~Term()
  {
  }

Term&
Term::operator=(const Term& rhs)
  {
  this->name = rhs.name;
  this->type = rhs.type;
  return *this;
  }

void
Term::operator=(int a)
  {
  std::stringstream s;

  s << this->type << "(" << a << ")";
  this->name = s.str();
  }

void
Term::operator()(int a)
  {
  std::stringstream s;

  s << this->type << "(" << a << ")";
  this->name = s.str();
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

std::string&
Term::Get()
  {
  return this->name;
  }

const std::string&
Term::Get() const
  {
  return this->name;
  }

std::ostream&
operator<<(std::ostream& out, Term& term)
  {
  out << term.Get();
  }

const Term
operator+(const Term& lhs, const Term& rhs)
  {
  Term r;

  r = "(" + lhs.name + " + " + rhs.name + ")";
  return r;
  }

const Term
operator-(const Term& lhs, const Term& rhs)
  {
  Term r;

  r = "(" + lhs.name + " - " + rhs.name + ")";
  return r;
  }

const Term
operator*(const Term& lhs, const Term& rhs)
  {
  Term r;

  r = lhs.name + " * " + rhs.name;
  return r;
  }

std::ostream&
operator<<(std::ostream& out, const Term& rhs)
  {
  out << rhs.Get();
  }

