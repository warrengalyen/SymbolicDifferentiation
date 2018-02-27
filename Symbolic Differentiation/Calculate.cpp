#include "stdafx.h"
#include "Calculate.h"
#include <math.h>
#include <vector>

int ExpressionItem::GetCalculation(bool bValue)
{
	if (m_nFunction != -1)
	{
		int nIndex = m_sInput.Find('(');
		CString str = m_sInput.Mid(nIndex + 1);
		// get the string between function perenthesis
		str = str.Left(str.ReverseFind(')'));

		double u;
		int nError = Calculate(str, u);
		if (nError != 0 && bValue == true)
			return nError;
		if (nError == 0)
		{
			switch (m_nFunction)
			{
			case 13:	// asin
			case 14:	// acos
			case 16:	// asec
			case 17:	// acosec
				if (u < -1 || u > 1)
					return ERROR_INVALID_ARGUMENT;
				break;
			case 25:	// sqrt
			case 26:	// log10
			case 27:	// log
			case 28:	// ln
				if (u <= 0)
					return ERROR_INVALID_ARGUMENT;
				break;
			}
			switch (m_nFunction)
			{
			case 0:		m_dOutput = u;			break;
			case 1:		m_dOutput = sin(u);		break;
			case 2:		m_dOutput = cos(u);		break;
			case 3:		m_dOutput = tan(u);		break;
			case 4:		m_dOutput = cos(u);		break;	// sec
			case 5:		m_dOutput = sin(u);		break;	// cosec
			case 6:		m_dOutput = tan(u);		break;	// cot
			case 7:		m_dOutput = sinh(u);	break;
			case 8:		m_dOutput = cosh(u);	break;
			case 9:		m_dOutput = tanh(u);	break;
			case 10:	m_dOutput = cosh(u);	break;	// sech
			case 11:	m_dOutput = sinh(u);	break;	// cosech
			case 12:	m_dOutput = tanh(u);	break;	// coth
			case 13:	m_dOutput = asin(u);	break;
			case 14:	m_dOutput = acos(u);	break;
			case 15:	m_dOutput = atan(u);	break;
			case 16:	m_dOutput = acos(u);	break;	// asec
			case 17:	m_dOutput = asin(u);	break;	// acosec
			case 18:	m_dOutput = atan(u);	break;	// acot
			case 19:	str = asinh(str);		break;
			case 20:	str = acosh(str);		break;
			case 21:	str = atanh(str);		break;
			case 22:	str = asech(str);		break;
			case 23:	str = acosech(str);		break;
			case 24:	str = acoth(str);		break;
			case 25:	m_dOutput = sqrt(u);	break;
			case 26:	m_dOutput = log10(u);	break;
			case 27:	m_dOutput = log(u);		break;
			case 28:	m_dOutput = log(u);		break;
			case 29:	m_dOutput = sign(u);	break;
			case 30:	m_dOutput = abs(u);		break;
			}
			switch (m_nFunction)
			{
			case 4:		// sec
			case 5:		// cosec
			case 6:		// cot
			case 10:	// sech
			case 11:	// cosech
			case 12:	// coth
			case 16:	// asec
			case 17:	// acosec
			case 18:	// acot
				if (m_dOutput == 0)
					return ERROR_INVALID_ARGUMENT;
				m_dOutput = 1 / m_dOutput;
				break;
			case 19:	// asinh
			case 20:	// acosh
			case 21:	// atanh
			case 22:	// asech
			case 23:	// acosech
			case 24:	// acoth
				if ((nError = Calculate(str, m_dOutput)) < 0)
					return nError;
				break;
			}
			m_dOutput *= m_nSign;
			m_sOutput = TrimFloat(m_dOutput);
		}
		else
		{
			if (m_nFunction == -1 && bValue == true)
				return ERROR_NOT_NUMERIC;
			switch (m_nFunction)
			{
			case 0:		m_sOutput = c(str, m_sStack);			break;
			case 1:		m_sOutput = c_sin(str, m_sStack);		break;
			case 2:		m_sOutput = c_cos(str, m_sStack);		break;
			case 3:		m_sOutput = c_tan(str, m_sStack);		break;
			case 4:		m_sOutput = c_sec(str, m_sStack);		break;
			case 5:		m_sOutput = c_cosec(str, m_sStack);		break;
			case 6:		m_sOutput = c_cot(str, m_sStack);		break;
			case 7:		m_sOutput = c_sinh(str, m_sStack);		break;
			case 8:		m_sOutput = c_cosh(str, m_sStack);		break;
			case 9:		m_sOutput = c_tanh(str, m_sStack);		break;
			case 10:	m_sOutput = c_sech(str, m_sStack);		break;
			case 11:	m_sOutput = c_cosech(str, m_sStack);	break;
			case 12:	m_sOutput = c_coth(str, m_sStack);		break;
			case 13:	m_sOutput = c_asin(str, m_sStack);		break;
			case 14:	m_sOutput = c_acos(str, m_sStack);		break;
			case 15:	m_sOutput = c_atan(str, m_sStack);		break;
			case 16:	m_sOutput = c_asec(str, m_sStack);		break;
			case 17:	m_sOutput = c_acosec(str, m_sStack);	break;
			case 18:	m_sOutput = c_acot(str, m_sStack);		break;
			case 19:	m_sOutput = c_asinh(str, m_sStack);		break;
			case 20:	m_sOutput = c_acosh(str, m_sStack);		break;
			case 21:	m_sOutput = c_atanh(str, m_sStack);		break;
			case 22:	m_sOutput = c_asech(str, m_sStack);		break;
			case 23:	m_sOutput = c_acosech(str, m_sStack);	break;
			case 24:	m_sOutput = c_acoth(str, m_sStack);		break;
			case 25:	m_sOutput = c_sqrt(str, m_sStack);		break;
			case 26:	m_sOutput = c_log10(str, m_sStack);		break;
			case 27:	m_sOutput = c_log(str, m_sStack);		break;
			case 28:	m_sOutput = c_ln(str, m_sStack);		break;
			case 29:	m_sOutput = c_sign(str, m_sStack);		break;
			case 30:	m_sOutput = c_abs(str, m_sStack);		break;
			}
			m_dOutput = m_nSign * ATOF(m_sOutput);
			m_sOutput = (m_nSign == -1 ? "-" : "") + m_sOutput;
			if (bValue == true && IsNumeric(m_sOutput) == false)
			{
				m_sStack.Empty();
				return ERROR_NOT_NUMERIC;
			}
		}
	}
	else
	{
		m_sOutput = m_sInput;
		if (bValue && IsNumeric(m_sOutput) == false)
		{
			m_sStack.Empty();
			return ERROR_NOT_NUMERIC;
		}
		m_dOutput = ATOF(m_sOutput);
	}

	return 0;
}

