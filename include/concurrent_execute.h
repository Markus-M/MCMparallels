
//          Copyright Markus Mohr 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(_CONCURRENT_EXECUTE_H_)
#define _CONCURRENT_EXECUTE_H_

#include "Task.h"
#include "TaskManager.h"

template<bool, typename Then, typename Else>
struct IF
{
	typedef Then RET;
};

template<typename Then, typename Else>
struct IF<false, Then, Else>
{
	typedef Else RET;
};

template<typename Then, typename Else>
struct EQUALS
{
	enum { RET = 0 };
};

template<typename Else>
struct EQUALS<Else, Else>
{
	enum { RET = 1 };
};

template<typename RET, typename F>
struct ASSIGN
{
	void set(F)
	{

	}
};

template<typename F>
struct ASSIGN<void, F>
{
	void set(F)
	{

	}
};

template<typename RET>
class BaseConcurrency
{
public:

	virtual RET operator()() = 0;
};


template<typename Ret>
class Future : public Task
{
public:

	Future<Ret>(void *execute) : m_ToExecute(execute)//	: m_Task(task)
	{}

	operator const Ret () const
	{
		while (!isDone())
		{
			TaskManager::getInstance().yield();
		}
		return m_Value;
	}

	operator Ret ()
	{
		while (!isDone())
		{
			TaskManager::getInstance().yield();
		}
		return m_Value;
	}

	void run()
	{
		m_Value = (*static_cast<BaseConcurrency<Ret>* >(m_ToExecute))();
	}

private:

	Ret					m_Value;
	void 				*m_ToExecute;
};


template<>
class Future<void> : public Task
{
public:

	Future<void>(void *execute) : m_ToExecute(execute)//	: m_Task(task)
	{}

	void operator ()(Task *caller) const
	{
		while (!isDone())
		{
			caller->yield();
		}
	}

	void run()
	{
		(*static_cast<BaseConcurrency<void>* >(m_ToExecute))();
	}

private:

	void *m_ToExecute;
};
// --------------------------------------------------------------------------------------------------------
// no param version
// --------------------------------------------------------------------------------------------------------
template <typename RET>
class CONCURRENT_CALL0 : public BaseConcurrency<RET>
{
public:

	typedef RET (*FunctionSignature)();

	CONCURRENT_CALL0(FunctionSignature function) : m_Function(function)
	{}


	RET operator()()
	{
		return m_Function();
	}

	virtual void run()
	{
		operator()();
	}

private:


	FunctionSignature	m_Function;
};


template <>
class CONCURRENT_CALL0<void> : public BaseConcurrency<void>
{
public:

	typedef void (*FunctionSignature)();

	CONCURRENT_CALL0(FunctionSignature function) : m_Function(function)
	{}


	void operator()()
	{
		m_Function();
	}

	virtual void run()
	{
		operator()();
	}
private:


	FunctionSignature	m_Function;
};
// --------------------------------------------------------------------------------------------------------
// end of no param version
// --------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------
// one param version
// --------------------------------------------------------------------------------------------------------
template <typename Param1, typename RET>
class CONCURRENT_CALL : public BaseConcurrency<RET>
{
public:

	typedef RET (*FunctionSignature)(Param1);

	CONCURRENT_CALL(FunctionSignature function, Param1 p) : m_Function(function), m_FirstParam(p)
	{}


	RET operator()(Param1 param) const
	{
		typename IF<EQUALS<RET, void>::RET, int, float>::RET t;
		return m_Function(param);
	}

	RET operator()()
	{
		return m_Function(m_FirstParam);
	}

	virtual void run()
	{
		operator()();
	}

private:


	FunctionSignature	m_Function;
	Param1				m_FirstParam;
};



template <typename Param1>
class CONCURRENT_CALL<Param1, void>  : public BaseConcurrency<void>
{
public:

	typedef void (*FunctionSignature)(Param1);

	CONCURRENT_CALL(FunctionSignature function, Param1 p) : m_Function(function), m_FirstParam(p)
	{}


	void operator()(Param1 param) const
	{
		m_Function(param);
	}

	void operator()()
	{
		printf("STarting");
		m_Function(m_FirstParam);
	}

	virtual void run()
	{
		operator()();
	}

private:


	FunctionSignature	m_Function;
	Param1				m_FirstParam;
};
// --------------------------------------------------------------------------------------------------------
// end of one param version
// --------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------
// two param version
// --------------------------------------------------------------------------------------------------------
template <typename Param1, typename Param2, typename RET>
class CONCURRENT_CALL2  : public Task
{
public:

	typedef RET (*FunctionSignature)(Param1, Param2);

	CONCURRENT_CALL2(FunctionSignature function, Param1 param1, Param2 param2) : 	m_Function(function),
																						m_Param1(param1),
																						m_Param2(param2)
	{}


	RET operator()(Param1 param1, Param2 param2) const
	{
		return m_Function(param1, param2);
	}

	RET operator()() const
	{
		return m_Function(m_Param1, m_Param2);
	}

	virtual void run()
	{
		operator()();
	}

private:


