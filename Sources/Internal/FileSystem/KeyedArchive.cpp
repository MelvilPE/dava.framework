#include "FileSystem/KeyedArchive.h"
#include "FileSystem/File.h"
#include "FileSystem/DynamicMemoryFile.h"
#include "FileSystem/UnmanagedMemoryFile.h"
#include "FileSystem/YamlParser.h"
#include "FileSystem/YamlNode.h"
#include "FileSystem/VariantType.h"
#include "FileSystem/YamlEmitter.h"
#include "FileSystem/Private/KeyedArchiveReflection.h"
#include "Reflection/ReflectionRegistrator.h"

#include "Logger/Logger.h"

namespace DAVA
{
VariantType PrepareValueForKeyedArchive(const Any& value, VariantType::eVariantType resultType)
{
    return PrepareValueForKeyedArchiveImpl(value, resultType);
}

DAVA_VIRTUAL_REFLECTION_IMPL(KeyedArchive)
{
    ReflectionRegistrator<KeyedArchive>::Begin(std::make_unique<KeyedArchiveStructureWrapper>())
    .End();
}

KeyedArchive::KeyedArchive()
{
}

KeyedArchive::KeyedArchive(const KeyedArchive& arc)
{
    for (const auto& obj : arc.GetArchieveData())
    {
        SetVariant(obj.first, *obj.second);
    }
}

KeyedArchive& KeyedArchive::operator=(const KeyedArchive& arc)
{
    if (this != &arc)
    {
        DeleteAllKeys();
        for (const auto& obj : arc.GetArchieveData())
        {
            SetVariant(obj.first, *obj.second);
        }
    }

    return *this;
}

KeyedArchive::~KeyedArchive()
{
    DeleteAllKeys();
}

bool KeyedArchive::Load(const FilePath& pathName)
{
    File* archive = File::Create(pathName, File::OPEN | File::READ);
    if (nullptr == archive)
    {
        return false;
    }
    bool ret = Load(archive);
    SafeRelease(archive);

    return ret;
}

bool KeyedArchive::Load(File* archive, KeyedArchive* registry)
{
    DAVA::Array<char, 2> header;
    uint32 wasRead = archive->Read(header.data(), 2);
    if (wasRead != 2)
    {
        Logger::Error("[KeyedArchive] error loading keyed archive from file: %s, filesize: %d", archive->GetFilename().GetAbsolutePathname().c_str(), archive->GetSize());
        return false;
    }

    if (header[0] != 'K' || header[1] != 'A')
    {
        Logger::Error("[KeyedArchive] error loading keyed archive signature from file: %s, filesize: %d", archive->GetFilename().GetAbsolutePathname().c_str(), archive->GetSize());
        return false;
    }

    eVersion version = eVersion::Legacy;
    if (sizeof(version) != archive->Read(&version, sizeof(version)))
    {
        Logger::Error("[KeyedArchive] failed reading version from file: %s, filesize: %d", archive->GetFilename().GetAbsolutePathname().c_str(), archive->GetSize());
        return false;
    }

    eRegisteredMapTag tag = eRegisteredMapTag::EmptyArchive;
    if (sizeof(tag) != archive->Read(&tag, sizeof(tag)))
    {
        Logger::Error("[KeyedArchive] failed reading registered map tag from file: %s, filesize: %d", archive->GetFilename().GetAbsolutePathname().c_str(), archive->GetSize());
        return false;
    }

    if (version != eVersion::StringMap && version != eVersion::RegisterMap)
    {
        Logger::Error("[KeyedArchive] error loading keyed archive, because version is incorrect");
        return false;
    }

    if (version == eVersion::StringMap)
    {
        uint32 numberOfItems = 0;
        if (sizeof(numberOfItems) != archive->Read(&numberOfItems, sizeof(numberOfItems)))
        {
            Logger::Error("[KeyedArchive] error loading keyed archive body, failed to read StringMap number of items");
            return false;
        }
        for (uint32 item = 0; item < numberOfItems; ++item)
        {
            VariantType key;

            if (archive->IsEof())
            {
                break;
            }
            if (!key.Read(archive))
            {
                Logger::Error("[KeyedArchive] error loading keyed archive body, failed to read a StringMap key");
                return false;
            }
            VariantType value;
            if (!value.Read(archive))
            {
                Logger::Error("[KeyedArchive] error loading keyed archive body, failed to read a StringMap value");
                return false;
            }

            SetVariant(key.AsString(), std::move(value));
        }

        return true;
    }

    if (version == eVersion::RegisterMap)
    {
        if (tag == eRegisteredMapTag::EmptyArchive)
        {
            return true;
        }

        if (tag == eRegisteredMapTag::TopArchive)
        {
            uint32 numberOfKeys = 0;
            if (sizeof(numberOfKeys) != archive->Read(&numberOfKeys, sizeof(numberOfKeys)))
            {
                Logger::Error("[KeyedArchive] error loading keyed archive, failed to read number of keys in TopArchive");
                return false;
            }

            Vector<String> keys;
            for (uint32 item = 0; item < numberOfKeys; ++item)
            {
                uint16 keySize = 0;
                if (sizeof(keySize) != archive->Read(&keySize, sizeof(keySize)))
                {
                    Logger::Error("[KeyedArchive] error loading keyed archive, failed to read keySize in TopArchive");
                    return false;
                }

                String key = "";
                if (keySize > 0)
                {
                    Vector<char> buffer;
                    for (uint16 charIndex = 0; charIndex < keySize; charIndex++)
                    {
                        char character;
                        if (sizeof(character) != archive->Read(&character, sizeof(character)))
                        {
                            Logger::Error("[KeyedArchive] error loading keyed archive, failed to read a string key character in TopArchive");
                            return false;
                        }

                        buffer.push_back(character);
                    }

                    key = String(buffer.begin(), buffer.end());
                }

                keys.push_back(key);
            }

            Vector<uint32> keyHashes;
            for (uint32 item = 0; item < numberOfKeys; ++item)
            {
                uint32 keyHash = 0;
                if (sizeof(keyHash) != archive->Read(&keyHash, sizeof(keyHash)))
                {
                    Logger::Error("[KeyedArchive] error loading keyed archive, failed to read keyHash in TopArchive");
                    return false;
                }

                keyHashes.push_back(keyHash);
            }

            // We want to use keyHash value to get back the key in itself
            ScopedPtr<KeyedArchive> registry(new KeyedArchive());
            for (uint32 item = 0; item < numberOfKeys; ++item)
            {
                VariantType key;
                key.SetString(std::to_string(keyHashes[item]));
                registry->SetString(key.AsString(), keys[item]);
            }

            uint32 numberOfItems = 0;
            if (sizeof(numberOfItems) != archive->Read(&numberOfItems, sizeof(numberOfItems)))
            {
                Logger::Error("[KeyedArchive] error loading keyed archive body, failed to read number of items in TopArchive");
                return false;
            }

            for (uint32 item = 0; item < numberOfItems; ++item)
            {
                uint32 keyHash = 0;
                if (sizeof(keyHash) != archive->Read(&keyHash, sizeof(keyHash)))
                {
                    Logger::Error("[KeyedArchive] error loading keyed archive body, failed to read keyHash in TopArchive");
                    return false;
                }

                VariantType key;
                key.SetString(registry->GetString(std::to_string(keyHash)));

                VariantType value;
                if (!value.Read(archive, registry))
                {
                    Logger::Error("[KeyedArchive] error loading keyed archive body, failed to read a TopArchive variant value");
                    return false;
                }

                SetVariant(key.AsString(), std::move(value));
            }

            return true;
        }

        if (tag == eRegisteredMapTag::SubArchive)
        {
            if (!registry)
            {
                Logger::Error("[KeyedArchive] error loading keyed archive body, registry is not initialized");
                return false;
            }

            uint32 numberOfItems = 0;
            if (sizeof(numberOfItems) != archive->Read(&numberOfItems, sizeof(numberOfItems)))
            {
                Logger::Error("[KeyedArchive] error loading keyed archive body, failed to read SubArchive number of items");
                return false;
            }

            for (uint32 item = 0; item < numberOfItems; ++item)
            {
                uint32 keyHash = 0;
                if (sizeof(keyHash) != archive->Read(&keyHash, sizeof(keyHash)))
                {
                    Logger::Error("[KeyedArchive] error loading keyed archive body, failed to read keyHash in SubArchive");
                    return false;
                }

                VariantType key;
                key.SetString(registry->GetString(std::to_string(keyHash)));

                VariantType value;
                if (!value.Read(archive, registry))
                {
                    Logger::Error("[KeyedArchive] error loading keyed archive body, failed to read a SubArchive variant value");
                    return false;
                }

                SetVariant(key.AsString(), std::move(value));
            }

            return true;
        }
    }

    Logger::Error("[KeyedArchive] error loading keyed archive, archive version is not supported: %d", static_cast<int>(version));
    return false;
}

bool KeyedArchive::Save(const FilePath& pathName) const
{
    File* archive = File::Create(pathName, File::CREATE | File::WRITE);
    if (nullptr == archive)
    {
        return false;
    }

    bool ret = Save(archive);
    SafeRelease(archive);

    return ret;
}

bool KeyedArchive::Save(File* archive) const
{
    Map<UnderlyingMap::key_type, UnderlyingMap::mapped_type> orderedMap;
    orderedMap.insert(objectMap.begin(), objectMap.end());

    Array<char, 2> header;
    uint16 version = 1;
    uint32 size = static_cast<uint32>(orderedMap.size());

    header[0] = 'K';
    header[1] = 'A';

    if (2 != archive->Write(header.data(), 2)
        || 2 != archive->Write(&version, 2)
        || 4 != archive->Write(&size, 4))
    {
        return false;
    }
    for (const auto& obj : orderedMap)
    {
        VariantType key;
        key.SetString(obj.first);
        if (!key.Write(archive)
            || !obj.second->Write(archive))
        {
            return false;
        }
    }

    return archive->Flush();
}

uint32 KeyedArchive::Save(uint8* data, uint32 size) const
{
    ScopedPtr<DynamicMemoryFile> buffer(DynamicMemoryFile::Create(File::CREATE | File::WRITE));

    if (!Save(buffer))
    {
        Logger::Error("failed to write KeyedArchive to memory_file");
    }

    auto archieveSize = buffer->GetSize();
    if ((nullptr != data) && (size >= archieveSize))
    { // if data is null, we just return requested size for data
        Memcpy(data, buffer->GetData(), static_cast<size_t>(archieveSize));
    }
    return static_cast<uint32>(archieveSize);
}

bool KeyedArchive::Load(const uint8* data, uint32 size)
{
    if (nullptr == data || 0 == size)
    {
        return false;
    }

    ScopedPtr<DynamicMemoryFile> buffer(DynamicMemoryFile::Create(File::CREATE | File::WRITE | File::READ));
    auto written = buffer->Write(data, size);
    DVASSERT(written == size);

    buffer->Seek(0, File::SEEK_FROM_START);

    return Load(buffer);
}

bool KeyedArchive::LoadFromYamlFile(const FilePath& pathName)
{
    RefPtr<YamlParser> parser(YamlParser::Create(pathName));
    return (parser.Get() != nullptr) && LoadFromYamlNode(parser->GetRootNode());
}

bool KeyedArchive::LoadFromYamlString(String inputString)
{
    RefPtr<YamlParser> parser(YamlParser::CreateAndParseString(inputString));
    return (parser.Get() != nullptr) && LoadFromYamlNode(parser->GetRootNode());
}

bool KeyedArchive::LoadFromYamlNode(const YamlNode* rootNode)
{
    if (nullptr == rootNode)
    {
        return false;
    }

    const YamlNode* archieveNode = rootNode->Get(VariantType::TYPENAME_KEYED_ARCHIVE);
    if (nullptr == archieveNode)
    {
        return false;
    }

    int32 count = archieveNode->GetCount();
    for (int32 i = 0; i < count; ++i)
    {
        const YamlNode* node = archieveNode->Get(i);
        const String& variableNameToArchMap = archieveNode->GetItemKeyName(i);

        VariantType value(node->AsVariantType());

        if (value.GetType() == VariantType::TYPE_NONE)
        {
            continue;
        }

        SetVariant(variableNameToArchMap, std::move(value));
    }

    return true;
}

bool KeyedArchive::SaveToYamlFile(const FilePath& pathName) const
{
    RefPtr<YamlNode> node(YamlNode::CreateMapNode());
    node->Set(VariantType::TYPENAME_KEYED_ARCHIVE, VariantType(const_cast<KeyedArchive*>(this)));

    return YamlEmitter::SaveToYamlFile(pathName, node.Get());
}

String KeyedArchive::SaveToYamlString() const
{
    RefPtr<YamlNode> node(YamlNode::CreateMapNode());
    node->Set(VariantType::TYPENAME_KEYED_ARCHIVE, VariantType(const_cast<KeyedArchive*>(this)));

    ScopedPtr<DynamicMemoryFile> buffer(DynamicMemoryFile::Create(File::CREATE | File::WRITE));
    if (!YamlEmitter::SaveToYamlFile(node.Get(), buffer))
    {
        Logger::Error("failed to save KeyedArchive to memory_file while saving to yaml string");
        return "";
    }

    const char* buffPtr = reinterpret_cast<const char*>(buffer->GetData());
    String result = String(buffPtr, buffer->GetSize());
    return result;
}

void KeyedArchive::SetBool(const String& key, bool value)
{
    SetVariant(key, value, &VariantType::SetBool);
}

void KeyedArchive::SetInt32(const String& key, int32 value)
{
    SetVariant(key, value, &VariantType::SetInt32);
}

void KeyedArchive::SetUInt32(const String& key, uint32 value)
{
    SetVariant(key, value, &VariantType::SetUInt32);
}

void KeyedArchive::SetFloat(const String& key, float32 value)
{
    SetVariant(key, value, &VariantType::SetFloat);
}

void KeyedArchive::SetFloat64(const String& key, float64 value)
{
    SetVariant(key, value, &VariantType::SetFloat64);
}

void KeyedArchive::SetString(const String& key, const String& value)
{
    SetVariant(key, value, &VariantType::SetString);
}

void KeyedArchive::SetWideString(const String& key, const WideString& value)
{
    SetVariant(key, value, &VariantType::SetWideString);
}

void KeyedArchive::SetFastName(const String& key, const FastName& value)
{
    SetVariant(key, value, &VariantType::SetFastName);
}

void KeyedArchive::SetByteArray(const String& key, const uint8* value, int32 arraySize)
{
    auto iter = objectMap.find(key);
    if (iter != objectMap.end())
    {
        (iter->second->SetByteArray)(value, arraySize);
    }
    else
    {
        objectMap[key] = new VariantType(value, arraySize);
    }
}

void KeyedArchive::SetVariant(const String& key, const VariantType& value)
{
    auto iter = objectMap.find(key);
    if (iter != objectMap.end())
    {
        *iter->second = value;
    }
    else
    {
        objectMap[key] = new VariantType(value);
    }
}

void KeyedArchive::SetVariant(const String& key, VariantType&& value)
{
    auto iter = objectMap.find(key);
    if (iter != objectMap.end())
    {
        *iter->second = std::move(value);
    }
    else
    {
        objectMap[key] = new VariantType(std::move(value));
    }
}

void KeyedArchive::SetByteArrayFromArchive(const String& key, KeyedArchive* archive)
{
    //DVWARNING(false, "Method is depriceted! Use SetArchive()");
    DynamicMemoryFile* file = DynamicMemoryFile::Create(File::CREATE | File::WRITE);
    if (!archive->Save(file))
    {
        Logger::Error("failed to write KeyedArchive to memory_file");
    }
    SetByteArray(key, file->GetData(), static_cast<uint32>(file->GetSize()));
    SafeRelease(file);
}

void KeyedArchive::SetArchive(const String& key, KeyedArchive* archive)
{
    SetVariant(key, archive, &VariantType::SetKeyedArchive);
}

void KeyedArchive::SetInt64(const String& key, const int64& value)
{
    SetVariant(key, value, &VariantType::SetInt64);
}

void KeyedArchive::SetUInt64(const String& key, const uint64& value)
{
    SetVariant(key, value, &VariantType::SetUInt64);
}

void KeyedArchive::SetVector2(const String& key, const Vector2& value)
{
    SetVariant(key, value, &VariantType::SetVector2);
}

void KeyedArchive::SetVector3(const String& key, const Vector3& value)
{
    SetVariant(key, value, &VariantType::SetVector3);
}

void KeyedArchive::SetVector4(const String& key, const Vector4& value)
{
    SetVariant(key, value, &VariantType::SetVector4);
}

void KeyedArchive::SetMatrix2(const String& key, const Matrix2& value)
{
    SetVariant(key, value, &VariantType::SetMatrix2);
}

void KeyedArchive::SetMatrix3(const String& key, const Matrix3& value)
{
    SetVariant(key, value, &VariantType::SetMatrix3);
}

void KeyedArchive::SetMatrix4(const String& key, const Matrix4& value)
{
    SetVariant(key, value, &VariantType::SetMatrix4);
}

void KeyedArchive::SetColor(const String& key, const Color& value)
{
    SetVariant(key, value, &VariantType::SetColor);
}

void KeyedArchive::SetRect(const String& key, const Rect& value)
{
    SetVariant(key, value, &VariantType::SetRect);
}

void KeyedArchive::SetVariantVector(const String& key, const Vector<VariantType>& value)
{
    SetVariant(key, value, &VariantType::SetVariantVector);
}

void KeyedArchive::SetQuaternion(const String& key, const Quaternion& value)
{
    SetVariant(key, value, &VariantType::SetQuaternion);
}

void KeyedArchive::SetTransform(const String& key, const Transform& value)
{
    SetVariant(key, value, &VariantType::SetTransform);
}

void KeyedArchive::SetAABBox2(const String& key, const AABBox2& value)
{
    SetVariant(key, value, &VariantType::SetAABBox2);
}

bool KeyedArchive::IsKeyExists(const String& key) const
{
    auto it = objectMap.find(key);
    if (it != objectMap.end())
    {
        return true;
    }
    return false;
}

bool KeyedArchive::GetBool(const String& key, bool defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsBool() : defaultValue;
}

int32 KeyedArchive::GetInt32(const String& key, int32 defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsInt32() : defaultValue;
}

uint32 KeyedArchive::GetUInt32(const String& key, uint32 defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsUInt32() : defaultValue;
}

float32 KeyedArchive::GetFloat(const String& key, float32 defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsFloat() : defaultValue;
}

float64 KeyedArchive::GetFloat64(const String& key, float64 defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsFloat64() : defaultValue;
}

String KeyedArchive::GetString(const String& key, const String& defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsString() : defaultValue;
}

WideString KeyedArchive::GetWideString(const String& key, const WideString& defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsWideString() : defaultValue;
}

FastName KeyedArchive::GetFastName(const String& key, const FastName& defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsFastName() : defaultValue;
}

const uint8* KeyedArchive::GetByteArray(const String& key, const uint8* defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsByteArray() : defaultValue;
}
int32 KeyedArchive::GetByteArraySize(const String& key, int32 defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsByteArraySize() : defaultValue;
}

KeyedArchive* KeyedArchive::GetArchiveFromByteArray(const String& key) const
{
    //DVWARNING(false, "Method is depriceted! Use GetArchive()");
    int32 size = GetByteArraySize(key);
    if (size == 0)
    {
        return nullptr;
    }

    KeyedArchive* archive = new KeyedArchive;
    ScopedPtr<UnmanagedMemoryFile> file(new UnmanagedMemoryFile(GetByteArray(key), size));
    if (!archive->Load(file))
    {
        SafeRelease(archive);
        return nullptr;
    }
    return archive;
}

KeyedArchive* KeyedArchive::GetArchive(const String& key, KeyedArchive* defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsKeyedArchive() : defaultValue;
}

VariantType* KeyedArchive::GetVariant(const String& key) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second : nullptr;
}

