#pragma once

#include <comdef.h>
#include <msxml2.h>
#include <string>

namespace _XmlWrap
{
    using namespace _com_util;

    _COM_SMARTPTR_TYPEDEF(IXMLDOMDocument, __uuidof(IXMLDOMDocument));
    _COM_SMARTPTR_TYPEDEF(IXMLDOMNodeList, __uuidof(IXMLDOMNodeList));
    _COM_SMARTPTR_TYPEDEF(IXMLDOMNode, __uuidof(IXMLDOMNode));
    _COM_SMARTPTR_TYPEDEF(IXMLDOMElement, __uuidof(IXMLDOMElement));

    class CXmlException
    {
    public:
        CXmlException(long err) : m_err(err) {}
        CXmlException(const _com_error& err) : m_err(err.Error()) {}
        long Error() const { return m_err; }
    private:
        long m_err;
    };

#define _COM_ERROR(exp)     try {(exp);} catch (_com_error err) {throw CXmlException(err);}
#define _NO_COM_ERROR(exp)  try {(exp);} catch (_com_error err) {}

    template<class _IPtr> class _CXmlObjT
    {
    public:
        typename _IPtr::Interface** operator &() { return &m_ptr; }
        operator typename _IPtr::Interface* () { return m_ptr; }
    protected:
        _IPtr m_ptr;
    };

    class CXmlElement; // Fwd decl;

    class CXmlEnum : public _CXmlObjT<IXMLDOMNodeListPtr>
    {
    public:
        inline bool NextElement(CXmlElement& elem);
    };

    class CXmlElement : public _CXmlObjT<IXMLDOMElementPtr>
    {
    public:
        CXmlElement() {}
        CXmlElement& operator =(const IXMLDOMNodePtr& node)
        {
            _COM_ERROR(m_ptr = node)
            return *this;
        }
        void FirstChildThrow(CXmlElement& elem)
        {
            IXMLDOMNodePtr node;
            _COM_ERROR(CheckError(m_ptr->get_firstChild(&node)))
            elem = node;
        }
        bool FirstChild(CXmlElement& elem)
        {
            IXMLDOMNodePtr node;
            if (m_ptr->get_firstChild(&node) == S_OK)
            {
                elem = node;
                return true;
            }
            return false;
        }
        void AppendChild(CXmlElement& elem) { _COM_ERROR(CheckError(m_ptr->appendChild(elem, NULL))) }
        void ReplaceChild(CXmlElement& elemNew, CXmlElement& elemOld)
        {
            _COM_ERROR(CheckError(m_ptr->replaceChild(elemNew, elemOld, NULL)))
        }
        template<class _Traits> void TagName(std::basic_string<char, _Traits>& str)
        {
            BSTR bstr;
            _COM_ERROR(CheckError(m_ptr->get_nodeName(&bstr)))
            _bstr_t bs;
            bs.Attach(bstr);
            str = bs;
        }
        template<typename T> void GetAttributeThrow(const char* cszAttr, T& val)
        {
            _variant_t var;
            _COM_ERROR(CheckError(m_ptr->getAttribute(_bstr_t(cszAttr), &var)))
            _COM_ERROR(val = var)
        }
        template<typename T> bool GetAttribute(const char* cszAttr, T& val)
        {
            _variant_t var;
            if (m_ptr->getAttribute(_bstr_t(cszAttr), &var) == S_OK)
            {
                _COM_ERROR(val = var)
                return true;
            }
            return false;
        }
        template<class _Traits>
        void GetAttributeThrow(const char* cszAttr, std::basic_string<char, _Traits>& val)
        {
            _variant_t var;
            _COM_ERROR(CheckError(m_ptr->getAttribute(_bstr_t(cszAttr), &var)))
            _COM_ERROR(val = _bstr_t(var))
        }
        template<class _Traits>
        bool GetAttribute(const char* cszAttr, std::basic_string<char, _Traits>& val)
        {
            _variant_t var;
            if (m_ptr->getAttribute(_bstr_t(cszAttr), &var) == S_OK)
            {
                _COM_ERROR(val = _bstr_t(var))
                return true;
            }
            return false;
        }
        template<typename T> void SetAttribute(const char* cszAttr, const T& val)
        {
            _COM_ERROR(CheckError(m_ptr->setAttribute(_bstr_t(cszAttr), _variant_t(val))))
        }
        template<class _Traits>
        void SetAttribute(const char* cszAttr, const std::basic_string<char, _Traits>& val)
        {
            _COM_ERROR(CheckError(m_ptr->setAttribute(_bstr_t(cszAttr), _variant_t(val.c_str()))))
        }
        void EnumChildren(CXmlEnum& enumer) { _COM_ERROR(CheckError(m_ptr->get_childNodes(&enumer))) }
    };

    inline bool CXmlEnum::NextElement(CXmlElement& elem)
    {
        IXMLDOMNodePtr node;
        if (m_ptr->nextNode(&node) == S_OK)
        {
            elem = node;
            return true;
        }
        return false;
    }

    class CXmlDoc : public _CXmlObjT<IXMLDOMDocumentPtr>
    {
    public:
        CXmlDoc() { Init(); }
        ~CXmlDoc() { Uninit(); }
        bool Load(const char* cszFile)
        {
            VARIANT_BOOL vbCfgFound = 0;
            _COM_ERROR(CheckError(m_ptr->load(_variant_t(cszFile), &vbCfgFound)))
            return vbCfgFound != 0;
        }
        void Save(const char* cszFile) { _COM_ERROR(CheckError(m_ptr->save(_variant_t(cszFile)))) }
        void CreateElement(const char* cszName, CXmlElement& elem)
        {
            _COM_ERROR(CheckError(m_ptr->createElement(_bstr_t(cszName), &elem)))
        }
        void FirstChild(CXmlElement& elem)
        {
            IXMLDOMNodePtr node;
            _COM_ERROR(CheckError(m_ptr->get_firstChild(&node)))
            elem = node;
        }
        void AppendChild(CXmlElement& elem) { _COM_ERROR(CheckError(m_ptr->appendChild(elem, NULL))) }
    private:
        bool Init()
        {
            (void)CoInitialize(NULL);
            return m_ptr.CreateInstance(__uuidof(DOMDocument)) == S_OK;
        }
        void Uninit()
        {
            _NO_COM_ERROR(m_ptr.Release())
            CoUninitialize();
        }
    };

}; // namespace _XmlWrap
