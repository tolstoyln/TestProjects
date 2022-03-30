#ifndef _ExportCall_Hpp
#define _ExportCall_Hpp

// Export APIs calling helpers.
// Shortcuts for common practice of typedefing, loading library, getting proc address, calling
// and then freeing library.

#ifndef NON_COPYABLE
#define NON_COPYABLE(x) private:                        \
                            x(const x &);               \
                            x & operator=(const x &);
#endif // NON_COPYABLE

// If you need calling convention other from __stdcall, define the following macro to what you need.

#ifdef ExportCall_CDecl
    #define ExportCall_ __cdecl
#else
    #define ExportCall_ __stdcall
#endif


class CLibModule
{
NON_COPYABLE(CLibModule)
public:
    CLibModule(LPCSTR cszModule, bool bAutoFree = false) : m_hLib(NULL), m_bFree(false)
    {
        if ((m_hLib = GetModuleHandleA(cszModule)) == NULL)
        {
            m_bFree = (m_hLib = LoadLibraryA(cszModule)) != NULL && bAutoFree;
        }
    }
    CLibModule(HMODULE hModule) : m_hLib(hModule), m_bFree(false) {}
    ~CLibModule()
    {
        if (m_bFree)
            FreeLibrary(m_hLib);
    }
    operator HMODULE() const {return m_hLib;}
private:
    HMODULE m_hLib;
    bool m_bFree;
};

class CExportCallBase
{
NON_COPYABLE(CExportCallBase)
public:
    CExportCallBase(LPCSTR cszModule, LPCSTR cszExport) : m_lib(cszModule, true), m_pf(NULL)
    {
        GetExportFunc(cszExport);
    }
    CExportCallBase(HMODULE hModule, LPCSTR cszExport) : m_lib(hModule), m_pf(NULL)
    {
        GetExportFunc(cszExport);
    }
    PROC m_pf;
private:
    void GetExportFunc(LPCSTR cszExport)
    {
        if (m_lib != NULL)
            m_pf = GetProcAddress(m_lib, cszExport);
    }
    CLibModule m_lib;
};

