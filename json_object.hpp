#include "json.hpp"
#include <iostream>
#include <list>
#include <vector>


/** ����ֶ� */
#define JsonAddField(field) addField(#field, field)
/** ��Ӷ���(����ʵ��JsonBaseObject�ӿ�) */
#define JsonAddObject(object) addObject(#object, object)
/** ��Ӷ�������(����ʵ��JsonBaseObject�ӿ�) */
#define JsonAddObjectArray(objectArray) addObjectArray(#objectArray, objectArray)

/** ��ȡ�ֶ� */
#define JsonGetField(p)        getField(#p, p)
/** ��ȡ���� */
#define JsonGetArray(p)        getArray(#p, p)
/** ��ȡ���� */
#define JsonGetObject(p)       getObject(#p, p)
/** ��ȡ�������� */
#define JsonGetObjectArray(p)  getObjectArray(#p, p)


/** Json���л�/�����л� ������ */
class JsonBaseObject {

public:
    JsonBaseObject() { }

    virtual~JsonBaseObject() { }

    /** �����Ҫ���л����ֶ� JsonAddField/JsonAddObject/JsonAddObjectArray */
    virtual void addJsonData() = 0;

    /** ��ȡ�������json���� */
    nlohmann::json toJson() {
        this->addJsonData();
        return _m_object_nlohmann_json;
    }

    /** ��ȡJson�ַ��� */
    std::string toJsonString(
        const int indent = -1,
        const char indent_char = ' ',
        const bool ensure_ascii = false) {
        this->toJson();
        return _m_object_nlohmann_json.dump(indent, indent_char, ensure_ascii);
    }

    /** ����ֶ� */
    template<typename T>
    nlohmann::json addField(const std::string& name, T& value) {
        _m_object_nlohmann_json[name] = value;
        return _m_object_nlohmann_json;
    }

    /** ��Ӷ��� */
    template<typename T>
    nlohmann::json addObject(const std::string& name, T& value) {
        _m_object_nlohmann_json[name] = value.toJson();
        return _m_object_nlohmann_json;
    }

    /** ���std::list */
    template<typename T>
    nlohmann::json addObjectArray(const std::string& name, std::list<T>& value) {
        nlohmann::json arr;
        for (typename std::list<T>::iterator it = value.begin(); it != value.end(); it++) {
            arr.push_back(it->toJson());
        }

        _m_object_nlohmann_json[name] = arr;
        return _m_object_nlohmann_json;
    }

    /** ���std::vector */
    template<typename T>
    nlohmann::json addObjectArray(const std::string& name, std::vector<T>& value) {
        nlohmann::json arr;
        for (typename std::vector<T>::iterator it = value.begin(); it != value.end(); it++) {
            arr.push_back(it->toJson());
        }

        _m_object_nlohmann_json[name] = arr;
        return _m_object_nlohmann_json;
    }


public:
    /** �����Ҫ�����л����ֶ� JsonGetField/JsonGetObject/JsonGetObjectArray */
    virtual void parseJsonData() = 0;

    /** ���ñ������json���� */
    void fromJson(nlohmann::json& objJson) {
        _m_object_nlohmann_json = objJson;

        parseJsonData();
    }

    /** ����Json�ַ��� */
    bool parseJsonString(const std::string& data) {
        try
        {
            nlohmann::json obj = nlohmann::json::parse(data);
            fromJson(obj);

            return true;
        }
        catch (const std::exception&)
        {
            return false;
        }
    }

    /** ��ȡJson�ֶ� */
    template<typename T>
    void getField(const std::string& name, T& value) {
        value = _m_object_nlohmann_json[name];
    }

    /** ��ȡJson���� */
    template<typename T>
    void getObject(const std::string& name, T& value) {
        value.fromJson(_m_object_nlohmann_json[name]);
    }

    template<typename T>
    void getArray(const std::string& name, std::list<T>& value) {
        auto obj = _m_object_nlohmann_json[name];

        for (nlohmann::json::iterator it = obj.begin(); it != obj.end(); it++) {
            T t = *it;

            value.push_back(t);
        }
    }

    /** ��ȡJson�����б� */
    template<typename T>
    void getObjectArray(const std::string& name, std::list<T>& value) {
        auto obj = _m_object_nlohmann_json[name];

        for (nlohmann::json::iterator it = obj.begin(); it != obj.end(); it++) {
            T t;
            t.fromJson(*it);

            value.push_back(t);
        }
    }

    /** ��ȡJson�������� */
    template<typename T>
    void getObjectArray(const std::string& name, std::vector<T>& value) {
        auto obj = _m_object_nlohmann_json[name];

        for (nlohmann::json::iterator it = obj.begin(); it != obj.end(); it++) {
            T t;
            t.fromJson(*it);

            value.push_back(t);
        }
    }

private:
    /** nlohmann::json ���� */
    nlohmann::json _m_object_nlohmann_json;
};

