#include "stdafx.h"
#include "tool.h"
#include "CTPTrade.h"
#include "StrategyBase.h"
#include "typedefine.h"
#include "data.h"
#include "tinyxml.h"



Tool::Tool()
{
}

Tool::~Tool()
{
}

CString Tool::TransBS(char bs,CBaseTradeApi* t)
{
	CString ret = "";

	if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(t))
	{
		if(bs==0)
		{
			bs = '1';
		}
		else if(bs==1)
		{
			bs = '2';
		}

	}

	switch (bs)
	{
	case '1':	ret = "����";break; 
	case '2':	ret = "����";break; 
	default:	ret = "δ֪";break;
		break;
	}

	return ret;
}

CString Tool::TransKP(char kp,CBaseTradeApi* t)
{
	CString ret = "";

	if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(t))
	{
		if(kp==0)
		{
			kp = '1';
		}
		else if(kp==1)
		{
			kp = '2';
		}

	}

	switch (kp)
	{
	case '1':	ret = "����";break; 
	case '2':	ret = "ƽ��";break; 
	default:	ret = "δ֪";break;
		break;
	}

	return ret;
}

CString Tool::TransStatus(char status,CBaseTradeApi* t)
{
	CString ret = "";


	if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(t))
	{

		switch (status)
		{
			case '0':	ret = "�ѳ�";break; 
			case '1':	ret = "����";break; 
			case '2':	ret = "���еȴ�";break; 
			case '3':	ret = "�ѱ�";break; 
			case '4':	ret = "���еȴ�";break; 
			case '5':	ret = "�ѳ�/����";break; 
			default:	ret = "δ֪";break;

			break;
		}
	
	}
	else 
	{
		switch (status)
		{
			case '0':	ret = "δ��";break; 
			case '1':	ret = "����";break; 
			case '2':	ret = "�ѱ�";break; 
			case '3':	ret = "�ѱ�����";break; 
			case '4':	ret = "���ɴ���";break; 
			case '5':	ret = "����";break; 
			case '6':	ret = "�ѳ�";break; 
			case '7':	ret = "����";break; 
			case '8':	ret = "�ѳ�";break; 
			case '9':	ret = "�ϵ�";break; 

			default:	ret = "δ֪";break;

			break;
		}
	
	
	}



	return ret;

}

int Tool:: TransExchId(char * exchid)
{
	if(strcmp(exchid,"F1")==0)
	{
		return 11;
	}
	else if(strcmp(exchid,"F2")==0)
	{
		return 12;
	}
	else if(strcmp(exchid,"F3")==0)
	{
		return 13;
	}
	else if(strcmp(exchid,"F4")==0)
	{
		return 14;
	}
	else if(strcmp(exchid,"F5")==0)
	{
		return 15;
	}

	return 0;
}


bool Tool:: isWeiBao(char status,CBaseTradeApi* t)    //δ��
{
	bool ret = false;

	if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(t))
	{
		if(status=='2'|| status=='4' )
			ret  = true;
	}
	else
	{
		if(status=='0')
			ret  = true;
	}

	return ret;
}

bool Tool:: isDaiBao(char status,CBaseTradeApi* t)    //����
{
	bool ret = false;

	if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(t))
	{
		if(status=='2'|| status=='4' )
			ret  = true;
	}
	else
	{
		if(status=='1')
			ret  = true;
	}

	return ret;
}


bool Tool:: isYiBao(char status,CBaseTradeApi* t)     //�ѱ�
{
	bool ret = false;

	if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(t))
	{
		if(status=='3')
			ret  = true;
	}
	else
	{
		if(status=='2')
			ret  = true;
	}

	return ret;
}


bool Tool:: isBuChe(char status,CBaseTradeApi* t)     //����
{
	bool ret = false;

	if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(t))
	{
		if(status=='5')
			ret  = true;
	}
	else
	{
		if(status=='5')
			ret  = true;
	}

	return ret;
}