	FunctionSignature	m_Function;
	Param1				m_Param1;
	Param2				m_Param2;
};

template <typename Param1, typename Param2>
class CONCURRENT_CALL2<Param1, Param2, void>  : public Task
{
public:

	typedef void (*FunctionSignature)(Param1, Param2);

	CONCURRENT_CALL2(FunctionSignature function, Param1 param1, Param2 param2) : 	m_Function(function),
																						m_Param1(param1),
																						m_Param2(param2)
	{}


	void operator()(Param1 param1, Param2 param2) const
	{
		m_Function(param1, param2);
	}

	void operator()() const
	{
		m_Function(m_Param1, m_Param2);
	}

	virtual void run()
	{
		operator()();
	}

private:


	FunctionSignature	m_Function;
	Param1				m_Param1;
	Param2				m_Param2;
};
// --------------------------------------------------------------------------------------------------------
// end of two param version
// --------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------
// three param version
// --------------------------------------------------------------------------------------------------------
template <typename Param1, typename Param2, typename Param3, typename RET>
class CONCURRENT_CALL3  : public Task
{
public:

	typedef RET (*FunctionSignature)(Param1, Param2, Param3);

	CONCURRENT_CALL3(FunctionSignature function, Param1 param1, Param2 param2, Param3 param3) : m_Function(function),
																									 m_Param1(param1),
																									 m_Param2(param2),
																									 m_Param3(param3)
	{}


	RET operator()(Param1 param1, Param2 param2, Param3 param3)
	{
		return m_Function(param1, param2, param3);
	}

	RET operator()()
	{
		return m_Function(m_Param1, m_Param2, m_Param3);
	}

	virtual void run()
	{
		operator()();
	}

private:


	FunctionSignature	m_Function;
	Param1				m_Param1;
	Param2				m_Param2;
	Param3				m_Param3;
};

template <typename Param1, typename Param2, typename Param3>
class CONCURRENT_CALL3<Param1, Param2, Param3, void>  : public Task
{
public:

	typedef void (*FunctionSignature)(Param1, Param2, Param3);

	CONCURRENT_CALL3(FunctionSignature function, Param1 param1, Param2 param2, Param3 param3) : m_Function(function),
																									 m_Param1(param1),
																									 m_Param2(param2),
																									 m_Param3(param3)
	{}


	void operator()(Param1 param1, Param2 param2, Param3 param3)
	{
		m_Function(param1, param2, param3);
	}

	void operator()()
	{
		m_Function(m_Param1, m_Param2, m_Param3);
	}

	virtual void run()
	{
		operator()();
	}

private:


	FunctionSignature	m_Function;
	Param1				m_Param1;
	Param2				m_Param2;
	Param3				m_Param3;
};


// --------------------------------------------------------------------------------------------------------
// end of three param version
// --------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------
// four param version
// --------------------------------------------------------------------------------------------------------
template <typename Param1, typename Param2, typename Param3, typename Param4, typename RET>
class CONCURRENT_CALL4  : public Task
{
public:

	typedef RET (*FunctionSignature)(Param1, Param2, Param3, Param4);

	CONCURRENT_CALL4(FunctionSignature function, Param1 param1, Param2 param2,
												 Param3 param3, Param4 param4 ) : m_Function(function),
												 	 	 	 	 	 	 	 	  m_Param1(param1),
																				  m_Param2(param2),
																				  m_Param3(param3),
																				  m_Param4(param4)
	{}


	RET operator()(Param1 param1, Param2 param2, Param3 param3, Param4 param4)
	{
		return m_Function(param1, param2, param3, param4);
	}

	RET operator()()
	{
		return m_Function(m_Param1, m_Param2, m_Param3, m_Param4);
	}

	virtual void run()
	{
		operator()();
	}

private:


	FunctionSignature	m_Function;
	Param1				m_Param1;
	Param2				m_Param2;
	Param3				m_Param3;
	Param4				m_Param4;
};

template <typename Param1, typename Param2, typename Param3, typename Param4>
class CONCURRENT_CALL4<Param1, Param2, Param3, Param4, void>  : public Task
{
public:

	typedef void (*FunctionSignature)(Param1, Param2, Param3, Param4);

	CONCURRENT_CALL4(FunctionSignature function, Param1 param1, Param2 param2,
												   Param3 param3, Param4 param4 ) : m_Function(function),
												 	 	 	 	 	 	 	 	  m_Param1(param1),
																				  m_Param2(param2),
																				  m_Param3(param3),
																				  m_Param4(param4)
	{}


	void operator()(Param1 param1, Param2 param2, Param3 param3, Param4 param4)
	{
		m_Function(param1, param2, param3, param4);
	}

	void operator()()
	{
		m_Function(m_Param1, m_Param2, m_Param3, m_Param4);
	}

	virtual void run()
	{
		operator()();
	}

private:


	FunctionSignature	m_Function;
	Param1				m_Param1;
	Param2				m_Param2;
	Param3				m_Param3;
	Param4				m_Param4;
};