template<typename T>
class CFallValue
{
protected:
    CFallValue(T FallValue) : m_FallValue(FallValue) {}
    T m_FallValue;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Functionals (up to 6 parameters; add if you need more):

template<typename RT>
class FExportCall0 : public CExportCallBase, public CFallValue<RT>
{
    typedef RT (ExportCall_* PF)();
public:
    FExportCall0(LPCSTR cszModule, LPCSTR cszExport, RT rtFallValue) :
        CExportCallBase(cszModule, cszExport), CFallValue<RT>(rtFallValue) {}
    FExportCall0(HMODULE hModule, LPCSTR cszExport, RT rtFallValue) :
        CExportCallBase(hModule, cszExport), CFallValue<RT>(rtFallValue) {}
    RT operator ()() const
        {return (m_pf != NULL) ? (reinterpret_cast<PF>(m_pf))() : CFallValue<RT>::m_FallValue;}
};
template<>
class FExportCall0<void> : public CExportCallBase
{
	typedef void (ExportCall_* PF)();
public:
	using CExportCallBase::CExportCallBase;
	void operator ()() const
	{
		if (m_pf != NULL) (reinterpret_cast<PF>(m_pf))();
	}
};

template<typename RT, typename P1>
class FExportCall1 : public CExportCallBase, public CFallValue<RT>
{
    typedef RT (ExportCall_* PF)(P1);
public:
    FExportCall1(LPCSTR cszModule, LPCSTR cszExport, RT rtFallValue) :
        CExportCallBase(cszModule, cszExport), CFallValue<RT>(rtFallValue) {}
    FExportCall1(HMODULE hModule, LPCSTR cszExport, RT rtFallValue) :
        CExportCallBase(hModule, cszExport), CFallValue<RT>(rtFallValue) {}
    RT operator ()(P1 p1) const
        {return (m_pf != NULL) ? (reinterpret_cast<PF>(m_pf))(p1) : CFallValue<RT>::m_FallValue;}
};
template<typename P1>
class FExportCall1<void, P1> : public CExportCallBase
{
	typedef void (ExportCall_* PF)(P1);
public:
	using CExportCallBase::CExportCallBase;
	void operator ()(P1 p1) const
	{
		if (m_pf != NULL) (reinterpret_cast<PF>(m_pf))(p1);
	}
};

template<typename RT, typename P1, typename P2>
class FExportCall2 : public CExportCallBase, public CFallValue<RT>
{
    typedef RT (ExportCall_* PF)(P1, P2);
public:
    FExportCall2(LPCSTR cszModule, LPCSTR cszExport, RT rtFallValue) :
        CExportCallBase(cszModule, cszExport), CFallValue<RT>(rtFallValue) {}
    FExportCall2(HMODULE hModule, LPCSTR cszExport, RT rtFallValue) :
        CExportCallBase(hModule, cszExport), CFallValue<RT>(rtFallValue) {}
    RT operator ()(P1 p1, P2 p2) const
        {return (m_pf != NULL) ? (reinterpret_cast<PF>(m_pf))(p1, p2) : CFallValue<RT>::m_FallValue;}
};
template<typename P1, typename P2>
class FExportCall2<void, P1, P2> : public CExportCallBase
{
	typedef void (ExportCall_* PF)(P1, P2);
public:
	using CExportCallBase::CExportCallBase;
	void operator ()(P1 p1, P2 p2) const
	{
		if (m_pf != NULL) (reinterpret_cast<PF>(m_pf))(p1, p2);
	}
};

template<typename RT, typename P1, typename P2, typename P3>
class FExportCall3 : public CExportCallBase, public CFallValue<RT>
{
    typedef RT (ExportCall_* PF)(P1, P2, P3);
public:
    FExportCall3(LPCSTR cszModule, LPCSTR cszExport, RT rtFallValue) :
        CExportCallBase(cszModule, cszExport), CFallValue<RT>(rtFallValue) {}
    FExportCall3(HMODULE hModule, LPCSTR cszExport, RT rtFallValue) :
        CExportCallBase(hModule, cszExport), CFallValue<RT>(rtFallValue) {}
    RT operator ()(P1 p1, P2 p2, P3 p3) const
        {return (m_pf != NULL) ? (reinterpret_cast<PF>(m_pf))(p1, p2, p3) : CFallValue<RT>::m_FallValue;}
};
template<typename P1, typename P2, typename P3>
class FExportCall3<void, P1, P2, P3> : public CExportCallBase
{
	typedef void (ExportCall_* PF)(P1, P2, P3);
public:
	using CExportCallBase::CExportCallBase;
	void operator ()(P1 p1, P2 p2, P3 p3) const
	{
		if (m_pf != NULL) (reinterpret_cast<PF>(m_pf))(p1, p2, p3);
	}
};

template<typename RT, typename P1, typename P2, typename P3, typename P4>
class FExportCall4 : public CExportCallBase, public CFallValue<RT>
{
    typedef RT (ExportCall_* PF)(P1, P2, P3, P4);
public:
    FExportCall4(LPCSTR cszModule, LPCSTR cszExport, RT rtFallValue) :
        CExportCallBase(cszModule, cszExport), CFallValue<RT>(rtFallValue) {}
    FExportCall4(HMODULE hModule, LPCSTR cszExport, RT rtFallValue) :
        CExportCallBase(hModule, cszExport), CFallValue<RT>(rtFallValue) {}
    RT operator ()(P1 p1, P2 p2, P3 p3, P4 p4) const
        {return (m_pf != NULL) ? (reinterpret_cast<PF>(m_pf))(p1, p2, p3, p4) : CFallValue<RT>::m_FallValue;}
};
template<typename P1, typename P2, typename P3, typename P4>
class FExportCall4<void, P1, P2, P3, P4> : public CExportCallBase
{
	typedef void (ExportCall_* PF)(P1, P2, P3, P4);
public:
	using CExportCallBase::CExportCallBase;
	void operator ()(P1 p1, P2 p2, P3 p3, P4 p4) const
	{
		if (m_pf != NULL) (reinterpret_cast<PF>(m_pf))(p1, p2, p3, p4);
	}
};

template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5>
class FExportCall5 : public CExportCallBase, public CFallValue<RT>
{
    typedef RT (ExportCall_* PF)(P1, P2, P3, P4, P5);
public:
    FExportCall5(LPCSTR cszModule, LPCSTR cszExport, RT rtFallValue) :
        CExportCallBase(cszModule, cszExport), CFallValue<RT>(rtFallValue) {}
    FExportCall5(HMODULE hModule, LPCSTR cszExport, RT rtFallValue) :
        CExportCallBase(hModule, cszExport), CFallValue<RT>(rtFallValue) {}
    RT operator ()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) const
        {return (m_pf != NULL) ? (reinterpret_cast<PF>(m_pf))(p1, p2, p3, p4, p5) : CFallValue<RT>::m_FallValue;}
};
template<typename P1, typename P2, typename P3, typename P4, typename P5>
class FExportCall5<void, P1, P2, P3, P4, P5> : public CExportCallBase
{
	typedef void (ExportCall_* PF)(P1, P2, P3, P4, P5);
public:
	using CExportCallBase::CExportCallBase;
	void operator ()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) const
	{
		if (m_pf != NULL) (reinterpret_cast<PF>(m_pf))(p1, p2, p3, p4, p5);
	}
};