bool Tool:: isYiChe(char status,CBaseTradeApi* t)     //�ѳ�
{
	bool ret = false;

   //�ж��Ƿ���ctp
	if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(t))
	{
		if(status=='5')
			ret  = true;
	}
	else
	{
		if(status=='6')
			ret  = true;
	}

	return ret;
}


bool Tool:: isBuCheng(char status,CBaseTradeApi* t)   //����
{
	bool ret = false;

	//�ж��Ƿ���ctp

	if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(t))
	{
		if(status=='1')
			ret  = true;
	}
	else
	{
		if(status=='7')
			ret  = true;
	}

	return ret;
}


bool Tool:: isYiCheng(char status,CBaseTradeApi* t)   //�ѳ�
{
	bool ret = false;

    //�ж��Ƿ���ctp
	if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(t))
	{
		if(status=='0')
			ret  = true;
	}
	else
	{
		if(status=='8')
			ret  = true;
	}

	return ret;
}


bool Tool:: isFeiDan(char status,CBaseTradeApi* t)    //�ϵ�
{
	bool ret = false;

	//�ж��Ƿ���ctp
	if(CCTPTrade *c = dynamic_cast<CCTPTrade*>(t))
	{
		if(status=='x')
			ret  = true;
	}
	else
	{
		if(status=='9')
			ret  = true;
	}

	return ret;
}



string Tool::GetProgramDir()
{ 
	char exeFullPath[MAX_PATH];

	string StrPath="";

	GetModuleFileName(NULL,exeFullPath,MAX_PATH);

	StrPath = (string)exeFullPath;

	int pos = StrPath.find_last_of("\\",StrPath.length());

	return StrPath.substr(0,pos);

}

string Tool::GetModelDir()
{
	char exeFullPath[MAX_PATH];

	string StrPath="";

	GetModuleFileName(NULL,exeFullPath,MAX_PATH);

	StrPath = (string)exeFullPath;

	int pos = StrPath.find_last_of("\\",StrPath.length());

	return StrPath.substr(0,pos) + "\\strmodels\\";

}

CString Tool::getTimeStr()
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	CString Y,M,D,h,m,s ;
	Y.Format("%02d",st.wYear);
	M.Format("%02d",st.wMonth);
	D.Format("%02d",st.wDay);
	h.Format("%02d:",st.wHour);
	m.Format("%02d:",st.wMinute);
	s.Format("%02d",st.wSecond);

	return (Y+M+D+" "+h+m+s);
}

string Tool::getCodePrefix(string code)
{
	string ret ="";
	for(unsigned int i=0;i<code.length();i++)
	{
		if(code[i]>='0' && code[i]<='9')
			break;

		ret = ret + code[i];
	}

	return ret;
}

int Tool::getDayofWeek()
{
	SYSTEMTIME st;

	GetLocalTime(&st);

	return st.wDayOfWeek;

}

//"<root Version=\"1.0\" FuncID=\"999\"/>";
char* Tool::getRootAttribute(int type,string s)
{
	char* ret = "";
	TiXmlDocument *tx = new TiXmlDocument();

	tx->Parse(s.c_str());

	TiXmlElement* re = tx->RootElement(); 

	if(type==0)
	{
		ret = (char*)re->Attribute("FuncID");
	}
	else if(type==1)
	{
		ret = (char*)re->Attribute("Account");
	}

	return ret;
}


int Tool::getSessionid(string s)
{
	int sid = -9999;
	TiXmlDocument *tx = new TiXmlDocument();

	tx->Parse(s.c_str());

	TiXmlElement* re = tx->RootElement();  
	re->Attribute("SessionID",&sid);
 
	return sid;

}

char* Tool::getErrorInfo(string s)
{
	string errorinfo = "";
	TiXmlDocument *tx = new TiXmlDocument();

	tx->Parse(s.c_str());

	TiXmlElement* re = tx->RootElement();  
	TiXmlElement* item = re->FirstChildElement();
	//errorinfo = item->Attribute("ErrorInfo");
 
	return (char*)item->Attribute("ErrorInfo");

}


