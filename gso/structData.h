#ifndef _STRUCTDATA_H
#define _STRUCTDATA_H


typedef struct 
{
	char* cmd;
	void (*cmd_handle)(CString& );
}CMD_CONTROL;



#define INIT		"/zkfinger/init"
#define COUNT		"/zkfinger/count"
static CMD_CONTROL cmd_control[35];


CMD_CONTROL CCmdParse::cmd_control[]=
{
	{ INIT,		init },			        //修改测试例参数
	{ COUNT,	dbcout },				//运行测试例
	{ NULL,NULL}
}
void CCmdParse::init(CString &strCmd)
{
}
void CCmdParse::dbcout(CString &strCmd)
{
}

void CCmdParse::RunCMDControl(CString &strCmd)
{
	vector<CString> t_vCMD = Split(strCmd," ");
	int i=0;
	while(cmd_control[i].cmd != NULL)
	{
		if(cmd_control[i].cmd == t_vCMD[0])
		{
			(*cmd_control[i].cmd_handle)(strCmd);
			break;
		}
		i++;
	}
}
#endif