int64 KeyedArchive::GetInt64(const String& key, int64 defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsInt64() : defaultValue;
}

uint64 KeyedArchive::GetUInt64(const String& key, uint64 defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsUInt64() : defaultValue;
}

Vector2 KeyedArchive::GetVector2(const String& key, const Vector2& defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsVector2() : defaultValue;
}

Vector3 KeyedArchive::GetVector3(const String& key, const Vector3& defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsVector3() : defaultValue;
}

Vector4 KeyedArchive::GetVector4(const String& key, const Vector4& defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsVector4() : defaultValue;
}

Matrix2 KeyedArchive::GetMatrix2(const String& key, const Matrix2& defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsMatrix2() : defaultValue;
}

Matrix3 KeyedArchive::GetMatrix3(const String& key, const Matrix3& defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsMatrix3() : defaultValue;
}

Matrix4 KeyedArchive::GetMatrix4(const String& key, const Matrix4& defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsMatrix4() : defaultValue;
}

Color KeyedArchive::GetColor(const String& key, const Color& defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsColor() : defaultValue;
}

Rect KeyedArchive::GetRect(const String& key, const Rect& defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsRect() : defaultValue;
}

Vector<VariantType> KeyedArchive::GetVariantVector(const String& key, const Vector<VariantType>& defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsVariantVector() : defaultValue;
}