int Tool::getEntrustNo(string s)
{
	int ret = -9999;
	TiXmlDocument *tx = new TiXmlDocument();

	tx->Parse(s.c_str());

	TiXmlElement* re = tx->RootElement();  
	TiXmlElement* item = re->FirstChildElement();
	//errorinfo = item->Attribute("ErrorInfo");
	item->QueryIntAttribute("EntrustNO",&ret);

	return ret;

}



int Tool::TransRspEntrust(string s)
{
	int entrustno = -9999;
	TiXmlDocument *tx = new TiXmlDocument();

	tx->Parse(s.c_str());

	TiXmlElement* re = tx->RootElement();  
	TiXmlElement* item = re->FirstChildElement();
	item->Attribute("EntrustNO",&entrustno);
 
	int ret = entrustno;

	return ret;
}

void Tool::TransRspWithdraw(string s,int &entrustno,int &withdraw,char *errorinfo)
{

	TiXmlDocument *tx = new TiXmlDocument();

	tx->Parse(s.c_str());

	TiXmlElement* re = tx->RootElement();  
	TiXmlElement* item = re->FirstChildElement();
	item->Attribute("EntrustNO",&entrustno);
	item->Attribute("WithdrawNO",&withdraw);
	errorinfo = (char*)item->Attribute("ErrorInfo");

}

void Tool::TransRspSub(string s,char *code,char *account,char *subtype,char *datatype,int *entrustno,int *bs,double *price,char *tradetime,int *vol)
{
	TiXmlDocument *tx = new TiXmlDocument();

	tx->Parse(s.c_str());

	//root
	TiXmlElement* re = tx->RootElement(); 
	/*subtype = (char*)re->Attribute("Data");
	datatype = (char*)re->Attribute("ChangeType");
	account = (char*)re->Attribute("Account");*/
	strcpy(subtype,re->Attribute("Data"));
	strcpy(datatype,re->Attribute("ChangeType"));
	strcpy(account,re->Attribute("Account"));

	//data
	TiXmlElement* data = re->FirstChildElement();

	//item
	TiXmlElement* item = data->FirstChildElement();
	if(item == NULL)
		return;

	//Add����
	if(strcmp(datatype,"Add")==0)
	{
		if(strcmp(subtype,"Deal")==0)
		{			
			strcpy(code,item->Attribute("stock_code"));
			item->QueryIntAttribute("entrust_bs",bs);
			item->QueryIntAttribute("entrust_no",entrustno);
			item->QueryDoubleAttribute("business_price",price);			
			strcpy(tradetime,item->Attribute("business_time"));
			item->QueryIntAttribute("business_amount",vol);

			//ͳһ����0��1��Ϊ��������  
			if(*bs == 1)
				*bs = 0;
			else 
				*bs = 1;
		}
	}

}

