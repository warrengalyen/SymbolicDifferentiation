#pragma once

#include <vector>
#include <atlstr.h> 

extern char* pFunctions[];

bool IsNumeric(LPCSTR lpcstr);
CString TrimFloat(double f);
void OptimizeSign(CString& str);
void Optimize(CString& str);
void InsertTabs(CString& str);
char *GetClose(char *p);
int GetFunction(LPCSTR pOpen);
int GetOperator(IN LPCSTR lpcstr, IN LPCSTR lpcstrOperators[]);

struct ExpressionItem
{
	ExpressionItem(LPCSTR lpcstrInput)
	{
		m_sInput = lpcstrInput;
		m_cOperator = 0;
		m_nFunction = -1;
		m_dOutput = 0;
		m_nSign = 1;
	}
	CString GetInput()
	{
		if (IsNumeric(m_sInput) == false && m_sInput.FindOneOf("+-/*^") != -1)
			return '(' + m_sInput + ')';
		return m_sInput;
	}
	char m_cOperator;
	CString m_sInput;
	CString m_sOutput;
	double m_dOutput;
	CString m_sStack;
	int m_nFunction;
	int m_nSign;
	void GetDifferentiation();
	// return error number, 0 on success
	int GetCalculation(bool bValue = false);
};

int FillStack(LPCSTR lpcstrInput, std::vector<ExpressionItem*>& vStack);