char* GetErrorMessage(int nError)
{
	switch (nError)
	{
		case ERROR_INVALID_ARGUMENT:
			return "Invalid argument";
		case ERROR_NOT_NUMERIC:
			return "Not numeric";
		case ERROR_DIVIDE_BY_ZERO:
			return "Divide by zero";
	}
	return "Unknown error";
}

int CalculateStack(std::vector<ExpressionItem*>& vStack, int& nExpression, CString& sOutput)
{
	ExpressionItem *pQI = vStack[nExpression++];
	int nError;
	if (pQI->m_cOperator)
	{
		CString cu, cv;
		// get left operand calculation
		if ((nError = CalculateStack(vStack, nExpression, cu)) < 0)
			return nError;
		// get right operand calculation
		if ((nError = CalculateStack(vStack, nExpression, cv)) < 0)
			return nError;

		if (IsNumeric(cu) == false || IsNumeric(cv) == false)
		{
			switch (pQI->m_cOperator)
			{
			case '-':	// c(u-v) = cu-cv
			case '+':	// c(u+v) = cu+cv
				pQI->m_sOutput = cu + pQI->m_cOperator + cv;
				break;
			case '*':	// c(u*v) = cu*cv
				pQI->m_sOutput = cu + '*' + cv;
				break;
			case '/':	// c(u/v) = cu/cv
				pQI->m_sOutput = cu + '/' + cv;
				break;
			case '^':	// c(u^v) = cu^cv
				pQI->m_sOutput = cu + '^' + cv;
				break;
			case '%':	// c(u%v) = cu%cv
				pQI->m_sOutput = cu + '%' + cv;
				break;
			}
			pQI->m_sOutput = '(' + pQI->m_sOutput + ')';
		}
		else
		{
			switch (pQI->m_cOperator)
			{
			case '-':	// c(u-v) = cu-cv
				pQI->m_sOutput.Format("%s", TrimFloat(ATOF(cu) - ATOF(cv)));
				break;
			case '+':	// c(u+v) = cu+cv
				pQI->m_sOutput.Format("%s", TrimFloat(ATOF(cu) + ATOF(cv)));
				break;
			case '*':	// c(u*v) = cu*cv
				pQI->m_sOutput.Format("%s", TrimFloat(ATOF(cu)*ATOF(cv)));
				break;
			case '/':	// c(u/v) = cu/cv
				if (ATOF(cv) == 0)
					return ERROR_DIVIDE_BY_ZERO;
				pQI->m_sOutput.Format("%s", TrimFloat(ATOF(cu) / ATOF(cv)));
				break;
			case '^':	// c(u^v) = cu^cv
				pQI->m_sOutput.Format("%s", TrimFloat(pow(ATOF(cu), ATOF(cv))));
				break;
			case '%':	// c(u%v) = cu%cv
				pQI->m_sOutput.Format("%s", TrimFloat((int)ATOF(cu) % (int)ATOF(cv)));
				break;
			}
		}
	}
	else
	{
		// get Expression calculation
		if ((nError = pQI->GetCalculation(false)) < 0)
		{
			sOutput = GetErrorMessage(nError);
			return nError;
		}
	}
	// return resulant calculation
	sOutput = pQI->m_sOutput;

	return 0;
}
int CalculateStack(std::vector<ExpressionItem*>& vStack, int& nExpression, double& dOutput)
{
	ExpressionItem *pQI = vStack[nExpression++];
	int nError;
	if (pQI->m_cOperator)
	{
		double cu, cv;
		// get left operand calculation
		if ((nError = CalculateStack(vStack, nExpression, cu)) < 0)
			return nError;
		// get right operand calculation
		if ((nError = CalculateStack(vStack, nExpression, cv)) < 0)
			return nError;

		switch (pQI->m_cOperator)
		{
		case '-':	// c(u-v) = cu-cv
			pQI->m_dOutput = cu - cv;
			break;
		case '+':	// c(u+v) = cu+cv
			pQI->m_dOutput = cu + cv;
			break;
		case '*':	// c(u*v) = cu*cv
			pQI->m_dOutput = cu * cv;
			break;
		case '/':	// c(u/v) = cu/cv
			if (cv == 0)
				return ERROR_DIVIDE_BY_ZERO;
			pQI->m_dOutput = cu / cv;
			break;
		case '^':	// d(u^v) = cu^cv
			if (cu < 0 && (int)cv != (double)cv)
				return ERROR_INVALID_ARGUMENT;
			pQI->m_dOutput = pow(cu, cv);
			break;
		case '%':	// d(u%v) = cu%cv
			pQI->m_dOutput = (int)cu % (int)cv;
			break;
		}
	}
	else
	{
		// get Expression calculation
		if ((nError = pQI->GetCalculation(true)) < 0)
			return nError;
	}
	// return resultant calculation
	dOutput = pQI->m_dOutput;

	return 0;
}