void Tool::TransRspQuery(string s,char*account,CAccData*info)
{
	//info->m_bReady = false;

	TiXmlDocument *tx = new TiXmlDocument();

	tx->Parse(s.c_str());

	TiXmlElement* re = tx->RootElement(); 
	char *datatype = (char*)re->Attribute("Data");
	account = (char*)re->Attribute("Account");

	TiXmlElement* data = re->FirstChildElement();

	TiXmlElement* item;

	if(data==NULL)
	{
		info->m_sEntrustInfo.clear();
		info->m_bReady = true;
		return;
	}

	if(strcmp(datatype,"Entrust")==0)
	{
		//�����ί�����ݱ�
		info->m_sEntrustInfo.clear();

		for ( item = data->FirstChildElement(); item != NULL; item = item->NextSiblingElement())
		{
			CEntrustInfo *c = new CEntrustInfo();
	
			//strcpy(c->account,(char*)re->Attribute("Account"));
			c->account = account;
			strcpy(c->szInstrument,(char*)item->Attribute("stock_code"));
			item->QueryIntAttribute("entrust_no",&c->entrustno);
			item->QueryIntAttribute("entrust_amount",&c->orderAmount);
			item->QueryIntAttribute("business_amount",&c->tradeAmount);
			item->QueryIntAttribute("withdraw_amount",&c->cancelAmount);
			c->bs = ((char*)item->Attribute("entrust_bs"))[0];
			c->kp = '0';

			CString log;
			log.Format("ί�в�ѯ,��Լ:%s,ί�к�:%d,ί��:%d,�ɽ�:%d,����:%d",c->szInstrument,c->entrustno,c->orderAmount,c->tradeAmount,c->cancelAmount);
			::SendMessage(info->m_hMsgWnd,WM_MESSAGE_TRADE,0,(long)log.GetBuffer(0));

			//ֻ����δ�ɽ�������, ����
			if( (c->orderAmount > (c->tradeAmount + c->cancelAmount)) && (c->bs=='1') )
				info->m_sEntrustInfo.push_back(*c);

			delete c;
	
		}
	}
	else if(strcmp(datatype,"Asset")==0)
	{
		item = data->FirstChildElement();
		item->QueryDoubleAttribute("enable_balance",&(info->m_dEnableMoney));

		CString log;
		log.Format("�ʽ��ѯ,�˻�:%s,����:%.4f",account,info->m_dEnableMoney);
		::SendMessage(info->m_hMsgWnd,WM_MESSAGE_TRADE,0,(long)log.GetBuffer(0));

	}
	else if(strcmp(datatype,"Hold")==0)
	{
		//�����ί�����ݱ�
		info->m_sHoldsInfo.clear();

		for ( item = data->FirstChildElement(); item != NULL; item = item->NextSiblingElement())
		{
			CHoldsinfo *c = new CHoldsinfo();
	

			strcpy(c->szInstrument,(char*)item->Attribute("stock_code"));
			item->QueryIntAttribute("current_amount",&c->Position);
			item->QueryIntAttribute("enable_amount",&c->TdAvaiable);
			item->QueryDoubleAttribute("cost_price",&c->dTradePrice);
			double rate = 0.0;
			item->QueryDoubleAttribute("income_balance_ratio",&rate);
			char* name = (char*)item->Attribute("stock_name");
			
			CString log;
			log.Format("�ֲֲ�ѯ,��Լ:%s,����:%s,���:%d,����:%d,�ɱ���:%.4f,ӯ����:%.4f",c->szInstrument,name,c->Position,c->TdAvaiable,c->dTradePrice,rate);
			::SendMessage(info->m_hMsgWnd,WM_MESSAGE_TRADE,0,(long)log.GetBuffer(0));

			//ֻ�������õ�����
			if(c->TdAvaiable > 0 )
			{
				info->m_sHoldsInfo.push_back(*c);
				//��Ҫ���ĺ�Լ,����Ϣ����
				//"subcode#" + "131810" + ";" + "STOCK;UFX;" + "#"
				char subinfo[128] ;
				sprintf(subinfo,"subcode#%s;STOCK;UFX;#",c->szInstrument);
				::SendMessage(info->m_hMsgWnd,WM_MESSAGE_ENTRUST,7,(long)subinfo);
			}
			delete c;
	
		}

	}

	info->m_bReady = true;

}