Quaternion KeyedArchive::GetQuaternion(const String& key, const Quaternion& defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsQuaternion() : defaultValue;
}

Transform KeyedArchive::GetTransform(const String& key, const Transform& defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsTransform() : defaultValue;
}

AABBox2 KeyedArchive::GetAABBox2(const String& key, const AABBox2& defaultValue) const
{
    auto it = objectMap.find(key);
    return it != objectMap.end() ? it->second->AsAABBox2() : defaultValue;
}

void KeyedArchive::DeleteKey(const String& key)
{
    auto it = objectMap.find(key);
    if (it != objectMap.end())
    {
        delete it->second;
        objectMap.erase(key);
    }
}

void KeyedArchive::DeleteAllKeys()
{
    for (const auto& obj : objectMap)
    {
        delete obj.second;
    }
    objectMap.clear();
}

uint32 KeyedArchive::Count(const String& key) const
{
    if (key.empty())
    {
        return static_cast<uint32>(objectMap.size());
    }
    else
    {
        return static_cast<uint32>(objectMap.count(key));
    }
}

void KeyedArchive::Dump() const
{
    Logger::FrameworkDebug("============================================================");
    Logger::FrameworkDebug("--------------- Archive Currently contain ----------------");
    for (const auto& obj : objectMap)
    {
        switch (obj.second->GetType())
        {
        case VariantType::TYPE_BOOLEAN:
        {
            if (obj.second->boolValue)
            {
                Logger::FrameworkDebug("%s : true", obj.first.c_str());
            }
            else
            {
                Logger::FrameworkDebug("%s : false", obj.first.c_str());
            }
        }
        break;
        case VariantType::TYPE_INT8:
        {
            Logger::FrameworkDebug("%s : %hhd", obj.first.c_str(), obj.second->int8Value);
        }
        break;
        case VariantType::TYPE_UINT8:
        {
            Logger::FrameworkDebug("%s : %hu", obj.first.c_str(), obj.second->uint8Value);
        }
        break;
        case VariantType::TYPE_INT16:
        {
            Logger::FrameworkDebug("%s : %hd", obj.first.c_str(), obj.second->int16Value);
        }
        break;
        case VariantType::TYPE_UINT16:
        {
            Logger::FrameworkDebug("%s : %hu", obj.first.c_str(), obj.second->uint16Value);
        }
        break;
        case VariantType::TYPE_INT32:
        {
            Logger::FrameworkDebug("%s : %d", obj.first.c_str(), obj.second->int32Value);
        }
        break;
        case VariantType::TYPE_UINT32:
        {
            Logger::FrameworkDebug("%s : %u", obj.first.c_str(), obj.second->uint32Value);
        }
        break;
        case VariantType::TYPE_FLOAT:
        {
            Logger::FrameworkDebug("%s : %f", obj.first.c_str(), obj.second->floatValue);
        }
        break;
        case VariantType::TYPE_STRING:
        {
            Logger::FrameworkDebug("%s : %s", obj.first.c_str(), obj.second->stringValue->c_str());
        }
        break;
        case VariantType::TYPE_WIDE_STRING:
        {
            Logger::FrameworkDebug("%s : %S", obj.first.c_str(), obj.second->wideStringValue->c_str());
        }
        break;

        default:
            break;
        }
    }
    Logger::FrameworkDebug("============================================================");
}

const KeyedArchive::UnderlyingMap& KeyedArchive::GetArchieveData() const
{
    return objectMap;
}

const char* KeyedArchive::GenKeyFromIndex(uint32 index)
{
    static char tmpKey[32];

    sprintf(tmpKey, "%04u", index);
    return tmpKey;
}
};
