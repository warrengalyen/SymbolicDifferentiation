#include "stdafx.h"
#include "Differentiate.h"

CString DifferentiateStack(std::vector<ExpressionItem*>& vStack, int& nExpression)
{
	ExpressionItem *pQI = vStack[nExpression++];
	if (pQI->m_cOperator)
	{
		// get left operand
		CString u = vStack[nExpression]->GetInput();
		// get left operand differentiation
		CString du = DifferentiateStack(vStack, nExpression);
		// get right operand
		CString v = vStack[nExpression]->GetInput();
		// get right operand differentiation
		CString dv = DifferentiateStack(vStack, nExpression);

		if (du == '0')   // u is a constant
		{
			switch (pQI->m_cOperator)
			{
			case '-':	// d(u-v) = -dv
				pQI->m_sOutput = "(-" + dv + ')';
				break;
			case '+':	// d(u+v) = dv
				pQI->m_sOutput = dv;
				break;
			case '*':	// d(u*v) = u*dv
				pQI->m_sOutput = u + '*' + dv;
				break;
			case '/':	// d(u/v) = (-u*dv)/v^2
				pQI->m_sOutput = "(-" + u + '*' + dv + ")/(" + v + ")^2";
				break;
			case '^':	// d(u^v) = dv*u^v*ln(u)
				pQI->m_sOutput = dv + "*" + u + "^" + v + (u == 'e' ? "" : "*ln(" + u + ")");
				break;
			}
		}
		else if (dv == '0')		// v is a constant
		{
			switch (pQI->m_cOperator)
			{
			case '-':	// d(u-v) = du
			case '+':   // d(u+v) = du
				pQI->m_sOutput = du;
				break;
			case '*':	// d(u*v) = du*v
				pQI->m_sOutput = du + '*' + v;
				break;
			case '/':	// d(u/v) = du/v
				pQI->m_sOutput = '(' + du + ")/" + v;
			case '^':	// d(u^v) = v*u^(v-1)*du
				pQI->m_sOutput.Format("%s*%s^%s*%s", v, u, TrimFloat(atof(v) - 1), du);
			}
		}
		else
		{
			switch (pQI->m_cOperator)
			{
			case '-':	// d(u-v) = du-dv
			case '+':	// d(u+v) = du+dv
				pQI->m_sOutput = '(' + du + pQI->m_cOperator + dv + ')';
				break;
			case '*':	// d(u*v) = u*dv+du*v
				pQI->m_sOutput = '(' + u + '*' + dv + '+' + du + '*' + v + ')';
				break;
			case '/':	// d(u/v) = (du*v-u*dv)/v^2
				pQI->m_sOutput = '(' + du + '*' + v + '-' + u + '*' + dv + ")/(" + v + ")^2";
				break;
			case '^':	// d(u^v) = v*u^(v-1)*du+u^v*ln(u)*dv
				pQI->m_sOutput = '(' + v + '*' + u + "^(" + v + "-1)*" + du + '+' + u + '^' + v + "*ln(" + u + ")*" + dv + ')';
				break;
			}
		}
	}
	else
	{
		// get Expression differentiation
		pQI->GetDifferentiation();
	}
	// return resultant differentiation
	return pQI->m_sOutput;
}

CString Differentiate(LPCSTR lpcstrInput, CString& sStack, bool bOptimize)
{
	CString sInput = lpcstrInput;
	// remove spaces
	sInput.Remove(' ');
	// convert to lower case
	sInput.MakeLower();
	// Optimize "--"
	OptimizeSign(sInput);

	std::vector<ExpressionItem*> vStack;
	// parse input equation and fill stack with operators and operands
	if (FillStack(sInput, vStack) < 0)
		return "Invalid input";

	int nExpression = 0;
	// apply operators to operands
	CString sOutput = DifferentiateStack(vStack, nExpression);

	// fill the stack string from the stack vector
	for (std::vector<ExpressionItem*>::iterator pQI = vStack.begin(); pQI != vStack.end(); pQI++)
	{
		if ((*pQI)->m_cOperator)
			sStack += (*pQI)->m_cOperator + CString("\r\n");
		else
			sStack += "d(" + (*pQI)->m_sInput + ")/dx = " + (*pQI)->m_sOutput + "\r\n";

		// insert tabs in front of the item stack
		InsertTabs((*pQI)->m_sStack);
		sStack += (*pQI)->m_sStack;
		delete *pQI;
	}

	if (bOptimize == true)
		// optimize the equation, removing unneeded elements
		Optimize(sOutput);

	// return output differentiation
	return sOutput;
}

