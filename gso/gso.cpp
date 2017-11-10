// gso.cpp : 定义控制台应用程序的入口点。
//

#include "stdio.h"
#include "soapH.h"
#include "add.nsmap"

#include "httppost.h"
#include "httpform.h"

#include "json/json.h"

#define MAX_THR (5)
#define BACKLOG (100)


int   myhttp_get(struct   soap   *soap);//curl -X GET IP:port/path   curl IP:port/path   
int   myhttp_put(struct   soap   *soap);//curl -X PUT IP:port/path
int   myhttp_post(struct   soap   *soap);//curl -d '{"id":1,"info":"hello"}' IP:port/path


int   main(int argc, char* argv[])
{
	int port = 8001;

	struct soap a_soap;  
	//初始化运行时环境  
	soap_init(&a_soap);  
	a_soap.fget=myhttp_get;
	a_soap.fput=myhttp_put;
	if (soap_register_plugin_arg(&a_soap, http_form, (void*)myhttp_post))
	{
		soap_print_fault(&a_soap, stderr);
	}

	struct soap * soap_thr[MAX_THR];  

	SOAP_SOCKET m,s;  

	//绑定服务端口  
	m = soap_bind(&a_soap,NULL,port,BACKLOG);  
	//循环直至服务套接字合法  
	int nLoopCount = 10;
	while (!soap_valid_socket(m)&& nLoopCount>0)  
	{  
		fprintf(stderr,"Bind port [%d] error !!!\n",port);
		Sleep(1000);
		nLoopCount--;
		m = soap_bind(&a_soap,NULL,port,BACKLOG);  
	}
	if (nLoopCount==0)
	{
		fprintf(stderr,"Bind port [%d] error !!!\n\n",port);
		return 0;
	}
	fprintf(stderr,"socket setup,socket:%d,port:%d \n",m,port);

	

	for(;;)  
	{  
		//接受客户端的连接  
		s = soap_accept(&a_soap); 
		printf("path :\n",a_soap.path); 
		if (!soap_valid_socket(s))   
		{  
			if (a_soap.errnum)   
			{  
				soap_print_fault(&a_soap,stderr);  
				continue;  
			}else  
			{  
				fprintf(stderr,"Server timed out ");  
				break;  
			}  
		}  
		//客户端的IP地址  
		fprintf(stderr,"Accepted connection from IP= %d.%d.%d.%d socket = %d ",  
			((a_soap.ip)>>24)&&0xFF,((a_soap.ip)>>16)&0xFF,((a_soap.ip)>>8)&0xFF,(a_soap.ip)&0xFF,(a_soap.socket));    
		//请求的套接字进入队列，如果队列已满则循环等待  
		soap_thr[0] = soap_copy(&a_soap); 
		soap_thr[0]->socket=s;
		//soap_begin_serve(soap_thr[0]);
		soap_serve(soap_thr[0]);
	}  
	//服务结束后的清理工作  

	soap_done(soap_thr[0]);  
	free(soap_thr[0]); 


	//分离运行时的环境  
	soap_done(&a_soap);  
	return 0;
}


char* getPostData(soap * soaptmp)
{
	soaptmp->buf[soaptmp->buflen] = 0;
	char* p = soaptmp->buf;
	if (!strncmp(p, "POST",strlen("POST")) || 
		!strncmp(p, "GET",strlen("GET")) || 
		!strncmp(p, "PUT",strlen("PUT")) ||
		!strncmp(p, "DEL",strlen("DEL"))
		)
	{
		for(;*p;p++)
		{
			if (!strncmp(p, "\r\n\r\n",strlen("\r\n\r\n")))
				break;
		}
		return p+strlen("\r\n\r\n");
	}
	else
		return p;
}
void getPostJsonData(soap *soaptmp)
{
	char *p = getPostData(soaptmp);
    Json::Reader reader;
	Json::Value root;
	if (reader.parse(p, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素
	{
		printf("reader.parse success:\n");

		int nid = root["id"].asInt();
		std::string strinfo = root["info"].asString(); 
		printf("parse id as int id:%d ,parse infor asString:%s\n",nid,strinfo.c_str());
	}
	else
	{
		printf("reader.parse erro");
	}
}
	
int   myhttp_get(struct   soap   *soap)
{
	int n = soap->buflen;
	int nl = strlen(soap->buf);
	int n2 = soap->count;
	int n3 = strlen(soap->path);
	return 0;
}
int   myhttp_put(struct   soap   *soap)
{
	int n = soap->buflen;
	int nl = strlen(soap->buf);
	int n2 = soap->count;
	int n3 = strlen(soap->path);
	soap->http_content;
	soap->http_version;
	soap->msgbuf;
	soap->state;
	return 0;
}

int   myhttp_post(struct   soap   *soap)
{
	int n = soap->buflen;
	int nl = strlen(soap->buf);
	getPostJsonData(soap);
	int n2 = soap->count;
	int n3 = strlen(soap->path);
	return 0;
}
int ns__add(struct soap *soap, int num1, int num2, int* result )   
{
	if (NULL == result)
	{
		printf("Error:The third argument should not be NULL!\n");
		return SOAP_ERR;
	}
	else
	{
		(*result) = num1 + num2;
		return SOAP_OK;
	}
	return SOAP_OK;
}