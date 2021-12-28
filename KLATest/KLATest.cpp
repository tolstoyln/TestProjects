// KLATest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>

#include "XmlWrap.h"
using namespace _XmlWrap;

#include "StringI.h"
using namespace _StrI;

enum class LogicalOperation
{
    NOP, // No operation: input
    AND,
    OR,
    XOR,
    NOT
};

class Gate
{
public:
    Gate(LogicalOperation op = LogicalOperation::NOP) : m_operation(op) {}
    void Connect(const Gate& child)
    {
        m_children.push_back(child);
    }
    bool Solve(const std::vector<bool>& input) const
    {
        auto it = input.begin();
        return SolveImp(it);
    }
private:
    bool SolveImp(std::vector<bool>::const_iterator& input) const
    {
        assert(m_children.empty() && m_operation == LogicalOperation::NOP || !m_children.empty() && m_operation != LogicalOperation::NOP);
        switch (m_operation)
        {
        case LogicalOperation::NOP:
            return *input++;
        case LogicalOperation::AND:
            for (const auto& child : m_children)
            {
                if (!child.SolveImp(input))
                {
                    return false;
                }
            }
            return true;
        case LogicalOperation::OR:
            for (const auto& child : m_children)
            {
                if (child.SolveImp(input))
                {
                    return true;
                }
            }
            return false;
        case LogicalOperation::XOR:
        {
            bool result = false;
            for (const auto& child : m_children)
            {
                result ^= child.SolveImp(input);
            }
            return result;
        }
        case LogicalOperation::NOT:
            assert(m_children.size() == 1);
            return !m_children[0].SolveImp(input);
        default:
            assert(false);
            return false;
        }
    }
    LogicalOperation m_operation = LogicalOperation::NOP;
    std::vector<Gate> m_children;
};

static std::unordered_map<LogicalOperation, string_i> _logicalOperationNames =
{
    {LogicalOperation::AND, "AND"}, {LogicalOperation::OR, "OR"}, {LogicalOperation::XOR, "XOR"}, {LogicalOperation::NOT, "NOT"}
};
static LogicalOperation _NameToLogicalOperation(const string_i& name)
{
    auto it = std::find_if(_logicalOperationNames.begin(), _logicalOperationNames.end(), [&](const auto& p) { return p.second == name; });
    return it != _logicalOperationNames.end() ? it->first : LogicalOperation::NOP;
}

class GatesTree
{
public:
    GatesTree(CXmlDoc& doc)
    {
        CXmlElement root;
        doc.FirstChild(root);
        string_i oper;
        root.GetAttribute("operation", oper);
        m_root = Gate(_NameToLogicalOperation(oper));
        ReadGatesFromConfig(root, m_root);
    }
    bool Solve(const std::vector<bool>& input) const
    {
        return m_root.Solve(input);
    }
private:
    void ReadGatesFromConfig(CXmlElement& parentElem, Gate& parentGate)
    {
        int inputs = 0;
        parentElem.GetAttribute("inputs", inputs);
        CXmlEnum children;
        parentElem.EnumChildren(children);
        int count = 0;
        CXmlElement elem;
        while (children.NextElement(elem))
        {
            string_i oper;
            elem.GetAttribute("operation", oper);
            Gate gate(_NameToLogicalOperation(oper));
            ReadGatesFromConfig(elem, gate);
            parentGate.Connect(gate);
            ++count;
        }
        while (count++ < inputs)
        {
            Gate gate;
            parentGate.Connect(gate);
        }
    }
    Gate m_root;
};

template<class _Node>
static CXmlElement AddGateToConfig(CXmlDoc& doc, _Node& parent, LogicalOperation operation, int inputs)
{
    CXmlElement elem;
    doc.CreateElement("Gate", elem);
    elem.SetAttribute("operation", _logicalOperationNames[operation]);
    elem.SetAttribute("inputs", inputs);
    parent.AppendChild(elem);
    return elem;
}

static void CreateConfig(CXmlDoc& doc, const char* file)
{
    CXmlElement root = AddGateToConfig(doc, doc, LogicalOperation::AND, 3);
    CXmlElement elem = AddGateToConfig(doc, root, LogicalOperation::OR, 3);
    AddGateToConfig(doc, elem, LogicalOperation::NOT, 1);
    AddGateToConfig(doc, elem, LogicalOperation::XOR, 2);
    elem = AddGateToConfig(doc, root, LogicalOperation::XOR, 2);
    AddGateToConfig(doc, elem, LogicalOperation::AND, 2);
    AddGateToConfig(doc, elem, LogicalOperation::XOR, 2);
    doc.Save(file);
}

int main()
{
    static const char* _file = "LogicalGates.xml";

    CXmlDoc doc;
    if (!doc.Load(_file))
    {
        CreateConfig(doc, _file);
    }

    GatesTree tree(doc);
    
    std::cout << tree.Solve({ 1,1,1,1,1,1,1,1,1 }) << " "
        << tree.Solve({ 1,1,1,1,1,1,1,1,0 }) << " "
        << tree.Solve({ 1,1,1,1,1,1,0,1,1 }) << " "
        << tree.Solve({ 1,1,1,0,1,1,1,1,1 });
}