void ExpressionItem::GetDifferentiation()
{
	int nIndex;
	if (m_nFunction != -1)
	{
		nIndex = m_sInput.Find('(');
		CString str = m_sInput.Mid(nIndex + 1);
		// get the string between function parenthesis
		str = str.Left(str.ReverseFind(')'));
		switch (m_nFunction)
		{
		case 0:		m_sOutput = d(str, m_sStack);			break;
		case 1:		m_sOutput = d_sin(str, m_sStack);		break;
		case 2:		m_sOutput = d_cos(str, m_sStack);		break;
		case 3:		m_sOutput = d_tan(str, m_sStack);		break;
		case 4:		m_sOutput = d_sec(str, m_sStack);		break;
		case 5:		m_sOutput = d_cosec(str, m_sStack);		break;
		case 6:		m_sOutput = d_cot(str, m_sStack);		break;
		case 7:		m_sOutput = d_sinh(str, m_sStack);		break;
		case 8:		m_sOutput = d_cosh(str, m_sStack);		break;
		case 9:		m_sOutput = d_tanh(str, m_sStack);		break;
		case 10:	m_sOutput = d_sech(str, m_sStack);		break;
		case 11:	m_sOutput = d_cosech(str, m_sStack);	break;
		case 12:	m_sOutput = d_coth(str, m_sStack);		break;
		case 13:	m_sOutput = d_asin(str, m_sStack);		break;
		case 14:	m_sOutput = d_acos(str, m_sStack);		break;
		case 15:	m_sOutput = d_atan(str, m_sStack);		break;
		case 16:	m_sOutput = d_asec(str, m_sStack);		break;
		case 17:	m_sOutput = d_acosec(str, m_sStack);	break;
		case 18:	m_sOutput = d_acot(str, m_sStack);		break;
		case 19:	m_sOutput = d_asinh(str, m_sStack);		break;
		case 20:	m_sOutput = d_acosh(str, m_sStack);		break;
		case 21:	m_sOutput = d_atanh(str, m_sStack);		break;
		case 22:	m_sOutput = d_asech(str, m_sStack);		break;
		case 23:	m_sOutput = d_acosech(str, m_sStack);	break;
		case 24:	m_sOutput = d_acoth(str, m_sStack);		break;
		case 25:	m_sOutput = d_sqrt(str, m_sStack);		break;
		case 26:	m_sOutput = d_log10(str, m_sStack);		break;
		case 27:	m_sOutput = d_log(str, m_sStack);		break;
		case 28:	m_sOutput = d_ln(str, m_sStack);		break;
		case 29:	m_sOutput = d_sign(str, m_sStack);		break;
		case 30:	m_sOutput = d_abs(str, m_sStack);		break;
		}
		m_sOutput = (m_nSign == -1 ? "-" : "") + m_sOutput;
	}
	else
	{
		// dx/dx = 1
		if (m_sInput == 'x' || m_sInput == "+x")
			m_sOutput = '1';
		else if (m_sInput == "-x")
			m_sOutput = "-1";
		else if (IsNumeric(m_sInput))
			// dc/dx = 0, where c is a constant
			m_sOutput = '0';
		else
			// du/dx, where u is a function of x
			m_sOutput = 'd' + m_sInput + "/dx";
	}
}