/*"<root Version=\"1.0\" FuncID=\"1004\" SessionID=\"1000001\">\
	<item Code=\"600570\" Market=\"Stock\" Account=\"10000013\" BS=\"Buy\" Dir=\"Open\" Amount=\"100\" Price=\"66.55\" Prop=\"0\"/>\
</root>";
*/
string Tool::TransEntrustInfo(char * instrument, int count, char direction, char offsetflag, double price,int orderref,char *account)
{

	string ret ="";

	TiXmlPrinter Pstr;

	string bs = "";
	string kp = "";
	string sessionid ="";
	sprintf((char*)sessionid.c_str(),"%d",orderref);

	if(direction=='0')
		bs= "Buy";
	else if(direction=='1')
		bs= "Sell";

	if(offsetflag=='0')
		kp= "Open";
	else if(offsetflag=='1')
		kp= "Close";

	TiXmlDocument *pDoc=new TiXmlDocument;

	TiXmlElement *root= new TiXmlElement("root");

	root->SetAttribute("Version","1.0");
	root->SetAttribute("FuncID","1004");
	root->SetAttribute("SessionID",sessionid.c_str());

	pDoc->LinkEndChild(root);

	TiXmlElement *item= new TiXmlElement("item");

	item->SetAttribute("Code",instrument);
	item->SetAttribute("Market","Stock");
	item->SetAttribute("Account",account);
	item->SetAttribute("BS",bs.c_str());
	item->SetAttribute("Dir",kp.c_str());
	item->SetAttribute("Amount",count);
	item->SetDoubleAttribute("Price",price);
	item->SetAttribute("Prop","0");

	root->LinkEndChild(item);

	pDoc->Accept(&Pstr);

	ret = Pstr.CStr();

	return ret;
}

string Tool::TransWithDraw(int entrustno,char *account)
{

	string ret ="";

	TiXmlPrinter Pstr;

	TiXmlDocument *pDoc=new TiXmlDocument;

	TiXmlElement *root= new TiXmlElement("root");

	root->SetAttribute("Version","1.0");
	root->SetAttribute("FuncID","1005");
	root->SetAttribute("SessionID","1000001");

	pDoc->LinkEndChild(root);

	TiXmlElement *item= new TiXmlElement("item");

	item->SetAttribute("Account",account);
	item->SetDoubleAttribute("EntrustNO",entrustno);

	root->LinkEndChild(item);

	pDoc->Accept(&Pstr);

	ret = Pstr.CStr();

	return ret;


}

string Tool::TransSubInfo(int type,char *account)
{

	string ret ="";

	char* subType ="";

	TiXmlPrinter Pstr;

	switch (type)
	{
	case 0:
		subType = "Entrust";
		break;
	case 1:
		subType = "Deal";
		break;
	case 2:
		subType = "Hold";
		break;
	case 3:
		subType = "Asset";
		break;
	default:
		break;
	}

	TiXmlDocument *pDoc=new TiXmlDocument;

	TiXmlElement *root= new TiXmlElement("root");

	root->SetAttribute("Version","1.0");
	root->SetAttribute("FuncID","1500");

	pDoc->LinkEndChild(root);

	TiXmlElement *item= new TiXmlElement("reg");

	item->SetAttribute("Data",subType);
	item->SetAttribute("Market","Stock");
	item->SetAttribute("Account",account);
	item->SetAttribute("Reg","True");

	root->LinkEndChild(item);

	pDoc->Accept(&Pstr);

	ret = Pstr.CStr();

	return ret;

}


string Tool::TransQueryInfo(int type,char *account)
{
	string ret ="";

	char* subType ="";

	TiXmlPrinter Pstr;

	switch (type)
	{
	case 0:
		subType = "Entrust";
		break;
	case 1:
		subType = "Deal";
		break;
	case 2:
		subType = "Hold";
		break;
	case 3:
		subType = "Asset";
		break;
	default:
		break;
	}

	TiXmlDocument *pDoc=new TiXmlDocument;

	TiXmlElement *root= new TiXmlElement("root");

	root->SetAttribute("Version","1.0");
	root->SetAttribute("FuncID","1501");

	pDoc->LinkEndChild(root);

	TiXmlElement *item= new TiXmlElement("query");

	item->SetAttribute("Data",subType);
	item->SetAttribute("Market","Stock");
	item->SetAttribute("Account",account);
	
	root->LinkEndChild(item);

	pDoc->Accept(&Pstr);

	ret = Pstr.CStr();

	return ret;

}

