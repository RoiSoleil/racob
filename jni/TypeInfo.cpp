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
#include "stdafx.h"
#include <objbase.h>
#include "Dispatch.h"
// Win32 support for Ole Automation
#include <wchar.h>
#include <string.h>
#include <atlbase.h>
#include <oleauto.h>
#include <olectl.h>
#include "util.h"

extern "C" {

#define P_TYPEINFO_FLD "m_pTypeInfo"

 // Total number of names to return from GetNames
#define MAX_NAMES 50

 // extract a ITypeInfo from a jobject
ITypeInfo *extractTypeInfo(JNIEnv *env, jobject arg)
{
  jclass argClass = env->GetObjectClass(arg);
  jfieldID ajf = env->GetFieldID( argClass, P_TYPEINFO_FLD, "I");
  
  return (ITypeInfo *) env->GetIntField(arg, ajf);
}

 JNIEXPORT jobject JNICALL Java_com_jacob_com_TypeInfo_getContainingTypeLib
  (JNIEnv *env, jobject obj)
 {
   ITypeInfo *typeInfo = extractTypeInfo(env, obj);
   ITypeLib* typeLib = NULL;
   unsigned int index = 0;
   HRESULT hr = typeInfo->GetContainingTypeLib(&typeLib, &index);
   if (!SUCCEEDED(hr)) {
      ThrowComFail(env, "GetContainingTypeLib failed", hr);
      return NULL;
   }

   int typeCount = typeLib->GetTypeInfoCount();
   if (typeCount == E_NOTIMPL) {
      ThrowComFail(env, "GetContainingTypeLib failed to get count", hr);
      return NULL;
   }

 TLIBATTR *libAttr = NULL;
 hr = typeLib->GetLibAttr(&libAttr);
 if (!SUCCEEDED(hr)) {
    ThrowComFail(env, "Automation.loadTypeLibEx failed", hr);
    return NULL;
 }

 jstring guid = makeGUIDString(env, libAttr->guid);
 jclass autoClass = env->FindClass("com/jacob/com/TypeLib");
 jmethodID autoCons = env->GetMethodID(autoClass, "<init>", "(ILjava/lang/String;IIII)V");
 jobject newAuto = env->NewObject(autoClass, autoCons, (jint) typeLib,
         guid, typeCount, libAttr->wLibFlags, libAttr->wMajorVerNum,
         libAttr->wMinorVerNum);

 typeLib->ReleaseTLibAttr(libAttr);

 return newAuto;
 }

  JNIEXPORT jobject JNICALL Java_com_jacob_com_TypeInfo_getDocumentation
  (JNIEnv *env, jobject obj, jint index)
 {
   ITypeInfo *typeInfo = extractTypeInfo(env, obj);
   BSTR name;
   BSTR docString;
   BSTR helpFile;
   HRESULT hr = typeInfo->GetDocumentation(index, &name, &docString, NULL,
           &helpFile);
   if (!SUCCEEDED(hr)) {
      freeDocumentationStrings(name, docString, helpFile);
      ThrowComFail(env, "getDocumentation failed", hr);
      return NULL;
   }

   jclass autoClass = env->FindClass("com/jacob/com/Documentation");
   jmethodID autoCons = env->GetMethodID(autoClass, "<init>",
           "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
   jobject newAuto = env->NewObject(autoClass, autoCons, makeString(env, name),
           makeString(env, docString), makeString(env, helpFile));

   freeDocumentationStrings(name, docString, helpFile);

   return newAuto;
 }

 JNIEXPORT jobject JNICALL Java_com_jacob_com_TypeInfo_getFuncDesc
  (JNIEnv *env, jobject obj, jint index)
 {
   ITypeInfo *typeInfo = extractTypeInfo(env, obj);

   FUNCDESC *funcDesc = NULL;
   HRESULT hr = typeInfo->GetFuncDesc(index, &funcDesc);
   if (!SUCCEEDED(hr)) {
      ThrowComFail(env, "getFuncDesc failed", hr);
      return NULL;
   }

   jclass autoClass = env->FindClass("com/jacob/com/FuncDesc");
   jmethodID autoCons = env->GetMethodID(autoClass, "<init>", "(I)V");
   jobject newAuto = env->NewObject(autoClass, autoCons, funcDesc->memid);

   typeInfo->ReleaseFuncDesc(funcDesc);

   return newAuto;
 }

JNIEXPORT jobjectArray JNICALL Java_com_jacob_com_TypeInfo_getNames
  (JNIEnv *env, jobject obj, jint memid)
{
   ITypeInfo *typeInfo = extractTypeInfo(env, obj);
   BSTR names[MAX_NAMES];
   unsigned int namesCount;
   HRESULT hr = typeInfo->GetNames(memid, names, MAX_NAMES, &namesCount);
   if (!SUCCEEDED(hr)) {
      ThrowComFail(env, "getNames failed", hr);
      return NULL;
   }

   // Make primitive String array for names
   jclass autoClass = env->FindClass("java/lang/String");
   jobjectArray array = env->NewObjectArray(namesCount, autoClass, NULL);
   if (!SUCCEEDED(hr)) {
      ThrowComFail(env, "getNames failed to make return array", hr);
      return NULL;
   }

   // Fill up array
   for (unsigned int i = 0; i < namesCount; i++) {
      int len = SysStringLen(names[i]);
      jstring string = env->NewString((const jchar *) names[i], len);
      env->SetObjectArrayElement(array, i, string);
      SysFreeString(names[i]);
   }

   return array;
}

JNIEXPORT jint JNICALL Java_com_jacob_com_TypeInfo_getRefTypeOfImplType
  (JNIEnv *env, jobject obj, jint index)
{
  ITypeInfo *typeInfo = extractTypeInfo(env, obj);
  HREFTYPE href;
  HRESULT hr = typeInfo->GetRefTypeOfImplType(index, &href);
  if (!SUCCEEDED(hr)) {
     ThrowComFail(env, "getRefTypeOfImplType failed", hr);
     return NULL;
  }

  return (jint) href;
}

JNIEXPORT jobject JNICALL Java_com_jacob_com_TypeInfo_getRefTypeInfo
  (JNIEnv *env, jobject obj, jint reftype)
{
   ITypeInfo *typeInfo = extractTypeInfo(env, obj);
   ITypeInfo *newTypeInfo = NULL;
   HRESULT hr = typeInfo->GetRefTypeInfo(reftype, &newTypeInfo);
   if (!SUCCEEDED(hr)) {
      ThrowComFail(env, "getRefTypeInfo failed", hr);
      return NULL;
   }

   return makeTypeInfo(env, typeInfo);
}

 JNIEXPORT jobject JNICALL Java_com_jacob_com_TypeInfo_getVarDesc
  (JNIEnv *env, jobject obj, jint index)
 {
   ITypeInfo *typeInfo = extractTypeInfo(env, obj);
   VARDESC *varDesc = NULL;
   HRESULT hr = typeInfo->GetVarDesc(index, &varDesc);
   if (!SUCCEEDED(hr)) {
      ThrowComFail(env, "getVarDesc failed", hr);
      return NULL;
   }

   // We have a constant so let's save the variant
   jobject cValue = NULL;
   if (varDesc->varkind == VAR_CONST) {
       jclass variantClass = env->FindClass("com/jacob/com/Variant");
       jmethodID variantCons = env->GetMethodID(variantClass, "<init>", "()V");
       cValue = env->NewObject(variantClass, variantCons);
       VARIANT *v = extractVariant(env, cValue);
       hr = VariantCopy(v, varDesc->lpvarValue);
       if (!SUCCEEDED(hr)) {
          ThrowComFail(env, "getVarDesc failed to copy constant", hr);
          return NULL;
       }
   }

   jclass autoClass = env->FindClass("com/jacob/com/VarDesc");
   jmethodID autoCons = env->GetMethodID(autoClass, "<init>", "(ILcom/jacob/com/Variant;I)V");
   jobject newAuto = env->NewObject(autoClass, autoCons, varDesc->memid,
           cValue, varDesc->varkind);

   typeInfo->ReleaseVarDesc(varDesc);

   return newAuto;
 }
}