int Calculate(LPCSTR lpcsInput, double& dOutput)
{
	CString sInput = lpcsInput;
	// remove spaces
	sInput.Remove(' ');
	// make lower case
	sInput.MakeLower();
	// Optimize "--"
	OptimizeSign(sInput);

	int nError;
	std::vector<ExpressionItem*> vStack;
	// parse input equation and fill stack with operators and operands
	if ((nError = FillStack(sInput, vStack)) < 0)
		return nError;

	int nExpression = 0;
	// apply operators to operands
	nError = CalculateStack(vStack, nExpression, dOutput);
	for (std::vector<ExpressionItem*>::iterator pQI = vStack.begin(); pQI != vStack.end(); pQI++)
		delete *pQI;
	return nError;
}

CString Calculate(LPCSTR lpcsInput, CString& sStack, bool bOptimize)
{
	CString sInput = lpcsInput;
	// remove spaces
	sInput.Remove(' ');
	// make all characters lower case
	sInput.MakeLower();
	// Optimize "--"
	OptimizeSign(sInput);

	int nError;
	std::vector<ExpressionItem*> vStack;
	// parse input equation and fill stack with operators and operands
	if ((nError = FillStack(sInput, vStack)) < 0)
		return GetErrorMessage(nError);

	CString strOutput;
	double nOutput;
	int nExpression = 0;
	// apply operators to operands
	if (CalculateStack(vStack, nExpression, nOutput) < 0)
	{
		nExpression = 0;
		// apply operators to operands
		CalculateStack(vStack, nExpression, strOutput);
	}
	else
		strOutput = TrimFloat(nOutput);

	// fill the stack string from the stack vector
	for (std::vector<ExpressionItem*>::iterator pQI = vStack.begin(); pQI != vStack.end(); pQI++)
	{
		if ((*pQI)->m_cOperator)
			sStack += (*pQI)->m_cOperator + CString("\r\n");
		else
			sStack += "c(" + (*pQI)->m_sInput + ") = " + (*pQI)->m_sOutput + "\r\n";

		// insert tabs in front of the item stack
		InsertTabs((*pQI)->m_sStack);
		sStack += (*pQI)->m_sStack;
		delete *pQI;
	}

	if (bOptimize == true)
		// optimize the equation from unneeded elements
		Optimize(strOutput);
	// return output calculation
	return strOutput;
}
