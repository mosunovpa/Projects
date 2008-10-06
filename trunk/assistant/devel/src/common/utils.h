/**
 * \file utils.h
 *
 * \since 2006/02/08
 * \author pavel
 */
#ifndef utils_h__
#define utils_h__

namespace Utils
{

template <typename T>
class CRestoredValueT
{
public:
	CRestoredValueT(T& variable, const T& value)
		:	m_variable(variable)
	{
		m_old_value = m_variable;
		m_variable = value;
	}
	~CRestoredValueT()
	{
		m_variable = m_old_value;
	};
private:
	T& m_variable;
	T m_old_value;
};

}

#endif // utils_h__