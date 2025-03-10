/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2025 The XCSoar Project
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

#include "IOIOHelper.hpp"
#include "PortBridge.hpp"
#include "java/Class.hxx"
#include "java/Env.hxx"

Java::TrivialClass IOIOHelper::cls;
jmethodID IOIOHelper::ctor,
  IOIOHelper::openUart_method,
  IOIOHelper::shutdown_method;

bool
IOIOHelper::Initialise(JNIEnv *env)
{
  assert(!cls.IsDefined());
  assert(env != nullptr);

  if (!cls.FindOptional(env, "au/org/narroginglidingclub/xcsoar/IOIOHelper"))
    return false;

  ctor = env->GetMethodID(cls, "<init>", "()V");
  if (Java::DiscardException(env)) {
    /* need to check for Java exceptions again because the first
       method lookup initializes the Java class */
    cls.Clear(env);
    return false;
  }

  openUart_method = env->GetMethodID(cls, "openUart",
                                     "(II)Lau/org/narroginglidingclub/xcsoar/AndroidPort;");
  shutdown_method = env->GetMethodID(cls, "shutdown", "()V");

  return true;
}

void
IOIOHelper::Deinitialise(JNIEnv *env)
{
  cls.ClearOptional(env);
}

PortBridge *
IOIOHelper::openUart(JNIEnv *env, unsigned ID, unsigned baud)
{
  auto obj = Java::CallObjectMethodRethrow(env, Get(), openUart_method,
                                           ID, (int)baud);
  if (obj == nullptr)
    return nullptr;

  return new PortBridge(env, obj);
}

IOIOHelper::IOIOHelper(JNIEnv *env)
  :Java::GlobalObject(Java::NewObjectRethrow(env, cls, ctor))
{
}
