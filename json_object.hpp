#pragma once

#include <iostream>
#include <list>
#include <vector>

#include "json.hpp"

/** ����ֶ� */
#define JsonAddField(field) addField(#field, field)
/** ������� */
#define JsonAddArray(field) addField(#field, field)
/** ��Ӷ���(����ʵ��JsonBaseObject�ӿ�) */
#define JsonAddObject(object) addObject(#object, object)
/** ��Ӷ�������(����ʵ��JsonBaseObject�ӿ�) */
#define JsonAddObjectArray(objectArray) addObjectArray(#objectArray, objectArray)

/** ��ȡ�ֶ� */
#define JsonGetField(p) getField(#p, p)
/** ��ȡ���� */
#define JsonGetArray(p) getArray(#p, p)
/** ��ȡ���� */
#define JsonGetObject(p) getObject(#p, p)
/** ��ȡ�������� */
#define JsonGetObjectArray(p) getObjectArray(#p, p)

/** Json���л�/�����л� ������ */
class JsonBaseObject {
public:
    JsonBaseObject() {}

    virtual ~JsonBaseObject() {}

    /** �����Ҫ���л����ֶ� JsonAddField/JsonAddObject/JsonAddObjectArray */
    virtual void addJsonData() = 0;

    /** ��ȡ�������json���� */
    nlohmann::json toJson() {
        this->addJsonData();
        return _m_object_nlohmann_json;
    }

    /** ��ȡJson�ַ��� */
    std::string toJsonString(const int indent = -1, const char indent_char = ' ',
        const bool ensure_ascii = false) {
        try {
            this->toJson();
            return _m_object_nlohmann_json.dump(indent, indent_char, ensure_ascii);
        }
        catch (const std::exception& e) {
            this->exception = this->exception.append(e.what());
            this->exception = this->exception.append("\n\n");
            return std::string();
        }
    }

    /** ����ֶ� */
    template <typename T>
    nlohmann::json addField(const std::string& name, T& value) {
        _m_object_nlohmann_json[name] = value;
        return _m_object_nlohmann_json;
    }

    /** ��Ӷ��� */
    template <typename T>
    nlohmann::json addObject(const std::string& name, T& value) {
        _m_object_nlohmann_json[name] = value.toJson();
        return _m_object_nlohmann_json;
    }

    /** ���std::list */
    template <typename T>
    nlohmann::json addObjectArray(const std::string& name, std::list<T>& value) {
        nlohmann::json arr;
        for (typename std::list<T>::iterator it = value.begin(); it != value.end();
            it++) {
            arr.push_back(it->toJson());
        }

        _m_object_nlohmann_json[name] = arr;
        return _m_object_nlohmann_json;
    }

    /** ���std::vector */
    template <typename T>
    nlohmann::json addObjectArray(const std::string& name,
        std::vector<T>& value) {
        nlohmann::json arr;
        for (typename std::vector<T>::iterator it = value.begin();
            it != value.end(); it++) {
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

    /** �쳣��Ϣ */
    const std::string& getException() { return this->exception; }

    /** ����Json�ַ��� */
    bool parseJsonString(const std::string& data) {
        try {
            nlohmann::json obj = nlohmann::json::parse(data);
            fromJson(obj);

            if (this->exception.empty()) {
                return true;
            }

            return false;
        }
        catch (const std::exception& e) {
            appendException("", std::string(e.what()));
            return false;
        }
    }

    /** ��ȡJson�ֶ� */
    template <typename T>
    void getField(const std::string& name, T& value) {
        try {
            auto obj = _m_object_nlohmann_json[name];
            if (obj.is_null()) {
                appendException(name, std::string("is null"));
                return;
            }
            value = obj;
        }
        catch (std::exception& e) {
            appendException(name, std::string(e.what()));
        }
    }

    /** ��ȡJson���� */
    template <typename T>
    void getObject(const std::string& name, T& value) {
        try {
            value.fromJson(_m_object_nlohmann_json[name]);
        }
        catch (std::exception& e) {
            appendException(name, std::string(e.what()));
        }
    }

    template <typename T>
    void getArray(const std::string& name, std::list<T>& value) {
        try {
            auto obj = _m_object_nlohmann_json[name];
            if (obj.is_null()) {
                appendException(name, std::string("is null"));
                return;
            }
            if (obj.is_array() == false) {
                appendException(name, std::string("not array"));
                return;
            }

            for (nlohmann::json::iterator it = obj.begin(); it != obj.end(); it++) {
                T t = *it;

                value.push_back(t);
            }
        }
        catch (std::exception& e) {
            appendException(name, std::string(e.what()));
        }
    }

    /** ��ȡJson�����б� */
    template <typename T>
    void getObjectArray(const std::string& name, std::list<T>& value) {
        try {
            auto obj = _m_object_nlohmann_json[name];
            if (obj.is_null()) {
                appendException(name, std::string("is null"));
                return;
            }
            if (obj.is_array() == false) {
                appendException(name, std::string("not array"));
                return;
            }

            for (nlohmann::json::iterator it = obj.begin(); it != obj.end(); it++) {
                T t;
                t.fromJson(*it);

                value.push_back(t);
            }

        }
        catch (std::exception& e) {
            appendException(name, std::string(e.what()));
        }
    }

    /** ��ȡJson�������� */
    template <typename T>
    void getObjectArray(const std::string& name, std::vector<T>& value) {
        try {
            auto obj = _m_object_nlohmann_json[name];
            if (obj.is_null()) {
                appendException(name, std::string("is null"));
                return;
            }
            if (obj.is_array() == false) {
                appendException(name, std::string("not array"));
                return;
            }

            for (nlohmann::json::iterator it = obj.begin(); it != obj.end(); it++) {
                T t;
                t.fromJson(*it);

                value.push_back(t);
            }
        }
        catch (std::exception& e) {
            appendException(name, std::string(e.what()));
        }
    }

private:
    void appendException(const std::string& name, const std::string& exception) {
        this->exception = this->exception.append("\n [");
        this->exception = this->exception.append(name);
        this->exception = this->exception.append("] ==>> ");
        this->exception = this->exception.append(exception);
        this->exception = this->exception.append("\n");
    }

private:
    /** nlohmann::json ���� */
    nlohmann::json _m_object_nlohmann_json;
    std::string exception;
};