template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class FExportCall6 : public CExportCallBase, public CFallValue<RT>
{
    typedef RT (ExportCall_* PF)(P1, P2, P3, P4, P5, P6);
public:
    FExportCall6(LPCSTR cszModule, LPCSTR cszExport, RT rtFallValue) :
        CExportCallBase(cszModule, cszExport), CFallValue<RT>(rtFallValue) {}
    FExportCall6(HMODULE hModule, LPCSTR cszExport, RT rtFallValue) :
        CExportCallBase(hModule, cszExport), CFallValue<RT>(rtFallValue) {}
    RT operator ()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) const
        {return (m_pf != NULL) ? (reinterpret_cast<PF>(m_pf))(p1, p2, p3, p4, p5, p6) : CFallValue<RT>::m_FallValue;}
};
template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class FExportCall6<void, P1, P2, P3, P4, P5, P6> : public CExportCallBase
{
	typedef void (ExportCall_* PF)(P1, P2, P3, P4, P5, P6);
public:
	using CExportCallBase::CExportCallBase;
	void operator ()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) const
	{
		if (m_pf != NULL) (reinterpret_cast<PF>(m_pf))(p1, p2, p3, p4, p5, p6);
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Single call functions:

template<typename RT>
inline RT export_call(LPCSTR cszModule, LPCSTR cszExport, RT rtFallValue) 
{
    return FExportCall0<RT>(cszModule, cszExport, rtFallValue)();
}
template<typename RT>
inline RT export_call(HMODULE hModule, LPCSTR cszExport, RT rtFallValue) 
{
    return FExportCall0<RT>(hModule, cszExport, rtFallValue)();
}

inline void void_export_call(LPCSTR cszModule, LPCSTR cszExport)
{
	FExportCall0<void>(cszModule, cszExport)();
}
inline void void_export_call(HMODULE hModule, LPCSTR cszExport)
{
	FExportCall0<void>(hModule, cszExport)();
}


template<typename RT, typename P1>
inline RT export_call(LPCSTR cszModule, LPCSTR cszExport, RT rtFallValue, P1 p1) 
{
    return FExportCall1<RT,P1>(cszModule, cszExport, rtFallValue)(p1);
}
template<typename RT, typename P1>
inline RT export_call(HMODULE hModule, LPCSTR cszExport, RT rtFallValue, P1 p1) 
{
    return FExportCall1<RT,P1>(hModule, cszExport, rtFallValue)(p1);
}

template<typename P1>
inline void void_export_call(LPCSTR cszModule, LPCSTR cszExport, P1 p1)
{
	FExportCall1<void, P1>(cszModule, cszExport)(p1);
}
template<typename P1>
inline void void_export_call(HMODULE hModule, LPCSTR cszExport, P1 p1)
{
	FExportCall1<void, P1>(hModule, cszExport)(p1);
}


template<typename RT, typename P1, typename P2>
inline RT export_call(LPCSTR cszModule, LPCSTR cszExport, RT rtFallValue, P1 p1, P2 p2) 
{
    return FExportCall2<RT,P1,P2>(cszModule, cszExport, rtFallValue)(p1, p2);
}
template<typename RT, typename P1, typename P2>
inline RT export_call(HMODULE hModule, LPCSTR cszExport, RT rtFallValue, P1 p1, P2 p2) 
{
    return FExportCall2<RT,P1,P2>(hModule, cszExport, rtFallValue)(p1, p2);
}

template<typename P1, typename P2>
inline void void_export_call(LPCSTR cszModule, LPCSTR cszExport, P1 p1, P2 p2)
{
	FExportCall2<void, P1, P2>(cszModule, cszExport)(p1, p2);
}
template<typename P1, typename P2>
inline void void_export_call(HMODULE hModule, LPCSTR cszExport, P1 p1, P2 p2)
{
	FExportCall2<void, P1, P2>(hModule, cszExport)(p1, p2);
}


template<typename RT, typename P1, typename P2, typename P3>
inline RT export_call(LPCSTR cszModule, LPCSTR cszExport, RT rtFallValue, P1 p1, P2 p2, P3 p3) 
{
    return FExportCall3<RT,P1,P2,P3>(cszModule, cszExport, rtFallValue)(p1, p2, p3);
}
template<typename RT, typename P1, typename P2, typename P3>
inline RT export_call(HMODULE hModule, LPCSTR cszExport, RT rtFallValue, P1 p1, P2 p2, P3 p3) 
{
    return FExportCall3<RT,P1,P2,P3>(hModule, cszExport, rtFallValue)(p1, p2, p3);
}

template<typename P1, typename P2, typename P3>
inline void void_export_call(LPCSTR cszModule, LPCSTR cszExport, P1 p1, P2 p2, P3 p3)
{
	FExportCall3<void, P1, P2, P3>(cszModule, cszExport)(p1, p2, p3);
}
template<typename P1, typename P2, typename P3>
inline void void_export_call(HMODULE hModule, LPCSTR cszExport, P1 p1, P2 p2, P3 p3)
{
	FExportCall3<void, P1, P2, P3>(hModule, cszExport)(p1, p2, p3);
}


template<typename RT, typename P1, typename P2, typename P3, typename P4>
inline RT export_call(LPCSTR cszModule, LPCSTR cszExport, RT rtFallValue, P1 p1, P2 p2, P3 p3, P4 p4) 
{
    return FExportCall4<RT,P1,P2,P3,P4>(cszModule, cszExport, rtFallValue)(p1, p2, p3, p4);
}
template<typename RT, typename P1, typename P2, typename P3, typename P4>
inline RT export_call(HMODULE hModule, LPCSTR cszExport, RT rtFallValue, P1 p1, P2 p2, P3 p3, P4 p4) 
{
    return FExportCall4<RT,P1,P2,P3,P4>(hModule, cszExport, rtFallValue)(p1, p2, p3, p4);
}

template<typename P1, typename P2, typename P3, typename P4>
inline void void_export_call(LPCSTR cszModule, LPCSTR cszExport, P1 p1, P2 p2, P3 p3, P4 p4)
{
	FExportCall4<void, P1, P2, P3, P4>(cszModule, cszExport)(p1, p2, p3, p4);
}
template<typename P1, typename P2, typename P3, typename P4>
inline void void_export_call(HMODULE hModule, LPCSTR cszExport, P1 p1, P2 p2, P3 p3, P4 p4)
{
	FExportCall4<void, P1, P2, P3, P4>(hModule, cszExport)(p1, p2, p3, p4);
}


template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5>
inline RT export_call(LPCSTR cszModule, LPCSTR cszExport, RT rtFallValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) 
{
    return FExportCall5<RT,P1,P2,P3,P4,P5>(cszModule, cszExport, rtFallValue)(p1, p2, p3, p4, p5);
}
template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5>
inline RT export_call(HMODULE hModule, LPCSTR cszExport, RT rtFallValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) 
{
    return FExportCall5<RT,P1,P2,P3,P4,P5>(hModule, cszExport, rtFallValue)(p1, p2, p3, p4, p5);
}

template<typename P1, typename P2, typename P3, typename P4, typename P5>
inline void void_export_call(LPCSTR cszModule, LPCSTR cszExport, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
{
	FExportCall5<void, P1, P2, P3, P4, P5>(cszModule, cszExport)(p1, p2, p3, p4, p5);
}
template<typename P1, typename P2, typename P3, typename P4, typename P5>
inline void void_export_call(HMODULE hModule, LPCSTR cszExport, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
{
	FExportCall5<void, P1, P2, P3, P4, P5>(hModule, cszExport)(p1, p2, p3, p4, p5);
}


template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline RT export_call(LPCSTR cszModule, LPCSTR cszExport, RT rtFallValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) 
{
    return FExportCall6<RT,P1,P2,P3,P4,P5,P6>(cszModule, cszExport, rtFallValue)(p1, p2, p3, p4, p5, p6);
}
template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline RT export_call(HMODULE hModule, LPCSTR cszExport, RT rtFallValue, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) 
{
    return FExportCall6<RT,P1,P2,P3,P4,P5,P6>(hModule, cszExport, rtFallValue)(p1, p2, p3, p4, p5, p6);
}

template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline void void_export_call(LPCSTR cszModule, LPCSTR cszExport, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
{
	FExportCall6<void, P1, P2, P3, P4, P5, P6>(cszModule, cszExport)(p1, p2, p3, p4, p5, p6);
}
template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
inline void void_export_call(HMODULE hModule, LPCSTR cszExport, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
{
	FExportCall6<void, P1, P2, P3, P4, P5, P6>(hModule, cszExport)(p1, p2, p3, p4, p5, p6);
}

#endif // _ExportCall_Hpp
