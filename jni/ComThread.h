/*
 * Copyright (c) 1999-2004 Sourceforge JACOB Project.
 * All rights reserved. Originator: Dan Adler (http://danadler.com).
 * Get more information about JACOB at http://sourceforge.net/projects/jacob-project
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <jni.h>
/* Header for class org_racob_com_ComThread */

#ifndef _Included_org_racob_com_ComThread
#define _Included_org_racob_com_ComThread
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_racob_com_ComThread
 * Method:    doCoInitialize
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_racob_com_ComThread_doCoInitialize
  (JNIEnv *, jclass, jint);

/*
 * Class:     org_racob_com_ComThread
 * Method:    doCoUninitialize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_racob_com_ComThread_doCoUninitialize
  (JNIEnv *, jclass);


#ifdef __cplusplus
}
#endif
#endif
