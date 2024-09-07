#include "Precompiled.h"
#include "SaveUtil.h"

using namespace SpringEngine;
using namespace SpringEngine::Graphics;
using namespace SpringEngine::Math;

void SaveUtil::SaveVector3(const char* key, const Math::Vector3& v, rapidjson::Document& doc, rapidjson::Value& member)
{
    rapidjson::GenericStringRef<char> str(key);
    rapidjson::Value vecArray(rapidjson::kArrayType);
    vecArray.PushBack(v.x, doc.GetAllocator());
    vecArray.PushBack(v.y, doc.GetAllocator());
    vecArray.PushBack(v.z, doc.GetAllocator());
    member.AddMember(str, vecArray, doc.GetAllocator());
}

void SaveUtil::SaveQuaternion(const char* key, const Math::Quaternion& q, rapidjson::Document& doc, rapidjson::Value& member)
{
    rapidjson::GenericStringRef<char> str(key);
    rapidjson::Value quatArray(rapidjson::kArrayType);
    quatArray.PushBack(q.x, doc.GetAllocator());
    quatArray.PushBack(q.y, doc.GetAllocator());
    quatArray.PushBack(q.z, doc.GetAllocator());
    quatArray.PushBack(q.w, doc.GetAllocator());
    member.AddMember(str, quatArray, doc.GetAllocator());
}

void SaveUtil::SaveColor(const char* key, const Color& c, rapidjson::Document& doc, rapidjson::Value& member)
{
    rapidjson::GenericStringRef<char> str(key);
    rapidjson::Value colorArray(rapidjson::kArrayType);
    colorArray.PushBack(c.r, doc.GetAllocator());
    colorArray.PushBack(c.g, doc.GetAllocator());
    colorArray.PushBack(c.b, doc.GetAllocator());
    colorArray.PushBack(c.a, doc.GetAllocator());
    member.AddMember(str, colorArray, doc.GetAllocator());
}

void SaveUtil::SaveFloat(const char* key, float value, rapidjson::Document& doc, rapidjson::Value& member)
{
    rapidjson::GenericStringRef<char> str(key);
    member.AddMember(str, value, doc.GetAllocator());
}

void SaveUtil::SaveInt(const char* key, int value, rapidjson::Document& doc, rapidjson::Value& member)
{
    rapidjson::GenericStringRef<char> str(key);
    member.AddMember(str, value, doc.GetAllocator());
}

void SaveUtil::SaveBool(const char* key, bool value, rapidjson::Document& doc, rapidjson::Value& member)
{
    rapidjson::GenericStringRef<char> str(key);
    member.AddMember(str, value, doc.GetAllocator());
}

void SaveUtil::SaveString(const char* key, const char* value, rapidjson::Document& doc, rapidjson::Value& member)
{
    rapidjson::GenericStringRef<char> str(key);
    rapidjson::GenericStringRef<char> strValue(value);
    member.AddMember(str, strValue, doc.GetAllocator());
}

void SaveUtil::SaveStringArray(const char* key, const std::vector<std::string>& values, rapidjson::Document& doc, rapidjson::Value& member)
{
    rapidjson::GenericStringRef<char> str(key);
    rapidjson::Value strArray(rapidjson::kArrayType);
    for (const std::string& v : values)
    {
        rapidjson::GenericStringRef<char> strValue(v.c_str());
        strArray.PushBack(strValue, doc.GetAllocator());
    }
    member.AddMember(str, strArray, doc.GetAllocator());
}