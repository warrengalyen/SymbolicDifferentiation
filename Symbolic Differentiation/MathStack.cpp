#include "StdAfx.h"
#include "MathStack.h"
#include <vector>
using namespace std;

#define DIGITS			"0123456789."
#define IsDigit(c)		(c && strchr(DIGITS, c) != NULL)

// list of supported functions
char* pFunctions[] = {
	"(",

	"sin(",
	"cos(",
	"tan(",
	"sec(",
	"cosec(",
	"cot(",

	"sinh(",
	"cosh(",
	"tanh(",
	"sech(",
	"cosech(",
	"coth(",

	"asin(",
	"acos(",
	"atan(",
	"asec(",
	"acosec(",
	"acot(",

	"asinh(",
	"acosh(",
	"atanh(",
	"asech(",
	"acosech(",
	"acoth(",

	"sqrt(",

	"log10(",
	"log(",
	"ln(",

	"sign(",
	"abs(",
	NULL
};

/*
 *  GetClose: saerch for the close parenthesis
 */
char *GetClose(char *p)
{
	// set parenthesis count to one
	int nOpen = 1;
	while (*(++p))
	{
		if (*p == '(')
			nOpen++;
		else if (*p == ')')
			if (--nOpen == 0)
				return ++p;
	}
	return NULL;
}

/*
 *  GetFunction: Check if the initial input string matches any of the supprted functions
 */
int GetFunction(LPCSTR pOpen, int& nSign)
{
	LPSTR p = (LPSTR)pOpen;
	if (*p == '-')
		nSign = -1, p++;
	else if (*p == '+')
		nSign = +1, p++;
	
	for (int nIndex = 0; pFunctions[nIndex]; nIndex++)
	{
		if (memcmp(p, pFunctions[nIndex], strlen(pFunctions[nIndex])) == 0)
		{
			if (GetClose(p + strlen(pFunctions[nIndex]) - 1) == NULL)
				return -1;
			return nIndex;
		}
	}
	return -1;
}

bool IsRightSign(IN char c, IN LPCSTR lpcsOperators[], IN int nIndex)
{
	for (; lpcsOperators[nIndex]; nIndex++)
	{
		if (strchr(lpcsOperators[nIndex], c) != NULL)
			return false;
	}
	return true;
}

/*
 * GetOperator: scan the input string to search for any of the input operators
 */
int GetOperator(IN LPCSTR lpcs, IN LPCSTR lpcsOperators[])
{
	for (int nIndex = 0; lpcsOperators[nIndex]; nIndex++)
	{
		int nOpen = 0;
		// scan the expression from its end
		LPSTR p = (LPSTR)lpcs + strlen(lpcs) - 1;
		// loop until start of expression
		while (p >= lpcs)
		{
			// check for close
			if (*p == ')')
				nOpen++;
			// check for open
			else if (*p == '(')
				nOpen--;
			// check for operator
			else if (nOpen == 0 && strchr(lpcsOperators[nIndex], *p) != NULL)
			{
				// check if the operator is not a sign mark
				if ((*p != '-' && *p != '+') || (p != lpcs && IsRightSign(*(p - 1), lpcsOperators, nIndex + 1)))
					// return operator index
					return (int)(p - lpcs);
			}
			p--;
		}
	}
	// operator not found
	return -1;
}

int FillStack(LPCSTR lpcsInput, std::vector<ExpressionItem*>& vStack)
{
	// operators array from high to low priority
	LPCSTR lpcsOperators[] = { "+-", "*/", "^%", NULL };

	// insert first input into the stack
	vStack.push_back(new ExpressionItem(lpcsInput));
	// loop through the Expression stack to check if any expression can be divided into two queries
	for (int nIndex = 0; nIndex < (int)vStack.size(); nIndex++)
	{
		// check if Expression item is an operator
		if (vStack[nIndex]->m_cOperator == 0)
		{
			// copy Expression string
			CString str = vStack[nIndex]->m_sInput;
			// parse expression to find operators
			int nOpIndex = GetOperator(str, lpcsOperators);
			if (nOpIndex != -1)
			{
				// split the Expression into two queries at the operator index
				vStack[nIndex]->m_cOperator = str[nOpIndex];
				// add the left operand of the operator as a new expression
				vStack.insert(vStack.begin() + nIndex + 1, new ExpressionItem(str.Left(nOpIndex)));
				// add the right operand of the operator as new expression
				vStack.insert(vStack.begin() + nIndex + 2, new ExpressionItem(str.Mid(nOpIndex + 1)));
			}
			else // check if Expression string starts with a function or parenthesis
			{
				if ((vStack[nIndex]->m_nFunction = GetFunction(str, vStack[nIndex]->m_nSign)) == 0 && vStack[nIndex]->m_nSign == 0)
				{
					// remove parenthesis and rescan the Expression
					vStack[nIndex--]->m_sInput = str.Mid(1, str.GetLength() - 2);
				}
			}
		}
	}
	return 0;
}

