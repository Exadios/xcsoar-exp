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

#include "NativeInputListener.hpp"
#include "io/DataHandler.hpp"
#include "java/Array.hxx"
#include "java/Class.hxx"
#include "au_org_narroginglidingclub_xcsoar_NativeInputListener.h"

#include <cstddef>

namespace NativeInputListener {
static Java::TrivialClass cls;
static jmethodID ctor;
static jfieldID ptr_field;
} // namespace NativeInputListener

JNIEXPORT void JNICALL
Java_au_org_narroginglidingclub_xcsoar_NativeInputListener_dataReceived(JNIEnv *env,
                                                 jobject obj,
                                                 jbyteArray data,
                                                 jint length)
{
  jlong ptr = env->GetLongField(obj, NativeInputListener::ptr_field);
  if (ptr == 0)
    /* not yet set */
    return;

  DataHandler &handler = *(DataHandler *)(void *)ptr;

  const Java::ByteArrayElements elems{env, data};
  handler.DataReceived({(const std::byte *)elems.get(), std::size_t(length)});
}

void
NativeInputListener::Initialise(JNIEnv *env)
{
  cls.Find(env, "au/org/narroginglidingclub/xcsoar/NativeInputListener");

  ctor = env->GetMethodID(cls, "<init>", "(J)V");
  ptr_field = env->GetFieldID(cls, "ptr", "J");
}

void
NativeInputListener::Deinitialise(JNIEnv *env)
{
  cls.Clear(env);
}

jobject
NativeInputListener::Create(JNIEnv *env, DataHandler &handler)
{
  assert(cls != nullptr);

  return env->NewObject(cls, ctor, (jlong)&handler);
}