// --------------------------------------------------------------------------------------------------------
// end of four param version
// --------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------
// five param version
// --------------------------------------------------------------------------------------------------------
template <typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename RET>
class CONCURRENT_CALL5  : public Task
{
public:

	typedef RET (*FunctionSignature)(Param1, Param2, Param3, Param4, Param5);

	CONCURRENT_CALL5(FunctionSignature function, Param1 param1, Param2 param2,
												   Param3 param3, Param4 param4,
												   Param5 param5) : m_Function(function),
												 	 	 	 	 	 	 	 	  m_Param1(param1),
																				  m_Param2(param2),
																				  m_Param3(param3),
																				  m_Param4(param4),
																				  m_Param5(param5)
	{}


	RET operator()(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5)
	{
		return m_Function(param1, param2, param3, param4, param5);
	}

	RET operator()()
	{
		return m_Function(m_Param1, m_Param2, m_Param3, m_Param4, m_Param5);
	}

	virtual void run()
	{
		operator()();
	}

private:


	FunctionSignature	m_Function;
	Param1				m_Param1;
	Param2				m_Param2;
	Param3				m_Param3;
	Param4				m_Param4;
	Param5				m_Param5;
};

template <typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
class CONCURRENT_CALL5<Param1, Param2, Param3, Param4, Param5, void>  : public Task
{
public:

	typedef void (*FunctionSignature)(Param1, Param2, Param3, Param4, Param5);

	CONCURRENT_CALL5(FunctionSignature function, Param1 param1, Param2 param2,
												   Param3 param3, Param4 param4,
												   Param5 param5) : m_Function(function),
																	  m_Param1(param1),
																	  m_Param2(param2),
																	  m_Param3(param3),
																	  m_Param4(param4),
																	  m_Param5(param5)
	{}


	void operator()(Param1 param1, Param2 param2, Param3 param3, Param4 param4, Param5 param5)
	{
		m_Function(param1, param2, param3, param4, param5);
	}

	void operator()()
	{
		m_Function(m_Param1, m_Param2, m_Param3, m_Param4, m_Param5);
	}

	virtual void run()
	{
		operator()();
	}

private:


	FunctionSignature	m_Function;
	Param1				m_Param1;
	Param2				m_Param2;
	Param3				m_Param3;
	Param4				m_Param4;
	Param5				m_Param5;
};


// --------------------------------------------------------------------------------------------------------
// end of five param version
// --------------------------------------------------------------------------------------------------------



template <typename RET>
CONCURRENT_CALL0<RET>* concurrent_call(RET (*f)())
{
	return new CONCURRENT_CALL0<RET>(f);
}


template <typename Param1, typename RET>
CONCURRENT_CALL<Param1, RET>* concurrent_call(RET (*f)(Param1), Param1 p1)
{
	return new CONCURRENT_CALL<Param1, RET>(f, p1);
}

template <typename Param1, typename Param2, typename RET>
CONCURRENT_CALL2<Param1, Param2, RET>* concurrent_call(RET (*f)(Param1, Param2),
															Param1 param1,
															Param2	param2)
{
	return new CONCURRENT_CALL2<Param1, Param2, RET>(f, param1, param2);
}

template <typename Param1, typename Param2, typename Param3, typename RET>
CONCURRENT_CALL3<Param1, Param2, Param3, RET>* concurrent_call(RET (*f)(Param1, Param2, Param3),
																	Param1 param1,
																	Param2 param2,
																	Param3	param3)
{
	return new CONCURRENT_CALL3<Param1, Param2, Param3, RET>(f, param1, param2, param3);
}

template <typename Param1, typename Param2, typename Param3, typename Param4, typename RET>
CONCURRENT_CALL4<Param1, Param2, Param3, Param4, RET>* concurrent_call(RET (*f)(Param1, Param2, Param3, Param4),
																						Param1 param1,
																						Param2 param2,
																						Param3	param3,
																						Param4	param4)
{
	return new CONCURRENT_CALL4<Param1, Param2, Param3, Param4, RET>(f, param1, param2, param3, param4);
}


template <typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename RET>
CONCURRENT_CALL5<Param1, Param2, Param3, Param4, Param5, RET>* concurrent_call(RET (*f)(Param1, Param2, Param3, Param4, Param5),
																								Param1 param1,
																								Param2 param2,
																								Param3	param3,
																								Param4	param4,
																								Param5 param5)
{
	return new CONCURRENT_CALL5<Param1, Param2, Param3, Param4, Param5, RET>(f, param1, param2, param3, param4, param5);
}

template <typename RET>
Future<RET>* concurrent_call2(RET (*f)())
{
	printf("Here 0\n");
	Future<RET>* ret = new Future<RET>(new CONCURRENT_CALL0<RET>(f));

	TaskManager::getInstance().addTask(ret);

	return ret;

}

template <typename Param1, typename RET>
Future<RET>* concurrent_call2(RET (*f)(Param1), Param1 p1)
{
	printf("Here 1\n");
	Future<RET>* ret = new Future<RET>(new CONCURRENT_CALL<Param1, RET>(f, p1));

	TaskManager::getInstance().addTask(ret);

	return ret;
}

#endif