void InsertTabs(CString& str)
{
	if (str.IsEmpty())
		return;
	
	str.Insert(0, '\t');
	str.Replace("\n", "\n\t");
	str.TrimRight('\t');
}

void OptimizeSign(CString& str)
{
	int nIndex = 0;
	// replace "--" with "" or "+"
	while ((nIndex = str.Find("--", nIndex)) != -1)
	{
		if (nIndex == 0 || strchr("(+-/*^", str[nIndex - 1]) != NULL)
			str.Delete(nIndex, 2);
		else
			str.Delete(nIndex, 1), str.SetAt(nIndex, '+');
	}
	nIndex = 0;
	// replace "+-" with "-"
	while ((nIndex = str.Find("+-", nIndex)) != -1)
		str.Delete(nIndex);
}

void Optimize(CString& str)
{
	int nLength = str.GetLength();

	OptimizeSign(str);

	char* pClose;
	int nIndex = -1;
	// replace "((....))"  with "(....)"
	while ((nIndex = str.Find("((", nIndex + 1)) != -1)
		if (*(pClose = GetClose(str.GetBuffer(0) + nIndex + 1)) == ')')
		{
			str.Delete((int)(pClose - str.GetBuffer(0)));
			str.Delete(nIndex);
		}

	nIndex = -1;
	// remove any 1*
	while ((nIndex = str.Find("1*", nIndex + 1)) != -1)
		if (nIndex == 0 || strchr("+-*(", str[nIndex - 1]) != NULL)
			str.Delete(nIndex, 2);

	nIndex = -1;
	// remove any *1
	while ((nIndex = str.Find("*1", nIndex + 1)) != -1)
		if (nIndex + 2 == str.GetLength() || strchr("+-*(", str[nIndex + 2]) != NULL)
			str.Delete(nIndex, 2);

	nIndex = -1;
	// remove any exponent equal 1
	while ((nIndex = str.Find("^1", nIndex + 1)) != -1)
		if (nIndex + 2 == str.GetLength() || strchr("+-*(", str[nIndex + 2]) != NULL)
			str.Delete(nIndex, 2);

	nIndex = 0;
	// remove unneeded parentheses 
	while ((nIndex = str.Find('(', nIndex)) != -1)
	{
		// "nscthg0" is the end characters of all supported functions
		if (nIndex > 0 && strchr("nscthg0", str[nIndex - 1]) != NULL)
		{
			nIndex++;
			continue;
		}
		// find the parenthesis close
		char* pClose = GetClose(str.GetBuffer(0) + nIndex);
		if (pClose == NULL)
			return;
		// get the index of the close char
		int nCloseIndex = (int)(pClose - str.GetBuffer(0) - 1);
		// check if the parentheses in the start and the end of the string
		if ((nIndex == 0 && nCloseIndex == str.GetLength() - 1) ||
			nCloseIndex == nIndex + 2 ||
			// check if the string doesn't include any operator
			IsNumeric(str.Mid(nIndex + 1, nCloseIndex - nIndex - 1)) == true)
		{
			// delete the far index of ')'
			str.Delete(nCloseIndex);
			// delete the near index of '('
			str.Delete(nIndex);
		}
		else
			nIndex++;
	}

	if (nLength != str.GetLength())
		Optimize(str);
}

CString TrimFloat(double f)
{
	CString str;
	str.Format("%f", f);
	if (str.Find('.') != -1)
		str.TrimRight('0');
	str.TrimRight('.');
	return str;
}

bool IsNumeric(LPCSTR lpcs)
{
	char* p = (char*)lpcs;
	if (*p == '-' || *p == '+')
		p++;
	if (*p == 'e' && *(p + 1) == 0)
		return true;
	if (*p == 'p' && *(p + 1) == 'i' && *(p + 2) == 0)
		return true;
	
	while (*p)
	{
		if (IsDigit(*p) == false)
			return false;
		p++;
	}
	return true;
}